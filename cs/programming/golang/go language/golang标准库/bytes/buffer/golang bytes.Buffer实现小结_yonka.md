**介绍**

> 重点在对底层存储的关注： buffer - slice - array。 避免无谓的分配和伸长。

**源码介绍**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/c795f08a-0e40-4427-a4f4-2bd756b2a638/index_files/aed01d5c-e209-4d87-9b17-6850087ae7b4.png)
>
> ```Go
> // A Buffer is a variable-sized buffer of bytes with Read and Write methods.
> // The zero value for Buffer is an empty buffer ready to use.
> type Buffer struct {
>    buf       []byte            // contents are the bytes buf[off : len(buf)]
>    off       int               // read at &buf[off], write at &buf[len(buf)]
>    runeBytes [utf8.UTFMax]byte // avoid allocation of slice on each WriteByte or Rune
>    bootstrap [64]byte          // memory to hold first slice; helps small buffers (Printf) avoid allocation.
>    lastRead  readOp            // last read operation, so that Unread* can work correctly.
> }
> ```

> > off标识read到哪了，从off（含）开始属于未读。
> >
> > buf为底层存储。
> >
> > runeBytes，定长数组，减少分配。
> >
> > lastRead，记录最近一次的操作类型。
> >
> > bootstrap，定长数组，避免小buffer频繁变大小导致的大量小内存分配。只要小于64就用这个。
> >
> > ![img]() 几个私有方法、属性
> >
> > readSlice(delim byte) (line []byte, err error)

> > > 以delim为分隔符，读取 当前位置（含） 到 该分隔符（第一次出现）（含） 的bytes，作为slice（不发生copy），更新 off和lastRead

> > grow(n int) int

> > > 将buffer的底层数据结构grow到能支持增加n个byte。
>
> > > 首先看buffer的buf（[]byte）的底层数组是否支持（cap 是否大于等于 len + n），是的话跳过下面的grow部分，直接 b.buf = b.buf[0, b.off+m+n]  （m是buffer的len，也即buf的len - off），也即 [0, len(b.buf) + n]
>
> > > > 1 如果buf == nil（说明是第一次使用） 并且 n<=len(b.bootstrap)，则使用b.bootstrap作为底层数组的slice作为buf；
>
> > > > 2 1不满足的话，如果 m+n小于buf底层数组长度的一半的话，则把已读部分去掉（copy的方式覆盖已读部分），而不是分配新数组 （也即此时认定底层存储够用）
>
> > > > 3 2还不满足的话，只能新开了，大小为 2*cap(b.buf) + n，然后做copy..
>
> > > > off = 0  // 清除掉已读的部分
> > >
> > > 

> > ![img]()对外方法
> >
> > Bytes() []byte

> > > 把底层未读部分返回 --- b.buf[b.off:]

> > Cap() int

> > > 注意不是 len(b.buf)，而是 cap(b.buf)，也即最底层数组的容量

> > Grow(n int)

> > > 调b.grow(n) ...

> > Len() int

> > > 未读长度，len(b.buf) - b.off

> > Next(n int) []byte

> > > // 当成功..时把lastRead置为opRead，否则置为opInvalid
> > >
> > > 取未读长度m，如果n>m则n = m。
> > >
> > > 取 off: off+n这个slice作为返回内容，off += n
> > >
> > > 如果 n > 0 才是为成功读...

> > Read(p []byte) (n int, err error)

> > > // 当成功..时把lastRead置为opRead，否则置为opInvalid
>
> > > 如果off>=len(b.buf)说明没有未读...，执行truncate操作。 如果 len(p) == 0 ，视为正常； >0的话则不能满足返回 0, io.EOF
> > >
> > > 用copy来把 [off:] 复制到p，把返回的n（复制的item数）加到off
> > >
> > > 同样只有当复制数>0时才是为成功...

> > ReadByte() (c byte, err error)

> > > // 当成功..时把lastRead置为opRead，否则置为opInvalid
> > > 如果off>=len(b.buf)说明没有未读...，执行truncate操作。 
> > >
> > > 返回 off处，off++，成功...

> > ReadBytes(delin byte) (line []byte, err error)

> > > 调b.readSlice(delim)，渠道的slice再.. 注意，这里会做一次copy，避免把共享底层数据结构的slice返回 --- line = append(line, slice...)

> > ReadFrom(r io.Reader) (n int64, err error)

