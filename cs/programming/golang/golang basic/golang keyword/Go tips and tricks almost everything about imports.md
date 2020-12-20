

# Go tips and tricks: almost everything about imports

Importing packages is an integral part of most programming languages today, and Go is no exception. Importing packages in Go is, for the most part, a simple process. For most people, the basics of importing is enough, however there are some pitfalls that can show up and it’s always good to be aware of them.



Lets quickly go over the basics. Say you want to output something to the terminal. The `fmt`package takes care of this. So lets go ahead and import it.

```golang
import "fmt"

func main() {
    fmt.Println("Go is great!")
}
```

When importing the `fmt` package, you can use the structs and functions that this package exports. All you need to do is prepend your function call with `fmt.`, like `fmt.Println` above, and you can use it in your programs.

When importing multiple packages, its most common to wrap them with `import ( )` rather than having to type out `import` everytime.

```golang
import (
    "fmt"
    "bytes"
)
```

This is purely stylistic, and doesn’t affect the code otherwise.

## Import aliases

A very useful feature of Go’s import statement are aliases. A common use case for import aliases is to provide a shorter alternative to a library’s package name.

In this example, we save ourselves having to type `discordgo` everytime we want to call one of the library’s functions, we just use `dg` instead.

```golang
import dg "github.com/bwmarrin/discordgo"

func main() {
    err := dg.New()
}
```

We can leverage this to bypass package name conflicts. If we have two packages with the same package name, we can simply give one an alias. For the next example, lets assume that we want to use two packages with the name `rand`.

```golang
import (
    "math/rand"
    crand "crypto/rand"
)
```

We can now reference the second import’s functions and variables by using `crand`, allowing us to reference the first import’s functions and variables by using its original package name, `rand`.

## Package names vs. package imports

