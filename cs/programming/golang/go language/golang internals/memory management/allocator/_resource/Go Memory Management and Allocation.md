

# Go: Memory Management and Allocation

[![Vincent Blanchon](https://miro.medium.com/fit/c/96/96/2*pJGo35TpLq8Xh01ac83l2g.jpeg)](https://medium.com/@blanchon.vincent?source=post_page-----a7396d430f44--------------------------------)

[Vincent Blanchon](https://medium.com/@blanchon.vincent?source=post_page-----a7396d430f44--------------------------------)Follow

[Nov 3, 2019](https://medium.com/a-journey-with-go/go-memory-management-and-allocation-a7396d430f44?source=post_page-----a7396d430f44--------------------------------) · 5 min read





Illustration created for “A Journey With Go”, made from the original Go Gopher, created by Renee French.

ℹ️ *This article is based on Go 1.13.*

Go memory management is automatically done by the standard library from the allocation of the memory to its collection when it is not used anymore. Although the developer does not have to deal with it, the underlying management done by Go is well optimized and full of interesting concepts.

# Allocation on the heap

The memory management is designed to be fast in a concurrent environment and integrated with the garbage collector. Let’s start with a simple example:

```
package main

type smallStruct struct {
   a, b int64
   c, d float64
}

func main() {
   smallAllocation()
}

//go:noinline
func smallAllocation() *smallStruct {
   return &smallStruct{}
}
```

The annotation `//go:noinline` will disable in-lining that would optimize the code by removing the function and, therefore, end up with no allocation.

Running the escape analysis command with `go tool compile "-m" main.go` will confirm the allocation made by Go:

```
main.go:14:9: &smallStruct literal escapes to heap
```

Dumping the assembly code for this program, thanks to `go tool compile -S main.go`, would also explicitly show us the allocation:

```
0x001d 00029 (main.go:14)   LEAQ   type."".smallStruct(SB), AX
0x0024 00036 (main.go:14)  PCDATA $0, $0
0x0024 00036 (main.go:14)  MOVQ   AX, (SP)
0x0028 00040 (main.go:14)  CALL   runtime.newobject(SB)
```

The function `newobject` is the built-in function for new allocations and proxy `mallocgc`, a function that manages them on the heap. There are two strategies in Go, one for the small allocations and one for larger ones.

# Small allocation

For the small allocations, under 32kb, Go will try to get the memory from a local cache called `mcache`. This cache handles a list of span (memory chunk of 32kb), called `mspan`, that contains the memory available for allocation:

![Image for post](https://miro.medium.com/max/60/1*CjfnqphbiHxVev_fz4tqlw.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_CjfnqphbiHxVev_fz4tqlw(1).png)

allocation with mcache

Each thread `M` is assigned to a processor `P` and handles at most one goroutine at a time. While allocating memory, our current goroutine will use the local cache of its current `P` to find the first free object available in the span list. Using this local cache does not require lock and makes the allocation more efficient.

The span list is divided into ~70 size classes, from 8 bytes to 32k bytes, that can store different object sizes:

![Image for post](https://miro.medium.com/max/60/1*DrCABMaTrikx-cr3Y8qxWQ.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_DrCABMaTrikx-cr3Y8qxWQ(1).png)

span size classes

Each span exists twice: one list for objects that do not contain pointer and another one that contains pointer. This distinction will make the life of the garbage collector easier since it will not have to scan the spans that do not contain any pointer.

In our previous example, the size of the structure is 32 bytes and will fit in the 32 bytes span:

![Image for post](https://miro.medium.com/max/60/1*D-7bs1DcbEaCTzHXUCZVOw.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_D-7bs1DcbEaCTzHXUCZVOw(1).png)

Now, we may wonder what would happen if the span does not have a free slot during the allocation. Go maintains central lists of spans per size classes, called `mcentral`, with the spans that contain free objects and the ones that do not:

![Image for post](https://miro.medium.com/max/60/1*uosVmYwGEqo9x4jhWBoXhg.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_uosVmYwGEqo9x4jhWBoXhg(1).png)

central lists of spans

`mcentral` maintains a double linked list of spans; each of them has a reference to the previous span and next span. A span in the non-empty list — “non-empty” means that at least one slot is free in the list for allocation — could contain some memory in-use already. Indeed, when the garbage collector sweeps the memory, it could clean a part of the span — the part marked as not used anymore — and would put it back in the non-empty list.

Our program can now request a span from the central list if it runs out of slots:

![Image for post](https://miro.medium.com/max/60/1*8addqzg5-8vRE4aKFkJ-ew.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_8addqzg5-8vRE4aKFkJ-ew(1).png)

span replacement from mcentral

Go needs a way to get new spans to the central list if none are available in the empty list. New spans will now be allocated from the heap and linked to the central list:

![Image for post](https://miro.medium.com/max/60/1*2XymjTsk44kRNueVFceneg.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_2XymjTsk44kRNueVFceneg(1).png)

span allocation from the heap

The heap pulls the memory from the OS when needed. If it needs more memory, the heap will allocate a large chunk of memory, called `arena`, of 64Mb for the 64bits architectures and 4Mb for most of the other architectures. The arena also maps the memory page with the spans:

![Image for post](https://miro.medium.com/max/60/1*3i6nEwjnHVdCnCIGBkzQFA.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_3i6nEwjnHVdCnCIGBkzQFA(1).png)

heap is composed by arenas

# Large allocation

Go does not manage the large allocations with a local cache. Those allocations, greater than 32kb, are rounded up to the page size and the pages are allocated directly to the heap.

![Image for post](https://miro.medium.com/max/60/1*LxVW4TmwYy2HCIY4oTGASA.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_LxVW4TmwYy2HCIY4oTGASA(1).png)

large allocation directly from the heap

# Big picture

We now have a good view of what is happening at a high level during the memory allocation. Let’s draw all the components together to get the full picture:

![Image for post](https://miro.medium.com/max/60/1*i5VF8aE_mFSZ6hhTqEOLZw.png?q=20)

![Image for post](_pics_Go Memory Management and Allocation/1_i5VF8aE_mFSZ6hhTqEOLZw(1).png)

components of the memory allocation

# Inspiration

The memory allocator is originally based on TCMalloc, a memory allocator optimized for the concurrent environment created by Google. The [documentation of TCMalloc](http://goog-perftools.sourceforge.net/doc/tcmalloc.html) is worth reading; you will also find the concepts explained previously.



