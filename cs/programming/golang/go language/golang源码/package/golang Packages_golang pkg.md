

# Packages

- [Standard library](http://golang.org/pkg/#stdlib)

- [Other packages](http://golang.org/pkg/#other)

  [Sub-repositories](http://golang.org/pkg/#subrepo)

  [Community](http://golang.org/pkg/#community)

## Standard library

| Name                                     | Synopsis                                 |
| ---------------------------------------- | ---------------------------------------- |
| [archive](http://golang.org/pkg/archive/) |                                          |
| [tar](http://golang.org/pkg/archive/tar/) | Package tar implements access to tar archives. |
| [zip](http://golang.org/pkg/archive/zip/) | Package zip provides support for reading and writing ZIP archives. |
| [bufio](http://golang.org/pkg/bufio/)    | Package bufio implements buffered I/O. It wraps an io.Reader or io.Writer object, creating another object (Reader or Writer) that also implements the interface but provides buffering and some help for textual I/O. |
| [builtin](http://golang.org/pkg/builtin/) | Package builtin provides documentation for Go's predeclared identifiers. |
| [bytes](http://golang.org/pkg/bytes/)    | Package bytes implements functions for the manipulation of byte slices. |
| [compress](http://golang.org/pkg/compress/) |                                          |
| [bzip2](http://golang.org/pkg/compress/bzip2/) | Package bzip2 implements bzip2 decompression. |
| [flate](http://golang.org/pkg/compress/flate/) | Package flate implements the DEFLATE compressed data format, described in RFC 1951. |
| [gzip](http://golang.org/pkg/compress/gzip/) | Package gzip implements reading and writing of gzip format compressed files, as specified in RFC 1952. |
| [lzw](http://golang.org/pkg/compress/lzw/) | Package lzw implements the Lempel-Ziv-Welch compressed data format, described in T. A. Welch, ``A Technique for High-Performance Data Compression'', Computer, 17(6) (June 1984), pp 8-19. |
| [zlib](http://golang.org/pkg/compress/zlib/) | Package zlib implements reading and writing of zlib format compressed data, as specified in RFC 1950. |
| [container](http://golang.org/pkg/container/) |                                          |
| [heap](http://golang.org/pkg/container/heap/) | Package heap provides heap operations for any type that implements heap.Interface. |
| [list](http://golang.org/pkg/container/list/) | Package list implements a doubly linked list. |
| [ring](http://golang.org/pkg/container/ring/) | Package ring implements operations on circular lists. |
| [crypto](http://golang.org/pkg/crypto/)  | Package crypto collects common cryptographic constants. |
| [aes](http://golang.org/pkg/crypto/aes/) | Package aes implements AES encryption (formerly Rijndael), as defined in U.S. Federal Information Processing Standards Publication 197. |
| [cipher](http://golang.org/pkg/crypto/cipher/) | Package cipher implements standard block cipher modes that can be wrapped around low-level block cipher implementations. |
| [des](http://golang.org/pkg/crypto/des/) | Package des implements the Data Encryption Standard (DES) and the Triple Data Encryption Algorithm (TDEA) as defined in U.S. Federal Information Processing Standards Publication 46-3. |
| [dsa](http://golang.org/pkg/crypto/dsa/) | Package dsa implements the Digital Signature Algorithm, as defined in FIPS 186-3. |
| [ecdsa](http://golang.org/pkg/crypto/ecdsa/) | Package ecdsa implements the Elliptic Curve Digital Signature Algorithm, as defined in FIPS 186-3. |
| [elliptic](http://golang.org/pkg/crypto/elliptic/) | Package elliptic implements several standard elliptic curves over prime fields. |
| [hmac](http://golang.org/pkg/crypto/hmac/) | Package hmac implements the Keyed-Hash Message Authentication Code (HMAC) as defined in U.S. Federal Information Processing Standards Publication 198. |
| [md5](http://golang.org/pkg/crypto/md5/) | Package md5 implements the MD5 hash algorithm as defined in RFC 1321. |
| [rand](http://golang.org/pkg/crypto/rand/) | Package rand implements a cryptographically secure pseudorandom number generator. |
| [rc4](http://golang.org/pkg/crypto/rc4/) | Package rc4 implements RC4 encryption, as defined in Bruce Schneier's Applied Cryptography. |
| [rsa](http://golang.org/pkg/crypto/rsa/) | Package rsa implements RSA encryption as specified in PKCS#1. |
| [sha1](http://golang.org/pkg/crypto/sha1/) | Package sha1 implements the SHA1 hash algorithm as defined in RFC 3174. |
| [sha256](http://golang.org/pkg/crypto/sha256/) | Package sha256 implements the SHA224 and SHA256 hash algorithms as defined in FIPS 180-4. |
| [sha512](http://golang.org/pkg/crypto/sha512/) | Package sha512 implements the SHA384 and SHA512 hash algorithms as defined in FIPS 180-2. |
| [subtle](http://golang.org/pkg/crypto/subtle/) | Package subtle implements functions that are often useful in cryptographic code but require careful thought to use correctly. |
| [tls](http://golang.org/pkg/crypto/tls/) | Package tls partially implements TLS 1.2, as specified in RFC 5246. |
| [x509](http://golang.org/pkg/crypto/x509/) | Package x509 parses X.509-encoded keys and certificates. |
| [pkix](http://golang.org/pkg/crypto/x509/pkix/) | Package pkix contains shared, low level structures used for ASN.1 parsing and serialization of X.509 certificates, CRL and OCSP. |
| [database](http://golang.org/pkg/database/) |                                          |
| [sql](http://golang.org/pkg/database/sql/) | Package sql provides a generic interface around SQL (or SQL-like) databases. |
| [driver](http://golang.org/pkg/database/sql/driver/) | Package driver defines interfaces to be implemented by database drivers as used by package sql. |
| [debug](http://golang.org/pkg/debug/)    |                                          |
| [dwarf](http://golang.org/pkg/debug/dwarf/) | Package dwarf provides access to DWARF debugging information loaded from executable files, as defined in the DWARF 2.0 Standard at http://dwarfstd.org/doc/dwarf-2.0.0.pdf |
| [elf](http://golang.org/pkg/debug/elf/)  | Package elf implements access to ELF object files. |
| [gosym](http://golang.org/pkg/debug/gosym/) | Package gosym implements access to the Go symbol and line number tables embedded in Go binaries generated by the gc compilers. |
| [macho](http://golang.org/pkg/debug/macho/) | Package macho implements access to Mach-O object files. |
| [pe](http://golang.org/pkg/debug/pe/)    | Package pe implements access to PE (Microsoft Windows Portable Executable) files. |
| [plan9obj](http://golang.org/pkg/debug/plan9obj/) | Package plan9obj implements access to Plan 9 a.out object files. |
| [encoding](http://golang.org/pkg/encoding/) | Package encoding defines interfaces shared by other packages that convert data to and from byte-level and textual representations. |
| [ascii85](http://golang.org/pkg/encoding/ascii85/) | Package ascii85 implements the ascii85 data encoding as used in the btoa tool and Adobe's PostScript and PDF document formats. |
| [asn1](http://golang.org/pkg/encoding/asn1/) | Package asn1 implements parsing of DER-encoded ASN.1 data structures, as defined in ITU-T Rec X.690. |
| [base32](http://golang.org/pkg/encoding/base32/) | Package base32 implements base32 encoding as specified by RFC 4648. |
| [base64](http://golang.org/pkg/encoding/base64/) | Package base64 implements base64 encoding as specified by RFC 4648. |
| [binary](http://golang.org/pkg/encoding/binary/) | Package binary implements simple translation between numbers and byte sequences and encoding and decoding of varints. |
| [csv](http://golang.org/pkg/encoding/csv/) | Package csv reads and writes comma-separated values (CSV) files. |
| [gob](http://golang.org/pkg/encoding/gob/) | Package gob manages streams of gobs - binary values exchanged between an Encoder (transmitter) and a Decoder (receiver). |
| [hex](http://golang.org/pkg/encoding/hex/) | Package hex implements hexadecimal encoding and decoding. |
| [json](http://golang.org/pkg/encoding/json/) | Package json implements encoding and decoding of JSON objects as defined in RFC 4627. |
| [pem](http://golang.org/pkg/encoding/pem/) | Package pem implements the PEM data encoding, which originated in Privacy Enhanced Mail. |
| [xml](http://golang.org/pkg/encoding/xml/) | Package xml implements a simple XML 1.0 parser that understands XML name spaces. |
| [errors](http://golang.org/pkg/errors/)  | Package errors implements functions to manipulate errors. |
| [expvar](http://golang.org/pkg/expvar/)  | Package expvar provides a standardized interface to public variables, such as operation counters in servers. |
| [flag](http://golang.org/pkg/flag/)      | Package flag implements command-line flag parsing. |
| [fmt](http://golang.org/pkg/fmt/)        | Package fmt implements formatted I/O with functions analogous to C's printf and scanf. |
| [go](http://golang.org/pkg/go/)          |                                          |
| [ast](http://golang.org/pkg/go/ast/)     | Package ast declares the types used to represent syntax trees for Go packages. |
| [build](http://golang.org/pkg/go/build/) | Package build gathers information about Go packages. |
| [doc](http://golang.org/pkg/go/doc/)     | Package doc extracts source code documentation from a Go AST. |
| [format](http://golang.org/pkg/go/format/) | Package format implements standard formatting of Go source. |
| [parser](http://golang.org/pkg/go/parser/) | Package parser implements a parser for Go source files. |
| [printer](http://golang.org/pkg/go/printer/) | Package printer implements printing of AST nodes. |
| [scanner](http://golang.org/pkg/go/scanner/) | Package scanner implements a scanner for Go source text. |
| [token](http://golang.org/pkg/go/token/) | Package token defines constants representing the lexical tokens of the Go programming language and basic operations on tokens (printing, predicates). |
| [hash](http://golang.org/pkg/hash/)      | Package hash provides interfaces for hash functions. |
| [adler32](http://golang.org/pkg/hash/adler32/) | Package adler32 implements the Adler-32 checksum. |
| [crc32](http://golang.org/pkg/hash/crc32/) | Package crc32 implements the 32-bit cyclic redundancy check, or CRC-32, checksum. |
| [crc64](http://golang.org/pkg/hash/crc64/) | Package crc64 implements the 64-bit cyclic redundancy check, or CRC-64, checksum. |
| [fnv](http://golang.org/pkg/hash/fnv/)   | Package fnv implements FNV-1 and FNV-1a, non-cryptographic hash functions created by Glenn Fowler, Landon Curt Noll, and Phong Vo. |
| [html](http://golang.org/pkg/html/)      | Package html provides functions for escaping and unescaping HTML text. |
| [template](http://golang.org/pkg/html/template/) | Package template (html/template) implements data-driven templates for generating HTML output safe against code injection. |
| [image](http://golang.org/pkg/image/)    | Package image implements a basic 2-D image library. |
| [color](http://golang.org/pkg/image/color/) | Package color implements a basic color library. |
| [palette](http://golang.org/pkg/image/color/palette/) | Package palette provides standard color palettes. |
| [draw](http://golang.org/pkg/image/draw/) | Package draw provides image composition functions. |
| [gif](http://golang.org/pkg/image/gif/)  | Package gif implements a GIF image decoder and encoder. |
| [jpeg](http://golang.org/pkg/image/jpeg/) | Package jpeg implements a JPEG image decoder and encoder. |
| [png](http://golang.org/pkg/image/png/)  | Package png implements a PNG image decoder and encoder. |
| [index](http://golang.org/pkg/index/)    |                                          |
| [suffixarray](http://golang.org/pkg/index/suffixarray/) | Package suffixarray implements substring search in logarithmic time using an in-memory suffix array. |
| [io](http://golang.org/pkg/io/)          | Package io provides basic interfaces to I/O primitives. |
| [ioutil](http://golang.org/pkg/io/ioutil/) | Package ioutil implements some I/O utility functions. |
| [log](http://golang.org/pkg/log/)        | Package log implements a simple logging package. |
| [syslog](http://golang.org/pkg/log/syslog/) | Package syslog provides a simple interface to the system log service. |
| [math](http://golang.org/pkg/math/)      | Package math provides basic constants and mathematical functions. |
| [big](http://golang.org/pkg/math/big/)   | Package big implements multi-precision arithmetic (big numbers). |
| [cmplx](http://golang.org/pkg/math/cmplx/) | Package cmplx provides basic constants and mathematical functions for complex numbers. |
| [rand](http://golang.org/pkg/math/rand/) | Package rand implements pseudo-random number generators. |
| [mime](http://golang.org/pkg/mime/)      | Package mime implements parts of the MIME spec. |
| [multipart](http://golang.org/pkg/mime/multipart/) | Package multipart implements MIME multipart parsing, as defined in RFC 2046. |
| [net](http://golang.org/pkg/net/)        | Package net provides a portable interface for network I/O, including TCP/IP, UDP, domain name resolution, and Unix domain sockets. |
| [http](http://golang.org/pkg/net/http/)  | Package http provides HTTP client and server implementations. |
| [cgi](http://golang.org/pkg/net/http/cgi/) | Package cgi implements CGI (Common Gateway Interface) as specified in RFC 3875. |
| [cookiejar](http://golang.org/pkg/net/http/cookiejar/) | Package cookiejar implements an in-memory RFC 6265-compliant http.CookieJar. |
| [fcgi](http://golang.org/pkg/net/http/fcgi/) | Package fcgi implements the FastCGI protocol. |
| [httptest](http://golang.org/pkg/net/http/httptest/) | Package httptest provides utilities for HTTP testing. |
| [httputil](http://golang.org/pkg/net/http/httputil/) | Package httputil provides HTTP utility functions, complementing the more common ones in the net/http package. |
| [pprof](http://golang.org/pkg/net/http/pprof/) | Package pprof serves via its HTTP server runtime profiling data in the format expected by the pprof visualization tool. |
| [mail](http://golang.org/pkg/net/mail/)  | Package mail implements parsing of mail messages. |
| [rpc](http://golang.org/pkg/net/rpc/)    | Package rpc provides access to the exported methods of an object across a network or other I/O connection. |
| [jsonrpc](http://golang.org/pkg/net/rpc/jsonrpc/) | Package jsonrpc implements a JSON-RPC ClientCodec and ServerCodec for the rpc package. |
| [smtp](http://golang.org/pkg/net/smtp/)  | Package smtp implements the Simple Mail Transfer Protocol as defined in RFC 5321. |
| [textproto](http://golang.org/pkg/net/textproto/) | Package textproto implements generic support for text-based request/response protocols in the style of HTTP, NNTP, and SMTP. |
| [url](http://golang.org/pkg/net/url/)    | Package url parses URLs and implements query escaping. |
| [os](http://golang.org/pkg/os/)          | Package os provides a platform-independent interface to operating system functionality. |
| [exec](http://golang.org/pkg/os/exec/)   | Package exec runs external commands.     |
| [signal](http://golang.org/pkg/os/signal/) | Package signal implements access to incoming signals. |
| [user](http://golang.org/pkg/os/user/)   | Package user allows user account lookups by name or id. |
| [path](http://golang.org/pkg/path/)      | Package path implements utility routines for manipulating slash-separated paths. |
| [filepath](http://golang.org/pkg/path/filepath/) | Package filepath implements utility routines for manipulating filename paths in a way compatible with the target operating system-defined file paths. |
| [reflect](http://golang.org/pkg/reflect/) | Package reflect implements run-time reflection, allowing a program to manipulate objects with arbitrary types. |
| [regexp](http://golang.org/pkg/regexp/)  | Package regexp implements regular expression search. |
| [syntax](http://golang.org/pkg/regexp/syntax/) | Package syntax parses regular expressions into parse trees and compiles parse trees into programs. |
| [runtime](http://golang.org/pkg/runtime/) | Package runtime contains operations that interact with Go's runtime system, such as functions to control goroutines. |
| [cgo](http://golang.org/pkg/runtime/cgo/) | Package cgo contains runtime support for code generated by the cgo tool. |
| [debug](http://golang.org/pkg/runtime/debug/) | Package debug contains facilities for programs to debug themselves while they are running. |
| [pprof](http://golang.org/pkg/runtime/pprof/) | Package pprof writes runtime profiling data in the format expected by the pprof visualization tool. |
| [race](http://golang.org/pkg/runtime/race/) | Package race implements data race detection logic. |
| [sort](http://golang.org/pkg/sort/)      | Package sort provides primitives for sorting slices and user-defined collections. |
| [strconv](http://golang.org/pkg/strconv/) | Package strconv implements conversions to and from string representations of basic data types. |
| [strings](http://golang.org/pkg/strings/) | Package strings implements simple functions to manipulate strings. |
| [sync](http://golang.org/pkg/sync/)      | Package sync provides basic synchronization primitives such as mutual exclusion locks. |
| [atomic](http://golang.org/pkg/sync/atomic/) | Package atomic provides low-level atomic memory primitives useful for implementing synchronization algorithms. |
| [syscall](http://golang.org/pkg/syscall/) | Package syscall contains an interface to the low-level operating system primitives. |
| [testing](http://golang.org/pkg/testing/) | Package testing provides support for automated testing of Go packages. |
| [iotest](http://golang.org/pkg/testing/iotest/) | Package iotest implements Readers and Writers useful mainly for testing. |
| [quick](http://golang.org/pkg/testing/quick/) | Package quick implements utility functions to help with black box testing. |
| [text](http://golang.org/pkg/text/)      |                                          |
| [scanner](http://golang.org/pkg/text/scanner/) | Package scanner provides a scanner and tokenizer for UTF-8-encoded text. |
| [tabwriter](http://golang.org/pkg/text/tabwriter/) | Package tabwriter implements a write filter (tabwriter.Writer) that translates tabbed columns in input into properly aligned text. |
| [template](http://golang.org/pkg/text/template/) | Package template implements data-driven templates for generating textual output. |
| [parse](http://golang.org/pkg/text/template/parse/) | Package parse builds parse trees for templates as defined by text/template and html/template. |
| [time](http://golang.org/pkg/time/)      | Package time provides functionality for measuring and displaying time. |
| [unicode](http://golang.org/pkg/unicode/) | Package unicode provides data and functions to test some properties of Unicode code points. |
| [utf16](http://golang.org/pkg/unicode/utf16/) | Package utf16 implements encoding and decoding of UTF-16 sequences. |
| [utf8](http://golang.org/pkg/unicode/utf8/) | Package utf8 implements functions and constants to support text encoded in UTF-8. |
| [unsafe](http://golang.org/pkg/unsafe/)  | Package unsafe contains operations that step around the type safety of Go programs. |

## Other packages

### Sub-repositories

​	These packages are part of the Go Project but outside the main Go tree.	They are developed under looser [compatibility requirements](http://golang.org/doc/go1compat) than the Go core.	Install them with "[go get](http://golang.org/cmd/go/#hdr-Download_and_install_packages_and_dependencies)".	

- [benchmarks](http://godoc.org/golang.org/x/benchmarks) — benchmarks to measure Go as it is developed.
- [blog](http://godoc.org/golang.org/x/blog) — [blog.golang.org](http://blog.golang.org/)'s implementation.
- [build](http://godoc.org/golang.org/x/build) — [build.golang.org](http://build.golang.org/)'s implementation.
- [crypto](http://godoc.org/golang.org/x/crypto) — additional cryptography packages.
- [debug](http://godoc.org/golang.org/x/debug) — an experimental debugger for Go.
- [image](http://godoc.org/golang.org/x/image) — additional imaging packages.
- [mobile](http://godoc.org/golang.org/x/mobile) — experimental support for Go on mobile platforms.
- [net](http://godoc.org/golang.org/x/net) — additional networking packages.
- [sys](http://godoc.org/golang.org/x/sys) — packages for making system calls.
- [text](http://godoc.org/golang.org/x/text) — packages for working with text.
- [tools](http://godoc.org/golang.org/x/tools) — godoc, goimports, gorename, and other tools.
- [tour](http://godoc.org/golang.org/x/tour) — [tour.golang.org](http://tour.golang.org/)'s implementation.
- [exp](http://godoc.org/golang.org/x/exp) — experimental and deprecated packages (handle with care; may change without warning).

### Community

​	These services can help you find Open Source packages provided by the community.	

- [GoDoc](http://godoc.org/) - a package index and search engine.
- [Go Search](http://go-search.org/) - a code search engine.
- [Projects at the Go Wiki](http://golang.org/wiki/Projects) - a curated list of Go projects.

Build version go1.4.2.
Except as [noted](https://developers.google.com/site-policies#restrictions),
the content of this page is licensed under the
Creative Commons Attribution 3.0 License,
and code is licensed under a [BSD license](http://golang.org/LICENSE).
[Terms of Service](http://golang.org/doc/tos.html) | 
[Privacy Policy](http://www.google.com/intl/en/policies/privacy/)

来源： <<http://golang.org/pkg/>>