> > > 从r读取数据append到buffer，直到EOF，需要的时候会做grow。
> > >
> > > 返回的值n是read的byte数。 遇到EOF以外的错误会直接返回n, e（此时n仍为已读数），遇到EOF则终止read，返回 n, nil
> > >
> > > buffer过大（reader里数据过多）的话可能panic（ErrTooLarge）
> > >
> > > // 当成功..时把lastRead置为opRead，否则置为opInvalid
> > > 如果off>=len(b.buf)说明没有未读...，执行truncate操作。 
> > >
> > > for循环里读直到遇到err（EOF or other），每次读之前检查空间，要 确保free（cap(b.buf) - len(b.buf)） >= MinRead（512），这里不调grow，而是自行重复grow的内容...

> > ReadRune() (r rune, size int, err error)

> > > // 当成功..时把lastRead置为opReadRune，否则置为opInvalid
>
> > > 如果off>=len(b.buf)说明没有未读...，执行truncate操作。 
>
> > > 同样的，先尝试第一个byte，如果是完整utf-8 rune，则可以返回了： rune(c), 1, nil。 如果不是，则需要尝试解析： utf8.DecodeRune ...

> > ReadString(delim byte) (line string, err error)

> > > 先readSlice(delim)，然后转成string返回... （自动copy）

> > Reset()

> > > b.Truncate(0)

> > String() string

> > > 把未读部分转成字符串： return string(b.buf[b.off:])。
>
> > > 兼容了nil buffer的情况： if b == nil {return "<nil>" }

> > Truncate(n int)

> > > lastRead设为opInvalid
>
> > > 检查n合法性：<0 或 >未读字节数 都会panic
>
> > > 把off清零，然后 b.buf = g.buf[0: n]  // 底层数组并未收缩。 未读数为n

> > UnreadByte() error

> > > 只有lastRead为合法（opReadRune或opRead）才能unreadbyte，否则return errors.New
> > >
> > > lastRead=opInvalid，如果off>0则off--

> > UnreadRune() error

> > > 只有最近的操作（lastRead）为opReadRune才能unreadrune，否则return errors.New
> > >
> > > lastRead=opInvalid， 如果off>0则尝试解析buf[0:off]的最后一个utf8 rune，得到其长度n后，b.off -= n

> > Write(p []byte)(n int, err error)

> > > lastRead = opInvalid
> > >
> > > m = b.grow(len(p))，不一定扩展长度，如果够的话不会变化，但一定会把已读部分去掉... // 也即写操作会去掉已读部分...
> > >
> > > m为原未读部分的最大index+1（其实是offset+len，len为原未读数，offset已经被清为0了，也即m为len）把p写到 buf[m:]

> > WriteByte(c byte) error

> > > 与上面操作类似，设置opInvalid、grow(1)、buf[m]=c ...

> > WriteRune(r rune) (n int, err error)

> > > 如果该rune是ascii兼容，则等同于writebyte。 如果不是，则需要解析为byte slice（utf8.EncodeRune(b.runeBytes[0:], r)）然后就是 writebytes了...
> >
> > > // WriteRune最后都是调WriteXX方法，所以对lastRead的影响也在调用方法里

> > WriteString(s string) (n int, err error)

> > > 等同于write bytes，不过不调Write，避免copy bytes。 而是自行写了一遍... 最后调copy...

> > WriteTo(w io.Writer) (n int64, err error)

> > > byte操作，lastRead = opInvalid。
> >
> > > 然后如果 off<len(b.buf)才需要...（不然没有可读的了）

> > > > w.Write(b.buf[b.off:])，如果写的数量m大于原未读数，则panic。 否则加到off...

> > > 做truncate，return

> ```Go
> // The readOp constants describe the last action performed on
> // the buffer, so that UnreadRune and UnreadByte can
> // check for invalid usage.
> type readOp int
>
> const (
>    opInvalid  readOp = iota // Non-read operation.
>    opReadRune               // Read rune.
>    opRead                   // Any other read operation.
> )
> ```
>
> ```Go
> // ErrTooLarge is passed to panic if memory cannot be allocated to store data in a buffer.
> var ErrTooLarge = errors.New("bytes.Buffer: too large")
> ```
>
> makeSlice(n int) []byte

