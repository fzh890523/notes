# Serviceability in HotSpot

The HotSpot Virtual Machine contains several technologies that allow its operation to be observed by another Java process:

- **The Serviceability Agent(SA)**. The Serviceability Agent is a Sun private component in the HotSpot repository that was developed by HotSpot engineers to assist in debugging HotSpot. They then realized that SA could be used to craft serviceability tools for end users since it can expose Java objects as well as HotSpot data structures both in running processes and in core files.
- **jvmstat performance counters**. HotSpot maintains several performance counters that are exposed to external processes via a Sun private shared memory mechanism. These counters are sometimes called perfdata.
- **The Java Virtual Machine Tool Interface (JVM TI)**. This is a standard C interface that is the reference implementation of [JSR 163 - JavaTM Platform Profiling Architecture](http://jcp.org/en/jsr/detail?id=163) JVM TI is implemented by HotSpot and allows a native code 'agent' to inspect and modify the state of the JVM.
- **The Monitoring and Management interface**. This is a Sun private API that allows aspects of HotSpot to be monitored and managed.
- **Dynamic Attach**. This is a Sun private mechanism that allows an external process to start a thread in HotSpot that can then be used to launch an agent to run in that HotSpot, and to send information about the state of HotSpot back to the external process.
- **DTrace**. [DTrace](http://en.wikipedia.org/wiki/DTrace) is the award winning dynamic trace facility built into Solaris 10 and later versions. DTrace probes have been added to HotSpot that allow monitoring of many aspects of operation when HotSpot runs on Solaris. In addition, HotSpot contains a jhelper.d file that enables dtrace to show Java frames in stack traces.
- **pstack support**. [pstack](http://docs.sun.com/app/docs/doc/816-5165/6mbb0m9pm?a=view) is a Solaris utility that prints stack traces of all threads in a process. HotSpot includes support that allows pstack to show Java stack frames.

> **Note**: HotSpot also includes the following mechanisms that will produce output on the standard output TTY. These mechanisms are not normally used by observability tools and won't be discussed further here.Command line options such as -verbose that cause trace type information to be written to the standard output tty deviceSIGQUIT/control-break handling that causes a stack dump of executing threads to be produced on the standard output tty device.

For each of these technologies, there is code in the J2SE repository that uses it, and/or allows user code to use it; see [Serviceability in the J2SE Repository](http://openjdk.java.net/groups/serviceability/svcjdk.html)

The following table contains links to more information about each of these technologies, shows where these technologies reside in the HotSpot repository, and contains links to information about the use of the technologies in the J2SE repository.

| Technology                               | Source Location                          | Usage in the J2SE Repository             |
| ---------------------------------------- | ---------------------------------------- | ---------------------------------------- |
| [JVM TI- Java Virtual Machine Tools Interface](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#bjvmti) | [hotspot/src/share/vm/prims/jvmtiGen.java](http://opengrok.neojava.org/hotspot/xref/src/share/vm/prims/jvmtiGen.java)[hotspot/src/share/vm/prims/jvmtiGen.java](http://opengrok.neojava.org/hotspot/xref/src/share/vm/prims/jvmtiEnvFill.java)[hotspot/src/share/vm/prims/jvmti.xml](http://opengrok.neojava.org/hotspot/xref/src/share/vm/prims/jvmti.xml)[hotspot/src/share/vm/prims/jvmti*](http://opengrok.neojava.org/hotspot/xref/src/share/vm/prims)build/.../generated/jvmtifiles/jvmtiEnter.cppbuild/.../generated/jvmtifiles/jvmtiEnterTrace.cppbuild/.../generated/jvmtifiles/jvmtiEnv.hppbuild/.../generated/jvmtifiles/jvmtiEnvRecommended.cppbuild/.../generated/jvmtifiles/jvmtiEnvStub.cppbuild/.../generated/jvmtifiles/jvmti.h    (copied to j2se/src/share/javavm/export/jvmti.h) | [J2SE Info](http://openjdk.java.net/groups/serviceability/svcjdk.html#tJDWP)[Bugs](http://bugs.sun.com/bugdatabase/search.do?process=1&category=hotspot&bugStatus=open&subcategory=jvmti) |
| [Monitoring and Management](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#bmanagement) | [hotspot/src/share/vm/services/](http://opengrok.neojava.org/hotspot/xref/src/share/vm/services/) (most but not all) | [J2SE Info](http://openjdk.java.net/groups/serviceability/svcjdk.html#tmanagement)[Bugs](http://bugs.sun.com/bugdatabase/search.do?process=1&category=hotspot&bugStatus=open&subcategory=monitoring_management) |
| [Dynamic attach mechanism](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#battach) | [src/share/vm/services/attachListener.*](http://opengrok.neojava.org/hotspot/xref/src/share/vm/services/)[src/os/linux/vm/attachListener_linux.cpp](http://opengrok.neojava.org/hotspot/xref/src/os/linux/vm/attachListener_linux.cpp)[src/os/solaris/vm/attachListener_solaris.cpp](http://opengrok.neojava.org/hotspot/xref/src/os/solaris/vm/attachListener_solaris.cpp)[src/os/win32/vm/attachListener_win32.cpp](http://opengrok.neojava.org/hotspot/xref/src/os/win32/vm/attachListener_win32.cpp) | [J2SE Info](http://openjdk.java.net/groups/serviceability/svcjdk.html#tattach) |
| [Jvmstat Performance Counters](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#bjvmstat) | [src/share/vm/prims/perf.cpp](http://opengrok.neojava.org/hotspot/xref/src/share/vm/prims/perf.cpp)[src/share/runtime/perfMemory.cpp](http://opengrok.neojava.org/hotspot/xref/src/share/vm/runtime/perfMemory.cpp)[src/share/runtime/perfData.cpp](http://opengrok.neojava.org/hotspot/xref/src/share/vm/runtime/perfData.cpp)[src/share/runtime/statSampler.cpp](http://opengrok.neojava.org/hotspot/xref/src/share/vm/runtime/statSampler.cpp)[src/share/vm/services/*Service.cpp](http://opengrok.neojava.org/hotspot/xref/src/share/vm/services/)[src/os/solaris/vm/perfMemory_solaris.cpp](http://opengrok.neojava.org/hotspot/xref/src/os/solaris/vm/perfMemory_solaris.cpp)[src/os/linux/vm/perfMemory_linux.cpp](http://opengrok.neojava.org/hotspot/xref/src/os/linux/vm/perfMemory_linux.cpp)[src/os/win32/vm/perfMemory_win32.cpp](http://opengrok.neojava.org/hotspot/xref/src/os/win32/vm/perfMemory_win32.cpp) | [J2SE Info](http://openjdk.java.net/groups/serviceability/svcjdk.html#tjvmstat)[Bugs](http://bugs.sun.com/bugdatabase/search.do?process=1&category=hotspot&bugStatus=open&subcategory=perfdata) |
| [Serviceability Agent](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#bsa) | [hotspot/agent/](http://opengrok.neojava.org/hotspot/xref/agent/)[hotspot/src/share/vm/runtime/vmStructs.hpp](http://opengrok.neojava.org/hotspot/xref/src/share/vm/runtime/vmStructs.hpp/)[hotspot/src/share/vm/runtime/vmStructs.cpp](http://opengrok.neojava.org/hotspot/xref/src/share/vm/runtime/vmStructs.cpp/)hotspot/cpu/*cpu*/vm/vmstructs_*cpu*.hpphotspot/os_cpu/*os-cpu*/vm/vmstructs_*os-cpu*.hpp | [J2SE Info](http://openjdk.java.net/groups/serviceability/svcjdk.html#tsa)[Usenix Serviceability Agent paper](http://www.usenix.org/events/jvm01/full_papers/russell/russell_html/index.html) |
| [DTrace Support (Solaris only)](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#bdtrace) | [hotspot/src/os/solaris/dtrace/](http://opengrok.neojava.org/hotspot/xref/src/os/solaris/dtrace/)[hotspot/build/solaris/makefiles/dtrace.make](http://opengrok.neojava.org/hotspot/xref/build/solaris/makefiles/dtrace.make) | [DTrace Probes in HotSpot](http://java.sun.com/javase/6/docs/technotes/guides/vm/dtrace.html)[User Guide](http://docs.sun.com/app/docs/doc/819-5488) |
| [pstack Support(Solaris only)](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#bpstack) | [hotspot/src/os/solaris/dtrace/](http://opengrok.neojava.org/hotspot/xref/src/os/solaris/dtrace/) | [User Guide](http://docs.sun.com/app/docs/doc/816-5165/6mbb0m9pm?a=view) |

## Build and Implementation Notes

## [HotSpot JVM TI]()

The base definition of JVM TI is contained in file 

jvmti.xml

 which is processed at HotSpot build time by 

hotspot/src/share/vm/prims/jvmtiGen.java

 and 

hotspot/src/share/vm/prims/jvmtiEnvFill.java

 to create the .cpp and .hpp files shown above in the build/.../ directory. These files are then compiled during the build. The resulting JVM TI implementation is included in libjvm.so/jvm.dll with the rest of HotSpot.

The HotSpot build process creates interface file jvmti.h which is used by JVM TI agents such as the [JPDA back-end](http://openjdk.java.net/groups/serviceability/svcjdk.html#tjdwp). jvmti.h is copied from the HotSpot build area and checked into the J2SE repository whenever changes are made to the interface. This file contains a JVM TI version number which is compiled into the back-end and is checked against the JVM TI version that is in HotSpot during back-end startup.

In addition to the files shown above, JVM TI has hooks in many other HotSpot files, mainly for detecting events that might need to be reported to JVM TI agents. You can see such usages by 'grep -i jvmti' in the other HotSpot files. For many debugging functions, JVM TI also needs hooks in the generated interpreter. Since the mere presence of these hooks can slow down applications, the interpreter is normally generated without these hooks. If debugging is to be done, then a -agentlib option must be used on the Java command line specifying the debugging agent that is to be run. This option will be detected early in HotSpot startup and will cause the [agent intialization](http://java.sun.com/javase/6/docs/platform/jvmti/jvmti.html#startup) code to be run before the interpreter is generated. The agent's startup code will request the JVM TI debugging [capabilities](http://java.sun.com/javase/6/docs/platform/jvmti/jvmti.html#capability) which will in turn cause the interpreter to be generated in debug mode.

We are investigating the possibility of allowing debugging agents to start dynamically after HotSpot is already running in JDK 7. See [4841257: Should be able to 'attach on demand' to debug](http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4841257)

[Here(.pdf)](http://openjdk.java.net/groups/hotspot/docs/jvmtiImpl.pdf) is a presentation about the JVM TI implementation.
[![up arrow](http://openjdk.java.net/groups/hotspot/docs/upArrow.jpg)](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#tjvmti)

## [HotSpot Monitoring and Management]()

File src/share/vm/services/jmm.h defines a Sun private interface that is implemented in HotSpot and is used by the monitoring and management code in the J2SE repository. jmm.h is copied into the J2SE repository so that monitoring and management native methods can use it to call into HotSpot to extract information. jmm.h contains a version number that is used at runtime to verify interface compatibility between the Java code and the HotSpot that is being monitored.

See [Monitoring and Management in the J2SE Repository](http://openjdk.java.net/groups/serviceability/svcjdk.html#tmanagement) for more information.
[![up arrow](http://openjdk.java.net/groups/hotspot/docs/upArrow.jpg)](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#tmanagement)

## [HotSpot Dynamic Attach Mechanism]()

This is a Sun extension that allows a tool to 'attach' to another process running Java code and launch a JVM TI agent or a java.lang.instrument agent in that process. This also allows the system properties to be obtained from the target JVM.

The Sun implementation of this API also includes some HotSpot specific methods that allow additional information to be obtained from HotSpot:

- The ctrl-break output from the local JVM
- The ctrl-break output from the remote JVM
- A dump of the heap
- A histogram showing the number of instances of the classes loaded in the target JVM. Either all instances or just 'live' instances can be counted.
- The value of a manageable command line flag. Such flags can also be set.

Dynamic attach has an attach listener thread in the target JVM. This is a thread that is started when the first attach request occurs. On Linux and Solaris, the client creates a file named .attach_pid(pid) and sends a SIGQUIT to the target JVM process. The existence of this file causes the SIGQUIT handler in HotSpot to start the attach listener thread. On Windows, the client uses the Win32 CreateRemoteThread function to create a new thread in the target process. The attach listener thread then communicates with the source JVM in an OS dependent manner:

- On Solaris, the Doors IPC mechanism is used. The door is attached to a file in the file system so that clients can access it.
- On Linux, a Unix domain socket is used. This socket is bound to a file in the filesystem so that clients can access it.
- On Windows, the created thread is given the name of a pipe which is served by the client. The result of the operations are written to this pipe by the target JVM.

![up arrow](http://openjdk.java.net/groups/hotspot/docs/upArrow.jpg)

## [HotSpot Jvmstat Performance Counters]()

The HotSpot JVM exports a set of instrumentation objects, or counters as they are typically called. The counters are always on and so are updated by HotSpot in such a way as to impose minimal overhead to the running application. The set of counters exported by a JVM is not static as a JVM may create certain counters only when appropriate arguments are specified on the command line. Furthermore, different versions of a JVM may export very different sets of instrumentation. The counters have structured names such as 

sun.gc.generation.1.name, java.threads.live, java.cls.loadedClasses

. The names of these counters and the data structures used to represent them are considered private, uncommitted interfaces to the HotSpot JVM. Users should not become dependent on any counter names, particularly those that start with prefixes other than "java.".

These counters are exposed to observers in different processes by means of a shared memory file. This allows observers in other processes to poll the counters without imposing any overhead on the target process. The java.io.tmpdir system property contains the pathname of the directory in which this file resides. The Solaris and Linux shared memory implementations use the mmap interface with a backing store file to implement named shared memory. Using the file system as the name space for shared memory allows a common name space to be supported across a variety of platforms. It also provides a name space that Java applications can deal with through simple file APIs. The Solaris and Linux implementations store the backing store file in a user specific temporary directory located in the /tmp file system, which is always a local file system and is sometimes a RAM based file system. The name of the file is:

> /tmp/hsperfdata_*user-name*/*vm-id*

The win32 shared memory implementation uses two objects to represent the shared memory: a windows kernel based file mapping object and a backing store file. On windows, the name space for shared memory is a kernel based name space that is disjoint from other win32 name spaces. Since Java is unaware of this name space, a parallel file system based name space is maintained, which provides a common file system based shared memory name space across the supported platforms and one that Java apps can deal with through simple file apis. For performance and resource cleanup reasons, it is recommended that the user specific directory and the backing store file be stored in either a RAM based file system or a local disk based file system. Network based file systems are not recommended for performance reasons. In addition, use of SMB network based file systems may result in unsuccessful cleanup of the disk based resource on exit of the JVM. The Windows TMP and TEMP environment variables, as used by the GetTempPath() Win32 API (see os::get_temp_directory() in os_win32.cpp), control the location of the user specific directory and the shared memory backing store file. This file must not be on a FAT filesystem.
[![up arrow](http://openjdk.java.net/groups/hotspot/docs/upArrow.jpg)](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#tjvmstat)

## [HotSpot Serviceability Agent]()

SA knows how to:

- Read bits out of an executing Java process, or a core file from a Java process
- Extract all the HotSpot data structures from the raw bits.
- Extract Java objects from the HotSpot data structures.

Note that SA runs in a separate process from the target process and executes no code in the target process. However, the target process is halted while SA observes it.

SA consists mostly of Java classes but it contains a small amount of native code to read raw bits from processes and core files.

- On Solaris SA uses [libproc](http://www.opensolaris.org/os/community/observability/process/libproc/;jsessionid=4C9B9B3195B58BAF8F78C89987506E58) to read bits from a process or a core file.
- On Linux, SA uses a mix of /proc and ptrace (mostly the latter) to read bits from a process. For core files, SA parses ELF files directly.
- On Windows, SA uses the Windows dbgeng.dll library to read the raw bits from processes and core files. An alternate implementation uses Windows process debugging primitives, but this only works for live processes.
  **Note that as of JDK 6, SA and tools that depend on it have not yet been released on Windows.**

File [src/share/vm/runtime/vmStructs.cpp](http://12.101.252.19/hotspot/xref/src/share/vm/runtime/vmStructs.cpp) contains 'declarations' of each HotSpot class and its fields as well as declarations of processor dependent items such as registers, sizeof types, ... For the latter, vmStructs.cpp includes arch/cpu dependent files, eg:

- [src/cpu/sparc/vm/vmStructs_sparc.hpp](http://12.101.252.19/hotspot/xref/src/cpu/sparc/vm/vmStructs_sparc.hpp)
- [src/os_cpu/solaris_sparc/vm/vmStructs_solaris_sparc.hpp](http://12.101.252.19/hotspot/xref/src/os_cpu/solaris_sparc/vm/vmStructs_solaris_sparc.hpp)

As an example, in file 

src/share/vm/oops/cpCacheOop.hpp

 we have:

```
      :
      class constantPoolCacheOopDesc: public arrayOopDesc {
      friend class VMStructs;
      private:
      constantPoolOop _constant_pool;   // the corresponding constant pool
      :
    
```

In vmStructs.cpp, the 

_constant_pool

 field is 'declared' like this:

```
      nonstatic_field(constantPoolCacheOopDesc, _constant_pool,  constantPoolOop) \
    
```

Note the 'friend class VMStructs' declaration in the above class. Most classes declare VMStructs to be a friend so that private fields can be accessed.

During the HotSpot build, vmStructs.cpp is compiled into vmStructs.o which is included in libjvm.so. vmStructs.o contains all the data that SA needs to read the HotSpot data structures. At runtime, SA reads this data from the target process or core file.

> **The names in vmStructs.cpp are used by the Java code in SA. Thus, if a field named in vmStructs.cpp is deleted or renamed, both vmStructs.cpp and the Java code that access that field have to be modified. If this isn't done, then SA will fail when it tries to examine a process/core file.**

The test in 

agent/jdi/sasanity.sh

 which runs this class: 

agent/jdi/SASanityChecker.java

 should be run to check this.

Lastly, the Java code in SA is basically a mirror of the C++ code in HotSpot. If algorithms are changed in HotSpot, the same changes might have to e made in the SA Java code. Because of the tight coupling between the Java classes in SA and the HotSpot data structures, we can only count on an instance of SA being able to analyze the HotSpot that was built from the same HotSpot repository state. In order to detect a mismatch, the HotSpot build places a sa.properties file into sa-jdi.jar. This file contains a version property, eg:

> sun.jvm.hotspot.runtime.VM.supportedVersion=1.7.0

At run time, SA Java code reads this property and compares it to the version of the HotSpot to be analyzed and throws a VMVersionMismatchException if the versions do not match. This check can be disabled by running the SA tool with

> -Dsun.jvm.hotspot.runtime.VM.disableVersionCheck

SA components are built as part of the standard build of the HotSpot repository:

- The native code component of SA is placed in these files:

  ​

  > jre/lib/sparc/libsaproc.so
  > jre/lib/i386/libsaproc.so

- The Java classes for SA are placed in

  ​

  > jdk/lib/sa-jdi.jar

  It includes an implementation of the

   

  Java Debug Interface(JDI)

   

  which allows JDI clients to do read-only debugging on core files and hung processes.

These two files are copied from the HotSpot build area to the JDK build area during a control build (a control build is a build of the control repository which first builds HotSpot and then builds the J2SE repository, so that the files built by the HotSpot build are available to the J2SE build.)

SA includes other components that are just used for debugging HotSpot and are not built as part of the normal HotSpot build. These components are built by doing a make in the hotspot/agent/make directory. For more information, please see[agent/doc/](http://opengrok.neojava.org/hotspot/xref/agent/doc) for documentation on these tools, and hints for cross machine core dump debugging.

See also [Usenix Serviceability Agent paper](http://www.usenix.org/events/jvm01/full_papers/russell/russell_html/index.html)
[![up arrow](http://openjdk.java.net/groups/hotspot/docs/upArrow.jpg)](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#tsa)

## [DTrace Support]()

(The files that support dtrace in HotSpot are in 

hotspot/src/os/solaris/dtrace/

 )

HotSpot contains functionality that allows the DTrace jstack() action to show Java frames. In addition, HotSpot contains several built-in USDT probes that allow HotSpot actions to be directly accessed from D programs.

#### [jstack() Support]()

HotSpot contains support for the dtrace jstack() action that allows Java stack frames to be shown. Here is the how this works.

- Files generateJvmOffsets.cpp and generateJvmOffsetsMain.cpp are compiled into executable program generateJvmOffets
- generateJvmOffsets is run which creates files JvmOffsets.cpp, JvmOffsets.h, JvmOffsetsIndex.h
  - JvmOffsets.hpp defines names of offsets to various HotSpot data structures
  - JvmOffsets.cpp contains an array of these offsets
  - JvmOffsetsIndex.h defines indexes into this array
- JvmOffsets.o is linked into libjvm.so
- jhelper.d includes jvmOffsetsIndex.h. It uses this to find entries in the table in JvmOffsets.o, and to copy the corresponding data from HotSpot into the jhelper.d program.
- jhelper.d is compiled into dtrace.o which contains a special Elf section - SUNW_dof.
- dtrace.o is linked into libjvm.so. When libjvm.so is loaded, the _init() method in the SUNW_dof section registers the contents of dtrace.o with dtrace.
- When a D script program calls the jstack() action, dtrace finds the related code in dtrace.o and calls it.

### USDT dtrace probes in HotSpot

A USDT dtrace probe in a HotSpot file is represented by a macro that calls a non-existent specially-named external function. The parameters that are passed to the function (through the macro) become arguments that the dtrace script client can access. For example, in hashtable.cpp, the new_entry method contains

```
        HS_DTRACE_PROBE4(hs_private, hashtable__new_entry,  this, hashValue, obj, entry);
    
```

hs_private

 is a dtrace 

provider

. HotSpot has three providers:

> hotspot, hotspot_jni, hs_private

These providers are defined in files hotspot.d, hotspot_jni.d, and hs_private.d. These files are combined (along with jhelper.d) into temp file dtrace.d which is compiled by a dtrace command into file dtrace.o. dtrace.o contains a special section (SUNW_dof) which contains a mapping of the probes to their location in the code. In addition to the dtrace.d file, the dtrace command is also given the .o files that contain the probes. dtrace generates new versions of these .o files in which the non-existent functions have been replaced by one or more 'nop' instructions, and the non-existant symbols are deleted have been deleted from the symbol table.

Finally, the modified .o files, and dtrace.o are linked into libjvm.so. When libjvm.so is loaded, the _init() method in the special SUNW_dof section registers the probes with dtrace in the kernel. When a dtrace script wants to trace a particular area, it interacts with the dtrace code in the kernel and causes a 'trap' instruction to replace the 'nop', and the kernel handles all the work to get the dtrace actions executed.

Because the probe points turn into nop instruction (except for the argument setup), the probes are relatively cost-free in the traced application when not actively probed. The argument setup can be somewhat costly at times which is why in hotspot the synchronization probes are protected by a command-line switch. Newer versions of dtrace have tricks for checking whether a probe is enabled to let you skip that argument setup, but because we have to compile on Solaris 8 and have a special backported version of dtrace, we don't have that functionality.

A problem is that currently, USDT probes cannot be placed in generated code. This makes tracing Java methods and object allocation tricky since that is done in generated code. To overcome this, there are a couple of stubs in static code in src/share/vm/runtime/sharedRuntime.cpp which contain the appropriate probes. When the

> -XX:+ExtendedDTraceProbes flag

is passed on the command line, runtime control flow is redirected thru these stubs which slows down execution.

![up arrow](http://openjdk.java.net/groups/hotspot/docs/upArrow.jpg)

## [pstack Support]()

pstack(1) is a Solaris utility that prints stack traces of all threads in a process. HotSpot contains support that allows pstack to find names of Java methods on a stack.

pstack does this by calling into libjvm_db.so to get the names of Java frames. libjvm_db.so is created from file libjvm_db.c which finds information in the HotSpot process by using the same JvmOffsets mechanism as does the [dtrace jstack() provider](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#jstack).
[![up arrow](http://openjdk.java.net/groups/hotspot/docs/upArrow.jpg)](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html#tdtrace)

------

Last Modified: 06/29/07