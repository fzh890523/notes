



```go
switch expression {
    case exp1: statement1
    case exp2: statement2
    case exp3: statement3
    ...
    default: statement4
}
```



```go
    switch v := 3; v {
        case 1:
        break
        case 2:
        fallthrough
        case 3:
        fmt.Println("3")
    }

    switch c {
        case 2:
        fmt.Println("2")
        fallthrough
        case 4:
        fmt.Println("4")
        case 8:
        fmt.Println("8")
    }

    switch v {
        case 1:
        fmt.Println("One")
        case 2:
        fmt.Println("Two")
        break
        // fmt.Println("Won't work") // will show the error "unreachable code"
        default:
        fmt.Println("The value is wrong")
             
    }

    // has an expression after switch
    switch v {
        case "Rob":              // just check the value
        fmt.Println("Hi, Rob!")
        default:
        fmt.Println("Hi, user!")
    }
     
    // doesn't have an expression after switch
    switch {  
        case v == "Rob":         // check the case here
        fmt.Println("Hi, Rob!")
        default:
        fmt.Println("Hi, user!")
    }

    // no expression means it is true
    switch {
        case v < 5:
        fmt.Println("Less than 5")
        case v < 10:
        fmt.Println("Less than 10")
        case v < 15:
        fmt.Println("Less than 15")
        default:
        fmt.Println("Too large")
    }
```



```go
    switch i.(type) {
        case string:
        return "A string value"
        case int:
        return "A number"
        default:
        return "Other"
    }


```