> > 工具方法，用来创建指定大小的slice（加了对panic的处理，该panic基本只会在申请的空间过大时抛出，所以把错误类型转为panic(ErrTooLarge)）
> >
> > ```Go
> > // The readOp constants describe the last action performed on
> > // the buffer, so that UnreadRune and UnreadByte can
> > // check for invalid usage.
> > type readOp int
> >
> > const (
> >    opInvalid  readOp = iota // Non-read operation.
> >    opReadRune               // Read rune.
> >    opRead                   // Any other read operation.
> > )
> > ```

> ```Go
> // MinRead is the minimum slice size passed to a Read call by
> // Buffer.ReadFrom.  As long as the Buffer has at least MinRead bytes beyond
> // what is required to hold the contents of r, ReadFrom will not grow the
> // underlying buffer.
> const MinRead = 512
> ```

> 两个构造器

> > 从byte[]或者从string
> >
> > ```Go
> > // NewBuffer creates and initializes a new Buffer using buf as its initial
> > // contents.  It is intended to prepare a Buffer to read existing data.  It
> > // can also be used to size the internal buffer for writing. To do that,
> > // buf should have the desired capacity but a length of zero.
> > //
> > // In most cases, new(Buffer) (or just declaring a Buffer variable) is
> > // sufficient to initialize a Buffer.
> > func NewBuffer(buf []byte) *Buffer { return &Buffer{buf: buf} }
> >
> > // NewBufferString creates and initializes a new Buffer using string s as its
> > // initial contents. It is intended to prepare a buffer to read an existing
> > // string.
> > //
> > // In most cases, new(Buffer) (or just declaring a Buffer variable) is
> > // sufficient to initialize a Buffer.
> > func NewBufferString(s string) *Buffer {
> >    return &Buffer{buf: []byte(s)}
> > }
> > ```

**源码**

