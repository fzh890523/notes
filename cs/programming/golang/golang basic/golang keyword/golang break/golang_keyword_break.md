



> [Break statements, The Go Programming Language Specification.](http://golang.org/ref/spec#Break_statements)
>
> A "break" statement terminates execution of the innermost "for", "switch" or "select" statement.
>
> ```golang
> BreakStmt = "break" [ Label ] .
> ```
>
> If there is a label, it must be that of an enclosing "for", "switch" or "select" statement, and that is the one whose execution terminates (§For statements, §Switch statements, §Select statements).
>
> ```golang
> L:
>   for i < n {
>       switch i {
>       case 5:
>           break L
>       }
>   }
> ```



