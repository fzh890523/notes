

Go 1.5 concurrent garbage collector pacing

Austin Clements

2015-03-10

golang.org/s/go15gcpacing

Comment at https://groups.google.com/forum/#!topic/golang-dev/YjoG9yJktg4

UPDATE 2015-08-27: I’ve added a section on the final design of the pacer released in Go 1.5 at the end of this document.

Introduction

Prior to Go 1.5, Go has used a parallel stop-the-world (STW) collector.  While STW collection has many downsides, it does at least have predictable and controllable heap growth behavior.  The sole tuning knob for the STW collector was “GOGC”, the relative heap growth between collections.  The default setting, 100%, triggered garbage collection every time the heap size doubled over the live heap size as of the previous collection, as shown in figure 1.

Figure 1.  GC timing in the STW collector.

Go 1.5 introduces a concurrent collector.  This has many advantages over STW collection, but it makes heap growth harder to control because the application can allocate memory while the garbage collector is running.  To achieve the same heap growth limit the runtime must start garbage collection earlier, but how much earlier depends on many variables, many of which cannot be predicted.  Start the collector too early, and the application will perform too many garbage collections, wasting CPU resources.  Start the collector too late, and the application will exceed the desired maximum heap growth.  Achieving the right balance without sacrificing concurrency requires carefully pacing the garbage collector.

This document proposes a mechanism to perform this pacing by adjusting the GC trigger point and scheduling the CPU to achieve the desired heap size and CPU utilization bounds.  We show simulation results indicating that this algorithm is stable and converges quickly under a wide range of workloads.

Optimization goals

GC pacing aims to optimize along two dimensions: heap growth, and CPU utilized by the garbage collector. 

Figure 2.  Concurrent garbage collection heap metrics.  

A Go user expresses the desired maximum heap growth by setting GOGC to percent heap growth from one garbage collection cycle to the next.  Let hg= GOGC/100 denote this goal growth ratio.  That is, if Hm(n) is the total size of marked objects following the nth GC cycle, then the goal heap size is Hg(n)=Hm(n-1)(1+hg).  Throughout this document, we will use the convention that H□(n)=Hm(n-1)(1+h□(n)) is the absolute heap size for the heap growth ratio h□.  Pacing must optimize for each cycle to terminate when the actual heap growth at the end of the cycle (prior to sweeping) ha(n) is as close as possible to the GOGC goal, as shown in figure 2.

Goal 1.  Minimize hg - ha(n).

A cycle may under- or overshoot hg, its goal heap growth.  Pacing must minimize heap growth overshoot to avoid consuming more memory than desired.  At the same time, pacing must minimize heap growth undershoot because regularly undershooting means GC is running too often, consuming more total CPU than intended, and slowing down the overall application.

In a STW collector, this goal is achieved by simply running the collector when allocated heap growth reaches hg.  In a concurrent collector, the runtime must trigger garbage collection before this point.  hT(n) denotes this trigger growth ratio, which the runtime will adjust to achieve its pacing goals.

Pacing must also optimize scheduling to achieve the desired garbage collector CPU utilization.  CPU utilization by the garbage collector during concurrent phases should be as close to 25% of GOMAXPROCS as possible.  This includes time in the background collector and assists from the mutator, but not time in write barriers (simply because the accounting would increase write barrier overhead) or secondary effects like increased cache misses.  Of course, if the mutator is using less than 75% CPU, the garbage collector will run in the remaining time; this idle utilization does not count against the GC CPU budget.  Let ug=0.25 denote this goal utilization and ua(n) be the actual average CPU utilization achieved by the nth GC cycle (not including idle utilization).

Goal 2.  Minimize ug - ua(n).

As with heap size, a cycle may under- or overutilize the CPU.  25% maximum utilization is a stated goal for the Go 1.5 collector, so pacing should minimize CPU overutilization.  However, this is a soft limit and necessarily so: if the runtime were to strictly enforce a 25% utilization limit, a rapidly allocating mutator can cause arbitrary heap overshoot.  Pacing should also minimize CPU underutilization because using as much of the 25% budget as possible minimizes the duration of the concurrent mark phase.  Since the concurrent mark phase enables write barriers, this in turn minimizes the impact of write barrier overhead on application performance.  It also reduces the amount of floating garbage, objects that are kept by the collector because they were reachable at some point during GC but are not reachable at GC termination.  The runtime will adjust how it schedules the CPU between mutators and background garbage collection to achieve the pacing goals.

