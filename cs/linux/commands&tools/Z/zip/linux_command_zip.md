
* 压缩

  * `zip squash.zip file1 file2 file3` 压缩几个文件

  * `zip -r squash.zip dir1` 压缩目录

    默认好像只包含一级文件。 需要`-r`递归加入才是预期行为。

  * `-e` 加密，prompt输入密码
  * `-P <password>`加密，需要显式带上密码

* 解压，详见unzip命令

  * `unzip squash.zip`



在 Linux 中，`zip` 和 `unzip` 是用于创建和解压缩 ZIP 文件的命令行工具。下面是这两个命令的详细用法：

**zip 命令用法：**

复制

```
zip [选项] [输出文件名] [输入文件/目录]
```

常用选项：

- `-r`：递归地压缩目录及其内容。
- `-q`：静默模式，不显示压缩过程中的输出信息。
- `-j`：仅压缩文件，不保留目录结构。
- `-9`：使用最高压缩级别。

示例：

1. 将单个文件压缩为 ZIP 文件：

   复制

   ```
   zip archive.zip file.txt
   ```

2. 递归地压缩目录及其内容：

   复制

   ```
   zip -r archive.zip directory/
   ```

3. 静默模式压缩文件：

   复制

   ```
   zip -q archive.zip file.txt
   ```

**unzip 命令用法：**

复制

```
unzip [选项] [输入文件名] -d [目标目录]
```

常用选项：

- `-l`：列出 ZIP 文件中的内容列表，但不解压缩。
- `-q`：静默模式，不显示解压缩过程中的输出信息。
- `-o`：覆盖已存在的文件。

示例：

1. 解压缩 ZIP 文件到当前目录：

   复制

   ```
   unzip archive.zip
   ```

2. 解压缩 ZIP 文件到指定目录：

   复制

   ```
   unzip archive.zip -d /path/to/directory
   ```

3. 列出 ZIP 文件中的内容列表：

   复制

   ```
   unzip -l archive.zip
   ```

4. 静默模式解压缩 ZIP 文件：

   复制

   ```
   unzip -q archive.zip
   ```

这些命令提供了许多其他选项和功能，可以通过命令行中的 `zip --help` 和 `unzip --help` 命令查看完整的帮助文档。