**简介**

> 应该叫 BufferedWriter，wrap了一个writer，本身只是起”buffer“的作用。
>
> 大的原则是写先写buffer，写满再flush再写buffer...

**源码简介**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/be7b4b84-de46-4f9d-96ed-d52ec5e62f12/index_files/6a990d42-7915-4163-8b3e-3a55342109aa.png)
>
> ```
> // Writer implements buffering for an io.Writer object.
> // If an error occurs writing to a Writer, no more data will be
> // accepted and all subsequent writes will return the error.
> // After all data has been written, the client should call the
> // Flush method to guarantee all data has been forwarded to
> // the underlying io.Writer.
> type Writer struct {
>    err error
>    buf []byte
>    n   int
>    wr  io.Writer
> }
> ```

> Available() int

> > 目前buf里有多少可用，return len(b.buf) - b.n

> Buffered() int
>
> 

> > 目前buf里已经用/缓冲了多少，return b.n

> Flush() error

> > 调b.flush()

> ReadFrom(r io.Reader) 

> > 如果没有使用buf的话，尝试直接写，尝试把 b.wr强转为 io.ReaderFrom，成功则调其ReadFrom方法，返回其返回值。
> >
> > 循环里...

> > > 可用buf为0则flush，flush出错则return。
> > >
> > > 如果从r里读100次都读不到东西并且都没报错的话， return n, io.ErrNoProgress。 当然如果有报错也返回（处理完读取数量后）
> > >
> > > 把读到的数量加到b.n

> > 如果错误是io.EOF的话，err = nil （如果可用buffer为空的话，会尝试flush）

> Reset(w io.Writer)

> > err置为nil，n置为0，w赋给wr

> Write(p []byte) (nn int, err error)

> > 和WriteString类似，在循环里... 区别在于，这里会判断buf是否使用，如果没有使用则直接b.wr.Write(p)避免copy； 如果使用了则没办法执行先继续填buf。
> >
> > ...

> WriteByte(c byte) error

> > 先判错（b.err != nil 则return）
> >
> > 如果可用buffer不足1，做flush，flush报错则return。 
> >
> > 把c赋给buf[b.n] （这会儿应该就是index 0了），然后b.n++。  // buf至少为1吧。

> WriteRune(r rune) (size int, err error)

> > 如果是单byte utf8则当byte写入 - 调 b.WriteByte...
>
> > 如果不是，则判断当前buffer可用部分是否小于 utf8.UTFMax也即4，是的话执行flush，而如果还小于的话，说明是buffer本来就小，则把r转为字符串（实际是转为[]byte）来写入 - 调b.WriteString(string(r))。 而如果够的话，直接入buffer（utf8.EncodeRune来...copy到..，然后把返回的数量值加到b.n）

> WriteString(s string) (int, error)

> > 循环中执行（跳出条件为 s长度小于等于可用buffer长度 或者 b.err != nil）
>
> > > 把s拷贝到n的可用部分，把拷贝的数量加到b.n同时也累计到总计数nn，把s赋为新值 s = s[n:]，flush buffer
>
> > 如果b.err != nil （也即因为...跳出），return nn, b.err。否则则是因为当前剩余字符串的长度足以放到buffer中，则做copy，更新b.n和nn计数，return nn, nil

> flush() error

> > 如果有err（b.err != nil）则返回err
>
> > 如果当前没有缓冲内容（b.n == 0），则返回
>
> > 把buf中的缓冲内容写入， n, err := b.wr.Writer(b.buf[0:b.n])  // 从index0往后写。
> >
> > 处理写入的错误。 如果没报错但实际写入byte数n小于buf长度的话，说明b.wr这个writer发生了写入错误，于是返回io.ErrShortWrite。 而如果err != nil的话err赋给b.err，如果n还是小于buf长度会去掉已经写的那部分（其实是把没写的部分copy到头部然后b.n -= n），然后返回。
> >
> > 正常写完的话，则b.n=0清空buffer计数。
>
> 
>

**源码**

