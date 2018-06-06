**源码**

> ```Go
> // ReadWriter stores pointers to a Reader and a Writer.
> // It implements io.ReadWriter.
> type ReadWriter struct {
>    *Reader
>    *Writer
> }
>
> // NewReadWriter allocates a new ReadWriter that dispatches to r and w.
> func NewReadWriter(r *Reader, w *Writer) *ReadWriter {
>    return &ReadWriter{r, w}
> }
> ```