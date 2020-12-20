

------

| if verb == "go:linkname" { |
| -------------------------- |
|                            |

| if imported_unsafe == 0 { |
| ------------------------- |
|                           |

| Yyerror("//go:linkname only allowed in Go files that import \"unsafe\"") |
| ---------------------------------------- |
|                                          |

| }    |
| ---- |
|      |

| f := strings.Fields(cmd) |
| ------------------------ |
|                          |

| if len(f) != 3 { |
| ---------------- |
|                  |

| Yyerror("usage: //go:linkname localname linkname") |
| ---------------------------------------- |
|                                          |

| return c |
| -------- |
|          |

| }    |
| ---- |
|      |

|      |
| ---- |
|      |

| Lookup(f[1]).Linkname = f[2] |
| ---------------------------- |
|                              |

| return c |
| -------- |
|          |

来源： <<https://github.com/golang/go/blob/88c08b06b97296e41fc3069f4afbc86d24707b05/src/cmd/internal/gc/lex.go#L1552>>

 



