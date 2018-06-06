# **问题**

## flag冲突问题

> If more than one package calls flag.Parse without any concerns about other packages defining other flags then you're in trouble (as you've experienced already). The state of the "flag" package is a global state, so it's more or less the same as if different packages would be competing to set a value of a global variable during init to different values. That may not end well, obviously.
>
> The easy way to prevent this: flag.Parse should be called only once (in the first approximation). That's why it is normally seen in package "main" only. If your non-main package calls flag.Parse then it typically will conflict with any flag.Parse called in package "main". Note that go test synthesizes a package main in order to test a package and flag.Parse is called from that synthesized "main" package.
>
> On the other hand, it is more "safe" (but conflicts are possible anyway) to only define flags in a non-main package and rely on flag.Parse would be called in package "main". In the non-main package, one can then verify that flag.Parse has been invoked by using flag.Parsed().
>
> Written above is simplified. For additional options check the package flag documentation. More "power" could be acquired in some scenarios by, for example, using flag.Flagset, i.e. by using local state for flag options within a package.
>
> However, I personally prefer not to use package "flag" outside of package "main" in any way and rather to set up any configurable package behavior through its API. Exceptions do exist, though, e.g. in *_test files or other special cases.
>
> Thanks. I actually fixed the code with trial and error by importing gocheck anonymously into my settings file,
>  and now all the flags work together. After reading your answer and 
> looking at the gocheck source I understand why that worked - gocheck 
> never calls flag.Parse(), so I just have to make sure gocheck gets 
> imported before my settings.init() method is called and all the flags 
> will work together. Good advice though about only parsing flags in main,
>  I'll keep that in mind for the future. 

**不允许未知参数**

> 

> Parse的时候不允许有未define参数 --- 简直匪夷所思
>
> 所以也不可能分阶段多次Parse了，没意义。

