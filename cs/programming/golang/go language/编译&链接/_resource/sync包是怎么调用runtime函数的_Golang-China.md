

| [Golang-China](https://groups.google.com/forum/?utm_medium=email&utm_source=footer#!forum/golang-china) › |
| ---------------------------------------- |
|                                          |
| sync包是怎么调用runtime函数的4 名作者发布了 7 个帖子 ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA0AAAANCAYAAABy6+R8AAABAUlEQVR42pVSywqCUBB1GfQB7YL+qYgWki/8ATcuXPkJ/kUboRZFq5YVlm3rEwQhMiik6HaOIFy0wg6Mlzl3zjh3ZhRFgq7rfdM0D4Zh3GEP2JU++KFShWVZHQRMXdeNkyQ5CQn0Pc+LIJwhrlcIVFVtQzAJw3AlfoD3TKxpWosljZBpLxqAcSh3TNG6WtI3MA5/2yr4XEoyCAIBv2bkS8A/U/QqiTzPa0L65CXRk6JcLiHLMuE4TiHgSV8G+BtFtTanaSp83y/Oj29CN1R0ZdewezHiB8WsoJ7/NSfCtu0uiCXngBKOHzYixv2itkrMwB3D5QaWsUscByziE7g5ZewbzQ7OUxr+cUAAAAAASUVORK5CYII=) |

| ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACIAAAAiCAYAAAA6RwvCAAABr0lEQVR42u2W2UsCURTG529usXqK9oXIggojodwgc0m0cMlKLUejSIsC+x9inNWxp9M9N+xB0hlnnMliHj64XO7Abw7nfN9hVqLvMApixo44GAU5IP8TZCrQhMUoD0sxHqaDTftBXATg8EoGtvEBlbcv4TlQlCmcLSDjPg6Ob5VvgG5FWIW+sRzEfSYC2wOio92MaD1I/K7VFwKVvG9ZD5J+VDVBLp5U60FSD9oVQVjLQXwFWRMkdKNYD4LjmX9u94S4fmnDTKhpj4/MhnnI1NQfe2M+wtvrrJN+DtYTAgRLMtVGUqB3Tuj9KogrwMEC6QV0WW9eAu+lBFvkjHeWZw0G3WZKpFlSbvQeXQy/aFUBN3k7SCIzev5+j2RH8bWt6R/dwm88OYn+hCkQLHO4rAwM0K14taVZnb4gnqxkGqKjA9JLhkH8Ouxcr9BrDIPMnfCQrqmmIXJ1lU6UqWZdjguQrRuHQdtfPRWGM76YHzi2bEM/AG5xMTLGuFwP1UcmyB66fS7SdNWCKJCx3UmLA2UPYyTssNT7xB9wtHEtREUqCr1bSxgLPyf0HJA/A/IJg1kQlSzpLVkAAAAASUVORK5CYII=) | 樊冰心  | ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAYAAACpF6WWAAAAdklEQVR42mNgGAVDHswnwCfLwP94+CSDBqgBCWh8sg1NgGruR+PD8HogLgBiBVINXI/DQHQ8nxjDYYoV0Pj48HsgNqCmS0k2GFeY7ofi+2ji94kNW1js9+OIfQGomvto6qmSTgWgQXae2jlKgJggIDeNjwIaAQBDo0wVdvRN2QAAAABJRU5ErkJggg==)9月28日 |
| ---------------------------------------- | ---- | ---------------------------------------- |
|                                          |      |                                          |

这个文件 <https://golang.org/src/sync/runtime.go> 
里面声明了runtime的一些函数，这是怎么做到的？ 
\-- 
樊冰心 

点击此处回复

| ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACIAAAAiCAYAAAA6RwvCAAABr0lEQVR42u2W2UsCURTG529usXqK9oXIggojodwgc0m0cMlKLUejSIsC+x9inNWxp9M9N+xB0hlnnMliHj64XO7Abw7nfN9hVqLvMApixo44GAU5IP8TZCrQhMUoD0sxHqaDTftBXATg8EoGtvEBlbcv4TlQlCmcLSDjPg6Ob5VvgG5FWIW+sRzEfSYC2wOio92MaD1I/K7VFwKVvG9ZD5J+VDVBLp5U60FSD9oVQVjLQXwFWRMkdKNYD4LjmX9u94S4fmnDTKhpj4/MhnnI1NQfe2M+wtvrrJN+DtYTAgRLMtVGUqB3Tuj9KogrwMEC6QV0WW9eAu+lBFvkjHeWZw0G3WZKpFlSbvQeXQy/aFUBN3k7SCIzev5+j2RH8bWt6R/dwm88OYn+hCkQLHO4rAwM0K14taVZnb4gnqxkGqKjA9JLhkH8Ouxcr9BrDIPMnfCQrqmmIXJ1lU6UqWZdjguQrRuHQdtfPRWGM76YHzi2bEM/AG5xMTLGuFwP1UcmyB66fS7SdNWCKJCx3UmLA2UPYyTssNT7xB9wtHEtREUqCr1bSxgLPyf0HJA/A/IJg1kQlSzpLVkAAAAASUVORK5CYII=) | 苏沛   | ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAYAAACpF6WWAAAAdklEQVR42mNgGAVDHswnwCfLwP94+CSDBqgBCWh8sg1NgGruR+PD8HogLgBiBVINXI/DQHQ8nxjDYYoV0Pj48HsgNqCmS0k2GFeY7ofi+2ji94kNW1js9+OIfQGomvto6qmSTgWgQXae2jlKgJggIDeNjwIaAQBDo0wVdvRN2QAAAABJRU5ErkJggg==)9月28日 |
| ---------------------------------------- | ---- | ---------------------------------------- |
|                                          |      |                                          |