> ```Go
> // Copyright 2009 The Go Authors. All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> package bytes
>
> // Simple byte buffer for marshaling data.
>
> import (
>    "errors"
>    "io"
>    "unicode/utf8"
> )
>
> // A Buffer is a variable-sized buffer of bytes with Read and Write methods.
> // The zero value for Buffer is an empty buffer ready to use.
> type Buffer struct {
>    buf       []byte            // contents are the bytes buf[off : len(buf)]
>    off       int               // read at &buf[off], write at &buf[len(buf)]
>    runeBytes [utf8.UTFMax]byte // avoid allocation of slice on each WriteByte or Rune
>    bootstrap [64]byte          // memory to hold first slice; helps small buffers (Printf) avoid allocation.
>    lastRead  readOp            // last read operation, so that Unread* can work correctly.
> }
>
> // The readOp constants describe the last action performed on
> // the buffer, so that UnreadRune and UnreadByte can
> // check for invalid usage.
> type readOp int
>
> const (
>    opInvalid  readOp = iota // Non-read operation.
>    opReadRune               // Read rune.
>    opRead                   // Any other read operation.
> )
>
> // ErrTooLarge is passed to panic if memory cannot be allocated to store data in a buffer.
> var ErrTooLarge = errors.New("bytes.Buffer: too large")
>
> // Bytes returns a slice of the contents of the unread portion of the buffer;
> // len(b.Bytes()) == b.Len().  If the caller changes the contents of the
> // returned slice, the contents of the buffer will change provided there
> // are no intervening method calls on the Buffer.
> func (b *Buffer) Bytes() []byte { return b.buf[b.off:] }
>
> // String returns the contents of the unread portion of the buffer
> // as a string.  If the Buffer is a nil pointer, it returns "<nil>".
> func (b *Buffer) String() string {
>    if b == nil {
>       // Special case, useful in debugging.
>       return "<nil>"
>    }
>    return string(b.buf[b.off:])
> }
>
> // Len returns the number of bytes of the unread portion of the buffer;
> // b.Len() == len(b.Bytes()).
> func (b *Buffer) Len() int { return len(b.buf) - b.off }
>
> // Cap returns the capacity of the buffer's underlying byte slice, that is, the
> // total space allocated for the buffer's data.
> func (b *Buffer) Cap() int { return cap(b.buf) }
>
> // Truncate discards all but the first n unread bytes from the buffer.
> // It panics if n is negative or greater than the length of the buffer.
> func (b *Buffer) Truncate(n int) {
>    b.lastRead = opInvalid
>    switch {
>    case n < 0 || n > b.Len():
>       panic("bytes.Buffer: truncation out of range")
>    case n == 0:
>       // Reuse buffer space.
>       b.off = 0
>    }
>    b.buf = b.buf[0 : b.off+n]
> }
>
> // Reset resets the buffer so it has no content.
> // b.Reset() is the same as b.Truncate(0).
> func (b *Buffer) Reset() { b.Truncate(0) }
>
> // grow grows the buffer to guarantee space for n more bytes.
> // It returns the index where bytes should be written.
> // If the buffer can't grow it will panic with ErrTooLarge.
> func (b *Buffer) grow(n int) int {
>    m := b.Len()
>    // If buffer is empty, reset to recover space.
>    if m == 0 && b.off != 0 {
>       b.Truncate(0)
>    }
>    if len(b.buf)+n > cap(b.buf) {
>       var buf []byte
>       if b.buf == nil && n <= len(b.bootstrap) {
>          buf = b.bootstrap[0:]
>       } else if m+n <= cap(b.buf)/2 {
>          // We can slide things down instead of allocating a new
>          // slice. We only need m+n <= cap(b.buf) to slide, but
>          // we instead let capacity get twice as large so we
>          // don't spend all our time copying.
>          copy(b.buf[:], b.buf[b.off:])
>          buf = b.buf[:m]
>       } else {
>          // not enough space anywhere
>          buf = makeSlice(2*cap(b.buf) + n)
>          copy(buf, b.buf[b.off:])
>       }
>       b.buf = buf
>       b.off = 0
>    }
>    b.buf = b.buf[0 : b.off+m+n]
>    return b.off + m
> }
>
> // Grow grows the buffer's capacity, if necessary, to guarantee space for
> // another n bytes. After Grow(n), at least n bytes can be written to the
> // buffer without another allocation.
> // If n is negative, Grow will panic.
> // If the buffer can't grow it will panic with ErrTooLarge.
> func (b *Buffer) Grow(n int) {
>    if n < 0 {
>       panic("bytes.Buffer.Grow: negative count")
>    }
>    m := b.grow(n)
>    b.buf = b.buf[0:m]
> }
>
> // Write appends the contents of p to the buffer, growing the buffer as
> // needed. The return value n is the length of p; err is always nil. If the
> // buffer becomes too large, Write will panic with ErrTooLarge.
> func (b *Buffer) Write(p []byte) (n int, err error) {
>    b.lastRead = opInvalid
>    m := b.grow(len(p))
>    return copy(b.buf[m:], p), nil
> }
>
> // WriteString appends the contents of s to the buffer, growing the buffer as
> // needed. The return value n is the length of s; err is always nil. If the
> // buffer becomes too large, WriteString will panic with ErrTooLarge.
> func (b *Buffer) WriteString(s string) (n int, err error) {
>    b.lastRead = opInvalid
>    m := b.grow(len(s))
>    return copy(b.buf[m:], s), nil
> }
>
> // MinRead is the minimum slice size passed to a Read call by
> // Buffer.ReadFrom.  As long as the Buffer has at least MinRead bytes beyond
> // what is required to hold the contents of r, ReadFrom will not grow the
> // underlying buffer.
> const MinRead = 512
>
> // ReadFrom reads data from r until EOF and appends it to the buffer, growing
> // the buffer as needed. The return value n is the number of bytes read. Any
> // error except io.EOF encountered during the read is also returned. If the
> // buffer becomes too large, ReadFrom will panic with ErrTooLarge.
> func (b *Buffer) ReadFrom(r io.Reader) (n int64, err error) {
>    b.lastRead = opInvalid
>    // If buffer is empty, reset to recover space.
>    if b.off >= len(b.buf) {
>       b.Truncate(0)
>    }
>    for {
>       if free := cap(b.buf) - len(b.buf); free < MinRead {
>          // not enough space at end
>          newBuf := b.buf
>          if b.off+free < MinRead {
>             // not enough space using beginning of buffer;
>             // double buffer capacity
>             newBuf = makeSlice(2*cap(b.buf) + MinRead)
>          }
>          copy(newBuf, b.buf[b.off:])
>          b.buf = newBuf[:len(b.buf)-b.off]
>          b.off = 0
>       }
>       m, e := r.Read(b.buf[len(b.buf):cap(b.buf)])
>       b.buf = b.buf[0 : len(b.buf)+m]
>       n += int64(m)
>       if e == io.EOF {
>          break
>       }
>       if e != nil {
>          return n, e
>       }
>    }
>    return n, nil // err is EOF, so return nil explicitly
> }
>
> // makeSlice allocates a slice of size n. If the allocation fails, it panics
> // with ErrTooLarge.
> func makeSlice(n int) []byte {
>    // If the make fails, give a known error.
>    defer func() {
>       if recover() != nil {
>          panic(ErrTooLarge)
>       }
>    }()
>    return make([]byte, n)
> }
>
> // WriteTo writes data to w until the buffer is drained or an error occurs.
> // The return value n is the number of bytes written; it always fits into an
> // int, but it is int64 to match the io.WriterTo interface. Any error
> // encountered during the write is also returned.
> func (b *Buffer) WriteTo(w io.Writer) (n int64, err error) {
>    b.lastRead = opInvalid
>    if b.off < len(b.buf) {
>       nBytes := b.Len()
>       m, e := w.Write(b.buf[b.off:])
>       if m > nBytes {
>          panic("bytes.Buffer.WriteTo: invalid Write count")
>       }
>       b.off += m
>       n = int64(m)
>       if e != nil {
>          return n, e
>       }
>       // all bytes should have been written, by definition of
>       // Write method in io.Writer
>       if m != nBytes {
>          return n, io.ErrShortWrite
>       }
>    }
>    // Buffer is now empty; reset.
>    b.Truncate(0)
>    return
> }
>
> // WriteByte appends the byte c to the buffer, growing the buffer as needed.
> // The returned error is always nil, but is included to match bufio.Writer's
> // WriteByte. If the buffer becomes too large, WriteByte will panic with
> // ErrTooLarge.
> func (b *Buffer) WriteByte(c byte) error {
>    b.lastRead = opInvalid
>    m := b.grow(1)
>    b.buf[m] = c
>    return nil
> }
>
> // WriteRune appends the UTF-8 encoding of Unicode code point r to the
> // buffer, returning its length and an error, which is always nil but is
> // included to match bufio.Writer's WriteRune. The buffer is grown as needed;
> // if it becomes too large, WriteRune will panic with ErrTooLarge.
> func (b *Buffer) WriteRune(r rune) (n int, err error) {
>    if r < utf8.RuneSelf {
>       b.WriteByte(byte(r))
>       return 1, nil
>    }
>    n = utf8.EncodeRune(b.runeBytes[0:], r)
>    b.Write(b.runeBytes[0:n])
>    return n, nil
> }
>
> // Read reads the next len(p) bytes from the buffer or until the buffer
> // is drained.  The return value n is the number of bytes read.  If the
> // buffer has no data to return, err is io.EOF (unless len(p) is zero);
> // otherwise it is nil.
> func (b *Buffer) Read(p []byte) (n int, err error) {
>    b.lastRead = opInvalid
>    if b.off >= len(b.buf) {
>       // Buffer is empty, reset to recover space.
>       b.Truncate(0)
>       if len(p) == 0 {
>          return
>       }
>       return 0, io.EOF
>    }
>    n = copy(p, b.buf[b.off:])
>    b.off += n
>    if n > 0 {
>       b.lastRead = opRead
>    }
>    return
> }
>
> // Next returns a slice containing the next n bytes from the buffer,
> // advancing the buffer as if the bytes had been returned by Read.
> // If there are fewer than n bytes in the buffer, Next returns the entire buffer.
> // The slice is only valid until the next call to a read or write method.
> func (b *Buffer) Next(n int) []byte {
>    b.lastRead = opInvalid
>    m := b.Len()
>    if n > m {
>       n = m
>    }
>    data := b.buf[b.off : b.off+n]
>    b.off += n
>    if n > 0 {
>       b.lastRead = opRead
>    }
>    return data
> }
>
> // ReadByte reads and returns the next byte from the buffer.
> // If no byte is available, it returns error io.EOF.
> func (b *Buffer) ReadByte() (c byte, err error) {
>    b.lastRead = opInvalid
>    if b.off >= len(b.buf) {
>       // Buffer is empty, reset to recover space.
>       b.Truncate(0)
>       return 0, io.EOF
>    }
>    c = b.buf[b.off]
>    b.off++
>    b.lastRead = opRead
>    return c, nil
> }
>
> // ReadRune reads and returns the next UTF-8-encoded
> // Unicode code point from the buffer.
> // If no bytes are available, the error returned is io.EOF.
> // If the bytes are an erroneous UTF-8 encoding, it
> // consumes one byte and returns U+FFFD, 1.
> func (b *Buffer) ReadRune() (r rune, size int, err error) {
>    b.lastRead = opInvalid
>    if b.off >= len(b.buf) {
>       // Buffer is empty, reset to recover space.
>       b.Truncate(0)
>       return 0, 0, io.EOF
>    }
>    b.lastRead = opReadRune
>    c := b.buf[b.off]
>    if c < utf8.RuneSelf {
>       b.off++
>       return rune(c), 1, nil
>    }
>    r, n := utf8.DecodeRune(b.buf[b.off:])
>    b.off += n
>    return r, n, nil
> }
>
> // UnreadRune unreads the last rune returned by ReadRune.
> // If the most recent read or write operation on the buffer was
> // not a ReadRune, UnreadRune returns an error.  (In this regard
> // it is stricter than UnreadByte, which will unread the last byte
> // from any read operation.)
> func (b *Buffer) UnreadRune() error {
>    if b.lastRead != opReadRune {
>       return errors.New("bytes.Buffer: UnreadRune: previous operation was not ReadRune")
>    }
>    b.lastRead = opInvalid
>    if b.off > 0 {
>       _, n := utf8.DecodeLastRune(b.buf[0:b.off])
>       b.off -= n
>    }
>    return nil
> }
>
> // UnreadByte unreads the last byte returned by the most recent
> // read operation.  If write has happened since the last read, UnreadByte
> // returns an error.
> func (b *Buffer) UnreadByte() error {
>    if b.lastRead != opReadRune && b.lastRead != opRead {
>       return errors.New("bytes.Buffer: UnreadByte: previous operation was not a read")
>    }
>    b.lastRead = opInvalid
>    if b.off > 0 {
>       b.off--
>    }
>    return nil
> }
>
> // ReadBytes reads until the first occurrence of delim in the input,
> // returning a slice containing the data up to and including the delimiter.
> // If ReadBytes encounters an error before finding a delimiter,
> // it returns the data read before the error and the error itself (often io.EOF).
> // ReadBytes returns err != nil if and only if the returned data does not end in
> // delim.
> func (b *Buffer) ReadBytes(delim byte) (line []byte, err error) {
>    slice, err := b.readSlice(delim)
>    // return a copy of slice. The buffer's backing array may
>    // be overwritten by later calls.
>    line = append(line, slice...)
>    return
> }
>
> // readSlice is like ReadBytes but returns a reference to internal buffer data.
> func (b *Buffer) readSlice(delim byte) (line []byte, err error) {
>    i := IndexByte(b.buf[b.off:], delim)
>    end := b.off + i + 1
>    if i < 0 {
>       end = len(b.buf)
>       err = io.EOF
>    }
>    line = b.buf[b.off:end]
>    b.off = end
>    b.lastRead = opRead
>    return line, err
> }
>
> // ReadString reads until the first occurrence of delim in the input,
> // returning a string containing the data up to and including the delimiter.
> // If ReadString encounters an error before finding a delimiter,
> // it returns the data read before the error and the error itself (often io.EOF).
> // ReadString returns err != nil if and only if the returned data does not end
> // in delim.
> func (b *Buffer) ReadString(delim byte) (line string, err error) {
>    slice, err := b.readSlice(delim)
>    return string(slice), err
> }
>
> // NewBuffer creates and initializes a new Buffer using buf as its initial
> // contents.  It is intended to prepare a Buffer to read existing data.  It
> // can also be used to size the internal buffer for writing. To do that,
> // buf should have the desired capacity but a length of zero.
> //
> // In most cases, new(Buffer) (or just declaring a Buffer variable) is
> // sufficient to initialize a Buffer.
> func NewBuffer(buf []byte) *Buffer { return &Buffer{buf: buf} }
>
> // NewBufferString creates and initializes a new Buffer using string s as its
> // initial contents. It is intended to prepare a buffer to read an existing
> // string.
> //
> // In most cases, new(Buffer) (or just declaring a Buffer variable) is
> // sufficient to initialize a Buffer.
> func NewBufferString(s string) *Buffer {
>    return &Buffer{buf: []byte(s)}
> }
> ```