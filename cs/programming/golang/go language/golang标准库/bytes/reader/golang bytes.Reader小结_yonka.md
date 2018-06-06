

**简介**

> 顾名思义，实现了Reader接口，同时是针对bytes。 也即source不是net stream不是file，而是既有的bytes - []byte。

**源码简介**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/71f26e22-4077-48ec-a124-2e8fd3d009bd/index_files/96f596b3-1ab4-4f26-ba75-997a16666303.png)
>
> ```Go
> // A Reader implements the io.Reader, io.ReaderAt, io.WriterTo, io.Seeker,
> // io.ByteScanner, and io.RuneScanner interfaces by reading from
> // a byte slice.
> // Unlike a Buffer, a Reader is read-only and supports seeking.
> type Reader struct {
>    s        []byte
>    i        int64 // current reading index
>    prevRune int   // index of previous rune; or < 0
> }
> ```

> > prevRune int

> > > 读取的前一个rune（int32）的位置（index）
> > >
> > > <0表示“无”，也即还没有开始read

> > i int64

> > > 已经读到的位置，>=i处的为未读，<i为已读

> > s []byte

> > > 底层数据，也即 bytes...

> > Len() int
> >
> > 未读长度，i以后的
> >
> > Size() int64

> > > 底层[]byte的总长度

> > Read(b []byte) (n int, err error)

> > > 几点情况
>
> > > 1 len(b) == 0 -- reutrn 0, nil
>
> > > 2 r.i >= int64(len(r.s)) --- return0, io.EOF
>
> > > 用内置copy方法...，返回值加到r.i
> > >
> > > 注意： 会把prevRune置为-1，因为既然按byte读了，那prevRune就不准确了...

> > ReadAt(b []byte, off int64) (n int, err error)

> > > 几点情况
>
> > > 1 off < 0 --- return 0, errors.New(...)
>
> > > 2 off >= int64(len(r.s)) --- return 0, io,.EOF
>
> > > 区别：
>
> > > 1 不考虑 i，也即把已读部分也包含在内
>
> > > 2 读取的byte数不加到i，也即不影响状态

> > ReadByte(b byte, err error)

> > > 同样，prevRune置为-1
>
> > > 几点情况
>
> > > 1 r.i >= int64(len(r.s)) --- return 0, io.EOF
>
> > > 返回  r.s[r.i]； i++

> > UnreadByte() error

> > > 也是byte操作，prevRune置为-1
>
> > > 几点情况
>
> > > 1 r.i <= 0 --- return erros.New(...)
>
> > > r.i --

> > ReadRune() (ch rune, size int, err error)

> > > 几点情况
>
> > > 1 r.i >= int64(len(r.s)) --- return 0, 0, io.EOF （同时还要把r.prevRune = -1，不理解...，既然没改变...）
> > >
> > > i赋给prevRune
> > >
> > > 开始读取，先看第一个byte是否ascii，是的话则OK，i++然后把这个byte作为rune返回，size为1； 如果不是的话，则需要解析出下一个utf8字符，通过utf8.DecodeRune(r.s[r.i:])，返回rune和size，把size加到i上...

> > UnreadRune() error

> > > 如果prevRune<0则说明前一次不是合法readrune操作，返回错误
> >
> > > 把prevRune赋给i
> >
> > > prevRune = -1

> > Seek(offset int64, whence int) (int64, error)

> > > 按byte移动，所以prevRune=  -1
> >
> > > 三种seek模式：
> >
> > > 0 绝对位置，不考虑i --- offset
> >
> > > 1 从未读开始，考虑i --- int64(r.i) + offset
> >
> > > 2 （好像是）从末尾往前的负offset（绝对位置） --- int64(len(r.s)) + offset
> >
> > > 结果如果<0则panic
> >
> > > 赋给i
> >
> > > 返回值和nil

> > WriteTo(w io.Writer) (n int64, err error)

> > > r.prevRune = -1  // 因为也是byte read
> > >
> > > r.i >= int64(len(r.s)) --- return 0, nil
> > >
> > > 把 b:= r.s[r.i:]写到writer， w.Write(...)，得到写入数量m和error，如果m>len(b)则panic
> > >
> > > 把m加到i
> > >
> > > ...

> 构造器

> > ```Go
> > // NewReader returns a new Reader reading from b.
> > func NewReader(b []byte) *Reader { return &Reader{b, 0, -1} }
> > ```

**源码**

