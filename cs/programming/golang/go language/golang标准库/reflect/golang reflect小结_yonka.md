

* `Value`

  * 和`interface{}`的差别

  * 一些“取值”/"赋值"方法

    `Int()`,`Uint()`,`Bool()`等，大致原则是：
    * 如果是对应（多个）类型之一，则转为该类类型的最大(size)值，如`Int8`,`Int16`等都转为`Int64`
    * 如果类型不对，则panic
    * 对应的赋值方法为`SetXX`，如 `SetBool(bool)`,`SetBytes([]byte)`

    * `Value.Interace() interface`返回原来的值，wrap为`interface{}`类型了
      
      ```go
      x := v.Interface()      // an interface{}
      i := x.(int)            // an int
      fmt.Printf("%d\n", i)   // "3"
      ```
      
  * 一些特定类型的操作方法(限定于实际类型为该类型，否则panic)
  
    * `Close` 顾名思义，只对于ch
    * `Bytes() []byte` 同样的，只对于`[]byte`
    * `Len() int` 只对于支持len操作的类型，如 `Array`,`Chan`,`Map`,`Slice`,`String`
    * `Cap() int` 类似的，只对 `Array`,`Chan`,`Slice`
    * `Index(i int) Value` 只对支持index操作的类型，如 `Array`,`Slice`,`String`
    * `CanAddr() bool`,`Addr() Value` 可寻址的场景： slice元素、可寻址array的元素、可寻址struct的字段、解引用指针的结果
    * `CanSet() bool`,`Set(x Value)` 可赋值(？)的场景： 可寻址且不是未导出的struct字段
    * `CanInterface() bool` 对应`Interface() interface`
    * `Recv/TryRecv() (Value, bool)`,`Send/TrySend(Value)` chan的操作
    * `Slice(int,int)`,`Slice3(int,int,int)` slice切片
    * `MapIndex(Value) Value`,`MapKeys() []Value`,`MapRange() *MapIter`,`SetMapIndex(Value,Value)` map的index、set、迭代等操作
    * `Call([]Value) []Value`,`CallSlice` 函数调用。`CallSlice`对应 `f(x, arr ...)` 风格调用
    * `Elem() Value` 取interface wrap的值 或 指针指向的值
    * `Field(int) Value`,`FieldByIndex([]int) Value`,`FieldByName()`,`FieldByNameFunc(func(string)bool) Value`,`NumField() int` 取field，`ByIndex`支持嵌套获取(如 `a.b.c`)
    * `Method(int) Value`,`MethodByName(string) Value`,`NumMethod() int` method 。。。
  
  * `Value.Type() Type`返回真实(具体)类型

    实现中value持有type指针。 不过对于method value比较特殊，这个指针是receiver的，要根据receiver类型拿到methods然后再从value的flag里拿到offset，然后。。。

  * `Value.Kind() Kind`返回类型的类别
  
    实现上，这个kind实际嵌入了value的值中

* `Type`
  
  类型信息，包含类型详细描述。
  
  * `Kind() Kind` 返回这个类型的“类别”
  * `Implements(u Type) bool` 
  * `AssignableTo(u Type) bool`
  * `ConvertibleTo(u Type) bool`
  * `Comparable() bool`



  * 特定类型相关
    * `Elem() Type` 只对 `Array`, `Chan`, `Map`, `Ptr`, or `Slice`
    * `Bits() int` 只对数字类型(`Int`,`Uint`,`Float`,`Complex`)，返回类型的长度(in bits)
    * `NumIn() int`,`NumOut() int` 
    * `chan`
      * `ChanDir() ChanDir` chan方向
    * `struct`
      * `Field(int) StructField`,`FieldByIndex([]int) StructField`,`FieldByName(string) (StructField, bool)`,`FieldByNameFunc(func(name)bool) (StructField, bool)`,`FieldAlign() int`,`NumField() int`  返回的是字段描述(而不像Value的对应的方法返回的是值)，`FieldAlign`可以获取到该字段在结构体中的byte偏移量  
  
  
* `Kind`

  感觉是 "类型的大类别"：
  
  * `Invalid`
  * `Int`,`Int8`,`Int16`,`Int32`,`Int64`
  * `Uint`,`Uint8`,`Uint16`,`Uint32`,`Uint64`,`Uintptr`
  * `Bool`
  * `String`
  * `Chan`
  * `Func`
  * `Ptr`
  * `Slice`
  * `Map`
  * `Array`
  * `Struct`
  * `Interface`

* `reflect.TypeOf`

  实际（or 具体）类型，而不是当前变量类型（如接口）。
  显然相同类型的值取到的不同type相同，也即 `reflect.TypeOf(&A{1}) == reflect.TypeOf(&A{2})`