Design

The design of GC pacing consists of four components: 1) an estimator for the amount of scanning work a GC cycle will require, 2) a mechanism for mutators to perform the estimated amount of scanning work by the time heap allocation reaches the heap goal, 3) a scheduler for background scanning when mutator assists underutilize the CPU budget, and 4) a proportional controller for the GC trigger.

The design balances two different views of time: CPU time and heap time.  CPU time is like standard wall clock time, but passes GOMAXPROCS times faster.  That is, if GOMAXPROCS is 8, then eight CPU seconds pass every wall second and GC gets two seconds of CPU time every wall second.  The CPU scheduler manages CPU time.  The passage of heap time is measured in bytes and moves forward as mutators allocate.  The relationship between heap time and wall time depends on the allocation rate and can change constantly.  Mutator assists manage the passage of heap time, ensuring the estimated scan work has been completed by the time the heap reaches the goal size.  Finally, the trigger controller creates a feedback loop that ties these two views of time together, optimizing for both heap time and CPU time goals.

Scan work estimator

Because Go 1.5’s collector is a mark-sweep collector, the CPU time consumed by the concurrent mark phase is dominated by scanning, the process of greying and subsequently blackening objects.  Hence, to pace the collector, the runtime needs to estimate the amount of work We that will be performed by scanning.

Scanning time is roughly linear in the number of pointer slots scanned, so we measure scan work in scanned pointer slots.  Alternatively, scan work could be estimated in total bytes scanned, including non-pointer bytes prior to the last pointer slot of an object (scanning stops after the last pointer slot).  We choose to measure only scanning of pointer slots because this is more computationally expensive and far more likely to cause cache misses.  We may revise scan work to count both, but to assign more weight to pointer slots.

The actual scan work Wa(n) performed by the nth cycle may vary significantly from cycle to cycle with heap size.  Hence, similar to the heap trigger and the heap goal, the garbage collector will track the scan work from cycle to cycle as a ratio w=W/Hm of pointers per marked heap byte, which should be much more stable.

There are several possible approaches to estimating w and finding a good estimator will likely require some experimentation with real workloads.  The worst-case estimator is 1/pointer size—the entire reachable heap is pointers—but this is far too pessimistic.  A better estimator is the scan work performed by the previous garbage collection cycle.  However, this may be too sensitive to transient changes in heap topology.  Hence, to smooth this out, we will use an exponentially weighted moving average (EWMA) of scan work ratios of recent cycles,

w(n+1)=KwWa(n)Hm(n)+(1-Kw)w(n)

where Kw is the weighting coefficient.  We’ll start with Kw=0.75 and tune this if necessary.  At the beginning of each cycle, the garbage collector will estimate the scan work We(n) for that cycle using this scan work ratio estimate and the marked heap size of the previous cycle as an estimate of the reachable heap in this cycle:

We(n)=w(n)Hm(n-1).

If this proves insufficient, it should be possible to use more sophisticated models to account for trends and patterns.  It may also be possible to revise the scan work estimate as collection runs, at least if it discovers more scan work than the current estimate.

Mutator assists

With only background garbage collection, a mutator may allocate faster than the garbage collector can mark.  At best, this causes the heap to always overshoot and saturates the trigger point ht at 0.  At worst, this leads to unbounded heap growth.

To address this, the garbage collector will enlist the help of the mutator since allocation by the mutator is what causes the heap size to approach (and potentially exceed) the maximum heap size.  Hence, allocation can assist the garbage collector by performing scanning work proportional to the size of the allocation.  Let A(x,n) denote the assist scan work that should be performed by an allocation of x bytes during the nth GC cycle.

The ideal assist work is

A(x,n)=We(n)Hg(n) - HT(n)x.

For example, if pointers are 8 bytes, the current scan work estimate We is 1GB/8, the trigger point HT is 1.5GB and the heap size goal Hg is 2GB, then A(x,n)=0.25x, so every 4 bytes  of allocation will scan 1 pointer.  Without background garbage collection, when the allocated heap size reaches 2GB, mutator assists will have performed exactly 1GB worth of scanning work.  If We is accurate, then collection will finish at exactly the target heap size.

