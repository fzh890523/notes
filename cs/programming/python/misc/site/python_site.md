

# 问题



## site目录不一致问题



* python路径： `/usr/bin/python`

* site：

  ```shell
  python -m site                                                                                           master
  sys.path = [
      '/Users/${USER}/git_root/external/go/kubernetes_kubernetes/staging/src',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python27.zip',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-darwin',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac/lib-scriptpackages',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-tk',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-old',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-dynload',
      '/Users/${USER}/Library/Python/2.7/lib/python/site-packages',
      '/Users/${USER}/Library/Python/2.7/lib/python/site-packages/oss-0.4.6-py2.7.egg',
      '/Library/Python/2.7/site-packages',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python',
      '/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/PyObjC',
  ]
  USER_BASE: '/Users/${USER}/Library/Python/2.7' (exists)
  USER_SITE: '/Users/${USER}/Library/Python/2.7/lib/python/site-packages' (exists)
  ENABLE_USER_SITE: True

  # 从这里来看，搜到的第一个非用户的site是： /Library/Python/2.7/site-packages
  ```

  ​

  ```shell
  python -m site --user-site                                                                               master
  /Users/${USER}/Library/Python/2.7/lib/python/site-packages
  ```

  ​

  ```shell
  python -c "import site; print(site.getsitepackages())"                                                   master
  ['/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages', '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/site-python', '/Library/Python/2.7/site-packages']
  ```

* pip install dir

  ```shell
  pip2 install redis

  Requirement already satisfied: redis in /usr/local/lib/python2.7/site-packages (2.10.6)
  # why? should be /Library/Python/2.7/site-packages for sys-site and /Users/${USER}/Library/Python/2.7/lib/python/site-packages for user-site
  ```

  ​


### 跟进



```shell
ll `which python`
-rwxr-xr-x  1 root  wheel    65K  1 13  2017 /usr/bin/python

head -n 1 `which ipython`
#!/usr/bin/python

head -n 1 `which ipython2`
#!/usr/bin/python

env | grep PATH
PATH=/Users/${user}/Library/Python/2.7/bin:/Users/${user}/bin:/usr/local/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/go/bin:/Applications/Wireshark.app/Contents/MacOS

ll `which pip`
-rwxr-xr-x  1 ${user}  staff   235B  4 18 06:10 /Users/${user}/Library/Python/2.7/bin/pip

ll `which pip2`
-rwxr-xr-x  1 ${user}  staff   235B  4 18 06:10 /Users/${user}/Library/Python/2.7/bin/pip2

head -n 1 /Users/zhihengfang/Library/Python/2.7/bin/pip
#!/usr/local/opt/python/bin/python2.7

ll /usr/local/opt/python/bin/python2.7
lrwxr-xr-x  1 ${USER}  admin    57B  9 17  2017 /usr/local/opt/python/bin/python2.7 -> ../Frameworks/Python.framework/Versions/2.7/bin/python2.7

pwd
/usr/local/opt/python/bin
ll ../Frameworks/Python.framework/Versions/2.7/bin/python2.7
-rwxr-xr-x  1 ${USER}  admin    13K 12 29 16:37 ../Frameworks/Python.framework/Versions/2.7/bin/python2.7
# /usr/local/opt/python/Frameworks/Python.framework/Versions/2.7/bin/python2.7
```



```shell
/usr/local/opt/python/Frameworks/Python.framework/Versions/2.7/bin/python2.7 -m site
sys.path = [
    '/usr/local/Cellar/python/2.7.14/bin',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python27.zip',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-darwin',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac/lib-scriptpackages',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-tk',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-old',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-dynload',
    '/Users/${user}/Library/Python/2.7/lib/python/site-packages',
    '/Users/${user}/Library/Python/2.7/lib/python/site-packages/oss-0.4.6-py2.7.egg',
    '/usr/local/lib/python2.7/site-packages',
    '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages',
]
USER_BASE: '/Users/${user}/Library/Python/2.7' (exists)
USER_SITE: '/Users/${user}/Library/Python/2.7/lib/python/site-packages' (exists)
ENABLE_USER_SITE: True

# 从这里来看，搜到的第一个非用户的site是： /usr/local/lib/python2.7/site-packages

/usr/local/opt/python/Frameworks/Python.framework/Versions/2.7/bin/python2.7 -c "import site; print(site.getsitepackages())"
['/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages', '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/site-python']

ll /usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages                   master
lrwxr-xr-x  1 zhihengfang  admin    54B 12 29 16:37 /usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages -> ../../../../../../../../../lib/python2.7/site-packages
```



```shell
/usr/bin/python --version
Python 2.7.10

/usr/local/opt/python/Frameworks/Python.framework/Versions/2.7/bin/python2.7  --version
Python 2.7.14
```



### 结论



另一版本