* `reflect.ValueOf`

  ```go
  v := reflect.ValueOf(3) // a reflect.Value
  fmt.Println(v)          // "3"
  fmt.Printf("%v\n", v)   // "3"
  fmt.Println(v.String()) // NOTE: "<int Value>"
  ```

  `Value.Interace()`是其逆操作


## Value

```go
// Value is the reflection interface to a Go value.
//
// Not all methods apply to all kinds of values. Restrictions,
// if any, are noted in the documentation for each method.
// Use the Kind method to find out the kind of value before
// calling kind-specific methods. Calling a method
// inappropriate to the kind of type causes a run time panic.
//
// The zero Value represents no value.
// Its IsValid method returns false, its Kind method returns Invalid,
// its String method returns "<invalid Value>", and all other methods panic.
// Most functions and methods never return an invalid value.
// If one does, its documentation states the conditions explicitly.
//
// A Value can be used concurrently by multiple goroutines provided that
// the underlying Go value can be used concurrently for the equivalent
// direct operations.
//
// To compare two Values, compare the results of the Interface method.
// Using == on two Values does not compare the underlying values
// they represent.
type Value struct {
	// typ holds the type of the value represented by a Value.
	typ *rtype

	// Pointer-valued data or, if flagIndir is set, pointer to data.
	// Valid when either flagIndir is set or typ.pointers() is true.
	ptr unsafe.Pointer

	// flag holds metadata about the value.
	// The lowest bits are flag bits:
	//	- flagStickyRO: obtained via unexported not embedded field, so read-only
	//	- flagEmbedRO: obtained via unexported embedded field, so read-only
	//	- flagIndir: val holds a pointer to the data
	//	- flagAddr: v.CanAddr is true (implies flagIndir)
	//	- flagMethod: v is a method value.
	// The next five bits give the Kind of the value.
	// This repeats typ.Kind() except for method values.
	// The remaining 23+ bits give a method number for method values.
	// If flag.kind() != Func, code can assume that flagMethod is unset.
	// If ifaceIndir(typ), code can assume that flagIndir is set.
	flag

	// A method value represents a curried method invocation
	// like r.Read for some receiver r. The typ+val+flag bits describe
	// the receiver r, but the flag's Kind bits say Func (methods are
	// functions), and the top bits of the flag give the method number
	// in r's type's method table.
}

type flag uintptr

const (
	flagKindWidth        = 5 // there are 27 kinds
	flagKindMask    flag = 1<<flagKindWidth - 1
	flagStickyRO    flag = 1 << 5
	flagEmbedRO     flag = 1 << 6
	flagIndir       flag = 1 << 7
	flagAddr        flag = 1 << 8
	flagMethod      flag = 1 << 9
	flagMethodShift      = 10
	flagRO          flag = flagStickyRO | flagEmbedRO
)

func (f flag) kind() Kind {
	return Kind(f & flagKindMask)
}
```

## Type