一些只声明的函数一般都在 src/runtime 里面用汇编实现的，
也有在同一个pkg目录下用汇编实现。

\- 显示引用文字 -

> \- 显示引用文字 -
>
> \--
> \--
> 官网: <http://golang-china.org/>
> IRC:  [irc.freenode.net](http://irc.freenode.net/)     #golang-china
> @golangchina
> \---
> 您收到此邮件是因为您订阅了 Google 网上论坛的“Golang-China”群组。
> 要退订此群组并停止接收此群组的电子邮件，请发送电子邮件到[golang-china+unsubscribe@googlegroups.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/957392f4-7766-4a6c-8d96-10e6a48f8a26/)。
> 要在网络上查看此讨论，请访问 <https://groups.google.com/d/msgid/golang-china/CABh5RMz18GxHoW2RZ_ubp32fz1-5OYS1VZoUxmB6EtgZBpatWg%40mail.gmail.com>。
> 要查看更多选项，请访问 <https://groups.google.com/d/optout>。

苏沛

| ![img]() | 樊冰心  | ![img]()9月28日 |
| -------- | ---- | ------------- |
|          |      |               |

有几个问题哈：

\- 有编译器选项吗？

\- 有名字上的规范吗？

\- 普通用户也可以使用吗？

\- 显示引用文字 -

| ![img]() | 苏沛   | ![img]()9月28日 |
| -------- | ---- | ------------- |
|          |      |               |

<[fanbin...@gmail.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/957392f4-7766-4a6c-8d96-10e6a48f8a26/)>

> 有几个问题哈：

<https://en.wikipedia.org/wiki/Linker_(computing)>

 

> \- 有编译器选项吗？

不需要额外的编辑器选项 

 

> \- 有名字上的规范吗？

函数签名和普通函数签名一样，汇编原文件后面加上架构，如  xxx_x86.s 或者 xxx_amd64.s

> \- 普通用户也可以使用吗？

应该是可以的， Go 标准库里面就有这样的用法，他们没有什么特殊地位。
比如把math 里面的 abs 扒出来

main.go

package main
import (
​        "fmt"
)
func Abs(float64) float64
func main() {
​        fmt.Printf("abs(%f) = %f\n", 1.2, Abs(1.2))
​        fmt.Printf("abs(%f) = %f\n", -1.2, Abs(1.2))
}
abs_amd64.s
\#include "textflag.h"
// func Abs(x float64) float64
TEXT ·Abs(SB),NOSPLIT,$0
​    MOVQ   $(1<<63), BX
​    MOVQ   BX, X0 // movsd $(-0.0), x0
​    MOVSD  x+0(FP), X1
​    ANDNPD X1, X0
​    MOVSD  X0, ret+8(FP)
​    RET

> \- 最关键的一点，golang的这些黑魔法在哪里可以找到说明。

文档，我现在也还没找到什么好的文档。
用法：没有记得有什么文档提到这种用法，只在 Go标准库和runtime里面见到。
汇编语法：Go 里面用的是一种基于plan 9 风格汇编的变种：
<https://golang.org/doc/asm> 
<http://plan9.bell-labs.com/sys/doc/asm.html>

> \- 显示引用文字 -
>
> 您收到此邮件是因为您订阅了Google网上论坛上的“
>
> 
>
> Golang-China”群组。
>
> 
>
> 要退订此群组并停止接收此群组的电子邮件，请发送电子邮件到
>
> golang-china+unsubscribe@googlegroups.com
>
> 。
>
> 
>
> 要在网络上查看此讨论，请访问
>
> https://groups.google.com/d/msgid/golang-china/2d26c41c-fd93-408f-b957-bf1ab3738b40%40googlegroups.com
>
> 。
>
> 
>
> 要查看更多选项，请访问
>
> https://groups.google.com/d/optout
>
> 。
>
> 

苏沛

| ![img]() | minux | ![img]()9月29日 |
| -------- | ----- | ------------- |
|          |       |               |

[将帖子翻译为中文]()  

On Sep 28, 2015 11:29 AM, "苏沛" <[sill...@gmail.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/957392f4-7766-4a6c-8d96-10e6a48f8a26/)> wrote:
\> 在 2015年9月28日 下午9:46，樊冰心 <[fanbin...@gmail.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/957392f4-7766-4a6c-8d96-10e6a48f8a26/)>写道：

\- 显示引用文字 -

就是用别的包的未导出符号的话不需要汇编。

用 //go:linkname，参见 godoc cmd/compile。

有两个用法，可以重命名本包的符号指向另一个包的符号也可以在另一个包里直接把符号重命名成这个包里的然后这边再声明一下即可。

当然如果被访问的符号是汇编写的，直接在命名的时候加上包名也可以达到目的。

不建议在自己的代码里用//go:linkname这种底层技巧，需要 import "unsafe".

| ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACIAAAAiCAYAAAA6RwvCAAABr0lEQVR42u2W2UsCURTG529usXqK9oXIggojodwgc0m0cMlKLUejSIsC+x9inNWxp9M9N+xB0hlnnMliHj64XO7Abw7nfN9hVqLvMApixo44GAU5IP8TZCrQhMUoD0sxHqaDTftBXATg8EoGtvEBlbcv4TlQlCmcLSDjPg6Ob5VvgG5FWIW+sRzEfSYC2wOio92MaD1I/K7VFwKVvG9ZD5J+VDVBLp5U60FSD9oVQVjLQXwFWRMkdKNYD4LjmX9u94S4fmnDTKhpj4/MhnnI1NQfe2M+wtvrrJN+DtYTAgRLMtVGUqB3Tuj9KogrwMEC6QV0WW9eAu+lBFvkjHeWZw0G3WZKpFlSbvQeXQy/aFUBN3k7SCIzev5+j2RH8bWt6R/dwm88OYn+hCkQLHO4rAwM0K14taVZnb4gnqxkGqKjA9JLhkH8Ouxcr9BrDIPMnfCQrqmmIXJ1lU6UqWZdjguQrRuHQdtfPRWGM76YHzi2bEM/AG5xMTLGuFwP1UcmyB66fS7SdNWCKJCx3UmLA2UPYyTssNT7xB9wtHEtREUqCr1bSxgLPyf0HJA/A/IJg1kQlSzpLVkAAAAASUVORK5CYII=) | 樊冰心  | ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAYAAACpF6WWAAAAdklEQVR42mNgGAVDHswnwCfLwP94+CSDBqgBCWh8sg1NgGruR+PD8HogLgBiBVINXI/DQHQ8nxjDYYoV0Pj48HsgNqCmS0k2GFeY7ofi+2ji94kNW1js9+OIfQGomvto6qmSTgWgQXae2jlKgJggIDeNjwIaAQBDo0wVdvRN2QAAAABJRU5ErkJggg==)9月29日 |
| ---------------------------------------- | ---- | ---------------------------------------- |
|                                          |      |                                          |

