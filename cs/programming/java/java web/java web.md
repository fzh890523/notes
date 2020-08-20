# 资源与路径



## 路径



容器&app模型下，常见路径格式为：

${webapps_root}/${app_name}/WEB-INF/{xmls,classes,lib}



其中：

* `${webapps_root}/${app_name}`组成了`app_root`，也即对于app来说自己的**root**。
* xmls包括：
  * web.xml - java web规范
  * ${servet_name}-servlet.xml - java web规范
  * applicationContext.xml等 - spring context

```
tree -L 2
.
├── _ehc.html  # 平台做healthcheck
├── index.jsp  # index 
├── META-INF  # meta
│   ├── MANIFEST.MF
│   └── maven
└── WEB-INF  # main
    ├── applicationContext.xml  # spring
    ├── classes  # ...
    ├── dispatcher-servlet.xml  # java web
    ├── lib  # libs
    └── web.xml  # java web
```



### 路径类型

* classpath
  * `class:${path}`
  * `class*:${path}`
* resource path
  * 相对于“app root”，也即`${path}` 实际路径为 `${app_root}/${path}`
  * `this.getServletContext().getRealPath("/")`可以获得`${app_root}`的真实路径



## 资源

* resource
  * classpath下，一般打包到 `${app_root}/WEB-INF/classes`下
* webResource
  * web root下，如 `${app_root}/WEB-INF`，对于app就是`/WEB-INF/${filename}`



Ref: [ClassLoader 详解及用途](http://blog.chinaunix.net/uid-21227800-id-65885.html)

```
文件载入（例如配置文件等）假设在com.rain.A类里想读取文件夹 /com/rain/config 里的文件sys.properties，读取文件可以通过绝对路径或相对路径，绝对路径很简单，在Windows下以盘号开始，在Unix下以"/"开始 
对于相对路径，其相对值是相对于ClassLoader的，因为ClassLoader是一棵树，所以这个相对路径和ClassLoader树上的任何一个ClassLoader相对比较后可以找到文件，那么文件就可以找到，当然，读取文件也使用委托模型 

1. 直接IO 

/**
 * 假设当前位置是 "C:/test"，通过执行如下命令来运行A "java com.rain.A"
 * 1. 在程序里可以使用绝对路径，Windows下的绝对路径以盘号开始，Unix下以"/"开始
 * 2. 也可以使用相对路径，相对路径前面没有"/"
 * 因为我们在 "C:/test" 目录下执行程序，程序入口点是"C:/test"，相对路径就
 * 是 "com/rain/config/sys.properties"
 * （例子中，当前程序的ClassLoader是App ClassLoader，system ClassLoader = 当前的
 * 程序的ClassLoader，入口点是"C:/test"）
 * 对于ClassLoader树，如果文件在jdk lib下，如果文件在jdk lib/ext下，如果文件在环境变量里，
 * 都可以通过相对路径"sys.properties"找到，lib下的文件最先被找到
 */
File f = new File("C:/test/com/rain/config/sys.properties"); // 使用绝对路径
//File f = new File("com/rain/config/sys.properties"); // 使用相对路径
InputStream is = new FileInputStream(f);

如果是配置文件，可以通过java.util.Properties.load(is)将内容读到Properties里，Properties默认认为is的编码是ISO-8859-1，如果配置文件是非英文的，可能出现乱码问题。 
2. 使用ClassLoader 

/**
 * 因为有3种方法得到ClassLoader，对应有如下3种方法读取文件
 * 使用的路径是相对于这个ClassLoader的那个点的相对路径，此处只能使用相对路径
 */
InputStream is = null;
is = this.getClass().getClassLoader().getResourceAsStream(
       "com/rain/config/sys.properties"); //方法1
//is = Thread.currentThread().getContextClassLoader().getResourceAsStream(
       "com/rain/config/sys.properties"); //方法2
//is = ClassLoader.getSystemResourceAsStream("com/rain/config/sys.properties"); //方法3

如果是配置文件，可以通过java.util.Properties.load(is)将内容读到Properties里，这里要注意编码问题。 
3. 使用ResourceBundle 

    ResourceBundle bundle = ResourceBundle.getBoundle("com.rain.config.sys");

这种用法通常用来载入用户的配置文件，关于ResourceBunlde更详细的用法请参考其他文档 
总结：有如下3种途径来载入文件 

    1. 绝对路径 ---> IO
    2. 相对路径 ---> IO
                ---> ClassLoader
    3. 资源文件 ---> ResourceBundle

2.3 如何在web应用里载入资源在web应用里当然也可以使用ClassLoader来载入资源，但更常用的情况是使用ServletContext，如下是web目录结构 
    ContextRoot
       |- JSP、HTML、Image等各种文件
        |- [WEB-INF]
              |- web.xml
              |- [lib] Web用到的JAR文件
                |- [classes] 类文件

用户程序通常在classes目录下，如果想读取classes目录里的文件，可以使用ClassLoader，如果想读取其他的文件，一般使用ServletContext.getResource() 
如果使用ServletContext.getResource(path)方法，路径必须以"/"开始，路径被解释成相对于ContextRoot的路径，此处载入文件的方法和ClassLoader不同，举例"/WEB-INF/web.xml","/download/WebExAgent.rar"
```