> ```Go
> // Copyright 2012 The Go Authors. All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> package bytes
>
> import (
>    "errors"
>    "io"
>    "unicode/utf8"
> )
>
> // A Reader implements the io.Reader, io.ReaderAt, io.WriterTo, io.Seeker,
> // io.ByteScanner, and io.RuneScanner interfaces by reading from
> // a byte slice.
> // Unlike a Buffer, a Reader is read-only and supports seeking.
> type Reader struct {
>    s        []byte
>    i        int64 // current reading index
>    prevRune int   // index of previous rune; or < 0
> }
>
> // Len returns the number of bytes of the unread portion of the
> // slice.
> func (r *Reader) Len() int {
>    if r.i >= int64(len(r.s)) {
>       return 0
>    }
>    return int(int64(len(r.s)) - r.i)
> }
>
> // Size returns the original length of the underlying byte slice.
> // Size is the number of bytes available for reading via ReadAt.
> // The returned value is always the same and is not affected by calls
> // to any other method.
> func (r *Reader) Size() int64 { return int64(len(r.s)) }
>
> func (r *Reader) Read(b []byte) (n int, err error) {
>    if len(b) == 0 {
>       return 0, nil
>    }
>    if r.i >= int64(len(r.s)) {
>       return 0, io.EOF
>    }
>    r.prevRune = -1
>    n = copy(b, r.s[r.i:])
>    r.i += int64(n)
>    return
> }
>
> func (r *Reader) ReadAt(b []byte, off int64) (n int, err error) {
>    // cannot modify state - see io.ReaderAt
>    if off < 0 {
>       return 0, errors.New("bytes.Reader.ReadAt: negative offset")
>    }
>    if off >= int64(len(r.s)) {
>       return 0, io.EOF
>    }
>    n = copy(b, r.s[off:])
>    if n < len(b) {
>       err = io.EOF
>    }
>    return
> }
>
> func (r *Reader) ReadByte() (b byte, err error) {
>    r.prevRune = -1
>    if r.i >= int64(len(r.s)) {
>       return 0, io.EOF
>    }
>    b = r.s[r.i]
>    r.i++
>    return
> }
>
> func (r *Reader) UnreadByte() error {
>    r.prevRune = -1
>    if r.i <= 0 {
>       return errors.New("bytes.Reader.UnreadByte: at beginning of slice")
>    }
>    r.i--
>    return nil
> }
>
> func (r *Reader) ReadRune() (ch rune, size int, err error) {
>    if r.i >= int64(len(r.s)) {
>       r.prevRune = -1
>       return 0, 0, io.EOF
>    }
>    r.prevRune = int(r.i)
>    if c := r.s[r.i]; c < utf8.RuneSelf {
>       r.i++
>       return rune(c), 1, nil
>    }
>    ch, size = utf8.DecodeRune(r.s[r.i:])
>    r.i += int64(size)
>    return
> }
>
> func (r *Reader) UnreadRune() error {
>    if r.prevRune < 0 {
>       return errors.New("bytes.Reader.UnreadRune: previous operation was not ReadRune")
>    }
>    r.i = int64(r.prevRune)
>    r.prevRune = -1
>    return nil
> }
>
> // Seek implements the io.Seeker interface.
> func (r *Reader) Seek(offset int64, whence int) (int64, error) {
>    r.prevRune = -1
>    var abs int64
>    switch whence {
>    case 0:
>       abs = offset
>    case 1:
>       abs = int64(r.i) + offset
>    case 2:
>       abs = int64(len(r.s)) + offset
>    default:
>       return 0, errors.New("bytes.Reader.Seek: invalid whence")
>    }
>    if abs < 0 {
>       return 0, errors.New("bytes.Reader.Seek: negative position")
>    }
>    r.i = abs
>    return abs, nil
> }
>
> // WriteTo implements the io.WriterTo interface.
> func (r *Reader) WriteTo(w io.Writer) (n int64, err error) {
>    r.prevRune = -1
>    if r.i >= int64(len(r.s)) {
>       return 0, nil
>    }
>    b := r.s[r.i:]
>    m, err := w.Write(b)
>    if m > len(b) {
>       panic("bytes.Reader.WriteTo: invalid Write count")
>    }
>    r.i += int64(m)
>    n = int64(m)
>    if m != len(b) && err == nil {
>       err = io.ErrShortWrite
>    }
>    return
> }
>
> // NewReader returns a new Reader reading from b.
> func NewReader(b []byte) *Reader { return &Reader{b, 0, -1} }
> ```