https://golang.org/src/runtime/sema.go

\- 显示引用文字 -

| ![img](https://www.google.com/s2/photos/public/AIbEiAIAAABDCK766Y3nnZjDDSILdmNhcmRfcGhvdG8qKGM0ZDA4YzExYTFjNjJkODRmNTRlZjMwMmVkMGY2NTVjY2Q2YTk2MWQwAXJJiw7mK5ASs--1b4RBODWE7d5v?authuser=0&sz=34) | Yuchen Ying | ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAYAAACpF6WWAAAAdklEQVR42mNgGAVDHswnwCfLwP94+CSDBqgBCWh8sg1NgGruR+PD8HogLgBiBVINXI/DQHQ8nxjDYYoV0Pj48HsgNqCmS0k2GFeY7ofi+2ji94kNW1js9+OIfQGomvto6qmSTgWgQXae2jlKgJggIDeNjwIaAQBDo0wVdvRN2QAAAABJRU5ErkJggg==)9月29日 |
| ---------------------------------------- | ----------- | ---------------------------------------- |
|                                          |             |                                          |

[将帖子翻译为中文]()  

https://blogs.oracle.com/ksplice/entry/hello_from_a_libc_free

\- 显示引用文字 -

https://groups.google.com/d/msgid/golang-china/cd70066a-1e79-482d-893d-3b28831f3119%40googlegroups.com

https://groups.google.com/d/optout

http://about.me/yegle

来源： <<https://groups.google.com/forum/?utm_medium=email&utm_source=footer#!msg/golang-china/Etcw7fZ36lU/o7rlj-lMBwAJ>>

 

