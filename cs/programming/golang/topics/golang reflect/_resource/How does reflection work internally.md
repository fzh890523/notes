

| ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACIAAAAiCAYAAAA6RwvCAAABr0lEQVR42u2W2UsCURTG529usXqK9oXIggojodwgc0m0cMlKLUejSIsC+x9inNWxp9M9N+xB0hlnnMliHj64XO7Abw7nfN9hVqLvMApixo44GAU5IP8TZCrQhMUoD0sxHqaDTftBXATg8EoGtvEBlbcv4TlQlCmcLSDjPg6Ob5VvgG5FWIW+sRzEfSYC2wOio92MaD1I/K7VFwKVvG9ZD5J+VDVBLp5U60FSD9oVQVjLQXwFWRMkdKNYD4LjmX9u94S4fmnDTKhpj4/MhnnI1NQfe2M+wtvrrJN+DtYTAgRLMtVGUqB3Tuj9KogrwMEC6QV0WW9eAu+lBFvkjHeWZw0G3WZKpFlSbvQeXQy/aFUBN3k7SCIzev5+j2RH8bWt6R/dwm88OYn+hCkQLHO4rAwM0K14taVZnb4gnqxkGqKjA9JLhkH8Ouxcr9BrDIPMnfCQrqmmIXJ1lU6UqWZdjguQrRuHQdtfPRWGM76YHzi2bEM/AG5xMTLGuFwP1UcmyB66fS7SdNWCKJCx3UmLA2UPYyTssNT7xB9wtHEtREUqCr1bSxgLPyf0HJA/A/IJg1kQlSzpLVkAAAAASUVORK5CYII=) | Han-Wen Nienhuys | ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAYAAACpF6WWAAAAdklEQVR42mNgGAVDHswnwCfLwP94+CSDBqgBCWh8sg1NgGruR+PD8HogLgBiBVINXI/DQHQ8nxjDYYoV0Pj48HsgNqCmS0k2GFeY7ofi+2ji94kNW1js9+OIfQGomvto6qmSTgWgQXae2jlKgJggIDeNjwIaAQBDo0wVdvRN2QAAAABJRU5ErkJggg==)12/6/20 |
| ---------------------------------------- | ---------------- | ---------------------------------------- |
|                                          |                  |                                          |

[将帖子翻译为中文]()  

Hi there, 
I'm digging into the gc and reflection internals, and specifically, 
I'm trying to understand how types are represented internally. 
I would be grateful if someone could point me into the right direction. 
\1. How does reflection of struct objects work? pkg/reflect declares a 
structType, but this is not instantiated on the go side, and there is 
no StructType equivalent in pkg/runtime/{type.h,runtime.h}. Why does 
the cast from reflect.commonType to reflect.structType work correctly? 
\2. How does the Type structure in cmd/gc/go.h relate to the Type 
structure in pkg/runtime/type.h? 
\3. How does the compiler allocate objects? The assembler output shows 
a call to runtime.new(), which I find in runtime/zmalloc_linux_*, but 
how does the compiler find the runtime.Type corresponding to what was 
generated internally? 
\-- 
Han-Wen Nienhuys 
Google Engineering Belo Horizonte 
[han...@google.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/7d9a0931-7779-4674-8e4f-bb2fe46e0c7f/) 

点击此处回复

| ![img]() | andrey mirtchovski | ![img]()12/6/20 |
| -------- | ------------------ | --------------- |
|          |                    |                 |

其他收件人： han...@google.com

[将帖子翻译为中文]()  

<http://blog.golang.org/2011/09/laws-of-reflection.html> 

| ![img]() | Ian Lance Taylor | ![img]()12/6/20 |
| -------- | ---------------- | --------------- |
|          |                  |                 |

其他收件人： han...@google.com

[将帖子翻译为中文]()  

\> 2. How does the Type structure in cmd/gc/go.h relate to the Type 
\> structure in pkg/runtime/type.h? 

| ![img]() | Han-Wen Nienhuys | ![img]()12/6/20 |
| -------- | ---------------- | --------------- |
|          |                  |                 |

其他收件人： ia...@google.com

\- 显示引用文字 -

| ![img]() | Thomas Bushnell, BSG | ![img]()12/6/20 |
| -------- | -------------------- | --------------- |
|          |                      |                 |

其他收件人： han...@google.com, ia...@google.com

[将帖子翻译为中文]()  

On Tue, Jun 19, 2012 at 5:33 PM, Han-Wen Nienhuys <[han...@google.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/7d9a0931-7779-4674-8e4f-bb2fe46e0c7f/)> wrote:The background is that I'm toying with the idea of trying to make the
compiler generate more exact indications which memory holds pointers.
If I understand things correctly, all allocations happen by running
either runtime.New or unsafe_New, with a Type (runtime/type.h)
argument specifying the type, and the Type object has all the
information to determine where pointers might be inside the block.
Is that correct? (does it generating better pointer annotations sound
feasible?)
From
 my perspective, I think it would be delightful to replace the 
