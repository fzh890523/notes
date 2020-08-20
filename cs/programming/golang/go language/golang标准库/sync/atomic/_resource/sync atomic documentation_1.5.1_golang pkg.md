

# Package atomic




## Overview ▾

Package atomic provides low-level atomic memory primitives useful for implementing synchronization algorithms.

These functions require great care to be used correctly. Except for special, low-level applications, synchronization is better done with channels or the facilities of the sync package. Share memory by communicating; don't communicate by sharing memory.

The swap operation, implemented by the SwapT functions, is the atomic equivalent of:

```
old = *addr
*addr = new
return old

```

The compare-and-swap operation, implemented by the CompareAndSwapT functions, is the atomic equivalent of:

```
if *addr == old {
	*addr = new
	return true
}
return false

```

The add operation, implemented by the AddT functions, is the atomic equivalent of:

```
*addr += delta
return *addr

```

The load and store operations, implemented by the LoadT and StoreT functions, are the atomic equivalents of "return *addr" and "*addr = val".

## Index ▾


#### Examples


#### Package files

[doc.go](https://golang.org/src/sync/atomic/doc.go) [value.go](https://golang.org/src/sync/atomic/value.go)

## func [AddInt32](https://golang.org/src/sync/atomic/doc.go?s=3560:3611#L82)

```
func AddInt32(addr *int32, delta int32) (new int32)
```

AddInt32 atomically adds delta to *addr and returns the new value.

## func [AddInt64](https://golang.org/src/sync/atomic/doc.go?s=3965:4016#L90)

```
func AddInt64(addr *int64, delta int64) (new int64)
```

AddInt64 atomically adds delta to *addr and returns the new value.

## func [AddUint32](https://golang.org/src/sync/atomic/doc.go?s=3838:3893#L87)

```
func AddUint32(addr *uint32, delta uint32) (new uint32)
```

AddUint32 atomically adds delta to *addr and returns the new value. To subtract a signed positive constant value c from x, do AddUint32(&x, ^uint32(c-1)). In particular, to decrement x, do AddUint32(&x, ^uint32(0)).

## func [AddUint64](https://golang.org/src/sync/atomic/doc.go?s=4243:4298#L95)

```
func AddUint64(addr *uint64, delta uint64) (new uint64)
```

AddUint64 atomically adds delta to *addr and returns the new value. To subtract a signed positive constant value c from x, do AddUint64(&x, ^uint64(c-1)). In particular, to decrement x, do AddUint64(&x, ^uint64(0)).

## func [AddUintptr](https://golang.org/src/sync/atomic/doc.go?s=4372:4431#L98)

```
func AddUintptr(addr *uintptr, delta uintptr) (new uintptr)
```

AddUintptr atomically adds delta to *addr and returns the new value.

## func [CompareAndSwapInt32](https://golang.org/src/sync/atomic/doc.go?s=2608:2676#L64)

```
func CompareAndSwapInt32(addr *int32, old, new int32) (swapped bool)
```

CompareAndSwapInt32 executes the compare-and-swap operation for an int32 value.

## func [CompareAndSwapInt64](https://golang.org/src/sync/atomic/doc.go?s=2761:2829#L67)

```
func CompareAndSwapInt64(addr *int64, old, new int64) (swapped bool)
```

CompareAndSwapInt64 executes the compare-and-swap operation for an int64 value.

## func [CompareAndSwapPointer](https://golang.org/src/sync/atomic/doc.go?s=3400:3488#L79)

```
func CompareAndSwapPointer(addr *unsafe.Pointer, old, new unsafe.Pointer) (swapped bool)
```

CompareAndSwapPointer executes the compare-and-swap operation for a unsafe.Pointer value.

## func [CompareAndSwapUint32](https://golang.org/src/sync/atomic/doc.go?s=2915:2986#L70)

```
func CompareAndSwapUint32(addr *uint32, old, new uint32) (swapped bool)
```

CompareAndSwapUint32 executes the compare-and-swap operation for a uint32 value.

## func [CompareAndSwapUint64](https://golang.org/src/sync/atomic/doc.go?s=3072:3143#L73)

```
func CompareAndSwapUint64(addr *uint64, old, new uint64) (swapped bool)
```

CompareAndSwapUint64 executes the compare-and-swap operation for a uint64 value.

## func [CompareAndSwapUintptr](https://golang.org/src/sync/atomic/doc.go?s=3231:3305#L76)

```
func CompareAndSwapUintptr(addr *uintptr, old, new uintptr) (swapped bool)
```

CompareAndSwapUintptr executes the compare-and-swap operation for a uintptr value.

## func [LoadInt32](https://golang.org/src/sync/atomic/doc.go?s=4470:4509#L101)

```
func LoadInt32(addr *int32) (val int32)
```

LoadInt32 atomically loads *addr.

## func [LoadInt64](https://golang.org/src/sync/atomic/doc.go?s=4548:4587#L104)

```
func LoadInt64(addr *int64) (val int64)
```

LoadInt64 atomically loads *addr.

## func [LoadPointer](https://golang.org/src/sync/atomic/doc.go?s=4878:4937#L116)

```
func LoadPointer(addr *unsafe.Pointer) (val unsafe.Pointer)
```

LoadPointer atomically loads *addr.

## func [LoadUint32](https://golang.org/src/sync/atomic/doc.go?s=4627:4669#L107)

```
func LoadUint32(addr *uint32) (val uint32)
```

LoadUint32 atomically loads *addr.

## func [LoadUint64](https://golang.org/src/sync/atomic/doc.go?s=4709:4751#L110)

```
func LoadUint64(addr *uint64) (val uint64)
```

LoadUint64 atomically loads *addr.

## func [LoadUintptr](https://golang.org/src/sync/atomic/doc.go?s=4792:4837#L113)

```
func LoadUintptr(addr *uintptr) (val uintptr)
```

LoadUintptr atomically loads *addr.

## func [StoreInt32](https://golang.org/src/sync/atomic/doc.go?s=4987:5026#L119)

```
func StoreInt32(addr *int32, val int32)
```

StoreInt32 atomically stores val into *addr.

## func [StoreInt64](https://golang.org/src/sync/atomic/doc.go?s=5076:5115#L122)

```
func StoreInt64(addr *int64, val int64)
```

StoreInt64 atomically stores val into *addr.

## func [StorePointer](https://golang.org/src/sync/atomic/doc.go?s=5450:5509#L134)

```
func StorePointer(addr *unsafe.Pointer, val unsafe.Pointer)
```

StorePointer atomically stores val into *addr.

## func [StoreUint32](https://golang.org/src/sync/atomic/doc.go?s=5166:5208#L125)

```
func StoreUint32(addr *uint32, val uint32)
```

StoreUint32 atomically stores val into *addr.

## func [StoreUint64](https://golang.org/src/sync/atomic/doc.go?s=5259:5301#L128)

```
func StoreUint64(addr *uint64, val uint64)
```

StoreUint64 atomically stores val into *addr.

## func [StoreUintptr](https://golang.org/src/sync/atomic/doc.go?s=5353:5398#L131)

```
func StoreUintptr(addr *uintptr, val uintptr)
```

StoreUintptr atomically stores val into *addr.

## func [SwapInt32](https://golang.org/src/sync/atomic/doc.go?s=1742:1792#L46)

```
func SwapInt32(addr *int32, new int32) (old int32)
```

SwapInt32 atomically stores new into *addr and returns the previous *addr value.

## func [SwapInt64](https://golang.org/src/sync/atomic/doc.go?s=1878:1928#L49)

```
func SwapInt64(addr *int64, new int64) (old int64)
```

SwapInt64 atomically stores new into *addr and returns the previous *addr value.

## func [SwapPointer](https://golang.org/src/sync/atomic/doc.go?s=2444:2523#L61)

```
func SwapPointer(addr *unsafe.Pointer, new unsafe.Pointer) (old unsafe.Pointer)
```

SwapPointer atomically stores new into *addr and returns the previous *addr value.

## func [SwapUint32](https://golang.org/src/sync/atomic/doc.go?s=2015:2069#L52)

```
func SwapUint32(addr *uint32, new uint32) (old uint32)
```

SwapUint32 atomically stores new into *addr and returns the previous *addr value.

## func [SwapUint64](https://golang.org/src/sync/atomic/doc.go?s=2156:2210#L55)

```
func SwapUint64(addr *uint64, new uint64) (old uint64)
```

SwapUint64 atomically stores new into *addr and returns the previous *addr value.

## func [SwapUintptr](https://golang.org/src/sync/atomic/doc.go?s=2298:2356#L58)

```
func SwapUintptr(addr *uintptr, new uintptr) (old uintptr)
```

SwapUintptr atomically stores new into *addr and returns the previous *addr value.

## type [Value](https://golang.org/src/sync/atomic/value.go?s=445:481#L5)

```
type Value struct {
        // contains filtered or unexported fields
}
```

A Value provides an atomic load and store of a consistently typed value. Values can be created as part of other data structures. The zero value for a Value returns nil from Load. Once Store has been called, a Value must not be copied.

▹ Example (Config)

▹ Example (ReadMostly)

### func (*Value) [Load](https://golang.org/src/sync/atomic/value.go?s=732:770#L17)

```
func (v *Value) Load() (x interface{})
```

Load returns the value set by the most recent Store. It returns nil if there has been no call to Store for this Value.

### func (*Value) [Store](https://golang.org/src/sync/atomic/value.go?s=1242:1278#L34)

```
func (v *Value) Store(x interface{})
```

Store sets the value of the Value to x. All calls to Store for a given Value must use values of the same concrete type. Store of an inconsistent type panics, as does Store(nil).

## Bugs

- [☞](https://golang.org/src/sync/atomic/doc.go?s=1207:1656#L36) On x86-32, the 64-bit functions use instructions unavailable before the Pentium MMX. On non-Linux ARM, the 64-bit functions use instructions unavailable before the ARMv6k core. On both ARM and x86-32, it is the caller's responsibility to arrange for 64-bit alignment of 64-bit words accessed atomically. The first word in a global variable or in an allocated struct or slice can be relied upon to be 64-bit aligned.

Build version go1.5.1.

来源： <<https://golang.org/pkg/sync/atomic/>>