> ```Go
> // buffered output
>
> // Writer implements buffering for an io.Writer object.
> // If an error occurs writing to a Writer, no more data will be
> // accepted and all subsequent writes will return the error.
> // After all data has been written, the client should call the
> // Flush method to guarantee all data has been forwarded to
> // the underlying io.Writer.
> type Writer struct {
>    err error
>    buf []byte
>    n   int
>    wr  io.Writer
> }
>
> // NewWriterSize returns a new Writer whose buffer has at least the specified
> // size. If the argument io.Writer is already a Writer with large enough
> // size, it returns the underlying Writer.
> func NewWriterSize(w io.Writer, size int) *Writer {
>    // Is it already a Writer?
>    b, ok := w.(*Writer)
>    if ok && len(b.buf) >= size {
>       return b
>    }
>    if size <= 0 {
>       size = defaultBufSize
>    }
>    return &Writer{
>       buf: make([]byte, size),
>       wr:  w,
>    }
> }
>
> // NewWriter returns a new Writer whose buffer has the default size.
> func NewWriter(w io.Writer) *Writer {
>    return NewWriterSize(w, defaultBufSize)
> }
>
> // Reset discards any unflushed buffered data, clears any error, and
> // resets b to write its output to w.
> func (b *Writer) Reset(w io.Writer) {
>    b.err = nil
>    b.n = 0
>    b.wr = w
> }
>
> // Flush writes any buffered data to the underlying io.Writer.
> func (b *Writer) Flush() error {
>    err := b.flush()
>    return err
> }
>
> func (b *Writer) flush() error {
>    if b.err != nil {
>       return b.err
>    }
>    if b.n == 0 {
>       return nil
>    }
>    n, err := b.wr.Write(b.buf[0:b.n])
>    if n < b.n && err == nil {
>       err = io.ErrShortWrite
>    }
>    if err != nil {
>       if n > 0 && n < b.n {
>          copy(b.buf[0:b.n-n], b.buf[n:b.n])
>       }
>       b.n -= n
>       b.err = err
>       return err
>    }
>    b.n = 0
>    return nil
> }
>
> // Available returns how many bytes are unused in the buffer.
> func (b *Writer) Available() int { return len(b.buf) - b.n }
>
> // Buffered returns the number of bytes that have been written into the current buffer.
> func (b *Writer) Buffered() int { return b.n }
>
> // Write writes the contents of p into the buffer.
> // It returns the number of bytes written.
> // If nn < len(p), it also returns an error explaining
> // why the write is short.
> func (b *Writer) Write(p []byte) (nn int, err error) {
>    for len(p) > b.Available() && b.err == nil {
>       var n int
>       if b.Buffered() == 0 {
>          // Large write, empty buffer.
>          // Write directly from p to avoid copy.
>          n, b.err = b.wr.Write(p)
>       } else {
>          n = copy(b.buf[b.n:], p)
>          b.n += n
>          b.flush()
>       }
>       nn += n
>       p = p[n:]
>    }
>    if b.err != nil {
>       return nn, b.err
>    }
>    n := copy(b.buf[b.n:], p)
>    b.n += n
>    nn += n
>    return nn, nil
> }
>
> // WriteByte writes a single byte.
> func (b *Writer) WriteByte(c byte) error {
>    if b.err != nil {
>       return b.err
>    }
>    if b.Available() <= 0 && b.flush() != nil {
>       return b.err
>    }
>    b.buf[b.n] = c
>    b.n++
>    return nil
> }
>
> // WriteRune writes a single Unicode code point, returning
> // the number of bytes written and any error.
> func (b *Writer) WriteRune(r rune) (size int, err error) {
>    if r < utf8.RuneSelf {
>       err = b.WriteByte(byte(r))
>       if err != nil {
>          return 0, err
>       }
>       return 1, nil
>    }
>    if b.err != nil {
>       return 0, b.err
>    }
>    n := b.Available()
>    if n < utf8.UTFMax {
>       if b.flush(); b.err != nil {
>          return 0, b.err
>       }
>       n = b.Available()
>       if n < utf8.UTFMax {
>          // Can only happen if buffer is silly small.
>          return b.WriteString(string(r))
>       }
>    }
>    size = utf8.EncodeRune(b.buf[b.n:], r)
>    b.n += size
>    return size, nil
> }
>
> // WriteString writes a string.
> // It returns the number of bytes written.
> // If the count is less than len(s), it also returns an error explaining
> // why the write is short.
> func (b *Writer) WriteString(s string) (int, error) {
>    nn := 0
>    for len(s) > b.Available() && b.err == nil {
>       n := copy(b.buf[b.n:], s)
>       b.n += n
>       nn += n
>       s = s[n:]
>       b.flush()
>    }
>    if b.err != nil {
>       return nn, b.err
>    }
>    n := copy(b.buf[b.n:], s)
>    b.n += n
>    nn += n
>    return nn, nil
> }
>
> // ReadFrom implements io.ReaderFrom.
> func (b *Writer) ReadFrom(r io.Reader) (n int64, err error) {
>    if b.Buffered() == 0 {
>       if w, ok := b.wr.(io.ReaderFrom); ok {
>          return w.ReadFrom(r)
>       }
>    }
>    var m int
>    for {
>       if b.Available() == 0 {
>          if err1 := b.flush(); err1 != nil {
>             return n, err1
>          }
>       }
>       nr := 0
>       for nr < maxConsecutiveEmptyReads {
>          m, err = r.Read(b.buf[b.n:])
>          if m != 0 || err != nil {
>             break
>          }
>          nr++
>       }
>       if nr == maxConsecutiveEmptyReads {
>          return n, io.ErrNoProgress
>       }
>       b.n += m
>       n += int64(m)
>       if err != nil {
>          break
>       }
>    }
>    if err == io.EOF {
>       // If we filled the buffer exactly, flush pre-emptively.
>       if b.Available() == 0 {
>          err = b.flush()
>       } else {
>          err = nil
>       }
>    }
>    return n, err
> }
> ```