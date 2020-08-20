
跟linux主要差别在于ps = =

```sh
ps -a -x -o rss,cputime,comm | awk '{
    mem=$1; cpu_str=$2; $1=$2=""; cmd=$0; 
    mems[cmd]+=mem; count[cmd]+=1; 
    n=split(cpu_str, a, "[:.]"); cpu=a[0]*3600+a[1]*60+a[2]; 
    cpus[cmd]+=cpu;
}END{
    for(k in count){
        printf("%d %d %d %s\n", count[k], mems[k], cpus[k], k);
    }
}' | sort -n -k 3
```

```s
...
10 716284 650   /Applications/Visual Studio Code.app/Contents/Frameworks/Code Helper (Renderer).app/Contents/MacOS/Code Helper (Renderer)
1 742128 178   /Applications/GoLand.app/Contents/MacOS/goland
1 1077684 926   /Applications/Parallels Desktop.app/Contents/MacOS//Parallels VM.app/Contents/MacOS/prl_vm_app
11 1930548 2345   /Applications/Firefox.app/Contents/MacOS/plugin-container.app/Contents/MacOS/plugin-container
45 2992392 2238   /Applications/Google Chrome.app/Contents/Frameworks/Google Chrome Framework.framework/Versions/80.0.3987.122/Helpers/Google Chrome Helper (Renderer).app/Contents/MacOS/Google Chrome Helper (Renderer)
``

