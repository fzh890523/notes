# [代码片段 - Golang 创建 .tar.gz 压缩包](http://www.cnblogs.com/golove/p/3454630.html)

Golang创建 .tar.gz 压缩包

 

```
　　tar 包实现了文件的打包功能，可以将多个文件或目录存储到单一的 .tar 文件中，tar 本身不具有压缩功能，只能打包文件或目录：

import "archive/tar"

　　这里以打包单个文件为例进行解说，后面会给出打包整个目录的详细示例。

　　向 tar 文件中写入数据是通过 tar.Writer 完成的，所以首先要创建 tar.Writer，可以通过 tar.NewWriter 方法来创建它，该方法要求提供一个 os.Writer 对象，以便将打包后的数据写入该对象中。可以先创建一个文件，然后将该文件提供给 tar.NewWriter 使用。这样就可以将打包后的数据写入文件中：

// 创建空文件 fw 用于保存打包后的数据// dstTar 是要创建的 .tar 文件的完整路径
fw, err := os.Create(dstTar)
if err != nil {
	return err
}
defer fw.Close()

// 通过 fw 创建 tar.Writer 对象
tw := tar.NewWriter(fw)
defer tw.Close()

　　此时，我们就拥有了一个 tar.Writer 对象 tw，可以用它来打包文件了。这里要注意一点，使用完 tw 后，一定要执行 tw.Close() 操作，因为 tar.Writer 使用了缓存，tw.Close() 会将缓存中的数据写入到文件中，同时 tw.Close() 还会向 .tar 文件的最后写入结束信息，如果不关闭 tw 而直接退出程序，那么将导致 .tar 文件不完整。

　　存储在 .tar 文件中的每个文件都由两部分组成：文件信息和文件内容，所以向 .tar 文件中写入每个文件都要分两步：第一步写入文件信息，第二步写入文件数据。对于目录来说，由于没有内容可写，所以只需要写入目录信息即可。

　　文件信息由 tar.Header 结构体定义：

type Header struct {
   Name       string    // 文件名称
   Mode       int64     // 文件的权限和模式位
   Uid        int       // 文件所有者的用户 ID
   Gid        int       // 文件所有者的组 ID
   Size       int64     // 文件的字节长度
   ModTime    time.Time // 文件的修改时间
   Typeflag   byte      // 文件的类型
   Linkname   string    // 链接文件的目标名称
   Uname      string    // 文件所有者的用户名
   Gname      string    // 文件所有者的组名
   Devmajor   int64     // 字符设备或块设备的主设备号
   Devminor   int64     // 字符设备或块设备的次设备号
   AccessTime time.Time // 文件的访问时间
   ChangeTime time.Time // 文件的状态更改时间
}

　　我们首先将被打包文件的信息填入 tar.Header 结构体中，然后再将结构体写入 .tar 文件中。这样就完成了第一步（写入文件信息）操作。

　　在 tar 包中有一个很方便的函数 tar.FileInfoHeader，它可以直接通过 os.FileInfo 创建 tar.Header，并自动填写 tar.Header 中的大部分信息，当然，还有一些信息无法从 os.FileInfo 中获取，所以需要你自己去补充：

// 获取文件信息// srcFile 是要打包的文件的完整路径
fi, err := os.Stat(srcFile)
if err != nil {
	return err
}

// 根据 os.FileInfo 创建 tar.Header 结构体
hdr, err := tar.FileInfoHeader(fi, "")
if err != nil {
	return err
}

　　这里的 hdr 就是文件信息结构体，已经填写完毕。如果你要填写的更详细，你可以自己将 hdr 补充完整。

　　下面通过 tw.WriteHeader 方法将 hdr 写入 .tar 文件中（tw 是我们刚才创建的 tar.Writer）：

// 将 tar.Header 写入 .tar 文件中
err = tw.WriteHeader(hdr)
if err != nil {
	return err
}

　　至此，第一步（写入文件信息）操作完毕，下面开始第二步（写入文件数据）操作，写入数据很简单，通过 tw.Write 方法写入数据即可：

// 打开要打包的文件准备读取
fr, err := os.Open(srcFile)
if err != nil {
	return err
}
defer fr.Close()

// 将文件数据写入 .tar 文件中，这里通过 io.Copy 函数实现数据的写入
_, err = io.Copy(tw, fr)
if err != nil {
	return err
}

　　下面说说解包的方法，从 .tar 文件中读出数据是通过 tar.Reader 完成的，所以首先要创建 tar.Reader，可以通过 tar.NewReader 方法来创建它，该方法要求提供一个 os.Reader 对象，以便从该对象中读出数据。可以先打开一个 .tar 文件，然后将该文件提供给 tar.NewReader 使用。这样就可以将 .tar 文件中的数据读出来了：

// 打开要解包的文件，srcTar 是要解包的 .tar 文件的路径
fr, er := os.Open(srcTar)
if er != nil {
	return er
}
defer fr.Close()

// 创建 tar.Reader，准备执行解包操作
tr := tar.NewReader(fr)

　　此时，我们就拥有了一个 tar.Reader 对象 tr，可以用 tr.Next() 来遍历包中的文件，然后将文件的数据保存到磁盘中：

// 遍历包中的文件for hdr, er := tr.Next(); er != io.EOF; hdr, er = tr.Next() {
	if er != nil {
		return er
	}

	// 获取文件信息
	fi := hdr.FileInfo()

	// 创建空文件，准备写入解压后的数据
	fw, _ := os.Create(dstFullPath)
	if er != nil {
		return er
	}
	defer fw.Close()

	// 写入解压后的数据
	_, er = io.Copy(fw, tr)
	if er != nil {
		return er
	}
	// 设置文件权限
	os.Chmod(dstFullPath, fi.Mode().Perm())
}

　　至此，单个文件的打包和解包都实现了。要打包和解包整个目录，可以通过递归的方法实现，下面给出完整的代码：

============================================================
package main

import (
	"archive/tar""errors""fmt""io""io/ioutil""os""path"
)

func main() {
	TarFile := "test.tar"
	src := "test"
	dstDir := "test_ext"if err := Tar(src, TarFile, false); err != nil {
		fmt.Println(err)
	}

	if err := UnTar(TarFile, dstDir); err != nil {
		fmt.Println(err)
	}
}

// 将文件或目录打包成 .tar 文件// src 是要打包的文件或目录的路径// dstTar 是要生成的 .tar 文件的路径// failIfExist 标记如果 dstTar 文件存在，是否放弃打包，如果否，则会覆盖已存在的文件func Tar(src string, dstTar string, failIfExist bool) (err error) {
	// 清理路径字符串
	src = path.Clean(src)

	// 判断要打包的文件或目录是否存在if !Exists(src) {
		return errors.New("要打包的文件或目录不存在：" + src)
	}

	// 判断目标文件是否存在if FileExists(dstTar) {
		if failIfExist { // 不覆盖已存在的文件return errors.New("目标文件已经存在：" + dstTar)
		} else { // 覆盖已存在的文件if er := os.Remove(dstTar); er != nil {
				return er
			}
		}
	}

	// 创建空的目标文件
	fw, er := os.Create(dstTar)
	if er != nil {
		return er
	}
	defer fw.Close()

	// 创建 tar.Writer，执行打包操作
	tw := tar.NewWriter(fw)
	defer func() {
		// 这里要判断 tw 是否关闭成功，如果关闭失败，则 .tar 文件可能不完整if er := tw.Close(); er != nil {
			err = er
		}
	}()

	// 获取文件或目录信息
	fi, er := os.Stat(src)
	if er != nil {
		return er
	}

	// 获取要打包的文件或目录的所在位置和名称
	srcBase, srcRelative := path.Split(path.Clean(src))

	// 开始打包if fi.IsDir() {
		tarDir(srcBase, srcRelative, tw, fi)
	} else {
		tarFile(srcBase, srcRelative, tw, fi)
	}

	return nil
}

// 因为要执行遍历操作，所以要单独创建一个函数func tarDir(srcBase, srcRelative string, tw *tar.Writer, fi os.FileInfo) (err error) {
	// 获取完整路径
	srcFull := srcBase + srcRelative

	// 在结尾添加 "/"
	last := len(srcRelative) - 1if srcRelative[last] != os.PathSeparator {
		srcRelative += string(os.PathSeparator)
	}

	// 获取 srcFull 下的文件或子目录列表
	fis, er := ioutil.ReadDir(srcFull)
	if er != nil {
		return er
	}

	// 开始遍历for _, fi := range fis {
		if fi.IsDir() {
			tarDir(srcBase, srcRelative+fi.Name(), tw, fi)
		} else {
			tarFile(srcBase, srcRelative+fi.Name(), tw, fi)
		}
	}

	// 写入目录信息if len(srcRelative) > 0 {
		hdr, er := tar.FileInfoHeader(fi, "")
		if er != nil {
			return er
		}
		hdr.Name = srcRelative

		if er = tw.WriteHeader(hdr); er != nil {
			return er
		}
	}

	return nil
}

// 因为要在 defer 中关闭文件，所以要单独创建一个函数func tarFile(srcBase, srcRelative string, tw *tar.Writer, fi os.FileInfo) (err error) {
	// 获取完整路径
	srcFull := srcBase + srcRelative

	// 写入文件信息
	hdr, er := tar.FileInfoHeader(fi, "")
	if er != nil {
		return er
	}
	hdr.Name = srcRelative

	if er = tw.WriteHeader(hdr); er != nil {
		return er
	}

	// 打开要打包的文件，准备读取
	fr, er := os.Open(srcFull)
	if er != nil {
		return er
	}
	defer fr.Close()

	// 将文件数据写入 tw 中if _, er = io.Copy(tw, fr); er != nil {
		return er
	}
	return nil
}

func UnTar(srcTar string, dstDir string) (err error) {
	// 清理路径字符串
	dstDir = path.Clean(dstDir) + string(os.PathSeparator)

	// 打开要解包的文件
	fr, er := os.Open(srcTar)
	if er != nil {
		return er
	}
	defer fr.Close()

	// 创建 tar.Reader，准备执行解包操作
	tr := tar.NewReader(fr)

	// 遍历包中的文件for hdr, er := tr.Next(); er != io.EOF; hdr, er = tr.Next() {
		if er != nil {
			return er
		}

		// 获取文件信息
		fi := hdr.FileInfo()

		// 获取绝对路径
		dstFullPath := dstDir + hdr.Name

		if hdr.Typeflag == tar.TypeDir {
			// 创建目录
			os.MkdirAll(dstFullPath, fi.Mode().Perm())
			// 设置目录权限
			os.Chmod(dstFullPath, fi.Mode().Perm())
		} else {
			// 创建文件所在的目录
			os.MkdirAll(path.Dir(dstFullPath), os.ModePerm)
			// 将 tr 中的数据写入文件中if er := unTarFile(dstFullPath, tr); er != nil {
				return er
			}
			// 设置文件权限
			os.Chmod(dstFullPath, fi.Mode().Perm())
		}
	}
	return nil
}

// 因为要在 defer 中关闭文件，所以要单独创建一个函数func unTarFile(dstFile string, tr *tar.Reader) error {
	// 创建空文件，准备写入解包后的数据
	fw, er := os.Create(dstFile)
	if er != nil {
		return er
	}
	defer fw.Close()

	// 写入解包后的数据
	_, er = io.Copy(fw, tr)
	if er != nil {
		return er
	}

	return nil
}

// 判断档案是否存在func Exists(name string) bool {
	_, err := os.Stat(name)
	return err == nil || os.IsExist(err)
}

// 判断文件是否存在func FileExists(filename string) bool {
	fi, err := os.Stat(filename)
	return (err == nil || os.IsExist(err)) && !fi.IsDir()
}

// 判断目录是否存在func DirExists(dirname string) bool {
	fi, err := os.Stat(dirname)
	return (err == nil || os.IsExist(err)) && fi.IsDir()
}
============================================================

　　如果要创建 .tar.gz 也很简单，只需要在创建 tar.Writer 或 tar.Reader 之前创建一个 gzip.Writer 或 gzip.Reader 就可以了，gzip.Writer 负责将 tar.Writer 中的数据压缩后写入文件，gzip.Reader 负责将文件中的数据解压后传递给 tar.Reader。要修改的部分如下：

============================================================
package main

import (
	// ..."compress/gzip" // 这里导入 compress/gzip 包// ...
)

func Tar(src string, dstTar string, failIfExist bool) (err error) {
	// ...
	fw, er := os.Create(dstTar)
	// ...
	gw := gzip.NewWriter(fw) // 这里添加一个 gzip.Writer// ...
	tw := tar.NewWriter(gw) // 这里传入 gw// ...
}

func UnTar(srcTar string, dstDir string) (err error) {
	// ...
	fr, er := os.Open(srcTar)
	// ...
	gr, er := gzip.NewReader(fr) // 这里添加一个 gzip.Reader// ...
	tr := tar.NewReader(gr) // 这里传入 gr// ...
}
============================================================

　　有个问题，用 golang 创建的 .tar 或 .tar.gz 文件无法在 Ubuntu 下用“归档管理器”修改，只能读取和解压，不知道为什么。



```

标签: [代码片段](http://www.cnblogs.com/golove/tag/%E4%BB%A3%E7%A0%81%E7%89%87%E6%AE%B5/)

来源： <<http://www.cnblogs.com/golove/p/3454630.html>>