
## 安装

### ubuntu

```sh
aptitude install -y imagemagick

apt list --installed | grep -i imagema
imagemagick/bionic-security,bionic-updates,now 8:6.9.7.4+dfsg-16ubuntu6.9 amd64 [installed]
imagemagick-6-common/bionic-security,bionic-updates,now 8:6.9.7.4+dfsg-16ubuntu6.9 all [installed,automatic]
imagemagick-6.q16/bionic-security,bionic-updates,now 8:6.9.7.4+dfsg-16ubuntu6.9 amd64 [installed,automatic]  # 主体在这个里面

# 相应的bin文件
dpkg-query -L imagemagick-6.q16  | grep bin
/usr/bin
/usr/bin/animate-im6.q16
/usr/bin/compare-im6.q16
/usr/bin/composite-im6.q16
/usr/bin/conjure-im6.q16
/usr/bin/convert-im6.q16
/usr/bin/display-im6.q16
/usr/bin/identify-im6.q16
/usr/bin/import-im6.q16
/usr/bin/mogrify-im6.q16
/usr/bin/montage-im6.q16
/usr/bin/stream-im6.q16

ls -lh `ls /usr/bin/*-im6.q16 | rev | cut -c9- | rev`
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /usr/bin/animate -> /etc/alternatives/animate
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /usr/bin/compare -> /etc/alternatives/compare
lrwxrwxrwx 1 root root 27 Feb 13 10:38 /usr/bin/composite -> /etc/alternatives/composite
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /usr/bin/conjure -> /etc/alternatives/conjure
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /usr/bin/convert -> /etc/alternatives/convert
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /usr/bin/display -> /etc/alternatives/display
lrwxrwxrwx 1 root root 26 Feb 13 10:38 /usr/bin/identify -> /etc/alternatives/identify
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /usr/bin/import -> /etc/alternatives/import
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /usr/bin/mogrify -> /etc/alternatives/mogrify
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /usr/bin/montage -> /etc/alternatives/montage
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /usr/bin/stream -> /etc/alternatives/stream

ls -lh `ls /usr/bin/*-im6.q16 | rev | cut -c9- | rev` | awk '{print $NF}' | xargs ls -l
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /etc/alternatives/animate -> /usr/bin/animate-im6.q16
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /etc/alternatives/compare -> /usr/bin/compare-im6.q16
lrwxrwxrwx 1 root root 26 Feb 13 10:38 /etc/alternatives/composite -> /usr/bin/composite-im6.q16
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /etc/alternatives/conjure -> /usr/bin/conjure-im6.q16
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /etc/alternatives/convert -> /usr/bin/convert-im6.q16
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /etc/alternatives/display -> /usr/bin/display-im6.q16
lrwxrwxrwx 1 root root 25 Feb 13 10:38 /etc/alternatives/identify -> /usr/bin/identify-im6.q16
lrwxrwxrwx 1 root root 23 Feb 13 10:38 /etc/alternatives/import -> /usr/bin/import-im6.q16
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /etc/alternatives/mogrify -> /usr/bin/mogrify-im6.q16
lrwxrwxrwx 1 root root 24 Feb 13 10:38 /etc/alternatives/montage -> /usr/bin/montage-im6.q16
lrwxrwxrwx 1 root root 23 Feb 13 10:38 /etc/alternatives/stream -> /usr/bin/stream-im6.q16
```


## 使用

### 转换： convert

#### 格式转换

支持后缀名格式语义，如： `convert xx.png xx.jpg`

#### 质量转换/压缩： -quality

> 可以目标文件与源文件同名

`convert -quality 20 xx.jpg xx.jpg`
