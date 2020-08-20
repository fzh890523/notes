

# The Go Programming Language Specification

## Version of November 11, 2014

| [Introduction](http://golang.org/ref/spec#Introduction)[Notation](http://golang.org/ref/spec#Notation)[Source code representation](http://golang.org/ref/spec#Source_code_representation)[Characters](http://golang.org/ref/spec#Characters)[Letters and digits](http://golang.org/ref/spec#Letters_and_digits)[Lexical elements](http://golang.org/ref/spec#Lexical_elements)[Comments](http://golang.org/ref/spec#Comments)[Tokens](http://golang.org/ref/spec#Tokens)[Semicolons](http://golang.org/ref/spec#Semicolons)[Identifiers](http://golang.org/ref/spec#Identifiers)[Keywords](http://golang.org/ref/spec#Keywords)[Operators and Delimiters](http://golang.org/ref/spec#Operators_and_Delimiters)[Integer literals](http://golang.org/ref/spec#Integer_literals)[Floating-point literals](http://golang.org/ref/spec#Floating-point_literals)[Imaginary literals](http://golang.org/ref/spec#Imaginary_literals)[Rune literals](http://golang.org/ref/spec#Rune_literals)[String literals](http://golang.org/ref/spec#String_literals)[Constants](http://golang.org/ref/spec#Constants)[Variables](http://golang.org/ref/spec#Variables)[Types](http://golang.org/ref/spec#Types)[Method sets](http://golang.org/ref/spec#Method_sets)[Boolean types](http://golang.org/ref/spec#Boolean_types)[Numeric types](http://golang.org/ref/spec#Numeric_types)[String types](http://golang.org/ref/spec#String_types)[Array types](http://golang.org/ref/spec#Array_types)[Slice types](http://golang.org/ref/spec#Slice_types)[Struct types](http://golang.org/ref/spec#Struct_types)[Pointer types](http://golang.org/ref/spec#Pointer_types)[Function types](http://golang.org/ref/spec#Function_types)[Interface types](http://golang.org/ref/spec#Interface_types)[Map types](http://golang.org/ref/spec#Map_types)[Channel types](http://golang.org/ref/spec#Channel_types)[Properties of types and values](http://golang.org/ref/spec#Properties_of_types_and_values)[Type identity](http://golang.org/ref/spec#Type_identity)[Assignability](http://golang.org/ref/spec#Assignability)[Blocks](http://golang.org/ref/spec#Blocks)[Declarations and scope](http://golang.org/ref/spec#Declarations_and_scope)[Label scopes](http://golang.org/ref/spec#Label_scopes)[Blank identifier](http://golang.org/ref/spec#Blank_identifier)[Predeclared identifiers](http://golang.org/ref/spec#Predeclared_identifiers)[Exported identifiers](http://golang.org/ref/spec#Exported_identifiers)[Uniqueness of identifiers](http://golang.org/ref/spec#Uniqueness_of_identifiers)[Constant declarations](http://golang.org/ref/spec#Constant_declarations)[Iota](http://golang.org/ref/spec#Iota)[Type declarations](http://golang.org/ref/spec#Type_declarations)[Variable declarations](http://golang.org/ref/spec#Variable_declarations)[Short variable declarations](http://golang.org/ref/spec#Short_variable_declarations)[Function declarations](http://golang.org/ref/spec#Function_declarations)[Method declarations](http://golang.org/ref/spec#Method_declarations)[Expressions](http://golang.org/ref/spec#Expressions)[Operands](http://golang.org/ref/spec#Operands)[Qualified identifiers](http://golang.org/ref/spec#Qualified_identifiers)[Composite literals](http://golang.org/ref/spec#Composite_literals)[Function literals](http://golang.org/ref/spec#Function_literals)[Primary expressions](http://golang.org/ref/spec#Primary_expressions)[Selectors](http://golang.org/ref/spec#Selectors)[Method expressions](http://golang.org/ref/spec#Method_expressions)[Method values](http://golang.org/ref/spec#Method_values)[Index expressions](http://golang.org/ref/spec#Index_expressions)[Slice expressions](http://golang.org/ref/spec#Slice_expressions) | [Type assertions](http://golang.org/ref/spec#Type_assertions)[Calls](http://golang.org/ref/spec#Calls)[Passing arguments to ... parameters](http://golang.org/ref/spec#Passing_arguments_to_..._parameters)[Operators](http://golang.org/ref/spec#Operators)[Operator precedence](http://golang.org/ref/spec#Operator_precedence)[Arithmetic operators](http://golang.org/ref/spec#Arithmetic_operators)[Integer overflow](http://golang.org/ref/spec#Integer_overflow)[Comparison operators](http://golang.org/ref/spec#Comparison_operators)[Logical operators](http://golang.org/ref/spec#Logical_operators)[Address operators](http://golang.org/ref/spec#Address_operators)[Receive operator](http://golang.org/ref/spec#Receive_operator)[Conversions](http://golang.org/ref/spec#Conversions)[Constant expressions](http://golang.org/ref/spec#Constant_expressions)[Order of evaluation](http://golang.org/ref/spec#Order_of_evaluation)[Statements](http://golang.org/ref/spec#Statements)[Terminating statements](http://golang.org/ref/spec#Terminating_statements)[Empty statements](http://golang.org/ref/spec#Empty_statements)[Labeled statements](http://golang.org/ref/spec#Labeled_statements)[Expression statements](http://golang.org/ref/spec#Expression_statements)[Send statements](http://golang.org/ref/spec#Send_statements)[IncDec statements](http://golang.org/ref/spec#IncDec_statements)[Assignments](http://golang.org/ref/spec#Assignments)[If statements](http://golang.org/ref/spec#If_statements)[Switch statements](http://golang.org/ref/spec#Switch_statements)[For statements](http://golang.org/ref/spec#For_statements)[Go statements](http://golang.org/ref/spec#Go_statements)[Select statements](http://golang.org/ref/spec#Select_statements)[Return statements](http://golang.org/ref/spec#Return_statements)[Break statements](http://golang.org/ref/spec#Break_statements)[Continue statements](http://golang.org/ref/spec#Continue_statements)[Goto statements](http://golang.org/ref/spec#Goto_statements)[Fallthrough statements](http://golang.org/ref/spec#Fallthrough_statements)[Defer statements](http://golang.org/ref/spec#Defer_statements)[Built-in functions](http://golang.org/ref/spec#Built-in_functions)[Close](http://golang.org/ref/spec#Close)[Length and capacity](http://golang.org/ref/spec#Length_and_capacity)[Allocation](http://golang.org/ref/spec#Allocation)[Making slices, maps and channels](http://golang.org/ref/spec#Making_slices_maps_and_channels)[Appending to and copying slices](http://golang.org/ref/spec#Appending_and_copying_slices)[Deletion of map elements](http://golang.org/ref/spec#Deletion_of_map_elements)[Manipulating complex numbers](http://golang.org/ref/spec#Complex_numbers)[Handling panics](http://golang.org/ref/spec#Handling_panics)[Bootstrapping](http://golang.org/ref/spec#Bootstrapping)[Packages](http://golang.org/ref/spec#Packages)[Source file organization](http://golang.org/ref/spec#Source_file_organization)[Package clause](http://golang.org/ref/spec#Package_clause)[Import declarations](http://golang.org/ref/spec#Import_declarations)[An example package](http://golang.org/ref/spec#An_example_package)[Program initialization and execution](http://golang.org/ref/spec#Program_initialization_and_execution)[The zero value](http://golang.org/ref/spec#The_zero_value)[Package initialization](http://golang.org/ref/spec#Package_initialization)[Program execution](http://golang.org/ref/spec#Program_execution)[Errors](http://golang.org/ref/spec#Errors)[Run-time panics](http://golang.org/ref/spec#Run_time_panics)[System considerations](http://golang.org/ref/spec#System_considerations)[Package unsafe](http://golang.org/ref/spec#Package_unsafe)[Size and alignment guarantees](http://golang.org/ref/spec#Size_and_alignment_guarantees) |
| ---------------------------------------- | ---------------------------------------- |
|                                          |                                          |

## Introduction

This is a reference manual for the Go programming language. Formore information and other documents, see [golang.org](http://golang.org/).

Go is a general-purpose language designed with systems programmingin mind. It is strongly typed and garbage-collected and has explicitsupport for concurrent programming.  Programs are constructed from*packages*, whose properties allow efficient management ofdependencies. The existing implementations use a traditionalcompile/link model to generate executable binaries.

The grammar is compact and regular, allowing for easy analysis byautomatic tools such as integrated development environments.

## Notation

The syntax is specified using Extended Backus-Naur Form (EBNF):

```
Production  = production_name "=" [ Expression ] "." .
Expression  = Alternative { "|" Alternative } .
Alternative = Term { Term } .
Term        = production_name | token [ "…" token ] | Group | Option | Repetition .
Group       = "(" Expression ")" .
Option      = "[" Expression "]" .
Repetition  = "{" Expression "}" .

```

Productions are expressions constructed from terms and the followingoperators, in increasing precedence:

```
|   alternation
()  grouping
[]  option (0 or 1 times)
{}  repetition (0 to n times)

```

Lower-case production names are used to identify lexical tokens.Non-terminals are in CamelCase. Lexical tokens are enclosed indouble quotes `""` or back quotes ````.

The form `a … b` represents the set of characters from`a` through `b` as alternatives. The horizontalellipsis `…` is also used elsewhere in the spec to informally denote variousenumerations or code snippets that are not further specified. The character `…`(as opposed to the three characters `...`) is not a token of the Golanguage.

## Source code representation

Source code is Unicode text encoded in[UTF-8](http://en.wikipedia.org/wiki/UTF-8). The text is notcanonicalized, so a single accented code point is distinct from thesame character constructed from combining an accent and a letter;those are treated as two code points.  For simplicity, this documentwill use the unqualified term *character* to refer to a Unicode code pointin the source text.

Each code point is distinct; for instance, upper and lower case lettersare different characters.

Implementation restriction: For compatibility with other tools, acompiler may disallow the NUL character (U+0000) in the source text.

Implementation restriction: For compatibility with other tools, acompiler may ignore a UTF-8-encoded byte order mark(U+FEFF) if it is the first Unicode code point in the source text.A byte order mark may be disallowed anywhere else in the source.

### Characters

The following terms are used to denote specific Unicode character classes:

```
newline        = /* the Unicode code point U+000A */ .
unicode_char   = /* an arbitrary Unicode code point except newline */ .
unicode_letter = /* a Unicode code point classified as "Letter" */ .
unicode_digit  = /* a Unicode code point classified as "Decimal Digit" */ .

```

In [The Unicode Standard 6.3](http://www.unicode.org/versions/Unicode6.3.0/),Section 4.5 "General Category"defines a set of character categories.  Go treatsthose characters in category Lu, Ll, Lt, Lm, or Lo as Unicode letters,and those in category Nd as Unicode digits.

### Letters and digits

The underscore character `_` (U+005F) is considered a letter.

```
letter        = unicode_letter | "_" .
decimal_digit = "0" … "9" .
octal_digit   = "0" … "7" .
hex_digit     = "0" … "9" | "A" … "F" | "a" … "f" .

```

## Lexical elements

### Comments

There are two forms of comments:

1. *Line comments* start with the character sequence `//`and stop at the end of the line. A line comment acts like a newline.
2. *General comments* start with the character sequence `/*`and continue through the character sequence `*/`. A generalcomment containing one or more newlines acts like a newline, otherwise it actslike a space.

Comments do not nest.

### Tokens

Tokens form the vocabulary of the Go language.There are four classes: *identifiers*, *keywords*, *operatorsand delimiters*, and *literals*.  *White space*, formed fromspaces (U+0020), horizontal tabs (U+0009),carriage returns (U+000D), and newlines (U+000A),is ignored except as it separates tokensthat would otherwise combine into a single token. Also, a newline or end of filemay trigger the insertion of a [semicolon](http://golang.org/ref/spec#Semicolons).While breaking the input into tokens,the next token is the longest sequence of characters that form avalid token.

### Semicolons

The formal grammar uses semicolons `";"` as terminators ina number of productions. Go programs may omit most of these semicolonsusing the following two rules:

1. When the input is broken into tokens, a semicolon is automatically insertedinto the token stream at the end of a non-blank line if the line's finaltoken is
   - an	    [identifier](http://golang.org/ref/spec#Identifiers)
   - an	    [integer](http://golang.org/ref/spec#Integer_literals),	    [floating-point](http://golang.org/ref/spec#Floating-point_literals),	    [imaginary](http://golang.org/ref/spec#Imaginary_literals),	    [rune](http://golang.org/ref/spec#Rune_literals), or	    [string](http://golang.org/ref/spec#String_literals) literal	
   - one of the [keywords](http://golang.org/ref/spec#Keywords)`break`,	    `continue`,	    `fallthrough`, or	    `return`
   - one of the [operators and delimiters](http://golang.org/ref/spec#Operators_and_Delimiters)`++`,	    `--`,	    `)`,	    `]`, or	    `}`
2. To allow complex statements to occupy a single line, a semicolonmay be omitted before a closing `")"` or `"}"`.

To reflect idiomatic use, code examples in this document elide semicolonsusing these rules.

### Identifiers

Identifiers name program entities such as variables and types.An identifier is a sequence of one or more letters and digits.The first character in an identifier must be a letter.

```
identifier = letter { letter | unicode_digit } .

```

```
a
_x9
ThisVariableIsExported
αβ

```

Some identifiers are [predeclared](http://golang.org/ref/spec#Predeclared_identifiers).

### Keywords

The following keywords are reserved and may not be used as identifiers.

```
break        default      func         interface    select
case         defer        go           map          struct
chan         else         goto         package      switch
const        fallthrough  if           range        type
continue     for          import       return       var

```

### Operators and Delimiters

The following character sequences represent [operators](http://golang.org/ref/spec#Operators), delimiters, and other special tokens:

```
+    &     +=    &=     &&    ==    !=    (    )
-    |     -=    |=     ||    <     <=    [    ]
*    ^     *=    ^=     <-    >     >=    {    }
/    <<    /=    <<=    ++    =     :=    ,    ;
%    >>    %=    >>=    --    !     ...   .    :
     &^          &^=

```

### Integer literals

An integer literal is a sequence of digits representing an[integer constant](http://golang.org/ref/spec#Constants).An optional prefix sets a non-decimal base: `0` for octal, `0x` or`0X` for hexadecimal.  In hexadecimal literals, letters`a-f` and `A-F` represent values 10 through 15.

```
int_lit     = decimal_lit | octal_lit | hex_lit .
decimal_lit = ( "1" … "9" ) { decimal_digit } .
octal_lit   = "0" { octal_digit } .
hex_lit     = "0" ( "x" | "X" ) hex_digit { hex_digit } .

```

```
42
0600
0xBadFace
170141183460469231731687303715884105727

```

### Floating-point literals

A floating-point literal is a decimal representation of a[floating-point constant](http://golang.org/ref/spec#Constants).It has an integer part, a decimal point, a fractional part,and an exponent part.  The integer and fractional part comprisedecimal digits; the exponent part is an `e` or `E`followed by an optionally signed decimal exponent.  One of theinteger part or the fractional part may be elided; one of the decimalpoint or the exponent may be elided.

```
float_lit = decimals "." [ decimals ] [ exponent ] |
            decimals exponent |
            "." decimals [ exponent ] .
decimals  = decimal_digit { decimal_digit } .
exponent  = ( "e" | "E" ) [ "+" | "-" ] decimals .

```

```
0.
72.40
072.40  // == 72.40
2.71828
1.e+0
6.67428e-11
1E6
.25
.12345E+5

```

### Imaginary literals

An imaginary literal is a decimal representation of the imaginary part of a[complex constant](http://golang.org/ref/spec#Constants).It consists of a[floating-point literal](http://golang.org/ref/spec#Floating-point_literals)or decimal integer followedby the lower-case letter `i`.

```
imaginary_lit = (decimals | float_lit) "i" .

```

```
0i
011i  // == 11i
0.i
2.71828i
1.e+0i
6.67428e-11i
1E6i
.25i
.12345E+5i

```

### Rune literals

A rune literal represents a [rune constant](http://golang.org/ref/spec#Constants),an integer value identifying a Unicode code point.A rune literal is expressed as one or more characters enclosed in single quotes.Within the quotes, any character may appear except singlequote and newline. A single quoted character represents the Unicode valueof the character itself,while multi-character sequences beginning with a backslash encodevalues in various formats.

The simplest form represents the single character within the quotes;since Go source text is Unicode characters encoded in UTF-8, multipleUTF-8-encoded bytes may represent a single integer value.  Forinstance, the literal `'a'` holds a single byte representinga literal `a`, Unicode U+0061, value `0x61`, while`'ä'` holds two bytes (`0xc3` `0xa4`) representinga literal `a`-dieresis, U+00E4, value `0xe4`.

Several backslash escapes allow arbitrary values to be encoded asASCII text.  There are four ways to represent the integer valueas a numeric constant: `\x` followed by exactly two hexadecimaldigits; `\u` followed by exactly four hexadecimal digits;`\U` followed by exactly eight hexadecimal digits, and aplain backslash `\` followed by exactly three octal digits.In each case the value of the literal is the value represented bythe digits in the corresponding base.

Although these representations all result in an integer, they havedifferent valid ranges.  Octal escapes must represent a value between0 and 255 inclusive.  Hexadecimal escapes satisfy this conditionby construction. The escapes `\u` and `\U`represent Unicode code points so within them some values are illegal,in particular those above `0x10FFFF` and surrogate halves.

After a backslash, certain single-character escapes represent special values:

```
\a   U+0007 alert or bell
\b   U+0008 backspace
\f   U+000C form feed
\n   U+000A line feed or newline
\r   U+000D carriage return
\t   U+0009 horizontal tab
\v   U+000b vertical tab
\\   U+005c backslash
\'   U+0027 single quote  (valid escape only within rune literals)
\"   U+0022 double quote  (valid escape only within string literals)

```

All other sequences starting with a backslash are illegal inside rune literals.

```
rune_lit         = "'" ( unicode_value | byte_value ) "'" .
unicode_value    = unicode_char | little_u_value | big_u_value | escaped_char .
byte_value       = octal_byte_value | hex_byte_value .
octal_byte_value = `\` octal_digit octal_digit octal_digit .
hex_byte_value   = `\` "x" hex_digit hex_digit .
little_u_value   = `\` "u" hex_digit hex_digit hex_digit hex_digit .
big_u_value      = `\` "U" hex_digit hex_digit hex_digit hex_digithex_digit hex_digit hex_digit hex_digit .
escaped_char     = `\` ( "a" | "b" | "f" | "n" | "r" | "t" | "v" | `\` | "'" | `"` ) .

```

```
'a'
'ä'
'本'
'\t'
'\000'
'\007'
'\377'
'\x07'
'\xff'
'\u12e4'
'\U00101234'
'aa'         // illegal: too many characters
'\xa'        // illegal: too few hexadecimal digits
'\0'         // illegal: too few octal digits
'\uDFFF'     // illegal: surrogate half
'\U00110000' // illegal: invalid Unicode code point

```

### String literals

A string literal represents a [string constant](http://golang.org/ref/spec#Constants)obtained from concatenating a sequence of characters. There are two forms:raw string literals and interpreted string literals.

Raw string literals are character sequences between back quotes````.  Within the quotes, any character is legal exceptback quote. The value of a raw string literal is thestring composed of the uninterpreted (implicitly UTF-8-encoded) charactersbetween the quotes;in particular, backslashes have no special meaning and the string maycontain newlines.Carriage return characters ('\r') inside raw string literalsare discarded from the raw string value.

Interpreted string literals are character sequences between doublequotes `""`. The text between the quotes,which may not contain newlines, forms thevalue of the literal, with backslash escapes interpreted as theyare in [rune literals](http://golang.org/ref/spec#Rune_literals) (except that `\'` is illegal and`\"` is legal), with the same restrictions.The three-digit octal (`\`*nnn*)and two-digit hexadecimal (`\x`*nn*) escapes represent individual*bytes* of the resulting string; all other escapes representthe (possibly multi-byte) UTF-8 encoding of individual *characters*.Thus inside a string literal `\377` and `\xFF` representa single byte of value `0xFF`=255, while `ÿ`,`\u00FF`, `\U000000FF` and `\xc3\xbf` representthe two bytes `0xc3` `0xbf` of the UTF-8 encoding of characterU+00FF.

```
string_lit             = raw_string_lit | interpreted_string_lit .
raw_string_lit         = "`" { unicode_char | newline } "`" .
interpreted_string_lit = `"` { unicode_value | byte_value } `"` .

```

```
`abc`  // same as "abc"
`\n
\n`    // same as "\\n\n\\n"
"\n"
""
"Hello, world!\n"
"日本語"
"\u65e5本\U00008a9e"
"\xff\u00FF"
"\uD800"       // illegal: surrogate half
"\U00110000"   // illegal: invalid Unicode code point

```

These examples all represent the same string:

```
"日本語"                                 // UTF-8 input text
`日本語`                                 // UTF-8 input text as a raw literal
"\u65e5\u672c\u8a9e"                    // the explicit Unicode code points
"\U000065e5\U0000672c\U00008a9e"        // the explicit Unicode code points
"\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e"  // the explicit UTF-8 bytes

```

If the source code represents a character as two code points, such asa combining form involving an accent and a letter, the result will bean error if placed in a rune literal (it is not a single codepoint), and will appear as two code points if placed in a stringliteral.

## Constants

There are *boolean constants*,*rune constants*,*integer constants*,*floating-point constants*, *complex constants*,and *string constants*. Rune, integer, floating-point,and complex constants arecollectively called *numeric constants*.

A constant value is represented by a[rune](http://golang.org/ref/spec#Rune_literals),[integer](http://golang.org/ref/spec#Integer_literals),[floating-point](http://golang.org/ref/spec#Floating-point_literals),[imaginary](http://golang.org/ref/spec#Imaginary_literals),or[string](http://golang.org/ref/spec#String_literals) literal,an identifier denoting a constant,a [constant expression](http://golang.org/ref/spec#Constant_expressions),a [conversion](http://golang.org/ref/spec#Conversions) with a result that is a constant, orthe result value of some built-in functions such as`unsafe.Sizeof` applied to any value,`cap` or `len` applied to[some expressions](http://golang.org/ref/spec#Length_and_capacity),`real` and `imag` applied to a complex constantand `complex` applied to numeric constants.The boolean truth values are represented by the predeclared constants`true` and `false`. The predeclared identifier[iota](http://golang.org/ref/spec#Iota) denotes an integer constant.

In general, complex constants are a form of[constant expression](http://golang.org/ref/spec#Constant_expressions)and are discussed in that section.

Numeric constants represent values of arbitrary precision and do not overflow.

Constants may be [typed](http://golang.org/ref/spec#Types) or *untyped*.Literal constants, `true`, `false`, `iota`,and certain [constant expressions](http://golang.org/ref/spec#Constant_expressions)containing only untyped constant operands are untyped.

A constant may be given a type explicitly by a [constant declaration](http://golang.org/ref/spec#Constant_declarations)or [conversion](http://golang.org/ref/spec#Conversions), or implicitly when used in a[variable declaration](http://golang.org/ref/spec#Variable_declarations) or an[assignment](http://golang.org/ref/spec#Assignments) or as anoperand in an [expression](http://golang.org/ref/spec#Expressions).It is an error if the constant valuecannot be represented as a value of the respective type.For instance, `3.0` can be given any integer or anyfloating-point type, while `2147483648.0` (equal to `1<<31`)can be given the types `float32`, `float64`, or `uint32` butnot `int32` or `string`.

An untyped constant has a *default type* which is the type to which theconstant is implicitly converted in contexts where a typed value is required,for instance, in a [short variable declaration](http://golang.org/ref/spec#Short_variable_declarations)such as `i := 0` where there is no explicit type.The default type of an untyped constant is `bool`, `rune`,`int`, `float64`, `complex128` or `string`respectively, depending on whether it is a boolean, rune, integer, floating-point,complex, or string constant.

There are no constants denoting the IEEE-754 infinity and not-a-number values,but the [`math` package](http://golang.org/pkg/math/)'s[Inf](http://golang.org/pkg/math/#Inf),[NaN](http://golang.org/pkg/math/#NaN),[IsInf](http://golang.org/pkg/math/#IsInf), and[IsNaN](http://golang.org/pkg/math/#IsNaN)functions return and test for those values at run time.

Implementation restriction: Although numeric constants have arbitraryprecision in the language, a compiler may implement them using aninternal representation with limited precision.  That said, everyimplementation must:

- Represent integer constants with at least 256 bits.
- Represent floating-point constants, including the parts of	    a complex constant, with a mantissa of at least 256 bits	    and a signed exponent of at least 32 bits.
- Give an error if unable to represent an integer constant	    precisely.
- Give an error if unable to represent a floating-point or	    complex constant due to overflow.
- Round to the nearest representable constant if unable to	    represent a floating-point or complex constant due to limits	    on precision.

These requirements apply both to literal constants and to the resultof evaluating [constantexpressions](http://golang.org/ref/spec#Constant_expressions).

## Variables

A variable is a storage location for holding a *value*.The set of permissible values is determined by thevariable's *type*.

A [variable declaration](http://golang.org/ref/spec#Variable_declarations)or, for function parameters and results, the signatureof a [function declaration](http://golang.org/ref/spec#Function_declarations)or [function literal](http://golang.org/ref/spec#Function_literals) reservesstorage for a named variable.Calling the built-in function [`new`](http://golang.org/ref/spec#Allocation)or taking the address of a [composite literal](http://golang.org/ref/spec#Composite_literals)allocates storage for a variable at run time.Such an anonymous variable is referred to via a (possibly implicit)[pointer indirection](http://golang.org/ref/spec#Address_operators).

*Structured* variables of [array](http://golang.org/ref/spec#Array_types), [slice](http://golang.org/ref/spec#Slice_types),and [struct](http://golang.org/ref/spec#Struct_types) types have elements and fields that maybe [addressed](http://golang.org/ref/spec#Address_operators) individually. Each such elementacts like a variable.

The *static type* (or just *type*) of a variable is the	type given in its declaration, the type provided in the`new` call or composite literal, or the type ofan element of a structured variable.Variables of interface type also have a distinct *dynamic type*,which is the concrete type of the value assigned to the variable at run time(unless the value is the predeclared identifier `nil`,which has no type).The dynamic type may vary during execution but values stored in interfacevariables are always [assignable](http://golang.org/ref/spec#Assignability)to the static type of the variable.	

```
var x interface{}  // x is nil and has static type interface{}
var v *T           // v has value nil, static type *T
x = 42             // x has value 42 and dynamic type int
x = v              // x has value (*T)(nil) and dynamic type *T

```

A variable's value is retrieved by referring to the variable in an[expression](http://golang.org/ref/spec#Expressions); it is the most recent value[assigned](http://golang.org/ref/spec#Assignments) to the variable.If a variable has not yet been assigned a value, its value is the[zero value](http://golang.org/ref/spec#The_zero_value) for its type.

## Types

A type determines the set of values and operations specific to values of thattype. Types may be *named* or *unnamed*. Named types are specifiedby a (possibly [qualified](http://golang.org/ref/spec#Qualified_identifiers))[*type name*](http://golang.org/ref/spec#Type_declarations); unnamed types are specifiedusing a *type literal*, which composes a new type from existing types.

```
Type      = TypeName | TypeLit | "(" Type ")" .
TypeName  = identifier | QualifiedIdent .
TypeLit   = ArrayType | StructType | PointerType | FunctionType | InterfaceType |
	    SliceType | MapType | ChannelType .

```

Named instances of the boolean, numeric, and string types are[predeclared](http://golang.org/ref/spec#Predeclared_identifiers).*Composite types*—array, struct, pointer, function,interface, slice, map, and channel types—may be constructed usingtype literals.

Each type `T` has an *underlying type*: If `T`is one of the predeclared boolean, numeric, or string types, or a type literal,the corresponding underlyingtype is `T` itself. Otherwise, `T`'s underlying typeis the underlying type of the type to which `T` refers in its[type declaration](http://golang.org/ref/spec#Type_declarations).

```
   type T1 string
   type T2 T1
   type T3 []T1
   type T4 T3

```

The underlying type of `string`, `T1`, and `T2`is `string`. The underlying type of `[]T1`, `T3`,and `T4` is `[]T1`.

### Method sets

A type may have a *method set* associated with it.The method set of an [interface type](http://golang.org/ref/spec#Interface_types) is its interface.The method set of any other type `T` consists of all[methods](http://golang.org/ref/spec#Method_declarations) declared with receiver type `T`.The method set of the corresponding [pointer type](http://golang.org/ref/spec#Pointer_types) `*T`is the set of all methods declared with receiver `*T` or `T`(that is, it also contains the method set of `T`).Further rules apply to structs containing anonymous fields, as describedin the section on [struct types](http://golang.org/ref/spec#Struct_types).Any other type has an empty method set.In a method set, each method must have a[unique](http://golang.org/ref/spec#Uniqueness_of_identifiers)non-[blank](http://golang.org/ref/spec#Blank_identifier) [method name](http://golang.org/ref/spec#MethodName).

The method set of a type determines the interfaces that thetype [implements](http://golang.org/ref/spec#Interface_types)and the methods that can be [called](http://golang.org/ref/spec#Calls)using a receiver of that type.

### Boolean types

A *boolean type* represents the set of Boolean truth valuesdenoted by the predeclared constants `true`and `false`. The predeclared boolean type is `bool`.

### Numeric types

A *numeric type* represents sets of integer or floating-point values.The predeclared architecture-independent numeric types are:

```
uint8       the set of all unsigned  8-bit integers (0 to 255)
uint16      the set of all unsigned 16-bit integers (0 to 65535)
uint32      the set of all unsigned 32-bit integers (0 to 4294967295)
uint64      the set of all unsigned 64-bit integers (0 to 18446744073709551615)

int8        the set of all signed  8-bit integers (-128 to 127)
int16       the set of all signed 16-bit integers (-32768 to 32767)
int32       the set of all signed 32-bit integers (-2147483648 to 2147483647)
int64       the set of all signed 64-bit integers (-9223372036854775808 to 9223372036854775807)

float32     the set of all IEEE-754 32-bit floating-point numbers
float64     the set of all IEEE-754 64-bit floating-point numbers

complex64   the set of all complex numbers with float32 real and imaginary parts
complex128  the set of all complex numbers with float64 real and imaginary parts

byte        alias for uint8
rune        alias for int32

```

The value of an *n*-bit integer is *n* bits wide and represented using[two's complement arithmetic](http://en.wikipedia.org/wiki/Two%27s_complement).

There is also a set of predeclared numeric types with implementation-specific sizes:

```
uint     either 32 or 64 bits
int      same size as uint
uintptr  an unsigned integer large enough to store the uninterpreted bits of a pointer value

```

To avoid portability issues all numeric types are distinct except`byte`, which is an alias for `uint8`, and`rune`, which is an alias for `int32`.Conversionsare required when different numeric types are mixed in an expressionor assignment. For instance, `int32` and `int`are not the same type even though they may have the same size on aparticular architecture.

### String types

A *string type* represents the set of string values.A string value is a (possibly empty) sequence of bytes.Strings are immutable: once created,it is impossible to change the contents of a string.The predeclared string type is `string`.

The length of a string `s` (its size in bytes) can be discovered usingthe built-in function [`len`](http://golang.org/ref/spec#Length_and_capacity).The length is a compile-time constant if the string is a constant.A string's bytes can be accessed by integer [indices](http://golang.org/ref/spec#Index_expressions)0 through `len(s)-1`.It is illegal to take the address of such an element; if`s[i]` is the `i`'th byte of astring, `&s[i]` is invalid.

### Array types

An array is a numbered sequence of elements of a singletype, called the element type.The number of elements is called the length and is nevernegative.

```
ArrayType   = "[" ArrayLength "]" ElementType .
ArrayLength = Expression .
ElementType = Type .

```

The length is part of the array's type; it must evaluate to anon-negative [constant](http://golang.org/ref/spec#Constants) representable by a valueof type `int`.The length of array `a` can be discoveredusing the built-in function [`len`](http://golang.org/ref/spec#Length_and_capacity).The elements can be addressed by integer [indices](http://golang.org/ref/spec#Index_expressions)0 through `len(a)-1`.Array types are always one-dimensional but may be composed to formmulti-dimensional types.

```
[32]byte
[2*N] struct { x, y int32 }
[1000]*float64
[3][5]int
[2][2][2]float64  // same as [2]([2]([2]float64))

```

### Slice types

A slice is a descriptor for a contiguous segment of an *underlying array* andprovides access to a numbered sequence of elements from that array.A slice type denotes the set of all slices of arrays of its element type.The value of an uninitialized slice is `nil`.

```
SliceType = "[" "]" ElementType .

```

Like arrays, slices are indexable and have a length.  The length of aslice `s` can be discovered by the built-in function[`len`](http://golang.org/ref/spec#Length_and_capacity); unlike with arrays it may change duringexecution.  The elements can be addressed by integer [indices](http://golang.org/ref/spec#Index_expressions)0 through `len(s)-1`.  The slice index of agiven element may be less than the index of the same element in theunderlying array.

A slice, once initialized, is always associated with an underlyingarray that holds its elements.  A slice therefore shares storagewith its array and with other slices of the same array; by contrast,distinct arrays always represent distinct storage.

The array underlying a slice may extend past the end of the slice.The *capacity* is a measure of that extent: it is the sum ofthe length of the slice and the length of the array beyond the slice;a slice of length up to that capacity can be created by[*slicing*](http://golang.org/ref/spec#Slice_expressions) a new one from the original slice.The capacity of a slice `a` can be discovered using thebuilt-in function [`cap(a)`](http://golang.org/ref/spec#Length_and_capacity).

A new, initialized slice value for a given element type `T` ismade using the built-in function[`make`](http://golang.org/ref/spec#Making_slices_maps_and_channels),which takes a slice typeand parameters specifying the length and optionally the capacity.A slice created with `make` always allocates a new, hidden arrayto which the returned slice value refers. That is, executing

```
make([]T, length, capacity)

```

produces the same slice as allocating an array and [slicing](http://golang.org/ref/spec#Slice_expressions)it, so these two expressions are equivalent:

```
make([]int, 50, 100)
new([100]int)[0:50]

```

Like arrays, slices are always one-dimensional but may be composed to constructhigher-dimensional objects.With arrays of arrays, the inner arrays are, by construction, always the same length;however with slices of slices (or arrays of slices), the inner lengths may vary dynamically.Moreover, the inner slices must be initialized individually.

### Struct types

A struct is a sequence of named elements, called fields, each of which has aname and a type. Field names may be specified explicitly (IdentifierList) orimplicitly (AnonymousField).Within a struct, non-[blank](http://golang.org/ref/spec#Blank_identifier) field names mustbe [unique](http://golang.org/ref/spec#Uniqueness_of_identifiers).

```
StructType     = "struct" "{" { FieldDecl ";" } "}" .
FieldDecl      = (IdentifierList Type | AnonymousField) [ Tag ] .
AnonymousField = [ "*" ] TypeName .
Tag            = string_lit .

```

```
// An empty struct.
struct {}

// A struct with 6 fields.
struct {
	x, y int
	u float32
	_ float32  // padding
	A *[]int
	F func()
}

```

A field declared with a type but no explicit field name is an *anonymous field*,also called an *embedded* field or an embedding of the type in the struct.An embedded type must be specified asa type name `T` or as a pointer to a non-interface type name `*T`,and `T` itself may not bea pointer type. The unqualified type name acts as the field name.

```
// A struct with four anonymous fields of type T1, *T2, P.T3 and *P.T4
struct {
	T1        // field name is T1
	*T2       // field name is T2
	P.T3      // field name is T3
	*P.T4     // field name is T4
	x, y int  // field names are x and y
}

```

The following declaration is illegal because field names must be uniquein a struct type:

```
struct {
	T     // conflicts with anonymous field *T and *P.T
	*T    // conflicts with anonymous field T and *P.T
	*P.T  // conflicts with anonymous field T and *T
}

```

A field or [method](http://golang.org/ref/spec#Method_declarations) `f` of ananonymous field in a struct `x` is called *promoted* if`x.f` is a legal [selector](http://golang.org/ref/spec#Selectors) that denotesthat field or method `f`.

Promoted fields act like ordinary fieldsof a struct except that they cannot be used as field names in[composite literals](http://golang.org/ref/spec#Composite_literals) of the struct.

Given a struct type `S` and a type named `T`,promoted methods are included in the method set of the struct as follows:

- ​	If `S` contains an anonymous field `T`,	the [method sets](http://golang.org/ref/spec#Method_sets) of `S`	and `*S` both include promoted methods with receiver	`T`. The method set of `*S` also	includes promoted methods with receiver `*T`.	
- ​	If `S` contains an anonymous field `*T`,	the method sets of `S` and `*S` both	include promoted methods with receiver `T` or	`*T`.	

A field declaration may be followed by an optional string literal *tag*,which becomes an attribute for all the fields in the correspondingfield declaration. The tags are madevisible through a [reflection interface](http://golang.org/pkg/reflect/#StructTag)and take part in [type identity](http://golang.org/ref/spec#Type_identity) for structsbut are otherwise ignored.

```
// A struct corresponding to the TimeStamp protocol buffer.
// The tag strings define the protocol buffer field numbers.
struct {
	microsec  uint64 "field 1"
	serverIP6 uint64 "field 2"
	process   string "field 3"
}

```

### Pointer types

A pointer type denotes the set of all pointers to [variables](http://golang.org/ref/spec#Variables) of a giventype, called the *base type* of the pointer.The value of an uninitialized pointer is `nil`.

```
PointerType = "*" BaseType .
BaseType    = Type .

```

```
*Point
*[4]int

```

### Function types

A function type denotes the set of all functions with the same parameterand result types. The value of an uninitialized variable of function typeis `nil`.

```
FunctionType   = "func" Signature .
Signature      = Parameters [ Result ] .
Result         = Parameters | Type .
Parameters     = "(" [ ParameterList [ "," ] ] ")" .
ParameterList  = ParameterDecl { "," ParameterDecl } .
ParameterDecl  = [ IdentifierList ] [ "..." ] Type .

```

Within a list of parameters or results, the names (IdentifierList)must either all be present or all be absent. If present, each namestands for one item (parameter or result) of the specified type andall non-[blank](http://golang.org/ref/spec#Blank_identifier) names in the signaturemust be [unique](http://golang.org/ref/spec#Uniqueness_of_identifiers).If absent, each type stands for one item of that type.Parameter and resultlists are always parenthesized except that if there is exactlyone unnamed result it may be written as an unparenthesized type.

The final parameter in a function signature may havea type prefixed with `...`.A function with such a parameter is called *variadic* andmay be invoked with zero or more arguments for that parameter.

```
func()
func(x int) int
func(a, _ int, z float32) bool
func(a, b int, z float32) (bool)
func(prefix string, values ...int)
func(a, b int, z float64, opt ...interface{}) (success bool)
func(int, int, float64) (float64, *[]int)
func(n int) func(p *T)

```

### Interface types

An interface type specifies a [method set](http://golang.org/ref/spec#Method_sets) called its *interface*.A variable of interface type can store a value of any type with a method setthat is any superset of the interface. Such a type is said to*implement the interface*.The value of an uninitialized variable of interface type is `nil`.

```
InterfaceType      = "interface" "{" { MethodSpec ";" } "}" .
MethodSpec         = MethodName Signature | InterfaceTypeName .
MethodName         = identifier .
InterfaceTypeName  = TypeName .

```

As with all method sets, in an interface type, each method must have a[unique](http://golang.org/ref/spec#Uniqueness_of_identifiers)non-[blank](http://golang.org/ref/spec#Blank_identifier) name.

```
// A simple File interface
interface {
	Read(b Buffer) bool
	Write(b Buffer) bool
	Close()
}

```

More than one type may implement an interface.For instance, if two types `S1` and `S2`have the method set

```
func (p T) Read(b Buffer) bool { return … }
func (p T) Write(b Buffer) bool { return … }
func (p T) Close() { … }

```

(where `T` stands for either `S1` or `S2`)then the `File` interface is implemented by both `S1` and`S2`, regardless of what other methods`S1` and `S2` may have or share.

A type implements any interface comprising any subset of its methodsand may therefore implement several distinct interfaces. Forinstance, all types implement the *empty interface*:

```
interface{}

```

Similarly, consider this interface specification,which appears within a [type declaration](http://golang.org/ref/spec#Type_declarations)to define an interface called `Locker`:

```
type Locker interface {
	Lock()
	Unlock()
}

```

If `S1` and `S2` also implement

```
func (p T) Lock() { … }
func (p T) Unlock() { … }

```

they implement the `Locker` interface as wellas the `File` interface.

An interface `T` may use a (possibly qualified) interface typename `E` in place of a method specification. This is called*embedding* interface `E` in `T`; it addsall (exported and non-exported) methods of `E` to the interface`T`.

```
type ReadWriter interface {
	Read(b Buffer) bool
	Write(b Buffer) bool
}

type File interface {
	ReadWriter  // same as adding the methods of ReadWriter
	Locker      // same as adding the methods of Locker
	Close()
}

type LockedFile interface {
	Locker
	File        // illegal: Lock, Unlock not unique
	Lock()      // illegal: Lock not unique
}

```

An interface type `T` may not embed itselfor any interface type that embeds `T`, recursively.

```
// illegal: Bad cannot embed itself
type Bad interface {
	Bad
}

// illegal: Bad1 cannot embed itself using Bad2
type Bad1 interface {
	Bad2
}
type Bad2 interface {
	Bad1
}

```

### Map types

A map is an unordered group of elements of one type, called theelement type, indexed by a set of unique *keys* of another type,called the key type.The value of an uninitialized map is `nil`.

```
MapType     = "map" "[" KeyType "]" ElementType .
KeyType     = Type .

```

The [comparison operators](http://golang.org/ref/spec#Comparison_operators)`==` and `!=` must be fully definedfor operands of the key type; thus the key type must not be a function, map, orslice.If the key type is an interface type, thesecomparison operators must be defined for the dynamic key values;failure will cause a [run-time panic](http://golang.org/ref/spec#Run_time_panics).

```
map[string]int
map[*T]struct{ x, y float64 }
map[string]interface{}

```

The number of map elements is called its length.For a map `m`, it can be discovered using thebuilt-in function [`len`](http://golang.org/ref/spec#Length_and_capacity)and may change during execution. Elements may be added during executionusing [assignments](http://golang.org/ref/spec#Assignments) and retrieved with[index expressions](http://golang.org/ref/spec#Index_expressions); they may be removed with the[`delete`](http://golang.org/ref/spec#Deletion_of_map_elements) built-in function.

A new, empty map value is made using the built-infunction [`make`](http://golang.org/ref/spec#Making_slices_maps_and_channels),which takes the map type and an optional capacity hint as arguments:

```
make(map[string]int)
make(map[string]int, 100)

```

The initial capacity does not bound its size:maps grow to accommodate the number of itemsstored in them, with the exception of `nil` maps.A `nil` map is equivalent to an empty map except that no elementsmay be added.

### Channel types

A channel provides a mechanism for[concurrently executing functions](http://golang.org/ref/spec#Go_statements)to communicate by[sending](http://golang.org/ref/spec#Send_statements) and[receiving](http://golang.org/ref/spec#Receive_operator)values of a specified element type.The value of an uninitialized channel is `nil`.

```
ChannelType = ( "chan" | "chan" "<-" | "<-" "chan" ) ElementType .

```

The optional `<-` operator specifies the channel *direction*,*send* or *receive*. If no direction is given, the channel is*bidirectional*.A channel may be constrained only to send or only to receive by[conversion](http://golang.org/ref/spec#Conversions) or [assignment](http://golang.org/ref/spec#Assignments).

```
chan T          // can be used to send and receive values of type T
chan<- float64  // can only be used to send float64s
<-chan int      // can only be used to receive ints

```

The `<-` operator associates with the leftmost `chan`possible:

```
chan<- chan int    // same as chan<- (chan int)
chan<- <-chan int  // same as chan<- (<-chan int)
<-chan <-chan int  // same as <-chan (<-chan int)
chan (<-chan int)

```

A new, initialized channelvalue can be made using the built-in function[`make`](http://golang.org/ref/spec#Making_slices_maps_and_channels),which takes the channel type and an optional *capacity* as arguments:

```
make(chan int, 100)

```

The capacity, in number of elements, sets the size of the buffer in the channel.If the capacity is zero or absent, the channel is unbuffered and communicationsucceeds only when both a sender and receiver are ready. Otherwise, the channelis buffered and communication succeeds without blocking if the bufferis not full (sends) or not empty (receives).A `nil` channel is never ready for communication.

A channel may be closed with the built-in function[`close`](http://golang.org/ref/spec#Close).The multi-valued assignment form of the[receive operator](http://golang.org/ref/spec#Receive_operator)reports whether a received value was sent beforethe channel was closed.

A single channel may be used in[send statements](http://golang.org/ref/spec#Send_statements),[receive operations](http://golang.org/ref/spec#Receive_operator),and calls to the built-in functions[`cap`](http://golang.org/ref/spec#Length_and_capacity) and[`len`](http://golang.org/ref/spec#Length_and_capacity)by any number of goroutines without further synchronization.Channels act as first-in-first-out queues.For example, if one goroutine sends values on a channeland a second goroutine receives them, the values arereceived in the order sent.

## Properties of types and values

### Type identity

Two types are either *identical* or *different*.

Two [named types](http://golang.org/ref/spec#Types) are identical if their type names originate in the same[TypeSpec](http://golang.org/ref/spec#Type_declarations).A named and an [unnamed type](http://golang.org/ref/spec#Types) are always different. Two unnamed types are identicalif the corresponding type literals are identical, that is, if they have the sameliteral structure and corresponding components have identical types. In detail:

- Two array types are identical if they have identical element types and	    the same array length.
- Two slice types are identical if they have identical element types.
- Two struct types are identical if they have the same sequence of fields,	    and if corresponding fields have the same names, and identical types,	    and identical tags.	    Two anonymous fields are considered to have the same name. Lower-case field	    names from different packages are always different.
- Two pointer types are identical if they have identical base types.
- Two function types are identical if they have the same number of parameters	    and result values, corresponding parameter and result types are	    identical, and either both functions are variadic or neither is.	    Parameter and result names are not required to match.
- Two interface types are identical if they have the same set of methods	    with the same names and identical function types. Lower-case method names from	    different packages are always different. The order of the methods is irrelevant.
- Two map types are identical if they have identical key and value types.
- Two channel types are identical if they have identical value types and	    the same direction.

Given the declarations

```
type (
	T0 []string
	T1 []string
	T2 struct{ a, b int }
	T3 struct{ a, c int }
	T4 func(int, float64) *T0
	T5 func(x int, y float64) *[]string
)

```

these types are identical:

```
T0 and T0
[]int and []int
struct{ a, b *T5 } and struct{ a, b *T5 }
func(x int, y float64) *[]string and func(int, float64) (result *[]string)

```

`T0` and `T1` are different because they are named typeswith distinct declarations; `func(int, float64) *T0` and`func(x int, y float64) *[]string` are different because `T0`is different from `[]string`.

### Assignability

A value `x` is *assignable* to a [variable](http://golang.org/ref/spec#Variables) of type `T`("`x` is assignable to `T`") in any of these cases:

- `x`'s type is identical to `T`.
- `x`'s type `V` and `T` have identical[underlying types](http://golang.org/ref/spec#Types) and at least one of `V`or `T` is not a [named type](http://golang.org/ref/spec#Types).
- `T` is an interface type and`x` [implements](http://golang.org/ref/spec#Interface_types) `T`.
- `x` is a bidirectional channel value, `T` is a channel type,`x`'s type `V` and `T` have identical element types,and at least one of `V` or `T` is not a named type.
- `x` is the predeclared identifier `nil` and `T`is a pointer, function, slice, map, channel, or interface type.
- `x` is an untyped [constant](http://golang.org/ref/spec#Constants) representableby a value of type `T`.

## Blocks

A *block* is a possibly empty sequence of declarations and statementswithin matching brace brackets.

```
Block = "{" StatementList "}" .
StatementList = { Statement ";" } .

```

In addition to explicit blocks in the source code, there are implicit blocks:

1. The *universe block* encompasses all Go source text.
2. Each [package](http://golang.org/ref/spec#Packages) has a *package block* containing all	    Go source text for that package.
3. Each file has a *file block* containing all Go source text	    in that file.
4. Each ["if"](http://golang.org/ref/spec#If_statements),	    ["for"](http://golang.org/ref/spec#For_statements), and	    ["switch"](http://golang.org/ref/spec#Switch_statements)	    statement is considered to be in its own implicit block.
5. Each clause in a ["switch"](http://golang.org/ref/spec#Switch_statements)	    or ["select"](http://golang.org/ref/spec#Select_statements) statement	    acts as an implicit block.

Blocks nest and influence [scoping](http://golang.org/ref/spec#Declarations_and_scope).

## Declarations and scope

A *declaration* binds a non-[blank](http://golang.org/ref/spec#Blank_identifier) identifier to a[constant](http://golang.org/ref/spec#Constant_declarations),[type](http://golang.org/ref/spec#Type_declarations),[variable](http://golang.org/ref/spec#Variable_declarations),[function](http://golang.org/ref/spec#Function_declarations),[label](http://golang.org/ref/spec#Labeled_statements), or[package](http://golang.org/ref/spec#Import_declarations).Every identifier in a program must be declared.No identifier may be declared twice in the same block, andno identifier may be declared in both the file and package block.

The [blank identifier](http://golang.org/ref/spec#Blank_identifier) may be used like any other identifierin a declaration, but it does not introduce a binding and thus is not declared.In the package block, the identifier `init` may only be used for[`init` function](http://golang.org/ref/spec#Package_initialization) declarations,and like the blank identifier it does not introduce a new binding.

```
Declaration   = ConstDecl | TypeDecl | VarDecl .
TopLevelDecl  = Declaration | FunctionDecl | MethodDecl .

```

The *scope* of a declared identifier is the extent of source text in whichthe identifier denotes the specified constant, type, variable, function, label, or package.

Go is lexically scoped using [blocks](http://golang.org/ref/spec#Blocks):

1. The scope of a [predeclared identifier](http://golang.org/ref/spec#Predeclared_identifiers) is the universe block.
2. The scope of an identifier denoting a constant, type, variable,	    or function (but not method) declared at top level (outside any	    function) is the package block.
3. The scope of the package name of an imported package is the file block	    of the file containing the import declaration.
4. The scope of an identifier denoting a method receiver, function parameter,	    or result variable is the function body.
5. The scope of a constant or variable identifier declared	    inside a function begins at the end of the ConstSpec or VarSpec	    (ShortVarDecl for short variable declarations)	    and ends at the end of the innermost containing block.
6. The scope of a type identifier declared inside a function	    begins at the identifier in the TypeSpec	    and ends at the end of the innermost containing block.

An identifier declared in a block may be redeclared in an inner block.While the identifier of the inner declaration is in scope, it denotesthe entity declared by the inner declaration.

The [package clause](http://golang.org/ref/spec#Package_clause) is not a declaration; the package namedoes not appear in any scope. Its purpose is to identify the files belongingto the same [package](http://golang.org/ref/spec#Packages) and to specify the default package name for importdeclarations.

### Label scopes

Labels are declared by [labeled statements](http://golang.org/ref/spec#Labeled_statements) and areused in the ["break"](http://golang.org/ref/spec#Break_statements),["continue"](http://golang.org/ref/spec#Continue_statements), and["goto"](http://golang.org/ref/spec#Goto_statements) statements.It is illegal to define a label that is never used.In contrast to other identifiers, labels are not block scoped and donot conflict with identifiers that are not labels. The scope of a labelis the body of the function in which it is declared and excludesthe body of any nested function.

### Blank identifier

The *blank identifier* is represented by the underscore character `_`.It serves as an anonymous placeholder instead of a regular (non-blank)identifier and has special meaning in [declarations](http://golang.org/ref/spec#Declarations_and_scope),as an [operand](http://golang.org/ref/spec#Operands), and in [assignments](http://golang.org/ref/spec#Assignments).

### Predeclared identifiers

The following identifiers are implicitly declared in the[universe block](http://golang.org/ref/spec#Blocks):

```
Types:
	bool byte complex64 complex128 error float32 float64
	int int8 int16 int32 int64 rune string
	uint uint8 uint16 uint32 uint64 uintptr

Constants:
	true false iota

Zero value:
	nil

Functions:
	append cap close complex copy delete imag len
	make new panic print println real recover

```

### Exported identifiers

An identifier may be *exported* to permit access to it from another package.An identifier is exported if both:

1. the first character of the identifier's name is a Unicode upper case	letter (Unicode class "Lu"); and
2. the identifier is declared in the [package block](http://golang.org/ref/spec#Blocks)	or it is a [field name](http://golang.org/ref/spec#Struct_types) or	[method name](http://golang.org/ref/spec#MethodName).

All other identifiers are not exported.

### Uniqueness of identifiers

Given a set of identifiers, an identifier is called *unique* if it is*different* from every other in the set.Two identifiers are different if they are spelled differently, or if theyappear in different [packages](http://golang.org/ref/spec#Packages) and are not[exported](http://golang.org/ref/spec#Exported_identifiers). Otherwise, they are the same.

### Constant declarations

A constant declaration binds a list of identifiers (the names ofthe constants) to the values of a list of [constant expressions](http://golang.org/ref/spec#Constant_expressions).The number of identifiers must be equalto the number of expressions, and the *n*th identifier onthe left is bound to the value of the *n*th expression on theright.

```
ConstDecl      = "const" ( ConstSpec | "(" { ConstSpec ";" } ")" ) .
ConstSpec      = IdentifierList [ [ Type ] "=" ExpressionList ] .

IdentifierList = identifier { "," identifier } .
ExpressionList = Expression { "," Expression } .

```

If the type is present, all constants take the type specified, andthe expressions must be [assignable](http://golang.org/ref/spec#Assignability) to that type.If the type is omitted, the constants take theindividual types of the corresponding expressions.If the expression values are untyped [constants](http://golang.org/ref/spec#Constants),the declared constants remain untyped and the constant identifiersdenote the constant values. For instance, if the expression is afloating-point literal, the constant identifier denotes a floating-pointconstant, even if the literal's fractional part is zero.

```
const Pi float64 = 3.14159265358979323846
const zero = 0.0         // untyped floating-point constant
const (
	size int64 = 1024
	eof        = -1  // untyped integer constant
)
const a, b, c = 3, 4, "foo"  // a = 3, b = 4, c = "foo", untyped integer and string constants
const u, v float32 = 0, 3    // u = 0.0, v = 3.0

```

Within a parenthesized `const` declaration list theexpression list may be omitted from any but the first declaration.Such an empty list is equivalent to the textual substitution of thefirst preceding non-empty expression list and its type if any.Omitting the list of expressions is therefore equivalent torepeating the previous list.  The number of identifiers must be equalto the number of expressions in the previous list.Together with the [`iota` constant generator](http://golang.org/ref/spec#Iota)this mechanism permits light-weight declaration of sequential values:

```
const (
	Sunday = iota
	Monday
	Tuesday
	Wednesday
	Thursday
	Friday
	Partyday
	numberOfDays  // this constant is not exported
)

```

### Iota

Within a [constant declaration](http://golang.org/ref/spec#Constant_declarations), the predeclared identifier`iota` represents successive untyped integer [constants](http://golang.org/ref/spec#Constants). It is reset to 0 whenever the reserved word `const`appears in the source and increments after each [ConstSpec](http://golang.org/ref/spec#ConstSpec).It can be used to construct a set of related constants:

```
const (  // iota is reset to 0
	c0 = iota  // c0 == 0
	c1 = iota  // c1 == 1
	c2 = iota  // c2 == 2
)

const (
	a = 1 << iota  // a == 1 (iota has been reset)
	b = 1 << iota  // b == 2
	c = 1 << iota  // c == 4
)

const (
	u         = iota * 42  // u == 0     (untyped integer constant)
	v float64 = iota * 42  // v == 42.0  (float64 constant)
	w         = iota * 42  // w == 84    (untyped integer constant)
)

const x = iota  // x == 0 (iota has been reset)
const y = iota  // y == 0 (iota has been reset)

```

Within an ExpressionList, the value of each `iota` is the same becauseit is only incremented after each ConstSpec:

```
const (
	bit0, mask0 = 1 << iota, 1<<iota - 1  // bit0 == 1, mask0 == 0
	bit1, mask1                           // bit1 == 2, mask1 == 1
	_, _                                  // skips iota == 2
	bit3, mask3                           // bit3 == 8, mask3 == 7
)

```

This last example exploits the implicit repetition of thelast non-empty expression list.

### Type declarations

A type declaration binds an identifier, the *type name*, to a new typethat has the same [underlying type](http://golang.org/ref/spec#Types) as an existing type,and operations defined for the existing type are also defined for the new type.The new type is [different](http://golang.org/ref/spec#Type_identity) from the existing type.

```
TypeDecl     = "type" ( TypeSpec | "(" { TypeSpec ";" } ")" ) .
TypeSpec     = identifier Type .

```

```
type IntArray [16]int

type (
	Point struct{ x, y float64 }
	Polar Point
)

type TreeNode struct {
	left, right *TreeNode
	value *Comparable
}

type Block interface {
	BlockSize() int
	Encrypt(src, dst []byte)
	Decrypt(src, dst []byte)
}

```

The declared type does not inherit any [methods](http://golang.org/ref/spec#Method_declarations)bound to the existing type, but the [method set](http://golang.org/ref/spec#Method_sets)of an interface type or of elements of a composite type remains unchanged:

```
// A Mutex is a data type with two methods, Lock and Unlock.
type Mutex struct         { /* Mutex fields */ }
func (m *Mutex) Lock()    { /* Lock implementation */ }
func (m *Mutex) Unlock()  { /* Unlock implementation */ }

// NewMutex has the same composition as Mutex but its method set is empty.
type NewMutex Mutex

// The method set of the base type of PtrMutex remains unchanged,
// but the method set of PtrMutex is empty.
type PtrMutex *Mutex

// The method set of *PrintableMutex contains the methods
// Lock and Unlock bound to its anonymous field Mutex.
type PrintableMutex struct {
	Mutex
}

// MyBlock is an interface type that has the same method set as Block.
type MyBlock Block

```

A type declaration may be used to define a different boolean, numeric, or stringtype and attach methods to it:

```
type TimeZone int

const (
	EST TimeZone = -(5 + iota)
	CST
	MST
	PST
)

func (tz TimeZone) String() string {
	return fmt.Sprintf("GMT+%dh", tz)
}

```

### Variable declarations

A variable declaration creates one or more variables, binds correspondingidentifiers to them, and gives each a type and an initial value.

```
VarDecl     = "var" ( VarSpec | "(" { VarSpec ";" } ")" ) .
VarSpec     = IdentifierList ( Type [ "=" ExpressionList ] | "=" ExpressionList ) .

```

```
var i int
var U, V, W float64
var k = 0
var x, y float32 = -1, -2
var (
	i       int
	u, v, s = 2.0, 3.0, "bar"
)
var re, im = complexSqrt(-1)
var _, found = entries[name]  // map lookup; only interested in "found"

```

If a list of expressions is given, the variables are initializedwith the expressions following the rules for [assignments](http://golang.org/ref/spec#Assignments).Otherwise, each variable is initialized to its [zero value](http://golang.org/ref/spec#The_zero_value).

If a type is present, each variable is given that type.Otherwise, each variable is given the type of the correspondinginitialization value in the assignment.If that value is an untyped constant, it is first[converted](http://golang.org/ref/spec#Conversions) to its [default type](http://golang.org/ref/spec#Constants);if it is an untyped boolean value, it is first converted to type `bool`.The predeclared value `nil` cannot be used to initialize a variablewith no explicit type.

```
var d = math.Sin(0.5)  // d is int64
var i = 42             // i is int
var t, ok = x.(T)      // t is T, ok is bool
var n = nil            // illegal

```

Implementation restriction: A compiler may make it illegal to declare a variableinside a [function body](http://golang.org/ref/spec#Function_declarations) if the variable isnever used.

### Short variable declarations

A *short variable declaration* uses the syntax:

```
ShortVarDecl = IdentifierList ":=" ExpressionList .

```

It is shorthand for a regular [variable declaration](http://golang.org/ref/spec#Variable_declarations)with initializer expressions but no types:

```
"var" IdentifierList = ExpressionList .

```

```
i, j := 0, 10
f := func() int { return 7 }
ch := make(chan int)
r, w := os.Pipe(fd)  // os.Pipe() returns two values
_, y, _ := coord(p)  // coord() returns three values; only interested in y coordinate

```

Unlike regular variable declarations, a short variable declaration may redeclare variables provided theywere originally declared earlier in the same block with the same type, and atleast one of the non-[blank](http://golang.org/ref/spec#Blank_identifier) variables is new.  As a consequence, redeclarationcan only appear in a multi-variable short declaration.Redeclaration does not introduce a newvariable; it just assigns a new value to the original.

```
field1, offset := nextField(str, 0)
field2, offset := nextField(str, offset)  // redeclares offset
a, a := 1, 2                              // illegal: double declaration of a or no new variable if a was declared elsewhere

```

Short variable declarations may appear only inside functions.In some contexts such as the initializers for["if"](http://golang.org/ref/spec#If_statements),["for"](http://golang.org/ref/spec#For_statements), or["switch"](http://golang.org/ref/spec#Switch_statements) statements,they can be used to declare local temporary variables.

### Function declarations

A function declaration binds an identifier, the *function name*,to a function.

```
FunctionDecl = "func" FunctionName ( Function | Signature ) .
FunctionName = identifier .
Function     = Signature FunctionBody .
FunctionBody = Block .

```

If the function's [signature](http://golang.org/ref/spec#Function_types) declaresresult parameters, the function body's statement list must end ina [terminating statement](http://golang.org/ref/spec#Terminating_statements).

```
func findMarker(c <-chan int) int {
	for i := range c {
		if x := <-c; isMarker(x) {
			return x
		}
	}
	// invalid: missing return statement.
}

```

A function declaration may omit the body. Such a declaration provides thesignature for a function implemented outside Go, such as an assembly routine.

```
func min(x int, y int) int {
	if x < y {
		return x
	}
	return y
}

func flushICache(begin, end uintptr)  // implemented externally

```

### Method declarations

A method is a [function](http://golang.org/ref/spec#Function_declarations) with a *receiver*.A method declaration binds an identifier, the *method name*, to a method,and associates the method with the receiver's *base type*.

```
MethodDecl   = "func" Receiver MethodName ( Function | Signature ) .
Receiver     = Parameters .

```

The receiver is specified via an extra parameter section preceeding the methodname. That parameter section must declare a single parameter, the receiver.Its type must be of the form `T` or `*T` (possibly usingparentheses) where `T` is a type name. The type denoted by `T` is calledthe receiver *base type*; it must not be a pointer or interface type andit must be declared in the same package as the method.The method is said to be *bound* to the base type and the method nameis visible only within selectors for that type.

A non-[blank](http://golang.org/ref/spec#Blank_identifier) receiver identifier must be[unique](http://golang.org/ref/spec#Uniqueness_of_identifiers) in the method signature.If the receiver's value is not referenced inside the body of the method,its identifier may be omitted in the declaration. The same applies ingeneral to parameters of functions and methods.

For a base type, the non-blank names of methods bound to it must be unique.If the base type is a [struct type](http://golang.org/ref/spec#Struct_types),the non-blank method and field names must be distinct.

Given type `Point`, the declarations

```
func (p *Point) Length() float64 {
	return math.Sqrt(p.x * p.x + p.y * p.y)
}

func (p *Point) Scale(factor float64) {
	p.x *= factor
	p.y *= factor
}

```

bind the methods `Length` and `Scale`,with receiver type `*Point`,to the base type `Point`.

The type of a method is the type of a function with the receiver as firstargument.  For instance, the method `Scale` has type

```
func(p *Point, factor float64)

```

However, a function declared this way is not a method.

## Expressions

An expression specifies the computation of a value by applyingoperators and functions to operands.

### Operands

Operands denote the elementary values in an expression. An operand may be aliteral, a (possibly [qualified](http://golang.org/ref/spec#Qualified_identifiers))non-[blank](http://golang.org/ref/spec#Blank_identifier) identifier denoting a[constant](http://golang.org/ref/spec#Constant_declarations),[variable](http://golang.org/ref/spec#Variable_declarations), or[function](http://golang.org/ref/spec#Function_declarations),a [method expression](http://golang.org/ref/spec#Method_expressions) yielding a function,or a parenthesized expression.

The [blank identifier](http://golang.org/ref/spec#Blank_identifier) may appear as anoperand only on the left-hand side of an [assignment](http://golang.org/ref/spec#Assignments).

```
Operand     = Literal | OperandName | MethodExpr | "(" Expression ")" .
Literal     = BasicLit | CompositeLit | FunctionLit .
BasicLit    = int_lit | float_lit | imaginary_lit | rune_lit | string_lit .
OperandName = identifier | QualifiedIdent.

```

### Qualified identifiers

A qualified identifier is an identifier qualified with a package name prefix.Both the package name and the identifier must not be[blank](http://golang.org/ref/spec#Blank_identifier).

```
QualifiedIdent = PackageName "." identifier .

```

A qualified identifier accesses an identifier in a different package, whichmust be [imported](http://golang.org/ref/spec#Import_declarations).The identifier must be [exported](http://golang.org/ref/spec#Exported_identifiers) anddeclared in the [package block](http://golang.org/ref/spec#Blocks) of that package.

```
math.Sin	// denotes the Sin function in package math

```

### Composite literals

Composite literals construct values for structs, arrays, slices, and mapsand create a new value each time they are evaluated.They consist of the type of the valuefollowed by a brace-bound list of composite elements. An element may bea single expression or a key-value pair.

```
CompositeLit  = LiteralType LiteralValue .
LiteralType   = StructType | ArrayType | "[" "..." "]" ElementType |
                SliceType | MapType | TypeName .
LiteralValue  = "{" [ ElementList [ "," ] ] "}" .
ElementList   = Element { "," Element } .
Element       = [ Key ":" ] Value .
Key           = FieldName | ElementIndex .
FieldName     = identifier .
ElementIndex  = Expression .
Value         = Expression | LiteralValue .

```

The LiteralType must be a struct, array, slice, or map type(the grammar enforces this constraint except when the type is givenas a TypeName).The types of the expressions must be [assignable](http://golang.org/ref/spec#Assignability)to the respective field, element, and key types of the LiteralType;there is no additional conversion.The key is interpreted as a field name for struct literals,an index for array and slice literals, and a key for map literals.For map literals, all elements must have a key. It is an errorto specify multiple elements with the same field name orconstant key value.

For struct literals the following rules apply:

- A key must be a field name declared in the LiteralType.	
- An element list that does not contain any keys must	    list an element for each struct field in the	    order in which the fields are declared.	
- If any element has a key, every element must have a key.	
- An element list that contains keys does not need to	    have an element for each struct field. Omitted fields	    get the zero value for that field.	
- A literal may omit the element list; such a literal evaluates	    to the zero value for its type.	
- It is an error to specify an element for a non-exported	    field of a struct belonging to a different package.	

Given the declarations

```
type Point3D struct { x, y, z float64 }
type Line struct { p, q Point3D }

```

one may write

```
origin := Point3D{}                            // zero value for Point3D
line := Line{origin, Point3D{y: -4, z: 12.3}}  // zero value for line.q.x

```

For array and slice literals the following rules apply:

- Each element has an associated integer index marking	    its position in the array.	
- An element with a key uses the key as its index; the	    key must be a constant integer expression.	
- An element without a key uses the previous element's index plus one.	    If the first element has no key, its index is zero.	

[Taking the address](http://golang.org/ref/spec#Address_operators) of a composite literalgenerates a pointer to a unique [variable](http://golang.org/ref/spec#Variables) initializedwith the literal's value.

```
var pointer *Point3D = &Point3D{y: 1000}

```

The length of an array literal is the length specified in the LiteralType.If fewer elements than the length are provided in the literal, the missingelements are set to the zero value for the array element type.It is an error to provide elements with index values outside the index rangeof the array. The notation `...` specifies an array length equalto the maximum element index plus one.

```
buffer := [10]string{}             // len(buffer) == 10
intSet := [6]int{1, 2, 3, 5}       // len(intSet) == 6
days := [...]string{"Sat", "Sun"}  // len(days) == 2

```

A slice literal describes the entire underlying array literal.Thus, the length and capacity of a slice literal are the maximumelement index plus one. A slice literal has the form

```
[]T{x1, x2, … xn}

```

and is shorthand for a slice operation applied to an array:

```
tmp := [n]T{x1, x2, … xn}
tmp[0 : n]

```

Within a composite literal of array, slice, or map type `T`,elements that are themselves composite literals may elide the respectiveliteral type if it is identical to the element type of `T`.Similarly, elements that are addresses of composite literals may elidethe `&T` when the element type is `*T`.

```
[...]Point{{1.5, -3.5}, {0, 0}}   // same as [...]Point{Point{1.5, -3.5}, Point{0, 0}}
[][]int{{1, 2, 3}, {4, 5}}        // same as [][]int{[]int{1, 2, 3}, []int{4, 5}}

[...]*Point{{1.5, -3.5}, {0, 0}}  // same as [...]*Point{&Point{1.5, -3.5}, &Point{0, 0}}

```

A parsing ambiguity arises when a composite literal using theTypeName form of the LiteralType appears as an operand between the[keyword](http://golang.org/ref/spec#Keywords) and the opening brace of the blockof an "if", "for", or "switch" statement, and the composite literalis not enclosed in parentheses, square brackets, or curly braces.In this rare case, the opening brace of the literal is erroneously parsedas the one introducing the block of statements. To resolve the ambiguity,the composite literal must appear within parentheses.

```
if x == (T{a,b,c}[i]) { … }
if (x == T{a,b,c}[i]) { … }

```

Examples of valid array, slice, and map literals:

```
// list of prime numbers
primes := []int{2, 3, 5, 7, 9, 2147483647}

// vowels[ch] is true if ch is a vowel
vowels := [128]bool{'a': true, 'e': true, 'i': true, 'o': true, 'u': true, 'y': true}

// the array [10]float32{-1, 0, 0, 0, -0.1, -0.1, 0, 0, 0, -1}
filter := [10]float32{-1, 4: -0.1, -0.1, 9: -1}

// frequencies in Hz for equal-tempered scale (A4 = 440Hz)
noteFrequency := map[string]float32{
	"C0": 16.35, "D0": 18.35, "E0": 20.60, "F0": 21.83,
	"G0": 24.50, "A0": 27.50, "B0": 30.87,
}

```

### Function literals

A function literal represents an anonymous [function](http://golang.org/ref/spec#Function_declarations).

```
FunctionLit = "func" Function .

```

```
func(a, b int, z float64) bool { return a*b < int(z) }

```

A function literal can be assigned to a variable or invoked directly.

```
f := func(x, y int) int { return x + y }
func(ch chan int) { ch <- ACK }(replyChan)

```

Function literals are *closures*: they may refer to variablesdefined in a surrounding function. Those variables are then shared betweenthe surrounding function and the function literal, and they survive as longas they are accessible.

### Primary expressions

Primary expressions are the operands for unary and binary expressions.

```
PrimaryExpr =
	Operand |
	Conversion |
	PrimaryExpr Selector |
	PrimaryExpr Index |
	PrimaryExpr Slice |
	PrimaryExpr TypeAssertion |
	PrimaryExpr Arguments .

Selector       = "." identifier .
Index          = "[" Expression "]" .
Slice          = "[" ( [ Expression ] ":" [ Expression ] ) |
                     ( [ Expression ] ":" Expression ":" Expression )
                 "]" .
TypeAssertion  = "." "(" Type ")" .
Arguments      = "(" [ ( ExpressionList | Type [ "," ExpressionList ] ) [ "..." ] [ "," ] ] ")" .

```

```
x
2
(s + ".txt")
f(3.1415, true)
Point{1, 2}
m["foo"]
s[i : j + 1]
obj.color
f.p[i].x()

```

### Selectors

For a [primary expression](http://golang.org/ref/spec#Primary_expressions) `x`that is not a [package name](http://golang.org/ref/spec#Package_clause), the*selector expression*

```
x.f

```

denotes the field or method `f` of the value `x`(or sometimes `*x`; see below).The identifier `f` is called the (field or method) *selector*;it must not be the [blank identifier](http://golang.org/ref/spec#Blank_identifier).The type of the selector expression is the type of `f`.If `x` is a package name, see the section on[qualified identifiers](http://golang.org/ref/spec#Qualified_identifiers).

A selector `f` may denote a field or method `f` ofa type `T`, or it may referto a field or method `f` of a nested[anonymous field](http://golang.org/ref/spec#Struct_types) of `T`.The number of anonymous fields traversedto reach `f` is called its *depth* in `T`.The depth of a field or method `f`declared in `T` is zero.The depth of a field or method `f` declared inan anonymous field `A` in `T` is thedepth of `f` in `A` plus one.

The following rules apply to selectors:

1. For a value `x` of type `T` or `*T`where `T` is not a pointer or interface type,`x.f` denotes the field or method at the shallowest depthin `T` where thereis such an `f`.If there is not exactly [one `f`](http://golang.org/ref/spec#Uniqueness_of_identifiers)with shallowest depth, the selector expression is illegal.
2. For a value `x` of type `I` where `I`is an interface type, `x.f` denotes the actual method with name`f` of the dynamic value of `x`.If there is no method with name `f` in the[method set](http://golang.org/ref/spec#Method_sets) of `I`, the selectorexpression is illegal.
3. As an exception, if the type of `x` is a named pointer typeand `(*x).f` is a valid selector expression denoting a field(but not a method), `x.f` is shorthand for `(*x).f`.
4. In all other cases, `x.f` is illegal.
5. If `x` is of pointer type and has the value`nil` and `x.f` denotes a struct field,assigning to or evaluating `x.f`causes a [run-time panic](http://golang.org/ref/spec#Run_time_panics).
6. If `x` is of interface type and has the value`nil`, [calling](http://golang.org/ref/spec#Calls) or[evaluating](http://golang.org/ref/spec#Method_values) the method `x.f`causes a [run-time panic](http://golang.org/ref/spec#Run_time_panics).

For example, given the declarations:

```
type T0 struct {
	x int
}

func (*T0) M0()

type T1 struct {
	y int
}

func (T1) M1()

type T2 struct {
	z int
	T1
	*T0
}

func (*T2) M2()

type Q *T2

var t T2     // with t.T0 != nil
var p *T2    // with p != nil and (*p).T0 != nil
var q Q = p

```

one may write:

```
t.z          // t.z
t.y          // t.T1.y
t.x          // (*t.TO).x

p.z          // (*p).z
p.y          // (*p).T1.y
p.x          // (*(*p).T0).x

q.x          // (*(*q).T0).x        (*q).x is a valid field selector

p.M2()       // p.M2()              M2 expects *T2 receiver
p.M1()       // ((*p).T1).M1()      M1 expects T1 receiver
p.M0()       // ((&(*p).T0)).M0()   M0 expects *T0 receiver, see section on Calls

```

but the following is invalid:

```
q.M0()       // (*q).M0 is valid but not a field selector

```

### Method expressions

If `M` is in the [method set](http://golang.org/ref/spec#Method_sets) of type `T`,`T.M` is a function that is callable as a regular functionwith the same arguments as `M` prefixed by an additionalargument that is the receiver of the method.

```
MethodExpr    = ReceiverType "." MethodName .
ReceiverType  = TypeName | "(" "*" TypeName ")" | "(" ReceiverType ")" .

```

Consider a struct type `T` with two methods,`Mv`, whose receiver is of type `T`, and`Mp`, whose receiver is of type `*T`.

```
type T struct {
	a int
}
func (tv  T) Mv(a int) int         { return 0 }  // value receiver
func (tp *T) Mp(f float32) float32 { return 1 }  // pointer receiver

var t T

```

The expression

```
T.Mv

```

yields a function equivalent to `Mv` butwith an explicit receiver as its first argument; it has signature

```
func(tv T, a int) int

```

That function may be called normally with an explicit receiver, sothese five invocations are equivalent:

```
t.Mv(7)
T.Mv(t, 7)
(T).Mv(t, 7)
f1 := T.Mv; f1(t, 7)
f2 := (T).Mv; f2(t, 7)

```

Similarly, the expression

```
(*T).Mp

```

yields a function value representing `Mp` with signature

```
func(tp *T, f float32) float32

```

For a method with a value receiver, one can derive a functionwith an explicit pointer receiver, so

```
(*T).Mv

```

yields a function value representing `Mv` with signature

```
func(tv *T, a int) int

```

Such a function indirects through the receiver to create a valueto pass as the receiver to the underlying method;the method does not overwrite the value whose address is passed inthe function call.

The final case, a value-receiver function for a pointer-receiver method,is illegal because pointer-receiver methods are not in the method setof the value type.

Function values derived from methods are called with function call syntax;the receiver is provided as the first argument to the call.That is, given `f := T.Mv`, `f` is invokedas `f(t, 7)` not `t.f(7)`.To construct a function that binds the receiver, use a[function literal](http://golang.org/ref/spec#Function_literals) or[method value](http://golang.org/ref/spec#Method_values).

It is legal to derive a function value from a method of an interface type.The resulting function takes an explicit receiver of that interface type.

### Method values

If the expression `x` has static type `T` and`M` is in the [method set](http://golang.org/ref/spec#Method_sets) of type `T`,`x.M` is called a *method value*.The method value `x.M` is a function value that is callablewith the same arguments as a method call of `x.M`.The expression `x` is evaluated and saved during the evaluation of themethod value; the saved copy is then used as the receiver in any calls,which may be executed later.

The type `T` may be an interface or non-interface type.

As in the discussion of [method expressions](http://golang.org/ref/spec#Method_expressions) above,consider a struct type `T` with two methods,`Mv`, whose receiver is of type `T`, and`Mp`, whose receiver is of type `*T`.

```
type T struct {
	a int
}
func (tv  T) Mv(a int) int         { return 0 }  // value receiver
func (tp *T) Mp(f float32) float32 { return 1 }  // pointer receiver

var t T
var pt *T
func makeT() T

```

The expression

```
t.Mv

```

yields a function value of type

```
func(int) int

```

These two invocations are equivalent:

```
t.Mv(7)
f := t.Mv; f(7)

```

Similarly, the expression

```
pt.Mp

```

yields a function value of type

```
func(float32) float32

```

As with [selectors](http://golang.org/ref/spec#Selectors), a reference to a non-interface method with a value receiverusing a pointer will automatically dereference that pointer: `pt.Mv` is equivalent to `(*pt).Mv`.

As with [method calls](http://golang.org/ref/spec#Calls), a reference to a non-interface method with a pointer receiverusing an addressable value will automatically take the address of that value: `t.Mp` is equivalent to `(&t).Mp`.

```
f := t.Mv; f(7)   // like t.Mv(7)
f := pt.Mp; f(7)  // like pt.Mp(7)
f := pt.Mv; f(7)  // like (*pt).Mv(7)
f := t.Mp; f(7)   // like (&t).Mp(7)
f := makeT().Mp   // invalid: result of makeT() is not addressable

```

Although the examples above use non-interface types, it is also legal to create a method valuefrom a value of interface type.

```
var i interface { M(int) } = myVal
f := i.M; f(7)  // like i.M(7)

```

### Index expressions

A primary expression of the form

```
a[x]

```

denotes the element of the array, pointer to array, slice, string or map `a` indexed by `x`.The value `x` is called the *index* or *map key*, respectively.The following rules apply:

If `a` is not a map:

- the index `x` must be of integer type or untyped;	    it is *in range* if `0 <= x < len(a)`,	    otherwise it is *out of range*
- a [constant](http://golang.org/ref/spec#Constants) index must be non-negative	    and representable by a value of type `int`

For `a` of [array type](http://golang.org/ref/spec#Array_types) `A`:

- a [constant](http://golang.org/ref/spec#Constants) index must be in range
- if `x` is out of range at run time,	    a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs
- `a[x]` is the array element at index `x` and the type of	    `a[x]` is the element type of `A`

For `a` of [pointer](http://golang.org/ref/spec#Pointer_types) to array type:

- `a[x]` is shorthand for `(*a)[x]`

For `a` of [slice type](http://golang.org/ref/spec#Slice_types) `S`:

- if `x` is out of range at run time,	    a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs
- `a[x]` is the slice element at index `x` and the type of	    `a[x]` is the element type of `S`

For `a` of [string type](http://golang.org/ref/spec#String_types):

- a [constant](http://golang.org/ref/spec#Constants) index must be in range	    if the string `a` is also constant
- if `x` is out of range at run time,	    a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs
- `a[x]` is the non-constant byte value at index `x` and the type of	    `a[x]` is `byte`
- `a[x]` may not be assigned to

For `a` of [map type](http://golang.org/ref/spec#Map_types) `M`:

- `x`'s type must be	    [assignable](http://golang.org/ref/spec#Assignability)	    to the key type of `M`
- if the map contains an entry with key `x`,	    `a[x]` is the map value with key `x`	    and the type of `a[x]` is the value type of `M`
- if the map is `nil` or does not contain such an entry,	    `a[x]` is the [zero value](http://golang.org/ref/spec#The_zero_value)	    for the value type of `M`

Otherwise `a[x]` is illegal.

An index expression on a map `a` of type `map[K]V`used in an [assignment](http://golang.org/ref/spec#Assignments) or initialization of the special form

```
v, ok = a[x]
v, ok := a[x]
var v, ok = a[x]

```

yields an additional untyped boolean value. The value of `ok` is`true` if the key `x` is present in the map, and`false` otherwise.

Assigning to an element of a `nil` map causes a[run-time panic](http://golang.org/ref/spec#Run_time_panics).

### Slice expressions

Slice expressions construct a substring or slice from a string, array, pointerto array, or slice. There are two variants: a simple form that specifies a lowand high bound, and a full form that also specifies a bound on the capacity.

#### Simple slice expressions

For a string, array, pointer to array, or slice `a`, the primary expression

```
a[low : high]

```

constructs a substring or slice. The *indices* `low` and`high` select which elements of operand `a` appearin the result. The result has indices starting at 0 and length equal to`high` - `low`.After slicing the array `a`

```
a := [5]int{1, 2, 3, 4, 5}
s := a[1:4]

```

the slice `s` has type `[]int`, length 3, capacity 4, and elements

```
s[0] == 2
s[1] == 3
s[2] == 4

```

For convenience, any of the indices may be omitted. A missing `low`index defaults to zero; a missing `high` index defaults to the length of thesliced operand:

```
a[2:]  // same as a[2 : len(a)]
a[:3]  // same as a[0 : 3]
a[:]   // same as a[0 : len(a)]

```

If `a` is a pointer to an array, `a[low : high]` is shorthand for`(*a)[low : high]`.

For arrays or strings, the indices are *in range* if`0` <= `low` <= `high` <= `len(a)`,otherwise they are *out of range*.For slices, the upper index bound is the slice capacity `cap(a)` rather than the length.A [constant](http://golang.org/ref/spec#Constants) index must be non-negative and representable by a value of type`int`; for arrays or constant strings, constant indices must also be in range.If both indices are constant, they must satisfy `low <= high`.If the indices are out of range at run time, a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs.

Except for [untyped strings](http://golang.org/ref/spec#Constants), if the sliced operand is a string or slice,the result of the slice operation is a non-constant value of the same type as the operand.For untyped string operands the result is a non-constant value of type `string`.If the sliced operand is an array, it must be [addressable](http://golang.org/ref/spec#Address_operators)and the result of the slice operation is a slice with the same element type as the array.

If the sliced operand of a valid slice expression is a `nil` slice, the resultis a `nil` slice. Otherwise, the result shares its underlying array with theoperand.

#### Full slice expressions

For an array, pointer to array, or slice `a` (but not a string), the primary expression

```
a[low : high : max]

```

constructs a slice of the same type, and with the same length and elements as the simple sliceexpression `a[low : high]`. Additionally, it controls the resulting slice's capacityby setting it to `max - low`. Only the first index may be omitted; it defaults to 0.After slicing the array `a`

```
a := [5]int{1, 2, 3, 4, 5}
t := a[1:3:5]

```

the slice `t` has type `[]int`, length 2, capacity 4, and elements

```
t[0] == 2
t[1] == 3

```

As for simple slice expressions, if `a` is a pointer to an array,`a[low : high : max]` is shorthand for `(*a)[low : high : max]`.If the sliced operand is an array, it must be [addressable](http://golang.org/ref/spec#Address_operators).

The indices are *in range* if `0 <= low <= high <= max <= cap(a)`,otherwise they are *out of range*.A [constant](http://golang.org/ref/spec#Constants) index must be non-negative and representable by a value of type`int`; for arrays, constant indices must also be in range.If multiple indices are constant, the constants that are present must be in range relative to eachother.If the indices are out of range at run time, a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs.

### Type assertions

For an expression `x` of [interface type](http://golang.org/ref/spec#Interface_types)and a type `T`, the primary expression

```
x.(T)

```

asserts that `x` is not `nil`and that the value stored in `x` is of type `T`.The notation `x.(T)` is called a *type assertion*.

More precisely, if `T` is not an interface type, `x.(T)` assertsthat the dynamic type of `x` is [identical](http://golang.org/ref/spec#Type_identity)to the type `T`.In this case, `T` must [implement](http://golang.org/ref/spec#Method_sets) the (interface) type of `x`;otherwise the type assertion is invalid since it is not possible for `x`to store a value of type `T`.If `T` is an interface type, `x.(T)` asserts that the dynamic typeof `x` implements the interface `T`.

If the type assertion holds, the value of the expression is the valuestored in `x` and its type is `T`. If the type assertion is false,a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs.In other words, even though the dynamic type of `x`is known only at run time, the type of `x.(T)` isknown to be `T` in a correct program.

```
var x interface{} = 7  // x has dynamic type int and value 7
i := x.(int)           // i has type int and value 7

type I interface { m() }
var y I
s := y.(string)        // illegal: string does not implement I (missing method m)
r := y.(io.Reader)     // r has type io.Reader and y must implement both I and io.Reader

```

A type assertion used in an [assignment](http://golang.org/ref/spec#Assignments) or initialization of the special form

```
v, ok = x.(T)
v, ok := x.(T)
var v, ok = x.(T)

```

yields an additional untyped boolean value. The value of `ok` is `true`if the assertion holds. Otherwise it is `false` and the value of `v` isthe [zero value](http://golang.org/ref/spec#The_zero_value) for type `T`.No run-time panic occurs in this case.

### Calls

Given an expression `f` of function type`F`,

```
f(a1, a2, … an)

```

calls `f` with arguments `a1, a2, … an`.Except for one special case, arguments must be single-valued expressions[assignable](http://golang.org/ref/spec#Assignability) to the parameter types of`F` and are evaluated before the function is called.The type of the expression is the result typeof `F`.A method invocation is similar but the method itselfis specified as a selector upon a value of the receiver type forthe method.

```
math.Atan2(x, y)  // function call
var pt *Point
pt.Scale(3.5)     // method call with receiver pt

```

In a function call, the function value and arguments are evaluated in[the usual order](http://golang.org/ref/spec#Order_of_evaluation).After they are evaluated, the parameters of the call are passed by value to the functionand the called function begins execution.The return parameters of the function are passed by valueback to the calling function when the function returns.

Calling a `nil` function valuecauses a [run-time panic](http://golang.org/ref/spec#Run_time_panics).

As a special case, if the return values of a function or method`g` are equal in number and individuallyassignable to the parameters of another function or method`f`, then the call `f(g(*parameters_of_g*))`will invoke `f` after binding the return values of`g` to the parameters of `f` in order.  The callof `f` must contain no parameters other than the call of `g`,and `g` must have at least one return value.If `f` has a final `...` parameter, it isassigned the return values of `g` that remain afterassignment of regular parameters.

```
func Split(s string, pos int) (string, string) {
	return s[0:pos], s[pos:]
}

func Join(s, t string) string {
	return s + t
}

if Join(Split(value, len(value)/2)) != value {
	log.Panic("test fails")
}

```

A method call `x.m()` is valid if the [method set](http://golang.org/ref/spec#Method_sets)of (the type of) `x` contains `m` and theargument list can be assigned to the parameter list of `m`.If `x` is [addressable](http://golang.org/ref/spec#Address_operators) and `&x`'s methodset contains `m`, `x.m()` is shorthandfor `(&x).m()`:

```
var p Point
p.Scale(3.5)

```

There is no distinct method type and there are no method literals.

### Passing arguments to `...` parameters

If `f` is [variadic](http://golang.org/ref/spec#Function_types) with a finalparameter `p` of type `...T`, then within `f`the type of `p` is equivalent to type `[]T`.If `f` is invoked with no actual arguments for `p`,the value passed to `p` is `nil`.Otherwise, the value passed is a new sliceof type `[]T` with a new underlying array whose successive elementsare the actual arguments, which all must be [assignable](http://golang.org/ref/spec#Assignability)to `T`. The length and capacity of the slice is thereforethe number of arguments bound to `p` and may differ for eachcall site.

Given the function and calls

```
func Greeting(prefix string, who ...string)
Greeting("nobody")
Greeting("hello:", "Joe", "Anna", "Eileen")

```

within `Greeting`, `who` will have the value`nil` in the first call, and`[]string{"Joe", "Anna", "Eileen"}` in the second.

If the final argument is assignable to a slice type `[]T`, it may bepassed unchanged as the value for a `...T` parameter if the argumentis followed by `...`. In this case no new slice is created.

Given the slice `s` and call

```
s := []string{"James", "Jasmine"}
Greeting("goodbye:", s...)

```

within `Greeting`, `who` will have the same value as `s`with the same underlying array.

### Operators

Operators combine operands into expressions.

```
Expression = UnaryExpr | Expression binary_op UnaryExpr .
UnaryExpr  = PrimaryExpr | unary_op UnaryExpr .

binary_op  = "||" | "&&" | rel_op | add_op | mul_op .
rel_op     = "==" | "!=" | "<" | "<=" | ">" | ">=" .
add_op     = "+" | "-" | "|" | "^" .
mul_op     = "*" | "/" | "%" | "<<" | ">>" | "&" | "&^" .

unary_op   = "+" | "-" | "!" | "^" | "*" | "&" | "<-" .

```

Comparisons are discussed [elsewhere](http://golang.org/ref/spec#Comparison_operators).For other binary operators, the operand types must be [identical](http://golang.org/ref/spec#Type_identity)unless the operation involves shifts or untyped [constants](http://golang.org/ref/spec#Constants).For operations involving constants only, see the section on[constant expressions](http://golang.org/ref/spec#Constant_expressions).

Except for shift operations, if one operand is an untyped [constant](http://golang.org/ref/spec#Constants)and the other operand is not, the constant is [converted](http://golang.org/ref/spec#Conversions)to the type of the other operand.

The right operand in a shift expression must have unsigned integer typeor be an untyped constant that can be converted to unsigned integer type.If the left operand of a non-constant shift expression is an untyped constant,the type of the constant is what it would be if the shift expression werereplaced by its left operand alone.

```
var s uint = 33
var i = 1<<s           // 1 has type int
var j int32 = 1<<s     // 1 has type int32; j == 0
var k = uint64(1<<s)   // 1 has type uint64; k == 1<<33
var m int = 1.0<<s     // 1.0 has type int
var n = 1.0<<s != i    // 1.0 has type int; n == false if ints are 32bits in size
var o = 1<<s == 2<<s   // 1 and 2 have type int; o == true if ints are 32bits in size
var p = 1<<s == 1<<33  // illegal if ints are 32bits in size: 1 has type int, but 1<<33 overflows int
var u = 1.0<<s         // illegal: 1.0 has type float64, cannot shift
var u1 = 1.0<<s != 0   // illegal: 1.0 has type float64, cannot shift
var u2 = 1<<s != 1.0   // illegal: 1 has type float64, cannot shift
var v float32 = 1<<s   // illegal: 1 has type float32, cannot shift
var w int64 = 1.0<<33  // 1.0<<33 is a constant shift expression

```

### Operator precedence

Unary operators have the highest precedence.As the  `++` and `--` operators formstatements, not expressions, they falloutside the operator hierarchy.As a consequence, statement `*p++` is the same as `(*p)++`.

There are five precedence levels for binary operators.Multiplication operators bind strongest, followed by additionoperators, comparison operators, `&&` (logical AND),and finally `||` (logical OR):

```
Precedence    Operator
    5             *  /  %  <<  >>  &  &^
    4             +  -  |  ^
    3             ==  !=  <  <=  >  >=
    2             &&
    1             ||

```

Binary operators of the same precedence associate from left to right.For instance, `x / y * z` is the same as `(x / y) * z`.

```
+x
23 + 3*x[i]
x <= f()
^a >> b
f() || g()
x == y+1 && <-chanPtr > 0

```

### Arithmetic operators

Arithmetic operators apply to numeric values and yield a result of the sametype as the first operand. The four standard arithmetic operators (`+`,`-`,  `*`, `/`) apply to integer,floating-point, and complex types; `+` also appliesto strings. All other arithmetic operators apply to integers only.

```
+    sum                    integers, floats, complex values, strings
-    difference             integers, floats, complex values
*    product                integers, floats, complex values
/    quotient               integers, floats, complex values
%    remainder              integers

&    bitwise AND            integers
|    bitwise OR             integers
^    bitwise XOR            integers
&^   bit clear (AND NOT)    integers

<<   left shift             integer << unsigned integer
>>   right shift            integer >> unsigned integer

```

Strings can be concatenated using the `+` operatoror the `+=` assignment operator:

```
s := "hi" + string(c)
s += " and good bye"

```

String addition creates a new string by concatenating the operands.

For two integer values `x` and `y`, the integer quotient`q = x / y` and remainder `r = x % y` satisfy the followingrelationships:

```
x = q*y + r  and  |r| < |y|

```

with `x / y` truncated towards zero(["truncated division"](http://en.wikipedia.org/wiki/Modulo_operation)).

```
 x     y     x / y     x % y
 5     3       1         2
-5     3      -1        -2
 5    -3      -1         2
-5    -3       1        -2

```

As an exception to this rule, if the dividend `x` is the mostnegative value for the int type of `x`, the quotient`q = x / -1` is equal to `x` (and `r = 0`).

```
			 x, q
int8                     -128
int16                  -32768
int32             -2147483648
int64    -9223372036854775808

```

If the divisor is a [constant](http://golang.org/ref/spec#Constants), it must not be zero.If the divisor is zero at run time, a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs.If the dividend is non-negative and the divisor is a constant power of 2,the division may be replaced by a right shift, and computing the remainder maybe replaced by a bitwise AND operation:

```
 x     x / 4     x % 4     x >> 2     x & 3
 11      2         3         2          3
-11     -2        -3        -3          1

```

The shift operators shift the left operand by the shift count specified by theright operand. They implement arithmetic shifts if the left operand is a signedinteger and logical shifts if it is an unsigned integer.There is no upper limit on the shift count. Shifts behaveas if the left operand is shifted `n` times by 1 for a shiftcount of `n`.As a result, `x << 1` is the same as `x*2`and `x >> 1` is the same as`x/2` but truncated towards negative infinity.

For integer operands, the unary operators`+`, `-`, and `^` are defined asfollows:

```
+x                          is 0 + x
-x    negation              is 0 - x
^x    bitwise complement    is m ^ x  with m = "all bits set to 1" for unsigned x
                                      and  m = -1 for signed x

```

For floating-point and complex numbers,`+x` is the same as `x`,while `-x` is the negation of `x`.The result of a floating-point or complex division by zero is not specified beyond theIEEE-754 standard; whether a [run-time panic](http://golang.org/ref/spec#Run_time_panics)occurs is implementation-specific.

### Integer overflow

For unsigned integer values, the operations `+`,`-`, `*`, and `<<` arecomputed modulo 2*n*, where *n* is the bit width ofthe [unsigned integer](http://golang.org/ref/spec#Numeric_types)'s type.Loosely speaking, these unsigned integer operationsdiscard high bits upon overflow, and programs may rely on ``wrap around''.

For signed integers, the operations `+`,`-`, `*`, and `<<` may legallyoverflow and the resulting value exists and is deterministically definedby the signed integer representation, the operation, and its operands.No exception is raised as a result of overflow. Acompiler may not optimize code under the assumption that overflow doesnot occur. For instance, it may not assume that `x < x + 1` is always true.

### Comparison operators

Comparison operators compare two operands and yield an untyped boolean value.

```
==    equal
!=    not equal
<     less
<=    less or equal
>     greater
>=    greater or equal

```

In any comparison, the first operandmust be [assignable](http://golang.org/ref/spec#Assignability)to the type of the second operand, or vice versa.

The equality operators `==` and `!=` applyto operands that are *comparable*.The ordering operators `<`, `<=`, `>`, and `>=`apply to operands that are *ordered*.These terms and the result of the comparisons are defined as follows:

- ​	Boolean values are comparable.	Two boolean values are equal if they are either both	`true` or both `false`.	
- ​	Integer values are comparable and ordered, in the usual way.	
- ​	Floating point values are comparable and ordered,	as defined by the IEEE-754 standard.	
- ​	Complex values are comparable.	Two complex values `u` and `v` are	equal if both `real(u) == real(v)` and	`imag(u) == imag(v)`.	
- ​	String values are comparable and ordered, lexically byte-wise.	
- ​	Pointer values are comparable.	Two pointer values are equal if they point to the same variable or if both have value `nil`.	Pointers to distinct [zero-size](http://golang.org/ref/spec#Size_and_alignment_guarantees) variables may or may not be equal.	
- ​	Channel values are comparable.	Two channel values are equal if they were created by the same call to	[`make`](http://golang.org/ref/spec#Making_slices_maps_and_channels)	or if both have value `nil`.	
- ​	Interface values are comparable.	Two interface values are equal if they have [identical](http://golang.org/ref/spec#Type_identity) dynamic types	and equal dynamic values or if both have value `nil`.	
- ​	A value `x` of non-interface type `X` and	a value `t` of interface type `T` are comparable when values	of type `X` are comparable and	`X` implements `T`.	They are equal if `t`'s dynamic type is identical to `X`	and `t`'s dynamic value is equal to `x`.	
- ​	Struct values are comparable if all their fields are comparable.	Two struct values are equal if their corresponding	non-[blank](http://golang.org/ref/spec#Blank_identifier) fields are equal.	
- ​	Array values are comparable if values of the array element type are comparable.	Two array values are equal if their corresponding elements are equal.	

A comparison of two interface values with identical dynamic typescauses a [run-time panic](http://golang.org/ref/spec#Run_time_panics) if valuesof that type are not comparable.  This behavior applies not only to direct interfacevalue comparisons but also when comparing arrays of interface valuesor structs with interface-valued fields.

Slice, map, and function values are not comparable.However, as a special case, a slice, map, or function value maybe compared to the predeclared identifier `nil`.Comparison of pointer, channel, and interface values to `nil`is also allowed and follows from the general rules above.

```
const c = 3 < 4            // c is the untyped bool constant true

type MyBool bool
var x, y int
var (
	// The result of a comparison is an untyped bool.
	// The usual assignment rules apply.
	b3        = x == y // b3 has type bool
	b4 bool   = x == y // b4 has type bool
	b5 MyBool = x == y // b5 has type MyBool
)

```

### Logical operators

Logical operators apply to [boolean](http://golang.org/ref/spec#Boolean_types) valuesand yield a result of the same type as the operands.The right operand is evaluated conditionally.

```
&&    conditional AND    p && q  is  "if p then q else false"
||    conditional OR     p || q  is  "if p then true else q"
!     NOT                !p      is  "not p"

```

### Address operators

For an operand `x` of type `T`, the address operation`&x` generates a pointer of type `*T` to `x`.The operand must be *addressable*,that is, either a variable, pointer indirection, or slice indexingoperation; or a field selector of an addressable struct operand;or an array indexing operation of an addressable array.As an exception to the addressability requirement, `x` may also be a(possibly parenthesized)[composite literal](http://golang.org/ref/spec#Composite_literals).If the evaluation of `x` would cause a [run-time panic](http://golang.org/ref/spec#Run_time_panics),then the evaluation of `&x` does too.

For an operand `x` of pointer type `*T`, the pointerindirection `*x` denotes the [variable](http://golang.org/ref/spec#Variables) of type `T` pointedto by `x`.If `x` is `nil`, an attempt to evaluate `*x`will cause a [run-time panic](http://golang.org/ref/spec#Run_time_panics).

```
&x
&a[f(2)]
&Point{2, 3}
*p
*pf(x)

var x *int = nil
*x   // causes a run-time panic
&*x  // causes a run-time panic

```

### Receive operator

For an operand `ch` of [channel type](http://golang.org/ref/spec#Channel_types),the value of the receive operation `<-ch` is the value receivedfrom the channel `ch`. The channel direction must permit receive operations,and the type of the receive operation is the element type of the channel.The expression blocks until a value is available.Receiving from a `nil` channel blocks forever.A receive operation on a [closed](http://golang.org/ref/spec#Close) channel can always proceedimmediately, yielding the element type's [zero value](http://golang.org/ref/spec#The_zero_value)after any previously sent values have been received.

```
v1 := <-ch
v2 = <-ch
f(<-ch)
<-strobe  // wait until clock pulse and discard received value

```

A receive expression used in an [assignment](http://golang.org/ref/spec#Assignments) or initialization of the special form

```
x, ok = <-ch
x, ok := <-ch
var x, ok = <-ch

```

yields an additional untyped boolean result reporting whether thecommunication succeeded. The value of `ok` is `true`if the value received was delivered by a successful send operation to thechannel, or `false` if it is a zero value generated because thechannel is closed and empty.

### Conversions

Conversions are expressions of the form `T(x)`where `T` is a type and `x` is an expressionthat can be converted to type `T`.

```
Conversion = Type "(" Expression [ "," ] ")" .

```

If the type starts with the operator `*` or `<-`,or if the type starts with the keyword `func`and has no result list, it must be parenthesized whennecessary to avoid ambiguity:

```
*Point(p)        // same as *(Point(p))
(*Point)(p)      // p is converted to *Point
<-chan int(c)    // same as <-(chan int(c))
(<-chan int)(c)  // c is converted to <-chan int
func()(x)        // function signature func() x
(func())(x)      // x is converted to func()
(func() int)(x)  // x is converted to func() int
func() int(x)    // x is converted to func() int (unambiguous)

```

A [constant](http://golang.org/ref/spec#Constants) value `x` can be converted totype `T` in any of these cases:

- `x` is representable by a value of type `T`.	
- `x` is a floating-point constant,	`T` is a floating-point type,	and `x` is representable by a value	of type `T` after rounding using	IEEE 754 round-to-even rules.	The constant `T(x)` is the rounded value.	
- `x` is an integer constant and `T` is a	[string type](http://golang.org/ref/spec#String_types).	The [same rule](http://golang.org/ref/spec#Conversions_to_and_from_a_string_type)	as for non-constant `x` applies in this case.	

Converting a constant yields a typed constant as result.

```
uint(iota)               // iota value of type uint
float32(2.718281828)     // 2.718281828 of type float32
complex128(1)            // 1.0 + 0.0i of type complex128
float32(0.49999999)      // 0.5 of type float32
string('x')              // "x" of type string
string(0x266c)           // "♬" of type string
MyString("foo" + "bar")  // "foobar" of type MyString
string([]byte{'a'})      // not a constant: []byte{'a'} is not a constant
(*int)(nil)              // not a constant: nil is not a constant, *int is not a boolean, numeric, or string type
int(1.2)                 // illegal: 1.2 cannot be represented as an int
string(65.0)             // illegal: 65.0 is not an integer constant

```

A non-constant value `x` can be converted to type `T`in any of these cases:

- `x` is [assignable](http://golang.org/ref/spec#Assignability)	to `T`.	
- `x`'s type and `T` have identical	[underlying types](http://golang.org/ref/spec#Types).	
- `x`'s type and `T` are unnamed pointer types	and their pointer base types have identical underlying types.	
- `x`'s type and `T` are both integer or floating	point types.	
- `x`'s type and `T` are both complex types.	
- `x` is an integer or a slice of bytes or runes	and `T` is a string type.	
- `x` is a string and `T` is a slice of bytes or runes.	

Specific rules apply to (non-constant) conversions between numeric types orto and from a string type.These conversions may change the representation of `x`and incur a run-time cost.All other conversions only change the type but not the representationof `x`.

There is no linguistic mechanism to convert between pointers and integers.The package [`unsafe`](http://golang.org/ref/spec#Package_unsafe)implements this functionality underrestricted circumstances.

#### Conversions between numeric types

For the conversion of non-constant numeric values, the following rules apply:

1. When converting between integer types, if the value is a signed integer, it issign extended to implicit infinite precision; otherwise it is zero extended.It is then truncated to fit in the result type's size.For example, if `v := uint16(0x10F0)`, then `uint32(int8(v)) == 0xFFFFFFF0`.The conversion always yields a valid value; there is no indication of overflow.
2. When converting a floating-point number to an integer, the fraction is discarded(truncation towards zero).
3. When converting an integer or floating-point number to a floating-point type,or a complex number to another complex type, the result value is roundedto the precision specified by the destination type.For instance, the value of a variable `x` of type `float32`may be stored using additional precision beyond that of an IEEE-754 32-bit number,but float32(x) represents the result of rounding `x`'s value to32-bit precision. Similarly, `x + 0.1` may use more than 32 bitsof precision, but `float32(x + 0.1)` does not.

In all non-constant conversions involving floating-point or complex values,if the result type cannot represent the value the conversionsucceeds but the result value is implementation-dependent.

#### Conversions to and from a string type

1. Converting a signed or unsigned integer value to a string type yields astring containing the UTF-8 representation of the integer. Values outsidethe range of valid Unicode code points are converted to 

   ```
   "\uFFFD"
   ```

   .

   ```
   string('a')       // "a"
   string(-1)        // "\ufffd" == "\xef\xbf\xbd"
   string(0xf8)      // "\u00f8" == "ø" == "\xc3\xb8"
   type MyString string
   MyString(0x65e5)  // "\u65e5" == "日" == "\xe6\x97\xa5"

   ```

2. Converting a slice of bytes to a string type yieldsa string whose successive bytes are the elements of the slice.

   ```
   string([]byte{'h', 'e', 'l', 'l', '\xc3', '\xb8'})   // "hellø"
   string([]byte{})                                     // ""
   string([]byte(nil))                                  // ""

   type MyBytes []byte
   string(MyBytes{'h', 'e', 'l', 'l', '\xc3', '\xb8'})  // "hellø"

   ```

3. Converting a slice of runes to a string type yieldsa string that is the concatenation of the individual rune valuesconverted to strings.

   ```
   string([]rune{0x767d, 0x9d6c, 0x7fd4})   // "\u767d\u9d6c\u7fd4" == "白鵬翔"
   string([]rune{})                         // ""
   string([]rune(nil))                      // ""

   type MyRunes []rune
   string(MyRunes{0x767d, 0x9d6c, 0x7fd4})  // "\u767d\u9d6c\u7fd4" == "白鵬翔"

   ```

4. Converting a value of a string type to a slice of bytes typeyields a slice whose successive elements are the bytes of the string.

   ```
   []byte("hellø")   // []byte{'h', 'e', 'l', 'l', '\xc3', '\xb8'}
   []byte("")        // []byte{}

   MyBytes("hellø")  // []byte{'h', 'e', 'l', 'l', '\xc3', '\xb8'}

   ```

5. Converting a value of a string type to a slice of runes typeyields a slice containing the individual Unicode code points of the string.

   ```
   []rune(MyString("白鵬翔"))  // []rune{0x767d, 0x9d6c, 0x7fd4}
   []rune("")                 // []rune{}

   MyRunes("白鵬翔")           // []rune{0x767d, 0x9d6c, 0x7fd4}

   ```

### Constant expressions

Constant expressions may contain only [constant](http://golang.org/ref/spec#Constants)operands and are evaluated at compile time.

Untyped boolean, numeric, and string constants may be used as operandswherever it is legal to use an operand of boolean, numeric, or string type,respectively.Except for shift operations, if the operands of a binary operation aredifferent kinds of untyped constants, the operation and, for non-boolean operations, the result usethe kind that appears later in this list: integer, rune, floating-point, complex.For example, an untyped integer constant divided by anuntyped complex constant yields an untyped complex constant.

A constant [comparison](http://golang.org/ref/spec#Comparison_operators) always yieldsan untyped boolean constant.  If the left operand of a constant[shift expression](http://golang.org/ref/spec#Operators) is an untyped constant, theresult is an integer constant; otherwise it is a constant of the sametype as the left operand, which must be of[integer type](http://golang.org/ref/spec#Numeric_types).Applying all other operators to untyped constants results in an untypedconstant of the same kind (that is, a boolean, integer, floating-point,complex, or string constant).

```
const a = 2 + 3.0          // a == 5.0   (untyped floating-point constant)
const b = 15 / 4           // b == 3     (untyped integer constant)
const c = 15 / 4.0         // c == 3.75  (untyped floating-point constant)
const Θ float64 = 3/2      // Θ == 1.0   (type float64, 3/2 is integer division)
const Π float64 = 3/2.     // Π == 1.5   (type float64, 3/2. is float division)
const d = 1 << 3.0         // d == 8     (untyped integer constant)
const e = 1.0 << 3         // e == 8     (untyped integer constant)
const f = int32(1) << 33   // illegal    (constant 8589934592 overflows int32)
const g = float64(2) >> 1  // illegal    (float64(2) is a typed floating-point constant)
const h = "foo" > "bar"    // h == true  (untyped boolean constant)
const j = true             // j == true  (untyped boolean constant)
const k = 'w' + 1          // k == 'x'   (untyped rune constant)
const l = "hi"             // l == "hi"  (untyped string constant)
const m = string(k)        // m == "x"   (type string)
const Σ = 1 - 0.707i       //            (untyped complex constant)
const Δ = Σ + 2.0e-4       //            (untyped complex constant)
const Φ = iota*1i - 1/1i   //            (untyped complex constant)

```

Applying the built-in function `complex` to untypedinteger, rune, or floating-point constants yieldsan untyped complex constant.

```
const ic = complex(0, c)   // ic == 3.75i  (untyped complex constant)
const iΘ = complex(0, Θ)   // iΘ == 1i     (type complex128)

```

Constant expressions are always evaluated exactly; intermediate values and theconstants themselves may require precision significantly larger than supportedby any predeclared type in the language. The following are legal declarations:

```
const Huge = 1 << 100         // Huge == 1267650600228229401496703205376  (untyped integer constant)
const Four int8 = Huge >> 98  // Four == 4                                (type int8)

```

The divisor of a constant division or remainder operation must not be zero:

```
3.14 / 0.0   // illegal: division by zero

```

The values of *typed* constants must always be accurately representable as valuesof the constant type. The following constant expressions are illegal:

```
uint(-1)     // -1 cannot be represented as a uint
int(3.14)    // 3.14 cannot be represented as an int
int64(Huge)  // 1267650600228229401496703205376 cannot be represented as an int64
Four * 300   // operand 300 cannot be represented as an int8 (type of Four)
Four * 100   // product 400 cannot be represented as an int8 (type of Four)

```

The mask used by the unary bitwise complement operator `^` matchesthe rule for non-constants: the mask is all 1s for unsigned constantsand -1 for signed and untyped constants.

```
^1         // untyped integer constant, equal to -2
uint8(^1)  // illegal: same as uint8(-2), -2 cannot be represented as a uint8
^uint8(1)  // typed uint8 constant, same as 0xFF ^ uint8(1) = uint8(0xFE)
int8(^1)   // same as int8(-2)
^int8(1)   // same as -1 ^ int8(1) = -2

```

Implementation restriction: A compiler may use rounding whilecomputing untyped floating-point or complex constant expressions; seethe implementation restriction in the sectionon [constants](http://golang.org/ref/spec#Constants).  This rounding may cause afloating-point constant expression to be invalid in an integercontext, even if it would be integral when calculated using infiniteprecision.

### Order of evaluation

At package level, [initialization dependencies](http://golang.org/ref/spec#Package_initialization)determine the evaluation order of individual initialization expressions in[variable declarations](http://golang.org/ref/spec#Variable_declarations).Otherwise, when evaluating the [operands](http://golang.org/ref/spec#Operands) of anexpression, assignment, or[return statement](http://golang.org/ref/spec#Return_statements),all function calls, method calls, andcommunication operations are evaluated in lexical left-to-rightorder.

For example, in the (function-local) assignment

```
y[f()], ok = g(h(), i()+x[j()], <-c), k()

```

the function calls and communication happen in the order`f()`, `h()`, `i()`, `j()`,`<-c`, `g()`, and `k()`.However, the order of those events compared to the evaluationand indexing of `x` and the evaluationof `y` is not specified.

```
a := 1
f := func() int { a++; return a }
x := []int{a, f()}            // x may be [1, 2] or [2, 2]: evaluation order between a and f() is not specified
m := map[int]int{a: 1, a: 2}  // m may be {2: 1} or {2: 2}: evaluation order between the two map assignments is not specified
n := map[int]int{a: f()}      // n may be {2: 3} or {3: 3}: evaluation order between the key and the value is not specified

```

At package level, initialization dependencies override the left-to-right rulefor individual initialization expressions, but not for operands within eachexpression:

```
var a, b, c = f() + v(), g(), sqr(u()) + v()

func f() int        { return c }
func g() int        { return a }
func sqr(x int) int { return x*x }

// functions u and v are independent of all other variables and functions

```

The function calls happen in the order`u()`, `sqr()`, `v()`,`f()`, `v()`, and `g()`.

Floating-point operations within a single expression are evaluated according tothe associativity of the operators.  Explicit parentheses affect the evaluationby overriding the default associativity.In the expression `x + (y + z)` the addition `y + z`is performed before adding `x`.

## Statements

Statements control execution.

```
Statement =
	Declaration | LabeledStmt | SimpleStmt |
	GoStmt | ReturnStmt | BreakStmt | ContinueStmt | GotoStmt |
	FallthroughStmt | Block | IfStmt | SwitchStmt | SelectStmt | ForStmt |
	DeferStmt .

SimpleStmt = EmptyStmt | ExpressionStmt | SendStmt | IncDecStmt | Assignment | ShortVarDecl .

```

### Terminating statements

A terminating statement is one of the following:

1. ​	A 

   "return"

    or    	

   "goto"

    statement.		

2. ​	A call to the built-in function	

   `panic`

   .		

3. ​	A 

   block

    in which the statement list ends in a terminating statement.		

4. ​	An 

   "if" statement

    in which:	

   - the "else" branch is present, and
   - both branches are terminating statements.

5. ​	A 

   "for" statement

    in which:	

   - there are no "break" statements referring to the "for" statement, and
   - the loop condition is absent.

6. ​	A 

   "switch" statement

    in which:	

   - there are no "break" statements referring to the "switch" statement,
   - there is a default case, and
   - the statement lists in each case, including the default, end in a terminating	    statement, or a possibly labeled ["fallthrough"	    statement](http://golang.org/ref/spec#Fallthrough_statements).

7. ​	A 

   "select" statement

    in which:	

   - there are no "break" statements referring to the "select" statement, and
   - the statement lists in each case, including the default if present,	    end in a terminating statement.

8. ​	A [labeled statement](http://golang.org/ref/spec#Labeled_statements) labeling	a terminating statement.

All other statements are not terminating.

A [statement list](http://golang.org/ref/spec#Blocks) ends in a terminating statement if the listis not empty and its final statement is terminating.

### Empty statements

The empty statement does nothing.

```
EmptyStmt = .

```

### Labeled statements

A labeled statement may be the target of a `goto`,`break` or `continue` statement.

```
LabeledStmt = Label ":" Statement .
Label       = identifier .

```

```
Error: log.Panic("error encountered")

```

### Expression statements

With the exception of specific built-in functions,function and method [calls](http://golang.org/ref/spec#Calls) and[receive operations](http://golang.org/ref/spec#Receive_operator)can appear in statement context. Such statements may be parenthesized.

```
ExpressionStmt = Expression .

```

The following built-in functions are not permitted in statement context:

```
append cap complex imag len make new real
unsafe.Alignof unsafe.Offsetof unsafe.Sizeof

```

```
h(x+y)
f.Close()
<-ch
(<-ch)
len("foo")  // illegal if len is the built-in function

```

### Send statements

A send statement sends a value on a channel.The channel expression must be of [channel type](http://golang.org/ref/spec#Channel_types),the channel direction must permit send operations,and the type of the value to be sent must be [assignable](http://golang.org/ref/spec#Assignability)to the channel's element type.

```
SendStmt = Channel "<-" Expression .
Channel  = Expression .

```

Both the channel and the value expression are evaluated before communicationbegins. Communication blocks until the send can proceed.A send on an unbuffered channel can proceed if a receiver is ready.A send on a buffered channel can proceed if there is room in the buffer.A send on a closed channel proceeds by causing a [run-time panic](http://golang.org/ref/spec#Run_time_panics).A send on a `nil` channel blocks forever.

```
ch <- 3  // send value 3 to channel ch

```

### IncDec statements

The "++" and "--" statements increment or decrement their operandsby the untyped [constant](http://golang.org/ref/spec#Constants) `1`.As with an assignment, the operand must be [addressable](http://golang.org/ref/spec#Address_operators)or a map index expression.

```
IncDecStmt = Expression ( "++" | "--" ) .

```

The following [assignment statements](http://golang.org/ref/spec#Assignments) are semanticallyequivalent:

```
IncDec statement    Assignment
x++                 x += 1
x--                 x -= 1

```

### Assignments

```
Assignment = ExpressionList assign_op ExpressionList .

assign_op = [ add_op | mul_op ] "=" .

```

Each left-hand side operand must be [addressable](http://golang.org/ref/spec#Address_operators),a map index expression, or (for `=` assignments only) the[blank identifier](http://golang.org/ref/spec#Blank_identifier).Operands may be parenthesized.

```
x = 1
*p = f()
a[i] = 23
(k) = <-ch  // same as: k = <-ch

```

An *assignment operation* `x` *op*`=``y` where *op* is a binary arithmetic operation is equivalentto `x` `=` `x` *op*`y` but evaluates `x`only once.  The *op*`=` construct is a single token.In assignment operations, both the left- and right-hand expression listsmust contain exactly one single-valued expression, and the left-handexpression must not be the blank identifier.

```
a[i] <<= 2
i &^= 1<<n

```

A tuple assignment assigns the individual elements of a multi-valuedoperation to a list of variables.  There are two forms.  In thefirst, the right hand operand is a single multi-valued expressionsuch as a function call, a [channel](http://golang.org/ref/spec#Channel_types) or[map](http://golang.org/ref/spec#Map_types) operation, or a [type assertion](http://golang.org/ref/spec#Type_assertions).The number of operands on the lefthand side must match the number of values.  For instance, if`f` is a function returning two values,

```
x, y = f()

```

assigns the first value to `x` and the second to `y`.In the second form, the number of operands on the left must equal the numberof expressions on the right, each of which must be single-valued, and the*n*th expression on the right is assigned to the *n*thoperand on the left:

```
one, two, three = '一', '二', '三'

```

The [blank identifier](http://golang.org/ref/spec#Blank_identifier) provides a way toignore right-hand side values in an assignment:

```
_ = x       // evaluate x but ignore it
x, _ = f()  // evaluate f() but ignore second result value

```

The assignment proceeds in two phases.First, the operands of [index expressions](http://golang.org/ref/spec#Index_expressions)and [pointer indirections](http://golang.org/ref/spec#Address_operators)(including implicit pointer indirections in [selectors](http://golang.org/ref/spec#Selectors))on the left and the expressions on the right are all[evaluated in the usual order](http://golang.org/ref/spec#Order_of_evaluation).Second, the assignments are carried out in left-to-right order.

```
a, b = b, a  // exchange a and b

x := []int{1, 2, 3}
i := 0
i, x[i] = 1, 2  // set i = 1, x[0] = 2

i = 0
x[i], i = 2, 1  // set x[0] = 2, i = 1

x[0], x[0] = 1, 2  // set x[0] = 1, then x[0] = 2 (so x[0] == 2 at end)

x[1], x[3] = 4, 5  // set x[1] = 4, then panic setting x[3] = 5.

type Point struct { x, y int }
var p *Point
x[2], p.x = 6, 7  // set x[2] = 6, then panic setting p.x = 7

i = 2
x = []int{3, 5, 7}
for i, x[i] = range x {  // set i, x[2] = 0, x[0]
	break
}
// after this loop, i == 0 and x == []int{3, 5, 3}

```

In assignments, each value must be [assignable](http://golang.org/ref/spec#Assignability)to the type of the operand to which it is assigned, with the following special cases:

1. ​	Any typed value may be assigned to the blank identifier.
2. ​	If an untyped constant	is assigned to a variable of interface type or the blank identifier,	the constant is first [converted](http://golang.org/ref/spec#Conversions) to its	 [default type](http://golang.org/ref/spec#Constants).
3. ​	If an untyped boolean value is assigned to a variable of interface type or	the blank identifier, it is first converted to type `bool`.

### If statements

"If" statements specify the conditional execution of two branchesaccording to the value of a boolean expression.  If the expressionevaluates to true, the "if" branch is executed, otherwise, ifpresent, the "else" branch is executed.

```
IfStmt = "if" [ SimpleStmt ";" ] Expression Block [ "else" ( IfStmt | Block ) ] .

```

```
if x > max {
	x = max
}

```

The expression may be preceded by a simple statement, whichexecutes before the expression is evaluated.

```
if x := f(); x < y {
	return x
} else if x > z {
	return z
} else {
	return y
}

```

### Switch statements

"Switch" statements provide multi-way execution.An expression or type specifier is compared to the "cases"inside the "switch" to determine which branchto execute.

```
SwitchStmt = ExprSwitchStmt | TypeSwitchStmt .

```

There are two forms: expression switches and type switches.In an expression switch, the cases contain expressions that are comparedagainst the value of the switch expression.In a type switch, the cases contain types that are compared against thetype of a specially annotated switch expression.

#### Expression switches

In an expression switch,the switch expression is evaluated andthe case expressions, which need not be constants,are evaluated left-to-right and top-to-bottom; the first one that equals theswitch expressiontriggers execution of the statements of the associated case;the other cases are skipped.If no case matches and there is a "default" case,its statements are executed.There can be at most one default case and it may appear anywhere in the"switch" statement.A missing switch expression is equivalent to the boolean value`true`.

```
ExprSwitchStmt = "switch" [ SimpleStmt ";" ] [ Expression ] "{" { ExprCaseClause } "}" .
ExprCaseClause = ExprSwitchCase ":" StatementList .
ExprSwitchCase = "case" ExpressionList | "default" .

```

In a case or default clause, the last non-empty statementmay be a (possibly [labeled](http://golang.org/ref/spec#Labeled_statements))["fallthrough" statement](http://golang.org/ref/spec#Fallthrough_statements) toindicate that control should flow from the end of this clause tothe first statement of the next clause.Otherwise control flows to the end of the "switch" statement.A "fallthrough" statement may appear as the last statement of allbut the last clause of an expression switch.

The expression may be preceded by a simple statement, whichexecutes before the expression is evaluated.

```
switch tag {
default: s3()
case 0, 1, 2, 3: s1()
case 4, 5, 6, 7: s2()
}

switch x := f(); {  // missing switch expression means "true"
case x < 0: return -x
default: return x
}

switch {
case x < y: f1()
case x < z: f2()
case x == 4: f3()
}

```

#### Type switches

A type switch compares types rather than values. It is otherwise similarto an expression switch. It is marked by a special switch expression thathas the form of a [type assertion](http://golang.org/ref/spec#Type_assertions)using the reserved word `type` rather than an actual type:

```
switch x.(type) {
// cases
}

```

Cases then match actual types `T` against the dynamic type of theexpression `x`. As with type assertions, `x` must be of[interface type](http://golang.org/ref/spec#Interface_types), and each non-interface type`T` listed in a case must implement the type of `x`.

```
TypeSwitchStmt  = "switch" [ SimpleStmt ";" ] TypeSwitchGuard "{" { TypeCaseClause } "}" .
TypeSwitchGuard = [ identifier ":=" ] PrimaryExpr "." "(" "type" ")" .
TypeCaseClause  = TypeSwitchCase ":" StatementList .
TypeSwitchCase  = "case" TypeList | "default" .
TypeList        = Type { "," Type } .

```

The TypeSwitchGuard may include a[short variable declaration](http://golang.org/ref/spec#Short_variable_declarations).When that form is used, the variable is declared at the beginning ofthe [implicit block](http://golang.org/ref/spec#Blocks) in each clause.In clauses with a case listing exactly one type, the variablehas that type; otherwise, the variable has the type of the expressionin the TypeSwitchGuard.

The type in a case may be [`nil`](http://golang.org/ref/spec#Predeclared_identifiers);that case is used when the expression in the TypeSwitchGuardis a `nil` interface value.

Given an expression `x` of type `interface{}`,the following type switch:

```
switch i := x.(type) {
case nil:
	printString("x is nil")                // type of i is type of x (interface{})
case int:
	printInt(i)                            // type of i is int
case float64:
	printFloat64(i)                        // type of i is float64
case func(int) float64:
	printFunction(i)                       // type of i is func(int) float64
case bool, string:
	printString("type is bool or string")  // type of i is type of x (interface{})
default:
	printString("don't know the type")     // type of i is type of x (interface{})
}

```

could be rewritten:

```
v := x  // x is evaluated exactly once
if v == nil {
	i := v                                 // type of i is type of x (interface{})
	printString("x is nil")
} else if i, isInt := v.(int); isInt {
	printInt(i)                            // type of i is int
} else if i, isFloat64 := v.(float64); isFloat64 {
	printFloat64(i)                        // type of i is float64
} else if i, isFunc := v.(func(int) float64); isFunc {
	printFunction(i)                       // type of i is func(int) float64
} else {
	_, isBool := v.(bool)
	_, isString := v.(string)
	if isBool || isString {
		i := v                         // type of i is type of x (interface{})
		printString("type is bool or string")
	} else {
		i := v                         // type of i is type of x (interface{})
		printString("don't know the type")
	}
}

```

The type switch guard may be preceded by a simple statement, whichexecutes before the guard is evaluated.

The "fallthrough" statement is not permitted in a type switch.

### For statements

A "for" statement specifies repeated execution of a block. The iteration iscontrolled by a condition, a "for" clause, or a "range" clause.

```
ForStmt = "for" [ Condition | ForClause | RangeClause ] Block .
Condition = Expression .

```

In its simplest form, a "for" statement specifies the repeated execution ofa block as long as a boolean condition evaluates to true.The condition is evaluated before each iteration.If the condition is absent, it is equivalent to the boolean value`true`.

```
for a < b {
	a *= 2
}

```

A "for" statement with a ForClause is also controlled by its condition, butadditionally it may specify an *init*and a *post* statement, such as an assignment,an increment or decrement statement. The init statement may be a[short variable declaration](http://golang.org/ref/spec#Short_variable_declarations), but the post statement must not.Variables declared by the init statement are re-used in each iteration.

```
ForClause = [ InitStmt ] ";" [ Condition ] ";" [ PostStmt ] .
InitStmt = SimpleStmt .
PostStmt = SimpleStmt .

```

```
for i := 0; i < 10; i++ {
	f(i)
}

```

If non-empty, the init statement is executed once before evaluating thecondition for the first iteration;the post statement is executed after each execution of the block (andonly if the block was executed).Any element of the ForClause may be empty but the[semicolons](http://golang.org/ref/spec#Semicolons) arerequired unless there is only a condition.If the condition is absent, it is equivalent to the boolean value`true`.

```
for cond { S() }    is the same as    for ; cond ; { S() }
for      { S() }    is the same as    for true     { S() }

```

A "for" statement with a "range" clauseiterates through all entries of an array, slice, string or map,or values received on a channel. For each entry it assigns *iteration values*to corresponding *iteration variables* if present and then executes the block.

```
RangeClause = [ ExpressionList "=" | IdentifierList ":=" ] "range" Expression .

```

The expression on the right in the "range" clause is called the *range expression*,which may be an array, pointer to an array, slice, string, map, or channel permitting[receive operations](http://golang.org/ref/spec#Receive_operator).As with an assignment, if present the operands on the left must be[addressable](http://golang.org/ref/spec#Address_operators) or map index expressions; theydenote the iteration variables. If the range expression is a channel, at mostone iteration variable is permitted, otherwise there may be up to two.If the last iteration variable is the [blank identifier](http://golang.org/ref/spec#Blank_identifier),the range clause is equivalent to the same clause without that identifier.

The range expression is evaluated once before beginning the loop,with one exception: if the range expression is an array or a pointer to an arrayand at most one iteration variable is present, only the range expression'slength is evaluated; if that length is constant,[by definition](http://golang.org/ref/spec#Length_and_capacity)the range expression itself will not be evaluated.

Function calls on the left are evaluated once per iteration.For each iteration, iteration values are produced as followsif the respective iteration variables are present:

```
Range expression                          1st value          2nd value

array or slice  a  [n]E, *[n]E, or []E    index    i  int    a[i]       E
string          s  string type            index    i  int    see below  rune
map             m  map[K]V                key      k  K      m[k]       V
channel         c  chan E, <-chan E       element  e  E

```

1. For an array, pointer to array, or slice value `a`, the index iterationvalues are produced in increasing order, starting at element index 0.If at most one iteration variable is present, the range loop producesiteration values from 0 up to `len(a)-1` and does not index into the arrayor slice itself. For a `nil` slice, the number of iterations is 0.
2. For a string value, the "range" clause iterates over the Unicode code pointsin the string starting at byte index 0.  On successive iterations, the index value will be theindex of the first byte of successive UTF-8-encoded code points in the string,and the second value, of type `rune`, will be the value ofthe corresponding code point.  If the iteration encounters an invalidUTF-8 sequence, the second value will be `0xFFFD`,the Unicode replacement character, and the next iteration will advancea single byte in the string.
3. The iteration order over maps is not specifiedand is not guaranteed to be the same from one iteration to the next.If map entries that have not yet been reached are removed during iteration,the corresponding iteration values will not be produced. If map entries arecreated during iteration, that entry may be produced during the iteration ormay be skipped. The choice may vary for each entry created and from oneiteration to the next.If the map is `nil`, the number of iterations is 0.
4. For channels, the iteration values produced are the successive values sent onthe channel until the channel is [closed](http://golang.org/ref/spec#Close). If the channelis `nil`, the range expression blocks forever.

The iteration values are assigned to the respectiveiteration variables as in an [assignment statement](http://golang.org/ref/spec#Assignments).

The iteration variables may be declared by the "range" clause using a form of[short variable declaration](http://golang.org/ref/spec#Short_variable_declarations)(`:=`).In this case their types are set to the types of the respective iteration valuesand their [scope](http://golang.org/ref/spec#Declarations_and_scope) is the block of the "for"statement; they are re-used in each iteration.If the iteration variables are declared outside the "for" statement,after execution their values will be those of the last iteration.

```
var testdata *struct {
	a *[7]int
}
for i, _ := range testdata.a {
	// testdata.a is never evaluated; len(testdata.a) is constant
	// i ranges from 0 to 6
	f(i)
}

var a [10]string
for i, s := range a {
	// type of i is int
	// type of s is string
	// s == a[i]
	g(i, s)
}

var key string
var val interface {}  // value type of m is assignable to val
m := map[string]int{"mon":0, "tue":1, "wed":2, "thu":3, "fri":4, "sat":5, "sun":6}
for key, val = range m {
	h(key, val)
}
// key == last map key encountered in iteration
// val == map[key]

var ch chan Work = producer()
for w := range ch {
	doWork(w)
}

// empty a channel
for range ch {}

```

### Go statements

A "go" statement starts the execution of a function callas an independent concurrent thread of control, or *goroutine*,within the same address space.

```
GoStmt = "go" Expression .

```

The expression must be a function or method call; it cannot be parenthesized.Calls of built-in functions are restricted as for[expression statements](http://golang.org/ref/spec#Expression_statements).

The function value and parameters are[evaluated as usual](http://golang.org/ref/spec#Calls)in the calling goroutine, butunlike with a regular call, program execution does not waitfor the invoked function to complete.Instead, the function begins executing independentlyin a new goroutine.When the function terminates, its goroutine also terminates.If the function has any return values, they are discarded when thefunction completes.

```
go Server()
go func(ch chan<- bool) { for { sleep(10); ch <- true; }} (c)

```

### Select statements

A "select" statement chooses which of a set of possible[send](http://golang.org/ref/spec#Send_statements) or[receive](http://golang.org/ref/spec#Receive_operator)operations will proceed.It looks similar to a["switch"](http://golang.org/ref/spec#Switch_statements) statement but with thecases all referring to communication operations.

```
SelectStmt = "select" "{" { CommClause } "}" .
CommClause = CommCase ":" StatementList .
CommCase   = "case" ( SendStmt | RecvStmt ) | "default" .
RecvStmt   = [ ExpressionList "=" | IdentifierList ":=" ] RecvExpr .
RecvExpr   = Expression .

```

A case with a RecvStmt may assign the result of a RecvExpr to one ortwo variables, which may be declared using a[short variable declaration](http://golang.org/ref/spec#Short_variable_declarations).The RecvExpr must be a (possibly parenthesized) receive operation.There can be at most one default case and it may appear anywherein the list of cases.

Execution of a "select" statement proceeds in several steps:

1. For all the cases in the statement, the channel operands of receive operationsand the channel and right-hand-side expressions of send statements areevaluated exactly once, in source order, upon entering the "select" statement.The result is a set of channels to receive from or send to,and the corresponding values to send.Any side effects in that evaluation will occur irrespective of which (if any)communication operation is selected to proceed.Expressions on the left-hand side of a RecvStmt with a short variable declarationor assignment are not yet evaluated.
2. If one or more of the communications can proceed,a single one that can proceed is chosen via a uniform pseudo-random selection.Otherwise, if there is a default case, that case is chosen.If there is no default case, the "select" statement blocks untilat least one of the communications can proceed.
3. Unless the selected case is the default case, the respective communicationoperation is executed.
4. If the selected case is a RecvStmt with a short variable declaration oran assignment, the left-hand side expressions are evaluated and thereceived value (or values) are assigned.
5. The statement list of the selected case is executed.

Since communication on `nil` channels can never proceed,a select with only `nil` channels and no default case blocks forever.

```
var a []int
var c, c1, c2, c3, c4 chan int
var i1, i2 int
select {
case i1 = <-c1:
	print("received ", i1, " from c1\n")
case c2 <- i2:
	print("sent ", i2, " to c2\n")
case i3, ok := (<-c3):  // same as: i3, ok := <-c3
	if ok {
		print("received ", i3, " from c3\n")
	} else {
		print("c3 is closed\n")
	}
case a[f()] = <-c4:
	// same as:
	// case t := <-c4
	//	a[f()] = t
default:
	print("no communication\n")
}

for {  // send random sequence of bits to c
	select {
	case c <- 0:  // note: no statement, no fallthrough, no folding of cases
	case c <- 1:
	}
}

select {}  // block forever

```

### Return statements

A "return" statement in a function `F` terminates the executionof `F`, and optionally provides one or more result values.Any functions [deferred](http://golang.org/ref/spec#Defer_statements) by `F`are executed before `F` returns to its caller.

```
ReturnStmt = "return" [ ExpressionList ] .

```

In a function without a result type, a "return" statement must notspecify any result values.

```
func noResult() {
	return
}

```

There are three ways to return values from a function with a resulttype:

1. The return value or values may be explicitly listed		in the "return" statement. Each expression must be single-valued		and 

   assignable

   ​		to the corresponding element of the function's result type.

   ```
   func simpleF() int {
   	return 2
   }

   func complexF1() (re float64, im float64) {
   	return -7.0, -4.0
   }

   ```

2. The expression list in the "return" statement may be a single		call to a multi-valued function. The effect is as if each value		returned from that function were assigned to a temporary		variable with the type of the respective value, followed by a		"return" statement listing these variables, at which point the		rules of the previous case apply.

   ```
   func complexF2() (re float64, im float64) {
   	return complexF1()
   }

   ```

3. The expression list may be empty if the function's result		type specifies names for its 

   result parameters

   .		The result parameters act as ordinary local variables		and the function may assign values to them as necessary.		The "return" statement returns the values of these variables.

   ```
   func complexF3() (re float64, im float64) {
   	re = 7.0
   	im = 4.0
   	return
   }

   func (devnull) Write(p []byte) (n int, _ error) {
   	n = len(p)
   	return
   }

   ```

Regardless of how they are declared, all the result values are initialized tothe [zero values](http://golang.org/ref/spec#The_zero_value) for their type upon entry to thefunction. A "return" statement that specifies results sets the result parameters beforeany deferred functions are executed.

Implementation restriction: A compiler may disallow an empty expression listin a "return" statement if a different entity (constant, type, or variable)with the same name as a result parameter is in[scope](http://golang.org/ref/spec#Declarations_and_scope) at the place of the return.

```
func f(n int) (res int, err error) {
	if _, err := f(n-1); err != nil {
		return  // invalid return statement: err is shadowed
	}
	return
}

```

### Break statements

A "break" statement terminates execution of the innermost["for"](http://golang.org/ref/spec#For_statements),["switch"](http://golang.org/ref/spec#Switch_statements), or["select"](http://golang.org/ref/spec#Select_statements) statementwithin the same function.

```
BreakStmt = "break" [ Label ] .

```

If there is a label, it must be that of an enclosing"for", "switch", or "select" statement,and that is the one whose execution terminates.

```
OuterLoop:
	for i = 0; i < n; i++ {
		for j = 0; j < m; j++ {
			switch a[i][j] {
			case nil:
				state = Error
				break OuterLoop
			case item:
				state = Found
				break OuterLoop
			}
		}
	}

```

### Continue statements

A "continue" statement begins the next iteration of theinnermost ["for" loop](http://golang.org/ref/spec#For_statements) at its post statement.The "for" loop must be within the same function.

```
ContinueStmt = "continue" [ Label ] .

```

If there is a label, it must be that of an enclosing"for" statement, and that is the one whose executionadvances.

```
RowLoop:
	for y, row := range rows {
		for x, data := range row {
			if data == endOfRow {
				continue RowLoop
			}
			row[x] = data + bias(x, y)
		}
	}

```

### Goto statements

A "goto" statement transfers control to the statement with the corresponding labelwithin the same function.

```
GotoStmt = "goto" Label .

```

```
goto Error

```

Executing the "goto" statement must not cause any variables to come into[scope](http://golang.org/ref/spec#Declarations_and_scope) that were not already in scope at the point of the goto.For instance, this example:

```
	goto L  // BAD
	v := 3
L:

```

is erroneous because the jump to label `L` skipsthe creation of `v`.

A "goto" statement outside a [block](http://golang.org/ref/spec#Blocks) cannot jump to a label inside that block.For instance, this example:

```
if n%2 == 1 {
	goto L1
}
for n > 0 {
	f()
	n--
L1:
	f()
	n--
}

```

is erroneous because the label `L1` is insidethe "for" statement's block but the `goto` is not.

### Fallthrough statements

A "fallthrough" statement transfers control to the first statement of thenext case clause in a [expression "switch" statement](http://golang.org/ref/spec#Expression_switches).It may be used only as the final non-empty statement in such a clause.

```
FallthroughStmt = "fallthrough" .

```

### Defer statements

A "defer" statement invokes a function whose execution is deferredto the moment the surrounding function returns, either because thesurrounding function executed a [return statement](http://golang.org/ref/spec#Return_statements),reached the end of its [function body](http://golang.org/ref/spec#Function_declarations),or because the corresponding goroutine is [panicking](http://golang.org/ref/spec#Handling_panics).

```
DeferStmt = "defer" Expression .

```

The expression must be a function or method call; it cannot be parenthesized.Calls of built-in functions are restricted as for[expression statements](http://golang.org/ref/spec#Expression_statements).

Each time a "defer" statementexecutes, the function value and parameters to the call are[evaluated as usual](http://golang.org/ref/spec#Calls)and saved anew but the actual function is not invoked.Instead, deferred functions are invoked immediately beforethe surrounding function returns, in the reverse orderthey were deferred.If a deferred function value evaluatesto `nil`, execution [panics](http://golang.org/ref/spec#Handling_panics)when the function is invoked, not when the "defer" statement is executed.

For instance, if the deferred function isa [function literal](http://golang.org/ref/spec#Function_literals) and the surroundingfunction has [named result parameters](http://golang.org/ref/spec#Function_types) thatare in scope within the literal, the deferred function may access and modifythe result parameters before they are returned.If the deferred function has any return values, they are discarded whenthe function completes.(See also the section on [handling panics](http://golang.org/ref/spec#Handling_panics).)

```
lock(l)
defer unlock(l)  // unlocking happens before surrounding function returns

// prints 3 2 1 0 before surrounding function returns
for i := 0; i <= 3; i++ {
	defer fmt.Print(i)
}

// f returns 1
func f() (result int) {
	defer func() {
		result++
	}()
	return 0
}

```

## Built-in functions

Built-in functions are[predeclared](http://golang.org/ref/spec#Predeclared_identifiers).They are called like any other function but some of themaccept a type instead of an expression as the first argument.

The built-in functions do not have standard Go types,so they can only appear in [call expressions](http://golang.org/ref/spec#Calls);they cannot be used as function values.

### Close

For a channel `c`, the built-in function `close(c)`records that no more values will be sent on the channel.It is an error if `c` is a receive-only channel.Sending to or closing a closed channel causes a [run-time panic](http://golang.org/ref/spec#Run_time_panics).Closing the nil channel also causes a [run-time panic](http://golang.org/ref/spec#Run_time_panics).After calling `close`, and after any previouslysent values have been received, receive operations will returnthe zero value for the channel's type without blocking.The multi-valued [receive operation](http://golang.org/ref/spec#Receive_operator)returns a received value along with an indication of whether the channel is closed.

### Length and capacity

The built-in functions `len` and `cap` take argumentsof various types and return a result of type `int`.The implementation guarantees that the result always fits into an `int`.

```
Call      Argument type    Result

len(s)    string type      string length in bytes
          [n]T, *[n]T      array length (== n)
          []T              slice length
          map[K]T          map length (number of defined keys)
          chan T           number of elements queued in channel buffer

cap(s)    [n]T, *[n]T      array length (== n)
          []T              slice capacity
          chan T           channel buffer capacity

```

The capacity of a slice is the number of elements for which there isspace allocated in the underlying array.At any time the following relationship holds:

```
0 <= len(s) <= cap(s)

```

The length of a `nil` slice, map or channel is 0.The capacity of a `nil` slice or channel is 0.

The expression `len(s)` is [constant](http://golang.org/ref/spec#Constants) if`s` is a string constant. The expressions `len(s)` and`cap(s)` are constants if the type of `s` is an arrayor pointer to an array and the expression `s` does not contain[channel receives](http://golang.org/ref/spec#Receive_operator) or (non-constant)[function calls](http://golang.org/ref/spec#Calls); in this case `s` is not evaluated.Otherwise, invocations of `len` and `cap` are notconstant and `s` is evaluated.

```
const (
	c1 = imag(2i)                    // imag(2i) = 2.0 is a constant
	c2 = len([10]float64{2})         // [10]float64{2} contains no function calls
	c3 = len([10]float64{c1})        // [10]float64{c1} contains no function calls
	c4 = len([10]float64{imag(2i)})  // imag(2i) is a constant and no function call is issued
	c5 = len([10]float64{imag(z)})   // invalid: imag(x) is a (non-constant) function call
)
var z complex128

```

### Allocation

The built-in function `new` takes a type `T`,allocates storage for a [variable](http://golang.org/ref/spec#Variables) of that typeat run time, and returns a value of type `*T`[pointing](http://golang.org/ref/spec#Pointer_types) to it.The variable is initialized as described in the section on[initial values](http://golang.org/ref/spec#The_zero_value).

```
new(T)

```

For instance

```
type S struct { a int; b float64 }
new(S)

```

allocates storage for a variable of type `S`,initializes it (`a=0`, `b=0.0`),and returns a value of type `*S` containing the addressof the location.

### Making slices, maps and channels

The built-in function `make` takes a type `T`,which must be a slice, map or channel type,optionally followed by a type-specific list of expressions.It returns a value of type `T` (not `*T`).The memory is initialized as described in the section on[initial values](http://golang.org/ref/spec#The_zero_value).

```
Call             Type T     Result

make(T, n)       slice      slice of type T with length n and capacity n
make(T, n, m)    slice      slice of type T with length n and capacity m

make(T)          map        map of type T
make(T, n)       map        map of type T with initial space for n elements

make(T)          channel    unbuffered channel of type T
make(T, n)       channel    buffered channel of type T, buffer size n

```

The size arguments `n` and `m` must be of integer type or untyped.A [constant](http://golang.org/ref/spec#Constants) size argument must be non-negative andrepresentable by a value of type `int`.If both `n` and `m` are provided and are constant, then`n` must be no larger than `m`.If `n` is negative or larger than `m` at run time,a [run-time panic](http://golang.org/ref/spec#Run_time_panics) occurs.

```
s := make([]int, 10, 100)       // slice with len(s) == 10, cap(s) == 100
s := make([]int, 1e3)           // slice with len(s) == cap(s) == 1000
s := make([]int, 1<<63)         // illegal: len(s) is not representable by a value of type int
s := make([]int, 10, 0)         // illegal: len(s) > cap(s)
c := make(chan int, 10)         // channel with a buffer size of 10
m := make(map[string]int, 100)  // map with initial space for 100 elements

```

### Appending to and copying slices

The built-in functions `append` and `copy` assist incommon slice operations.For both functions, the result is independent of whether the memory referencedby the arguments overlaps.

The [variadic](http://golang.org/ref/spec#Function_types) function `append`appends zero or more values `x`to `s` of type `S`, which must be a slice type, andreturns the resulting slice, also of type `S`.The values `x` are passed to a parameter of type `...T`where `T` is the [element type](http://golang.org/ref/spec#Slice_types) of`S` and the respective[parameter passing rules](http://golang.org/ref/spec#Passing_arguments_to_..._parameters) apply.As a special case, `append` also accepts a first argumentassignable to type `[]byte` with a second argument ofstring type followed by `...`. This form appends thebytes of the string.

```
append(s S, x ...T) S  // T is the element type of S

```

If the capacity of `s` is not large enough to fit the additionalvalues, `append` allocates a new, sufficiently large underlyingarray that fits both the existing slice elements and the additional values.Otherwise, `append` re-uses the underlying array.

```
s0 := []int{0, 0}
s1 := append(s0, 2)                // append a single element     s1 == []int{0, 0, 2}
s2 := append(s1, 3, 5, 7)          // append multiple elements    s2 == []int{0, 0, 2, 3, 5, 7}
s3 := append(s2, s0...)            // append a slice              s3 == []int{0, 0, 2, 3, 5, 7, 0, 0}
s4 := append(s3[3:6], s3[2:]...)   // append overlapping slice    s4 == []int{3, 5, 7, 2, 3, 5, 7, 0, 0}

var t []interface{}
t = append(t, 42, 3.1415, "foo")                                  t == []interface{}{42, 3.1415, "foo"}

var b []byte
b = append(b, "bar"...)            // append string contents      b == []byte{'b', 'a', 'r' }

```

The function `copy` copies slice elements froma source `src` to a destination `dst` and returns thenumber of elements copied.Both arguments must have [identical](http://golang.org/ref/spec#Type_identity) element type `T` and must be[assignable](http://golang.org/ref/spec#Assignability) to a slice of type `[]T`.The number of elements copied is the minimum of`len(src)` and `len(dst)`.As a special case, `copy` also accepts a destination argument assignableto type `[]byte` with a source argument of a string type.This form copies the bytes from the string into the byte slice.

```
copy(dst, src []T) int
copy(dst []byte, src string) int

```

Examples:

```
var a = [...]int{0, 1, 2, 3, 4, 5, 6, 7}
var s = make([]int, 6)
var b = make([]byte, 5)
n1 := copy(s, a[0:])            // n1 == 6, s == []int{0, 1, 2, 3, 4, 5}
n2 := copy(s, s[2:])            // n2 == 4, s == []int{2, 3, 4, 5, 4, 5}
n3 := copy(b, "Hello, World!")  // n3 == 5, b == []byte("Hello")

```

### Deletion of map elements

The built-in function `delete` removes the element with key`k` from a [map](http://golang.org/ref/spec#Map_types) `m`. Thetype of `k` must be [assignable](http://golang.org/ref/spec#Assignability)to the key type of `m`.

```
delete(m, k)  // remove element m[k] from map m

```

If the map `m` is `nil` or the element `m[k]`does not exist, `delete` is a no-op.

### Manipulating complex numbers

Three functions assemble and disassemble complex numbers.The built-in function `complex` constructs a complexvalue from a floating-point real and imaginary part, while`real` and `imag`extract the real and imaginary parts of a complex value.

```
complex(realPart, imaginaryPart floatT) complexT
real(complexT) floatT
imag(complexT) floatT

```

The type of the arguments and return value correspond.For `complex`, the two arguments must be of the samefloating-point type and the return type is the complex typewith the corresponding floating-point constituents:`complex64` for `float32`,`complex128` for `float64`.The `real` and `imag` functionstogether form the inverse, so for a complex value `z`,`z` `==` `complex(real(z),` `imag(z))`.

If the operands of these functions are all constants, the returnvalue is a constant.

```
var a = complex(2, -2)             // complex128
var b = complex(1.0, -1.4)         // complex128
x := float32(math.Cos(math.Pi/2))  // float32
var c64 = complex(5, -x)           // complex64
var im = imag(b)                   // float64
var rl = real(c64)                 // float32

```

### Handling panics

 Two built-in functions, `panic` and `recover`,assist in reporting and handling [run-time panics](http://golang.org/ref/spec#Run_time_panics)and program-defined error conditions.

```
func panic(interface{})
func recover() interface{}

```

While executing a function `F`,an explicit call to `panic` or a [run-time panic](http://golang.org/ref/spec#Run_time_panics)terminates the execution of `F`.Any functions [deferred](http://golang.org/ref/spec#Defer_statements) by `F`are then executed as usual.Next, any deferred functions run by `F's` caller are run,and so on up to any deferred by the top-level function in the executing goroutine.At that point, the program is terminated and the errorcondition is reported, including the value of the argument to `panic`.This termination sequence is called *panicking*.

```
panic(42)
panic("unreachable")
panic(Error("cannot parse"))

```

The `recover` function allows a program to manage behaviorof a panicking goroutine.Suppose a function `G` defers a function `D` that calls`recover` and a panic occurs in a function on the same goroutine in which `G`is executing.When the running of deferred functions reaches `D`,the return value of `D`'s call to `recover` will be the value passed to the call of `panic`.If `D` returns normally, without starting a new`panic`, the panicking sequence stops. In that case,the state of functions called between `G` and the call to `panic`is discarded, and normal execution resumes.Any functions deferred by `G` before `D` are then run and `G`'sexecution terminates by returning to its caller.

The return value of `recover` is `nil` if any of the following conditions holds:

- `panic`'s argument was `nil`;
- the goroutine is not panicking;
- `recover` was not called directly by a deferred function.

The `protect` function in the example below invokesthe function argument `g` and protects callers fromrun-time panics raised by `g`.

```
func protect(g func()) {
	defer func() {
		log.Println("done")  // Println executes normally even if there is a panic
		if x := recover(); x != nil {
			log.Printf("run time panic: %v", x)
		}
	}()
	log.Println("start")
	g()
}

```

### Bootstrapping

Current implementations provide several built-in functions useful duringbootstrapping. These functions are documented for completeness but are notguaranteed to stay in the language. They do not return a result.

```
Function   Behavior

print      prints all arguments; formatting of arguments is implementation-specific
println    like print but prints spaces between arguments and a newline at the end

```

## Packages

Go programs are constructed by linking together *packages*.A package in turn is constructed from one or more source filesthat together declare constants, types, variables and functionsbelonging to the package and which are accessible in all filesof the same package. Those elements may be[exported](http://golang.org/ref/spec#Exported_identifiers) and used in another package.

### Source file organization

Each source file consists of a package clause defining the packageto which it belongs, followed by a possibly empty set of importdeclarations that declare packages whose contents it wishes to use,followed by a possibly empty set of declarations of functions,types, variables, and constants.

```
SourceFile       = PackageClause ";" { ImportDecl ";" } { TopLevelDecl ";" } .

```

### Package clause

A package clause begins each source file and defines the packageto which the file belongs.

```
PackageClause  = "package" PackageName .
PackageName    = identifier .

```

The PackageName must not be the [blank identifier](http://golang.org/ref/spec#Blank_identifier).

```
package math

```

A set of files sharing the same PackageName form the implementation of a package.An implementation may require that all source files for a package inhabit the same directory.

### Import declarations

An import declaration states that the source file containing the declarationdepends on functionality of the *imported* package([§Program initialization and execution](http://golang.org/ref/spec#Program_initialization_and_execution))and enables access to [exported](http://golang.org/ref/spec#Exported_identifiers) identifiersof that package.The import names an identifier (PackageName) to be used for access and an ImportPaththat specifies the package to be imported.

```
ImportDecl       = "import" ( ImportSpec | "(" { ImportSpec ";" } ")" ) .
ImportSpec       = [ "." | PackageName ] ImportPath .
ImportPath       = string_lit .

```

The PackageName is used in [qualified identifiers](http://golang.org/ref/spec#Qualified_identifiers)to access exported identifiers of the package within the importing source file.It is declared in the [file block](http://golang.org/ref/spec#Blocks).If the PackageName is omitted, it defaults to the identifier specified in the[package clause](http://golang.org/ref/spec#Package_clause) of the imported package.If an explicit period (`.`) appears instead of a name, all thepackage's exported identifiers declared in that package's[package block](http://golang.org/ref/spec#Blocks) will be declared in the importing sourcefile's file block and must be accessed without a qualifier.

The interpretation of the ImportPath is implementation-dependent butit is typically a substring of the full file name of the compiledpackage and may be relative to a repository of installed packages.

Implementation restriction: A compiler may restrict ImportPaths tonon-empty strings using only characters belonging to[Unicode's](http://www.unicode.org/versions/Unicode6.3.0/)L, M, N, P, and S general categories (the Graphic characters withoutspaces) and may also exclude the characters`!"#$%&'()*,:;<=>?[\]^`{|}`and the Unicode replacement character U+FFFD.

Assume we have compiled a package containing the package clause`package math`, which exports function `Sin`, andinstalled the compiled package in the file identified by`"lib/math"`.This table illustrates how `Sin` is accessed in filesthat import the package after thevarious types of import declaration.

```
Import declaration          Local name of Sin

import   "lib/math"         math.Sin
import m "lib/math"         m.Sin
import . "lib/math"         Sin

```

An import declaration declares a dependency relation betweenthe importing and imported package.It is illegal for a package to import itself, directly or indirectly,or to directly import a package withoutreferring to any of its exported identifiers. To import a package solely forits side-effects (initialization), use the [blank](http://golang.org/ref/spec#Blank_identifier)identifier as explicit package name:

```
import _ "lib/math"

```

### An example package

Here is a complete Go package that implements a concurrent prime sieve.

```
package main

import "fmt"

// Send the sequence 2, 3, 4, … to channel 'ch'.
func generate(ch chan<- int) {
	for i := 2; ; i++ {
		ch <- i  // Send 'i' to channel 'ch'.
	}
}

// Copy the values from channel 'src' to channel 'dst',
// removing those divisible by 'prime'.
func filter(src <-chan int, dst chan<- int, prime int) {
	for i := range src {  // Loop over values received from 'src'.
		if i%prime != 0 {
			dst <- i  // Send 'i' to channel 'dst'.
		}
	}
}

// The prime sieve: Daisy-chain filter processes together.
func sieve() {
	ch := make(chan int)  // Create a new channel.
	go generate(ch)       // Start generate() as a subprocess.
	for {
		prime := <-ch
		fmt.Print(prime, "\n")
		ch1 := make(chan int)
		go filter(ch, ch1, prime)
		ch = ch1
	}
}

func main() {
	sieve()
}

```

## Program initialization and execution

### The zero value

When storage is allocated for a [variable](http://golang.org/ref/spec#Variables),either through a declaration or a call of `new`, or whena new value is created, either through a composite literal or a callof `make`,and no explicit initialization is provided, the variable or value isgiven a default value.  Each element of such a variable or value isset to the *zero value* for its type: `false` for booleans,`0` for integers, `0.0` for floats, `""`for strings, and `nil` for pointers, functions, interfaces, slices, channels, and maps.This initialization is done recursively, so for instance each element of anarray of structs will have its fields zeroed if no value is specified.

These two simple declarations are equivalent:

```
var i int
var i int = 0

```

After

```
type T struct { i int; f float64; next *T }
t := new(T)

```

the following holds:

```
t.i == 0
t.f == 0.0
t.next == nil

```

The same would also be true after

```
var t T

```

### Package initialization

Within a package, package-level variables are initialized in*declaration order* but after any of the variablesthey *depend* on.

More precisely, a package-level variable is considered *ready forinitialization* if it is not yet initialized and either hasno [initialization expression](http://golang.org/ref/spec#Variable_declarations) orits initialization expression has no dependencies on uninitialized variables.Initialization proceeds by repeatedly initializing the next package-levelvariable that is earliest in declaration order and ready for initialization,until there are no variables ready for initialization.

If any variables are still uninitialized when thisprocess ends, those variables are part of one or more initialization cycles,and the program is not valid.

The declaration order of variables declared in multiple files is determinedby the order in which the files are presented to the compiler: Variablesdeclared in the first file are declared before any of the variables declaredin the second file, and so on.

Dependency analysis does not rely on the actual values of thevariables, only on lexical *references* to them in the source,analyzed transitively. For instance, if a variable `x`'sinitialization expression refers to a function whose body refers tovariable `y` then `x` depends on `y`.Specifically:

- A reference to a variable or function is an identifier denoting thatvariable or function.
- A reference to a method `m` is a[method value](http://golang.org/ref/spec#Method_values) or[method expression](http://golang.org/ref/spec#Method_expressions) of the form`t.m`, where the (static) type of `t` isnot an interface type, and the method `m` is in the[method set](http://golang.org/ref/spec#Method_sets) of `t`.It is immaterial whether the resulting function value`t.m` is invoked.
- A variable, function, or method `x` depends on a variable`y` if `x`'s initialization expression or body(for functions and methods) contains a reference to `y`or to a function or method that depends on `y`.

Dependency analysis is performed per package; only references referringto variables, functions, and methods declared in the current packageare considered.

For example, given the declarations

```
var (
	a = c + b
	b = f()
	c = f()
	d = 3
)

func f() int {
	d++
	return d
}

```

the initialization order is `d`, `b`, `c`, `a`.

Variables may also be initialized using functions named `init`declared in the package block, with no arguments and no result parameters.

```
func init() { … }

```

Multiple such functions may be defined, even within a singlesource file. The `init` identifier is not[declared](http://golang.org/ref/spec#Declarations_and_scope) and thus`init` functions cannot be referred to from anywherein a program.

A package with no imports is initialized by assigning initial valuesto all its package-level variables followed by calling all `init`functions in the order they appear in the source, possibly in multiple files,as presented to the compiler.If a package has imports, the imported packages are initializedbefore initializing the package itself. If multiple packages importa package, the imported package will be initialized only once.The importing of packages, by construction, guarantees that therecan be no cyclic initialization dependencies.

Package initialization—variable initialization and the invocation of`init` functions—happens in a single goroutine,sequentially, one package at a time.An `init` function may launch other goroutines, which can runconcurrently with the initialization code. However, initializationalways sequencesthe `init` functions: it will not invoke the next oneuntil the previous one has returned.

To ensure reproducible initialization behavior, build systems are encouragedto present multiple files belonging to the same package in lexical file nameorder to a compiler.

### Program execution

A complete program is created by linking a single, unimported packagecalled the *main package* with all the packages it imports, transitively.The main package musthave package name `main` anddeclare a function `main` that takes noarguments and returns no value.

```
func main() { … }

```

Program execution begins by initializing the main package and theninvoking the function `main`.When that function invocation returns, the program exits.It does not wait for other (non-`main`) goroutines to complete.

## Errors

The predeclared type `error` is defined as

```
type error interface {
	Error() string
}

```

It is the conventional interface for representing an error condition,with the nil value representing no error.For instance, a function to read data from a file might be defined:

```
func Read(f *File, b []byte) (n int, err error)

```

## Run-time panics

Execution errors such as attempting to index an array outof bounds trigger a *run-time panic* equivalent to a call ofthe built-in function [`panic`](http://golang.org/ref/spec#Handling_panics)with a value of the implementation-defined interface type `runtime.Error`.That type satisfies the predeclared interface type[`error`](http://golang.org/ref/spec#Errors).The exact error values thatrepresent distinct run-time error conditions are unspecified.

```
package runtime

type Error interface {
	error
	// and perhaps other methods
}

```

## System considerations

### Package `unsafe`

The built-in package `unsafe`, known to the compiler,provides facilities for low-level programming including operationsthat violate the type system. A package using `unsafe`must be vetted manually for type safety and may not be portable.The package provides the following interface:

```
package unsafe

type ArbitraryType int  // shorthand for an arbitrary Go type; it is not a real type
type Pointer *ArbitraryType

func Alignof(variable ArbitraryType) uintptr
func Offsetof(selector ArbitraryType) uintptr
func Sizeof(variable ArbitraryType) uintptr

```

A `Pointer` is a [pointer type](http://golang.org/ref/spec#Pointer_types) but a `Pointer`value may not be [dereferenced](http://golang.org/ref/spec#Address_operators).Any pointer or value of [underlying type](http://golang.org/ref/spec#Types) `uintptr` can be converted toa `Pointer` type and vice versa.The effect of converting between `Pointer` and `uintptr` is implementation-defined.

```
var f float64
bits = *(*uint64)(unsafe.Pointer(&f))

type ptr unsafe.Pointer
bits = *(*uint64)(ptr(&f))

var p ptr = nil

```

The functions `Alignof` and `Sizeof` take an expression `x`of any type and return the alignment or size, respectively, of a hypothetical variable `v`as if `v` was declared via `var v = x`.

The function `Offsetof` takes a (possibly parenthesized) [selector](http://golang.org/ref/spec#Selectors)`s.f`, denoting a field `f` of the struct denoted by `s`or `*s`, and returns the field offset in bytes relative to the struct's address.If `f` is an [embedded field](http://golang.org/ref/spec#Struct_types), it must be reachablewithout pointer indirections through fields of the struct.For a struct `s` with field `f`:

```
uintptr(unsafe.Pointer(&s)) + unsafe.Offsetof(s.f) == uintptr(unsafe.Pointer(&s.f))

```

Computer architectures may require memory addresses to be *aligned*;that is, for addresses of a variable to be a multiple of a factor,the variable's type's *alignment*.  The function `Alignof`takes an expression denoting a variable of any type and returns thealignment of the (type of the) variable in bytes.  For a variable`x`:

```
uintptr(unsafe.Pointer(&x)) % unsafe.Alignof(x) == 0

```

Calls to `Alignof`, `Offsetof`, and`Sizeof` are compile-time constant expressions of type `uintptr`.

### Size and alignment guarantees

For the [numeric types](http://golang.org/ref/spec#Numeric_types), the following sizes are guaranteed:

```
type                                 size in bytes

byte, uint8, int8                     1
uint16, int16                         2
uint32, int32, float32                4
uint64, int64, float64, complex64     8
complex128                           16

```

The following minimal alignment properties are guaranteed:

1. For a variable `x` of any type: `unsafe.Alignof(x)` is at least 1.
2. For a variable `x` of struct type: `unsafe.Alignof(x)` is the largest of   all the values `unsafe.Alignof(x.f)` for each field `f` of `x`, but at least 1.
3. For a variable `x` of array type: `unsafe.Alignof(x)` is the same as   `unsafe.Alignof(x[0])`, but at least 1.

A struct or array type has size zero if it contains no fields (or elements, respectively) that have a size greater than zero. Two distinct zero-size variables may have the same address in memory.

Build version go1.4.2.
Except as [noted](https://developers.google.com/site-policies#restrictions),
the content of this page is licensed under the
Creative Commons Attribution 3.0 License,
and code is licensed under a [BSD license](http://golang.org/LICENSE).
[Terms of Service](http://golang.org/doc/tos.html) | 
[Privacy Policy](http://www.google.com/intl/en/policies/privacy/)

来源： <<http://golang.org/ref/spec>>

 

