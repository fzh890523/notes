**简介**

> 有一个特点，发生的错误有时保存在 b.err，而该err返回一次就就置nil了。 --- 通过 readErr 方法
>
> 实际为 BufferedReader

 

**源码简介**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/e8c91f09-6423-4de4-862c-1a7ea425e51a/index_files/584caea9-6bf2-472f-aac3-f2bf2a68d5b3.png)
>
> ```
> // Reader implements buffering for an io.Reader object.
> type Reader struct {
>    buf          []byte
>    rd           io.Reader // reader provided by the client
>    r, w         int       // buf read and write positions
>    err          error
>    lastByte     int
>    lastRuneSize int
> }
> ```
>
> Buffered() int

> > 当前缓存的内容大小， return b.w - b.r

> Discard(n int) (discarded int, err error)

> > // 等于执行读操作但是外部并不需要这部分读取内容
> >
> > // 如果buf够n的话，移动b.r即可，不够则需要不断的 移动b.r - fill - 移动b.r - ...
> >
> > 先检查错误，如果 n<0则返回错误。 n == 0则直接返回（非错误）。 
> >
> > ...

> Peek(n int) ([]byte, error)

> > // 取（当前读位置）后面n个byte并返回，同时不影响读位置 --- 区别于Discard
> >
> > 检查n，如果<0或者大于buf长度的话都会返回错误。
> >
> > 如果已有buf中不足n并且b.err == nil则会做fill。
> >
> > 如果还是不够的话，有可能是因为err，用b.readErr()来取，如果err == nil则说明只是buffer不够，err = ErrBufferFull。
> >
> > return b.buf[b.r:b.r+n], err，并不影响当前实际读位置...