However, mutator assists alone may underutilize the GC CPU budget, so the collector must perform background collection in addition to mutator assists.  Work performed by background collection is not accounted for above.  Hence, rather than unconditionally performing A(x,n) scan work per allocation, the collector will use a system of work credit in which scanning u pointers creates u units of credit.  The background garbage collector continuously creates work credits as it scans.  Mutator allocation creates A(x,n) units of work deficit, which the mutator can correct by either stealing credit from the background collector (as long as this doesn’t put the background collector into debt) or by performing its own scanning work.

This system of work credit is quite flexible.  For example, it’s difficult to scan exactly A(x,n) pointer slots since scanning is done an object at a time, but this approach lets a mutator accumulate credit for additional scanning work that it can absorb in later allocations.  We can also reduce contention by adding hysteresis: allowing a mutator to accumulate a small amount of deficit without scanning.

CPU scheduling

Mutator assists alone may under- or overutilize the GC CPU budget, depending on the mutator allocation rate.  Both situations are undesirable.

To address underutilization, the runtime will track CPU time spent in mutator assists and background collection since the beginning of the concurrent mark phase.  If this is below the 25% budget, it will schedule the background garbage collector thread in order to bring it up to 25%.  This indirectly helps smooth out transient overutilization as well.  If mutator assists briefly surpass the 25% budget, the scheduler will not run the background collector until the average comes back down below 25%.  Likewise, if the background collector has built up work credit, mutator assists that would exceed the 25% budget without background credit are more likely to consume the background credit and not expend CPU time on scanning work.

However, the CPU scheduler does not address long-term overutilization, as limiting mutator assists would allow rapidly allocating mutators to grow the heap arbitrarily.  Instead, this is handled by the trigger ratio controller.

Trigger ratio controller

While the runtime has direct and continuous control over GC CPU utilization, it has only indirect control over ha, the heap growth when GC completes.  Given constraints on GC CPU utilization, this indirect control comes primarily from when the runtime decides to start a GC cycle, hT.

The appropriate value of hT to avoid heap under- or overshoot depends on several factors that will vary between applications and during execution.  Hence, the runtime will use a proportional controller to adapt hT after every garbage collection:

hT(n+1)=0, hT(n)+KTe(n)

where KT[0,1] is the trigger controller’s proportional gain and e(n) is the error term as a heap ratio delta.  The value of hT(0) is unlikely to have significant impact.  Based on current heuristics, we’ll set hT(0)=7/8 and adjust if this is too aggressive.  KT may also require some tuning.  We’ll start with KT=0.5.

This leaves the error term.  Perhaps the obvious way would be to adjust hT according to how much the heap over- or undershot, e*(n)=hg-ha(n).  However, this doesn’t account for CPU utilization, which leads to instability: if the heap undershoots, this will increase the trigger size, which will increase the amount of scanning work done by mutator assists per allocation byte, increasing the GC CPU utilization and probably causing the heap to undershoot again.

Instead, the runtime will adjust hT based on an estimate of what the heap growth would have been if GC CPU utilization was ug=0.25.  This leads to the error term

e(n)=hg-hT(n)-ua(n)ug(ha(n)-hT(n)).

The details of deriving this equation are in appendix A.  Note that this reduces to the simpler error term above, e*, if CPU utilization is exactly the goal utilization; that is, if ua(n)=ug.  Otherwise, it uses a scaled heap growth ratio to account for CPU over/underutilization; for example, if utilization is 50%, this assumes the heap would have grown twice as much during garbage collection if utilization were limited to 25%.

Combined with mutator assists and CPU scheduling, the trigger ratio controller creates a feedback loop that couples CPU utilization and heap growth optimization to achieve the optimization goals.  If the trigger is too high, mutator assists will handle the estimated scan work by the time heap size reaches the heap goal, but will force GC CPU utilization over 25%.  As a result, the scaled heap growth in the error term will exceed the heap goal, so the trigger controller will decrease the trigger for the next cycle.  This will spread the assist scan work over a longer period of heap growth in the next cycle, decreasing its GC CPU utilization.  On the other hand, if the trigger is too low, CPU utilization from mutator assists will be low, so the CPU scheduler will schedule background GC to ensure utilization is at least 25%.  This will cause the heap to undershoot, and because utilization was forced to 25%, the error will simply be the difference between the actual heap growth and the goal, causing the trigger controller to increase the trigger for the next cycle.