conservative garbage collector with an exact one. The current one has 
had serious leaking bugs on 32-bit systems (as conservative GC is prone 
to do), and, provided it's reasonably efficient, it would be great.
Thomas 

| ![img]() | bradfitz | ![img]()12/6/20 |
| -------- | -------- | --------------- |
|          |          |                 |

其他收件人： tbus...@google.com, han...@google.com, ia...@google.com

[将帖子翻译为中文]()  

\- 显示引用文字 -That would be delightful from everybody's perspective, and work is ongoing on golang-dev.  Look for the Atom symbol.

| ![img]() | Ian Lance Taylor | ![img]()12/6/20 |
| -------- | ---------------- | --------------- |
|          |                  |                 |

其他收件人： han...@google.com

[将帖子翻译为中文]()  

http://codereview.appspot.com/6114046/

| ![img]() | ⚛    | ![img]()12/6/20 |
| -------- | ---- | --------------- |
|          |      |                 |

其他收件人： han...@google.com

[将帖子翻译为中文]()  

> Han-Wen Nienhuys <[han...@google.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/7d9a0931-7779-4674-8e4f-bb2fe46e0c7f/)> writes: 
> \> Is that correct? (does it generating better pointer annotations sound 
> \> feasible?) 
> This is correct and feasible, though there are various difficulties. 
> See <http://codereview.appspot.com/6114046/> . 

A curious difficulty (that I encountered a few days ago while working on issue 6114046) is that the pointer in a Go slice may point to a completely different object:

Slice is represented as tuple (array,length,capacity). The array points to the first element of the slice. If we create another slice like this:

  x := a[cap(a):cap(a)]

then the low-level representation of x is (&a[cap(a)], 0, 0).

There are also other ways of creating such a low-level value, for example by repeated execution of the statement a=a[1:].

The pointer &a[cap(a)] does not point to memory allocated for the underlying array. There are three options where the pointer may point to:

  1. Free memory

  2. Memory allocated to an object

  3. Somewhere into the .data section of the running program

Cases 1 and 3 are causing no problems to a garbage collector. The problematic case is case 2 because it may under certain conditions lead to a memory leak.

In relation to precise GC:   A precise GC needs to be checking the capacity of a slice prior to looking at the pointer. If capacity is zero, the pointer needs to be completely ignored during GC because it may point to another object.   After multiple garbage collections the pointer may point totally anywhere in the heap, not just to an address adjacent to the backing array - because the garbage collector may have succeeded to deallocate the backing array.