* 装了两个py2，系统默认（2.7.10）的和cellar（2.7.14）的，路径混了

  2.7.10的binary在前； 而2.7.14的pip在前；

  * 默认

    * 版本： 2.7.10

    * binary：  `/usr/bin/python`

    * pip：

    * `get_config_var("PYTHONFRAMEWORK")`： `Python`

    * `site.PREFIXES`

      ```python
      ['/System/Library/Frameworks/Python.framework/Versions/2.7',
       '/System/Library/Frameworks/Python.framework/Versions/2.7']
      ```

    * `site.getsitepackages`

      ```python
      ['/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages', '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/site-python', '/Library/Python/2.7/site-packages']
      ```

    * `-m site`

      ```python
      sys.path = [
          '/Users/${USER}/git_root/external/go/kubernetes_kubernetes/staging/src',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python27.zip',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-darwin',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac/lib-scriptpackages',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-tk',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-old',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-dynload',
          '/Users/${USER}/Library/Python/2.7/lib/python/site-packages',
          '/Users/${USER}/Library/Python/2.7/lib/python/site-packages/oss-0.4.6-py2.7.egg',
          '/Library/Python/2.7/site-packages',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python',
          '/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/PyObjC',
      ]
      USER_BASE: '/Users/${USER}/Library/Python/2.7' (exists)
      USER_SITE: '/Users/${USER}/Library/Python/2.7/lib/python/site-packages' (exists)
      ENABLE_USER_SITE: True
      ```

      ​

      ​

  * cellar

    * 版本： 2.7.14

    * binary： `/usr/local/opt/python/Frameworks/Python.framework/Versions/2.7/bin/python2.7` or `/usr/local/opt/python/bin/python2.7`

    * pip： `/Users/${USER}/Library/Python/2.7/bin/pip`

    * `get_config_var("PYTHONFRAMEWORK")`： `Python`

    * `site.PREFIXES`

      ```python
      ['/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7',
       '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7']
      ```

    * `site.getsitepackages`

      ```python
      ['/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages', '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/site-python']
      ```

    * `-m site`

      ```python
      sys.path = [
          '/usr/local/Cellar/python/2.7.14/bin',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python27.zip',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-darwin',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/plat-mac/lib-scriptpackages',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-tk',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-old',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-dynload',
          '/Users/${user}/Library/Python/2.7/lib/python/site-packages',
          '/Users/${user}/Library/Python/2.7/lib/python/site-packages/oss-0.4.6-py2.7.egg',
          '/usr/local/lib/python2.7/site-packages',
          '/usr/local/Cellar/python/2.7.14/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages',
      ]
      USER_BASE: '/Users/${user}/Library/Python/2.7' (exists)
      USER_SITE: '/Users/${user}/Library/Python/2.7/lib/python/site-packages' (exists)
      ENABLE_USER_SITE: True
      ```

      ​

* 2.7.10的py



### 处理



* 给2.7.10的系统自带python安装pip

  ```shell
  sudo /usr/bin/python ~/Downloads/tmp/get-pip.py
  ```

  > lib安装到了 `/Library/Python/2.7/site-packages/pip/`
  >
  > binary安装到了`/usr/local/bin/pip` - **这个pip比之前2.7.14的更“前面”，所以，ok了，pip和py一致了**

* 可以把2.7.10的python binary放在PATH后面（和pip一致）； 或者把2.7.14的python binary放在PATH前面（和pip一致）







### 后续



#### python解释器/运行时如何确定site-packages



```python
def getsitepackages():
# site.py 看这个实现，ref： https://leemendelowitz.github.io/blog/how-does-python-find-packages.html
```



```python
def getsitepackages():
    """Returns a list containing all global site-packages directories
    (and possibly site-python).

    For each directory present in the global ``PREFIXES``, this function
    will find its `site-packages` subdirectory depending on the system
    environment, and will return a list of full paths.
    """
    sitepackages = []
    seen = set()

    for prefix in PREFIXES:
        if not prefix or prefix in seen:
            continue
        seen.add(prefix)

        if sys.platform in ('os2emx', 'riscos'):
            sitepackages.append(os.path.join(prefix, "Lib", "site-packages"))
        elif os.sep == '/':
            sitepackages.append(os.path.join(prefix, "local/lib",
                                        "python" + sys.version[:3],
                                        "dist-packages"))
            sitepackages.append(os.path.join(prefix, "lib",
                                        "python" + sys.version[:3],
                                        "dist-packages"))
        else:
            sitepackages.append(prefix)
            sitepackages.append(os.path.join(prefix, "lib", "site-packages"))
        if sys.platform == "darwin":
            # for framework builds *only* we add the standard Apple
            # locations.
            from sysconfig import get_config_var
            framework = get_config_var("PYTHONFRAMEWORK")
            if framework:
                sitepackages.append(
                        os.path.join("/Library", framework,
                            sys.version[:3], "site-packages"))
    return sitepackages
```



#### TODO python的sys.path如何确定

看前面…还有不少莫名奇妙的东西。