Simulation results

A simulator for the proposed algorithm is available on GitHub.  This simulator makes it possible to explore a range of workloads and configurations in a controlled setting.

Figure 3 shows the results of simulating the algorithm on a variety of allocation and scan rates.  Each configuration is represented by a pair of graphs showing heap growth ratios and CPU utilization over time.  Both graphs show the goal as well as the achieved value for each cycle.  The top graph of the pair also shows the heap trigger, which is the primary variable directly adjusted by the controller.  The results are noisy as a result of Gaussian noise injected into various parameters by the simulator.

The simulation results indicate that this algorithm is stable and converges quickly over a wide range of workloads.

Figure 3.  Simulated heap size and CPU utilization over a range of allocation and scan rates.  These graphs show only one work ratio; other work ratios produce similar results.

Appendix A.  Derivation of the trigger controller error term

We start with the assumption that GC CPU utilization and heap growth during the concurrent mark phase are inversely proportional.  Using this, the estimated heap size X supposing GC CPU utilization had been ug instead of uais

(X-HT(n))ug=(Ha(n)-HT(n))ua(n)

X=ua(n)ug(Ha(n)-HT(n))+HT(n).

Given this, the absolute error in the trigger heap size is,

E(n)=Hg(n)-X.

Using the relation between absolute heap sizes and heap growth ratios, 

H□(n)=Hm(n-1)(1+h□(n))

or

h□(n)=H□(n)Hm(n-1)-1,

we can translate this into a trigger ratio error,

e(n)
​	

=Hg(n)Hm(n-1)-1-XHm(n-1)-1

​	

=1Hm(n-1)Hg(n)-X

​	

=1Hm(n-1)Hg(n)-HT(n)-ua(n)ugHa(n)-HT(n)

​	

=(1+hg)-(1+hT(n))-ua(n)ug(1+ha(n))-(1+hT(n))

​	

=hg-hT(n)-ua(n)ug(ha(n)-hT(n)).




Go 1.5 final implementation

Added 2015-08-27

Go 1.5 was released in August, 2015 with the new, low-pause, concurrent garbage collector, including an implementation of the pacing algorithm. The final implementation of the pacer followed the fundamental theory laid out by the original proposal closely, but, unsurprisingly, many details changed from the proposal as we learned from real-world workloads.

Here we describe the changes between the proposed algorithm and the final implementation as released in Go 1.5. The overall structure is the same, with the addition of a proportional sweep system that mirrors the proportional mark system. The scan work estimator is the biggest change from the original proposal: it takes a much more conservative approach that often leads to heap under-shoot but prevents significant heap over-shoot. The CPU scheduling policy has changed to enable robust implementation. Finally, the trigger ratio controller enforces new upper and lower bounds to ensure both proportional marking and proportional sweeping have time to work.

Scan work estimator

We found that some modal workloads could rapidly increase the scan density of the heap, which caused the history-based smoothed work estimator in the original proposal to under-assist, which in turn allowed for significant heap growth beyond the heap goal.

Hence, the final implementation instead uses the current, total scannable heap as a conservative estimation of the scan work. Since this is based solely on the current state of the heap, not on past densities or reachable heap sizes, it isn’t susceptible to falling behind during phase changes. Furthermore, the runtime recomputes the assist ratio every time it flushes memory statistics during a garbage collection so that mutator assists will keep up as the scannable heap continues to grow during a garbage collection.

In order to efficiently compute the total scannable heap, we also changed the definition of scan work. Instead of counting pointer slots, the garbage collector counts scannable slots, which is every slot of an object up to and including the last pointer slot. Unlike the number of pointer slots, this information is readily available. This also matches the asymptotic behavior of object scanning, so it better captures the cost of scanning large objects with few pointers.

Mutator assists

With the exception of continuously updating the assist ratio, mutator assists work as described in the proposal. However, there were implementation aspects not specified in the proposal that were important to prevent heap overshoot when transitioning between GC phases. The original proposal did not specify what to do if there was neither enough background credit, nor enough work to satisfy the work deficit. This can happen at the beginning or end of the GC cycle, especially if the mutator is allocating rapidly. In order to control heap growth during these periods, we found it necessary for the assist to yield the CPU and retry until it could either satisfy the deficit or the GC cycle ended. Similarly, it was necessary to enable mutator assists as soon as possible after allocation reached the heap trigger, so the implementation enables assists at the very beginning of the concurrent scan phase and causes allocation to yield the CPU if the garbage collector has not yet reached this phase. We hope to improve on both of these solutions in Go 1.6 with a decentralized GC coordinator.