```go
// Type is the representation of a Go type.
//
// Not all methods apply to all kinds of types. Restrictions,
// if any, are noted in the documentation for each method.
// Use the Kind method to find out the kind of type before
// calling kind-specific methods. Calling a method
// inappropriate to the kind of type causes a run-time panic.
//
// Type values are comparable, such as with the == operator,
// so they can be used as map keys.
// Two Type values are equal if they represent identical types.
type Type interface {
	// Methods applicable to all types.

	// Align returns the alignment in bytes of a value of
	// this type when allocated in memory.
	Align() int

	// FieldAlign returns the alignment in bytes of a value of
	// this type when used as a field in a struct.
	FieldAlign() int

	// Method returns the i'th method in the type's method set.
	// It panics if i is not in the range [0, NumMethod()).
	//
	// For a non-interface type T or *T, the returned Method's Type and Func
	// fields describe a function whose first argument is the receiver.
	//
	// For an interface type, the returned Method's Type field gives the
	// method signature, without a receiver, and the Func field is nil.
	//
	// Only exported methods are accessible and they are sorted in
	// lexicographic order.
	Method(int) Method

	// MethodByName returns the method with that name in the type's
	// method set and a boolean indicating if the method was found.
	//
	// For a non-interface type T or *T, the returned Method's Type and Func
	// fields describe a function whose first argument is the receiver.
	//
	// For an interface type, the returned Method's Type field gives the
	// method signature, without a receiver, and the Func field is nil.
	MethodByName(string) (Method, bool)

	// NumMethod returns the number of exported methods in the type's method set.
	NumMethod() int

	// Name returns the type's name within its package for a defined type.
	// For other (non-defined) types it returns the empty string.
	Name() string

	// PkgPath returns a defined type's package path, that is, the import path
	// that uniquely identifies the package, such as "encoding/base64".
	// If the type was predeclared (string, error) or not defined (*T, struct{},
	// []int, or A where A is an alias for a non-defined type), the package path
	// will be the empty string.
	PkgPath() string

	// Size returns the number of bytes needed to store
	// a value of the given type; it is analogous to unsafe.Sizeof.
	Size() uintptr

	// String returns a string representation of the type.
	// The string representation may use shortened package names
	// (e.g., base64 instead of "encoding/base64") and is not
	// guaranteed to be unique among types. To test for type identity,
	// compare the Types directly.
	String() string

	// Kind returns the specific kind of this type.
	Kind() Kind

	// Implements reports whether the type implements the interface type u.
	Implements(u Type) bool

	// AssignableTo reports whether a value of the type is assignable to type u.
	AssignableTo(u Type) bool

	// ConvertibleTo reports whether a value of the type is convertible to type u.
	ConvertibleTo(u Type) bool

	// Comparable reports whether values of this type are comparable.
	Comparable() bool

	// Methods applicable only to some types, depending on Kind.
	// The methods allowed for each kind are:
	//
	//	Int*, Uint*, Float*, Complex*: Bits
	//	Array: Elem, Len
	//	Chan: ChanDir, Elem
	//	Func: In, NumIn, Out, NumOut, IsVariadic.
	//	Map: Key, Elem
	//	Ptr: Elem
	//	Slice: Elem
	//	Struct: Field, FieldByIndex, FieldByName, FieldByNameFunc, NumField

	// Bits returns the size of the type in bits.
	// It panics if the type's Kind is not one of the
	// sized or unsized Int, Uint, Float, or Complex kinds.
	Bits() int

	// ChanDir returns a channel type's direction.
	// It panics if the type's Kind is not Chan.
	ChanDir() ChanDir

	// IsVariadic reports whether a function type's final input parameter
	// is a "..." parameter. If so, t.In(t.NumIn() - 1) returns the parameter's
	// implicit actual type []T.
	//
	// For concreteness, if t represents func(x int, y ... float64), then
	//
	//	t.NumIn() == 2
	//	t.In(0) is the reflect.Type for "int"
	//	t.In(1) is the reflect.Type for "[]float64"
	//	t.IsVariadic() == true
	//
	// IsVariadic panics if the type's Kind is not Func.
	IsVariadic() bool

	// Elem returns a type's element type.
	// It panics if the type's Kind is not Array, Chan, Map, Ptr, or Slice.
	Elem() Type

	// Field returns a struct type's i'th field.
	// It panics if the type's Kind is not Struct.
	// It panics if i is not in the range [0, NumField()).
	Field(i int) StructField

	// FieldByIndex returns the nested field corresponding
	// to the index sequence. It is equivalent to calling Field
	// successively for each index i.
	// It panics if the type's Kind is not Struct.
	FieldByIndex(index []int) StructField

	// FieldByName returns the struct field with the given name
	// and a boolean indicating if the field was found.
	FieldByName(name string) (StructField, bool)

	// FieldByNameFunc returns the struct field with a name
	// that satisfies the match function and a boolean indicating if
	// the field was found.
	//
	// FieldByNameFunc considers the fields in the struct itself
	// and then the fields in any embedded structs, in breadth first order,
	// stopping at the shallowest nesting depth containing one or more
	// fields satisfying the match function. If multiple fields at that depth
	// satisfy the match function, they cancel each other
	// and FieldByNameFunc returns no match.
	// This behavior mirrors Go's handling of name lookup in
	// structs containing embedded fields.
	FieldByNameFunc(match func(string) bool) (StructField, bool)

	// In returns the type of a function type's i'th input parameter.
	// It panics if the type's Kind is not Func.
	// It panics if i is not in the range [0, NumIn()).
	In(i int) Type

	// Key returns a map type's key type.
	// It panics if the type's Kind is not Map.
	Key() Type

	// Len returns an array type's length.
	// It panics if the type's Kind is not Array.
	Len() int

	// NumField returns a struct type's field count.
	// It panics if the type's Kind is not Struct.
	NumField() int

	// NumIn returns a function type's input parameter count.
	// It panics if the type's Kind is not Func.
	NumIn() int

	// NumOut returns a function type's output parameter count.
	// It panics if the type's Kind is not Func.
	NumOut() int

	// Out returns the type of a function type's i'th output parameter.
	// It panics if the type's Kind is not Func.
	// It panics if i is not in the range [0, NumOut()).
	Out(i int) Type

	common() *rtype
	uncommon() *uncommonType
}
```