Garbage collection in some other programming languages (Java, C#) seems simpler: no pointers into the middle of an object or an array, no values of type [1e6]T in the middle of a struct type, and no slices with zero capacity.

| ![img]() | rog  | ![img]()12/6/20 |
| -------- | ---- | --------------- |
|          |      |                 |

其他收件人： 0xe2.0x...@gmail.com, han...@google.com

[将帖子翻译为中文]()  

| ![img]() | Dave Cheney | ![img]()12/6/20 |
| -------- | ----------- | --------------- |
|          |             |                 |

其他收件人： 0xe2.0x...@gmail.com, han...@google.com

[将帖子翻译为中文]()  

\- 显示引用文字 -

| ![img]() | Han-Wen Nienhuys | ![img]()12/6/20 |
| -------- | ---------------- | --------------- |
|          |                  |                 |

其他收件人： 0xe2.0x...@gmail.com

[将帖子翻译为中文]()  

On Wed, Jun 20, 2012 at 9:22 AM, ⚛ <[0xe2.0x...@gmail.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/7d9a0931-7779-4674-8e4f-bb2fe46e0c7f/)> wrote: 
\> On Wednesday, June 20, 2012 7:17:43 AM UTC+2, Ian Lance Taylor wrote: 

\- 显示引用文字 -

| ![img]() | rog  | ![img]()12/6/20 |
| -------- | ---- | --------------- |
|          |      |                 |

其他收件人： han...@google.com, 0xe2.0x...@gmail.com

[将帖子翻译为中文]()  

| ![img]() | ⚛    | ![img]()12/6/21 |
| -------- | ---- | --------------- |
|          |      |                 |

其他收件人： 0xe2.0x...@gmail.com

> On Wed, Jun 20, 2012 at 9:22 AM, ⚛ <[0xe2.0x...@gmail.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/7d9a0931-7779-4674-8e4f-bb2fe46e0c7f/)> wrote: 
> \> On Wednesday, June 20, 2012 7:17:43 AM UTC+2, Ian Lance Taylor wrote: 
> \> Slice is represented as tuple (array,length,capacity). The array points to 
> \> the first element of the slice. If we create another slice like this: 
> \> 
> \>   x := a[cap(a):cap(a)] 
> \> 
> \> then the low-level representation of x is (&a[cap(a)], 0, 0). 
> \> 
> \> There are also other ways of creating such a low-level value, for example by 
> \> repeated execution of the statement a=a[1:]. 
> \> 
> \> The pointer &a[cap(a)] does not point to memory allocated for the underlying 
> \> array. There are three options where the pointer may point to: 
> \> 
> \>   1. Free memory 
> \>   2. Memory allocated to an object 
> \>   3. Somewhere into the .data section of the running program 
> \> 
> \> Cases 1 and 3 are causing no problems to a garbage collector. The 
> \> problematic case is case 2 because it may under certain conditions lead to a 
> \> memory leak. 
> Makes sense. Since &a[0] is not valid in that case anyway, the slice 
> value could be assigned to be simply nil?

It would be possible to change the pointer to a predefined value, but detecting when to change it and then changing it would add a couple of CPU instructions. Doing nothing at execution time and postponing the detection of capacity==0 to GC time seems to be the best option.

> I notice that you do lots of logic (figuring out the type, deciding 
> what to mark as pointer) during scan-time. Any reason for that? 

There
 doesn't seem to be any additional reason other than figuring out the 
type and using it to interpret the meaning of binary data during GC.

 

> Since memory is already annotated with per-word "no-pointer" bits, I 

Currently, only the 1st word of a block is marked "no-pointers", in which case the whole block contains no pointers.

Using per-word annotations is a valid approach. I believe that having full type information, instead of having just 1 bit saying whether the word is a pointer, is better.

> figured it would be cheaper and easier to fill in this information at 
> the time the memory is allocated.

\- It is true that it may be easier to implement, but it may not be cheaper. Allocating or deallocating a block of size 128 bytes would require on a 64-bit machine to set 128/8=16 bits somewhere in computer memory. The cost is linear in respect to block size.   In contrast to this, associating a type pointer with a block has constant cost per allocation and per deallocation.

\-
 Full type information can be used to print out the approximate number 
of objects of each type. Per-word bits cannot be used for this purpose.

\- Go maps are implemented in C. The map implementation (insertion, deletion, lookup) does not care about the precise type of keys and values in the map, it is sufficient to know their size in bytes. However, a precise garbage collector needs more information about the type of keys and values. In case of "no-pointer" bits per word, the garbage collector would need to somehow obtain the "no-pointer" bits of all key-value pairs stored in the map.

> This would leave the scanning code 
> simpler, and  allow the GC information added type by type, rather than 
> in  a large change. 

I am not sure what you mean by "allow the GC information added type by type".

> I am leafing through your patch; is there a discussion thread 
> somewhere I could look at to understand it better? 

Unfortunately, there isn't.

| ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACIAAAAiCAYAAAA6RwvCAAABr0lEQVR42u2W2UsCURTG529usXqK9oXIggojodwgc0m0cMlKLUejSIsC+x9inNWxp9M9N+xB0hlnnMliHj64XO7Abw7nfN9hVqLvMApixo44GAU5IP8TZCrQhMUoD0sxHqaDTftBXATg8EoGtvEBlbcv4TlQlCmcLSDjPg6Ob5VvgG5FWIW+sRzEfSYC2wOio92MaD1I/K7VFwKVvG9ZD5J+VDVBLp5U60FSD9oVQVjLQXwFWRMkdKNYD4LjmX9u94S4fmnDTKhpj4/MhnnI1NQfe2M+wtvrrJN+DtYTAgRLMtVGUqB3Tuj9KogrwMEC6QV0WW9eAu+lBFvkjHeWZw0G3WZKpFlSbvQeXQy/aFUBN3k7SCIzev5+j2RH8bWt6R/dwm88OYn+hCkQLHO4rAwM0K14taVZnb4gnqxkGqKjA9JLhkH8Ouxcr9BrDIPMnfCQrqmmIXJ1lU6UqWZdjguQrRuHQdtfPRWGM76YHzi2bEM/AG5xMTLGuFwP1UcmyB66fS7SdNWCKJCx3UmLA2UPYyTssNT7xB9wtHEtREUqCr1bSxgLPyf0HJA/A/IJg1kQlSzpLVkAAAAASUVORK5CYII=) | Han-Wen Nienhuys | ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAYAAACpF6WWAAAAdklEQVR42mNgGAVDHswnwCfLwP94+CSDBqgBCWh8sg1NgGruR+PD8HogLgBiBVINXI/DQHQ8nxjDYYoV0Pj48HsgNqCmS0k2GFeY7ofi+2ji94kNW1js9+OIfQGomvto6qmSTgWgQXae2jlKgJggIDeNjwIaAQBDo0wVdvRN2QAAAABJRU5ErkJggg==)12/6/21 |
| ---------------------------------------- | ---------------- | ---------------------------------------- |
|                                          |                  |                                          |

其他收件人： 0xe2.0x...@gmail.com

\- 显示引用文字 -