Sweep assists

The mutator assist mechanism in this proposal uses a proportional work system to ensure that the marking process completes before reaching the heap goal. We found that the same idea was necessary in the context of sweeping between GC cycles. Previously, Go used a classic lazy sweep approach where each allocation swept enough of the heap for that allocation. However, since sweep must be completed before marking begins, the first step of GC was to finish sweeping any unswept parts of the heap. Finishing the sweep can require a large amount of work, especially if the heap trigger is low, and it introduces delay between when the heap trigger is reached and when the garbage collector can enable mutator assists to put back-pressure on allocation.

Hence, Go 1.5 introduces proportional sweeping that works much like the proportional marking performed by mutator assists. At the end of mark termination, the garbage collector computes the total number of sweepable pages S(n) and the distance between the current heap size and the heap trigger, HT(n+1)-Hm(n), and uses this to compute the sweep work AS(x,n) for an allocation of x bytes

AS(x,n)=S(n)HT(n+1)-Hm(n)x.

Every allocation deducts from the global sweep credit and, if necessary, sweeps pages until the credit is positive or there are no more pages to sweep. This way, the entire heap is always swept by the time the next GC cycle begins. Furthermore, unlike for mutator assists, the total amount of sweep work is known at the beginning of the sweep phase and does not change, so there’s no need to estimate or revise the sweep ratio.

CPU scheduling

The proposed CPU scheduling policy turned out to be unusable in practice for several reasons. First, in the worst case, scheduling decisions were made only once every 10 milliseconds, which led to coarse-grained control over utilization. Second, scheduling decisions at the beginning of a GC cycle were based on very short utilization measurements, which made them noisy to the point of being random. Finally, these initial decisions snowballed for the rest of the GC cycle, leading to a bistable condition: in one case, increased mutator assist CPU utilization decreased background marking CPU utilization, which decreased background credit generation, which increased mutator assist CPU utilization; while in the other case, increased background marking CPU utilization increased background credit generation, which decreased mutator assist CPU utilization, which increased background marking CPU utilization. The desired state was the metastable point between these two extremes, which the system rarely achieved.

Hence, the final implementation follows a different policy that simply dedicates 25% of GOMAXPROCS to background marking. At the beginning of each GC cycle, the runtime dedicates d=⌊0.25GOMAXPROCS⌋ Ps to background marking and time shares the remaining 0.25GOMAXPROCS -d of the background marking utilization across the remaining Ps. This works well in practice because it doesn’t require fine-grained measurement or control over utilization and because it eliminates the bistability while allowing the background credit system to work as intended.

Trigger ratio controller

The trigger ratio computation is implemented as originally proposed, with the addition of upper and lower bounds that ensure reasonable mutator assist and sweep assist ratios.

The implementation caps the trigger ratio hT(n) at 0.95hg because as hT approaches hg the mutator assist ratio goes to infinity, which is undesirable. Likewise, the implementation bounds the absolute heap trigger HT(n) from below to be at least 1MB greater than the live heap size at the end of the GC cycle (Hm(n-1)+1MB) to ensure the sweep ratio is bounded and positive. This bound is imposed on the absolute heap trigger rather than the trigger ratio so the live heap size does not feed into the trigger ratio computed in future cycles.

Finally, rather than using the marked heap size to compute the absolute heap trigger and heap goal for the next cycle, the runtime instead uses an estimate of the reachable heap size. In a STW collector, there’s no difference between the reachable size and the marked size, but in a concurrent collector, the marked size includes floating garbage—objects that are retained because they were reachable at some point during the GC cycle, but aren’t reachable at the end. Hence, using the marked heap size can lead to a positive feedback loop: if the mutator allocates and creates floating garbage quickly during a GC cycle, Hm(n) will be large, so the distance between the next trigger and goal, Hm(n)(hg-hT(n+1)), will be large, so the next GC cycle will take longer, allowing the mutator to allocate even more floating garbage, and so on. To avoid this bad feedback, the implementation computes the absolute heap trigger and goal using the reachable heap, which it estimates by subtracting the amount of allocation during the GC cycle from the marked heap size.