> Read(p p[byte)(n int, err error)

> > ...
>
> > 如果当前已有buf为0则用buf： 如果比buf大则直接b.rd.Read(p)然后...返回； 否则fill后再继续下一步...
>
> > 而如果当前已有buf不为0则只从buf copy。
>
> > // 所以可能读取的数量会比p的长度小。

> ReadByte() (c byte, err error)

> > 因为是byte操作，lastRuneSize置为-1
> >
> > 如果buf为空，则尝试fill（先检查b.err， != nil的话 return 0, b.readErr()
> >
> > 因为buf长度不可能是0，所以直接 c = b.buf[b.r]然后b.r++。 把c赋给b.lastByte

> ReadBytes() (delim byte) (line []byte, err error)

> > // 因为buf长度有限，所以可能 delim 之前的byte数会超过buf长度，所以这里用二级slice - full来保存读取内容...
>
> > for循环里一直调 b.ReadSlice(delim) 来...，如果遇到ErrBufferFul则说明”还不够“，append到full然后继续； 而如果遇到其他err则说明遇到错误，break； 没遇到err则说明读完了，break...
>
> > 最后会计算所有读到内容的长度一次性make slice... 然后把full中的slicescopy进...。 即使前面遇到非ErrBufferFul的err也会做这部分操作。

> ReadLine() (line []byte, isPrefix bool, err error)

> > // 返回的内容里不包含 \r\n 和 \n
> >
> > // 最多读一个buf，所以是比较low-level的函数，多数情况下应该用 ReadBytes('\n')或者ReadString('\n')

> ReadRune() (r rune, size int, err error)

> > 已有buf是否够utf8.UTFMax； 已有buf内容是否包含一个完整utf8 rune； b.err == nil； buf不满... 如果有其一不满足的话需要b.fill()
>
> > // 构造器里会强制buf size最小为 minReadBufferSize
>
> > ...

> ReadSlice(delim byte) (line []byte, err error)

> > 如果buf满了还不够的话，只能先返回，并且告知 ErrBufferFul...
> >
> > ... 

> ReadString(delim byte) (line string, err error)

> > 调b.ReadBytes(delim)（所以会适配行长度超过buf的情况），然后转为string，额，好像重复copy了... = =

> Reset(r io.Reader)

> > 调 b.reset(b.buf, r)

> UnreadByte() error

> > 前提是前一个需要是byte操作，也即 b.lastByte不能<0。 然后b.r不能为0，否则没有对前面读内容的”记忆“。 但允许 r和w都为0的情况。 = =
>
> > 如果r>0，减一即可。 如果r == 0（w也得==0）那么w = 1 （囧，也即即使极端情况也是能recover回来的，用lastByte。 w = 1表示正好有一个已buf的...）
>
> > 把lastByte赋给 b.buf[b.r]，然后 lastByte = -1，lastRuneSize = -1 ...

> UnreadRune() error

> > 需要lastRuneSize不能<0同时b.r不能小于lastRuneSize（不然”记忆“的不够）
>
> > b.r -= b.lastRuneSize。 lastByte置为-1，lastRuneSize置为-1 ...

> WriteTo(w io.Writer) (n int64, err error)

> > 先调 b.writeBuf(w)
> >
> > 然后如果rd能转成io.WriterTo的话，转成...后调 r.WriterTo(w)... 再return。 不能的话尝试把w转成io.ReaderFrom，如果ok则调 w.ReadFrom(b.rd)，然后返回。 都不行的话，只能自己... 循环的 fill和writeBuf... 直到EOF或其他错误...

> fill()

> > 先把buf已有内容reset，也即如果已经读了部分（b.r>0）的话，则把剩余未读部分copy到buf头部（0:）并且更新b.w（b.w -= b.r）和b.r（b.r = 0）
>
> > 校验b.w，如果大于buf长度的话，panic
>
> > 最多做100次尝试读取到b.buf[b.w:]，如果读到的数量n<0则panic，n加到b.w，如果遇到err则赋给b.err并返回，正常读取则return。 100后还没返回的话则 b.io = io.ErrNoProgress。

> readErr() error

> > 取b.err返回并且把b.err = nil

> reset(buf []byte, r io.Reader)

> > 把b重新复制，*b = ...，新构建一个...

> writeBuf(w io.Writer) (int64, error)

> > w.Write(b.buf[b.r:b.w])，写入数量n<0则panic。把n加到b.r，返回...

> 构造器

> > ```
> > // NewReader returns a new Reader whose buffer has the default size.
> > func NewReader(rd io.Reader) *Reader {
> >    return NewReaderSize(rd, defaultBufSize)
> > }
> > ```
> >
> > ```
> > // NewReaderSize returns a new Reader whose buffer has at least the specified
> > // size. If the argument io.Reader is already a Reader with large enough
> > // size, it returns the underlying Reader.
> > func NewReaderSize(rd io.Reader, size int) *Reader {
> >    // Is it already a Reader?
> >    b, ok := rd.(*Reader)
> >    if ok && len(b.buf) >= size {
> >       return b
> >    }
> >    if size < minReadBufferSize {
> >       size = minReadBufferSize
> >    }
> >    r := new(Reader)
> >    r.reset(make([]byte, size), rd)
> >    return r
> > }
> > ```

> > 这里会强制最小buf size为 minReadBufferSize 也即16，保证至少可以存一个rune... = =
> >
> > wrap了一个buf和一个rd... 

**源码**

> ```Go
> // Reader implements buffering for an io.Reader object.
> type Reader struct {
>    buf          []byte
>    rd           io.Reader // reader provided by the client
>    r, w         int       // buf read and write positions
>    err          error
>    lastByte     int
>    lastRuneSize int
> }
>
> const minReadBufferSize = 16
> const maxConsecutiveEmptyReads = 100
>
> // NewReaderSize returns a new Reader whose buffer has at least the specified
> // size. If the argument io.Reader is already a Reader with large enough
> // size, it returns the underlying Reader.
> func NewReaderSize(rd io.Reader, size int) *Reader {
>    // Is it already a Reader?
>    b, ok := rd.(*Reader)
>    if ok && len(b.buf) >= size {
>       return b
>    }
>    if size < minReadBufferSize {
>       size = minReadBufferSize
>    }
>    r := new(Reader)
>    r.reset(make([]byte, size), rd)
>    return r
> }
>
> // NewReader returns a new Reader whose buffer has the default size.
> func NewReader(rd io.Reader) *Reader {
>    return NewReaderSize(rd, defaultBufSize)
> }
>
> // Reset discards any buffered data, resets all state, and switches
> // the buffered reader to read from r.
> func (b *Reader) Reset(r io.Reader) {
>    b.reset(b.buf, r)
> }
>
> func (b *Reader) reset(buf []byte, r io.Reader) {
>    *b = Reader{
>       buf:          buf,
>       rd:           r,
>       lastByte:     -1,
>       lastRuneSize: -1,
>    }
> }
>
> var errNegativeRead = errors.New("bufio: reader returned negative count from Read")
>
> // fill reads a new chunk into the buffer.
> func (b *Reader) fill() {
>    // Slide existing data to beginning.
>    if b.r > 0 {
>       copy(b.buf, b.buf[b.r:b.w])
>       b.w -= b.r
>       b.r = 0
>    }
>
>    if b.w >= len(b.buf) {
>       panic("bufio: tried to fill full buffer")
>    }
>
>    // Read new data: try a limited number of times.
>    for i := maxConsecutiveEmptyReads; i > 0; i-- {
>       n, err := b.rd.Read(b.buf[b.w:])
>       if n < 0 {
>          panic(errNegativeRead)
>       }
>       b.w += n
>       if err != nil {
>          b.err = err
>          return
>       }
>       if n > 0 {
>          return
>       }
>    }
>    b.err = io.ErrNoProgress
> }
>
> func (b *Reader) readErr() error {
>    err := b.err
>    b.err = nil
>    return err
> }
>
> // Peek returns the next n bytes without advancing the reader. The bytes stop
> // being valid at the next read call. If Peek returns fewer than n bytes, it
> // also returns an error explaining why the read is short. The error is
> // ErrBufferFull if n is larger than b's buffer size.
> func (b *Reader) Peek(n int) ([]byte, error) {
>    if n < 0 {
>       return nil, ErrNegativeCount
>    }
>    if n > len(b.buf) {
>       return nil, ErrBufferFull
>    }
>    // 0 <= n <= len(b.buf)
>    for b.w-b.r < n && b.err == nil {
>       b.fill() // b.w-b.r < len(b.buf) => buffer is not full
>    }
>
>    var err error
>    if avail := b.w - b.r; avail < n {
>       // not enough data in buffer
>       n = avail
>       err = b.readErr()
>       if err == nil {
>          err = ErrBufferFull
>       }
>    }
>    return b.buf[b.r : b.r+n], err
> }
>
> // Discard skips the next n bytes, returning the number of bytes discarded.
> //
> // If Discard skips fewer than n bytes, it also returns an error.
> // If 0 <= n <= b.Buffered(), Discard is guaranteed to succeed without
> // reading from the underlying io.Reader.
> func (b *Reader) Discard(n int) (discarded int, err error) {
>    if n < 0 {
>       return 0, ErrNegativeCount
>    }
>    if n == 0 {
>       return
>    }
>    remain := n
>    for {
>       skip := b.Buffered()
>       if skip == 0 {
>          b.fill()
>          skip = b.Buffered()
>       }
>       if skip > remain {
>          skip = remain
>       }
>       b.r += skip
>       remain -= skip
>       if remain == 0 {
>          return n, nil
>       }
>       if b.err != nil {
>          return n - remain, b.readErr()
>       }
>    }
> }
>
> // Read reads data into p.
> // It returns the number of bytes read into p.
> // It calls Read at most once on the underlying Reader,
> // hence n may be less than len(p).
> // At EOF, the count will be zero and err will be io.EOF.
> func (b *Reader) Read(p []byte) (n int, err error) {
>    n = len(p)
>    if n == 0 {
>       return 0, b.readErr()
>    }
>    if b.r == b.w {
>       if b.err != nil {
>          return 0, b.readErr()
>       }
>       if len(p) >= len(b.buf) {
>          // Large read, empty buffer.
>          // Read directly into p to avoid copy.
>          n, b.err = b.rd.Read(p)
>          if n < 0 {
>             panic(errNegativeRead)
>          }
>          if n > 0 {
>             b.lastByte = int(p[n-1])
>             b.lastRuneSize = -1
>          }
>          return n, b.readErr()
>       }
>       b.fill() // buffer is empty
>       if b.r == b.w {
>          return 0, b.readErr()
>       }
>    }
>
>    // copy as much as we can
>    n = copy(p, b.buf[b.r:b.w])
>    b.r += n
>    b.lastByte = int(b.buf[b.r-1])
>    b.lastRuneSize = -1
>    return n, nil
> }
>
> // ReadByte reads and returns a single byte.
> // If no byte is available, returns an error.
> func (b *Reader) ReadByte() (c byte, err error) {
>    b.lastRuneSize = -1
>    for b.r == b.w {
>       if b.err != nil {
>          return 0, b.readErr()
>       }
>       b.fill() // buffer is empty
>    }
>    c = b.buf[b.r]
>    b.r++
>    b.lastByte = int(c)
>    return c, nil
> }
>
> // UnreadByte unreads the last byte.  Only the most recently read byte can be unread.
> func (b *Reader) UnreadByte() error {
>    if b.lastByte < 0 || b.r == 0 && b.w > 0 {
>       return ErrInvalidUnreadByte
>    }
>    // b.r > 0 || b.w == 0
>    if b.r > 0 {
>       b.r--
>    } else {
>       // b.r == 0 && b.w == 0
>       b.w = 1
>    }
>    b.buf[b.r] = byte(b.lastByte)
>    b.lastByte = -1
>    b.lastRuneSize = -1
>    return nil
> }
>
> // ReadRune reads a single UTF-8 encoded Unicode character and returns the
> // rune and its size in bytes. If the encoded rune is invalid, it consumes one byte
> // and returns unicode.ReplacementChar (U+FFFD) with a size of 1.
> func (b *Reader) ReadRune() (r rune, size int, err error) {
>    for b.r+utf8.UTFMax > b.w && !utf8.FullRune(b.buf[b.r:b.w]) && b.err == nil && b.w-b.r < len(b.buf) {
>       b.fill() // b.w-b.r < len(buf) => buffer is not full
>    }
>    b.lastRuneSize = -1
>    if b.r == b.w {
>       return 0, 0, b.readErr()
>    }
>    r, size = rune(b.buf[b.r]), 1
>    if r >= 0x80 {
>       r, size = utf8.DecodeRune(b.buf[b.r:b.w])
>    }
>    b.r += size
>    b.lastByte = int(b.buf[b.r-1])
>    b.lastRuneSize = size
>    return r, size, nil
> }
>
> // UnreadRune unreads the last rune.  If the most recent read operation on
> // the buffer was not a ReadRune, UnreadRune returns an error.  (In this
> // regard it is stricter than UnreadByte, which will unread the last byte
> // from any read operation.)
> func (b *Reader) UnreadRune() error {
>    if b.lastRuneSize < 0 || b.r < b.lastRuneSize {
>       return ErrInvalidUnreadRune
>    }
>    b.r -= b.lastRuneSize
>    b.lastByte = -1
>    b.lastRuneSize = -1
>    return nil
> }
>
> // Buffered returns the number of bytes that can be read from the current buffer.
> func (b *Reader) Buffered() int { return b.w - b.r }
>
> // ReadSlice reads until the first occurrence of delim in the input,
> // returning a slice pointing at the bytes in the buffer.
> // The bytes stop being valid at the next read.
> // If ReadSlice encounters an error before finding a delimiter,
> // it returns all the data in the buffer and the error itself (often io.EOF).
> // ReadSlice fails with error ErrBufferFull if the buffer fills without a delim.
> // Because the data returned from ReadSlice will be overwritten
> // by the next I/O operation, most clients should use
> // ReadBytes or ReadString instead.
> // ReadSlice returns err != nil if and only if line does not end in delim.
> func (b *Reader) ReadSlice(delim byte) (line []byte, err error) {
>    for {
>       // Search buffer.
>       if i := bytes.IndexByte(b.buf[b.r:b.w], delim); i >= 0 {
>          line = b.buf[b.r : b.r+i+1]
>          b.r += i + 1
>          break
>       }
>
>       // Pending error?
>       if b.err != nil {
>          line = b.buf[b.r:b.w]
>          b.r = b.w
>          err = b.readErr()
>          break
>       }
>
>       // Buffer full?
>       if b.Buffered() >= len(b.buf) {
>          b.r = b.w
>          line = b.buf
>          err = ErrBufferFull
>          break
>       }
>
>       b.fill() // buffer is not full
>    }
>
>    // Handle last byte, if any.
>    if i := len(line) - 1; i >= 0 {
>       b.lastByte = int(line[i])
>       b.lastRuneSize = -1
>    }
>
>    return
> }
>
> // ReadLine is a low-level line-reading primitive. Most callers should use
> // ReadBytes('\n') or ReadString('\n') instead or use a Scanner.
> //
> // ReadLine tries to return a single line, not including the end-of-line bytes.
> // If the line was too long for the buffer then isPrefix is set and the
> // beginning of the line is returned. The rest of the line will be returned
> // from future calls. isPrefix will be false when returning the last fragment
> // of the line. The returned buffer is only valid until the next call to
> // ReadLine. ReadLine either returns a non-nil line or it returns an error,
> // never both.
> //
> // The text returned from ReadLine does not include the line end ("\r\n" or "\n").
> // No indication or error is given if the input ends without a final line end.
> // Calling UnreadByte after ReadLine will always unread the last byte read
> // (possibly a character belonging to the line end) even if that byte is not
> // part of the line returned by ReadLine.
> func (b *Reader) ReadLine() (line []byte, isPrefix bool, err error) {
>    line, err = b.ReadSlice('\n')
>    if err == ErrBufferFull {
>       // Handle the case where "\r\n" straddles the buffer.
>       if len(line) > 0 && line[len(line)-1] == '\r' {
>          // Put the '\r' back on buf and drop it from line.
>          // Let the next call to ReadLine check for "\r\n".
>          if b.r == 0 {
>             // should be unreachable
>             panic("bufio: tried to rewind past start of buffer")
>          }
>          b.r--
>          line = line[:len(line)-1]
>       }
>       return line, true, nil
>    }
>
>    if len(line) == 0 {
>       if err != nil {
>          line = nil
>       }
>       return
>    }
>    err = nil
>
>    if line[len(line)-1] == '\n' {
>       drop := 1
>       if len(line) > 1 && line[len(line)-2] == '\r' {
>          drop = 2
>       }
>       line = line[:len(line)-drop]
>    }
>    return
> }
>
> // ReadBytes reads until the first occurrence of delim in the input,
> // returning a slice containing the data up to and including the delimiter.
> // If ReadBytes encounters an error before finding a delimiter,
> // it returns the data read before the error and the error itself (often io.EOF).
> // ReadBytes returns err != nil if and only if the returned data does not end in
> // delim.
> // For simple uses, a Scanner may be more convenient.
> func (b *Reader) ReadBytes(delim byte) (line []byte, err error) {
>    // Use ReadSlice to look for array,
>    // accumulating full buffers.
>    var frag []byte
>    var full [][]byte
>
>    for {
>       var e error
>       frag, e = b.ReadSlice(delim)
>       if e == nil { // got final fragment
>          break
>       }
>       if e != ErrBufferFull { // unexpected error
>          err = e
>          break
>       }
>
>       // Make a copy of the buffer.
>       buf := make([]byte, len(frag))
>       copy(buf, frag)
>       full = append(full, buf)
>    }
>
>    // Allocate new buffer to hold the full pieces and the fragment.
>    n := 0
>    for i := range full {
>       n += len(full[i])
>    }
>    n += len(frag)
>
>    // Copy full pieces and fragment in.
>    buf := make([]byte, n)
>    n = 0
>    for i := range full {
>       n += copy(buf[n:], full[i])
>    }
>    copy(buf[n:], frag)
>    return buf, err
> }
>
> // ReadString reads until the first occurrence of delim in the input,
> // returning a string containing the data up to and including the delimiter.
> // If ReadString encounters an error before finding a delimiter,
> // it returns the data read before the error and the error itself (often io.EOF).
> // ReadString returns err != nil if and only if the returned data does not end in
> // delim.
> // For simple uses, a Scanner may be more convenient.
> func (b *Reader) ReadString(delim byte) (line string, err error) {
>    bytes, err := b.ReadBytes(delim)
>    line = string(bytes)
>    return line, err
> }
>
> // WriteTo implements io.WriterTo.
> func (b *Reader) WriteTo(w io.Writer) (n int64, err error) {
>    n, err = b.writeBuf(w)
>    if err != nil {
>       return
>    }
>
>    if r, ok := b.rd.(io.WriterTo); ok {
>       m, err := r.WriteTo(w)
>       n += m
>       return n, err
>    }
>
>    if w, ok := w.(io.ReaderFrom); ok {
>       m, err := w.ReadFrom(b.rd)
>       n += m
>       return n, err
>    }
>
>    if b.w-b.r < len(b.buf) {
>       b.fill() // buffer not full
>    }
>
>    for b.r < b.w {
>       // b.r < b.w => buffer is not empty
>       m, err := b.writeBuf(w)
>       n += m
>       if err != nil {
>          return n, err
>       }
>       b.fill() // buffer is empty
>    }
>
>    if b.err == io.EOF {
>       b.err = nil
>    }
>
>    return n, b.readErr()
> }
>
> var errNegativeWrite = errors.New("bufio: writer returned negative count from Write")
>
> // writeBuf writes the Reader's buffer to the writer.
> func (b *Reader) writeBuf(w io.Writer) (int64, error) {
>    n, err := w.Write(b.buf[b.r:b.w])
>    if n < 0 {
>       panic(errNegativeWrite)
>    }
>    b.r += n
>    return int64(n), err
> }
> ```