It has been a common practice in the past to name go package repositories either with `go-`prefix (like [go-bindata](https://github.com/jteeuwen/go-bindata) or [go-iter](https://github.com/hgfischer/go-iter),…). If you’re using [gopkg.in](https://labix.org/gopkg.in) you might also reference some packages with versions:

```
gopkg.in/pkg.v3      → github.com/go-pkg/pkg (branch/tag v3, v3.N, or v3.N.M)
gopkg.in/user/pkg.v3 → github.com/user/pkg   (branch/tag v3, v3.N, or v3.N.M)
```

For example: You wish to use [json-iterator/go](https://github.com/json-iterator/go) in your project, you will use it like this:

```golang
import "github.com/json-iterator/go"

var json = jsoniter.ConfigCompatibleWithStandardLibrary
json.Marshal(&data)
```

“But wait a minute!” you say. “It’s called `json-iterator/go`, so shouldn’t its functions and variables be referenced with `go.` instead of `jsoniter`? That’s how it works for packages like `fmt` and `encoding/json` in the standard library, right?”

Yes, people tend to write packages so they match their URL. But the URL doesn’t define how you use the package. If you look at the source code for `json-iterator/go`, you’ll see that each file has `package jsoniter` at the very top. This is what defines how one should reference its variables and functions. The import paths for the standard library packages just happen to be the same as how you use them, that’s how they’re defined in the source code, so it’s easy to think that the trend holds true for all imports.

Another example of this is with the [Youtube](https://scene-si.org/2018/01/25/go-tips-and-tricks-almost-everything-about-imports/google.golang.org/api/youtube/v3) package from Google. The import path for it is `google.golang.org/api/youtube/v3`, so you might initially think that you prepend `v3` to function calls and variable references to use the package, however once again you’ll see in the source code that the package name is defined as `youtube`. Therefore, your code will look like this:

```golang
import "google.golang.org/api/youtube/v3"

youtubeService, err := youtube.New(oauthHttpClient)
```

## Dot imports

Dot imports are a lesser known and therefore rarely used import method. What it does is it imports the package into the same namespace as the current package, so you no longer have to use the imported package’s name to reference its variables and functions, giving us direct access to them. To dot import a package, you simply give it a full-stop as it’s alias. The example below compares a dot import to a standard import.

```golang
import (
    . "math"
    "fmt"
)

fmt.Println(Pi)
import (
    "math"
    "fmt"
)

fmt.Println(math.Pi)
```

As you can see, when we dot imported `math`, we didn’t have to prepend `Pi` with `math.`

One thing to look out for when using dot imports is that you cannot have functions or variables that are also defined in the dot imported package. For example, if we dot import `fmt` and we define a function called `Println` in our package, the compiler will give out to us for having duplicate function names.

This import style is commonly used in testing. The dot import will import only the public structs and functions, without exposing any private details of the package. This is good for testing because you can be sure that the public interface to your package is fully functional. If you share the namespace of your package with your tests, you may overlook that some property or function isn’t public, and wouldn’t be available to the users of your package.

## Relative imports

If you’ve ever done any filesystem work on the command line, you’ll most likely have come across the term ‘relative path’ before. Naturally, a relative import is when you specify the import location relative to the current package. In the example below, if we wanted to relatively import `greeting` in helloworld.go, we would type `import "./greeting"`.

```
$GOPATH 
├──bin  
│   └──hello  
├──pkg  
└──src  
    └──someFolder  
        ├──helloworld.go  
        └──greeting  
            └──greeting.go 
```

Unfortunately, this doesn’t work and will give you the following error: `local import "./greeting" in non-local package`. This is because relative imports aren’t allowed in your workspace (most commonly `$GOPATH/src`). They are however allowed anywhere outside of this.

This was a deliberate design decision by the developers of Go, with the intended use of relative imports being for quick testing and experimentation outside of the normal workspace. There has been a lot of discussion surrounding this, with advantages such as not having to change all the imports when forking a project being brought up. However, it looks like this behavior is staying for another while.

## Blank imports

If you’ve ever been annoyed at Go complaining about an unused import, then you’ve most likely come across blank imports. Blank imports are commonly used while coding to stop Go complaining about an import that you’re not using, but might use later, that you don’t want to keep around for when you do need it. However, there is another use for them. If you’ve ever worked with images or databases in Go, you may have seen one of the following being used:

```golang
import (
    "database/sql"
    _ "github.com/go-sql-driver/mysql"
)
import (  
    "image"
    _ "image/gif"
    _ "image/png"
    _ "image/jpeg"
)
```

You probably wondered why they’re being imported that way. Doesn’t that just ignore them? What actually happens when you import them like this is that, instead of being ignored, their `init()` function, if any, is run.

For example, the `init()` function of `image/png` looks like this:

```golang
func init() {  
        image.RegisterFormat("png", pngHeader, Decode, DecodeConfig)
}
```

Here, the package registers the png format with the `image` package, basically giving it instructions on what to do when it encounters a png image.

The same happens in [`github.com/go-sql-driver/mysql`](https://scene-si.org/2018/01/25/go-tips-and-tricks-almost-everything-about-imports/github.com/go-sql-driver/mysql):

```golang
func init() {
	sql.Register("mysql", &MySQLDriver{})
}
```

The package gives `database/sql` the necessary data with which it can work with a MySQL database.

## Conclusion

A lot of the languages I know about have some form of an import statement. There’s PHP’s `include`, there’s Node.js/ES6 `import` and `require`, there’s Java with their own `import` and even C/C++ with `#include`, and the list goes on. Each of them behaves very differently. As you can see above, Go is significantly different to other languages and provides some additional functionality behind a seemingly simple statement.

## While I have you here...

It would be great if you buy one of my books:

- [API Foundations in Go](https://leanpub.com/api-foundations)
- [12 Factor Apps with Docker and Go](https://leanpub.com/12fa-docker-golang)

I promise you'll learn a lot more if you buy one. Buying a copy supports me writing more about similar topics. Say thank you and buy my books.



Feel free to [send me an email](mailto:black@scene-si.org) if you want to book my time for consultancy/freelance services. I'm great at APIs, Go, Docker, VueJS and scaling services, [among many other things](https://scene-si.org/about).

