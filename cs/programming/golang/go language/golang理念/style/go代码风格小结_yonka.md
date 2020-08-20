

**包名**

> go反对驼峰法和pascal风格用于包名，只建议用小写单词，允许“生硬”组合，如 strconv。
>
> 所以当包名需要表达的含义比较复杂时，要么“组合”要么分层，但同时go也不建议轻易分层，所以需要权衡。 比如 favpromo这种整体是一个意思的话就组合，而cronfavpromo就太多了，可以分为cron/favpromo。
>
> 如：
>
> bili@bili-PC MINGW64 /c/Go_1.5/src
>
> $ find ./ -type d -name "*_*"
>
> ./cmd/go/testdata/src/main_test

**代码文件名**

> 代码文件go允许pascal风格命名，go里代码文件名并不那么重要。
>
> bili@bili-PC MINGW64 /c/Go_1.5/src
>
> $ find ./ -name "*_*" | grep -v "\.git" | grep "\.go" | grep -v ".*_test\.go"  | grep -v windows | grep -v ios | grep -v unix | grep -v linux | grep -v plan9 | grep -v solaris | grep -v openbsd | grep -v freebsd | grep -v darwin | grep -v netbsd | grep -v nacl | grep -v bsd | grep -v dragonfly | grep -v arm | grep -v android | grep -v ppc | grep -v 386 | grep -v amd | grep -v posix
>
> ./archive/tar/stat_atim.go
>
> ./archive/tar/stat_atimespec.go
>
> ./bytes/bytes_decl.go
>
> ./cmd/compile/internal/big/accuracy_string.go
>
> ./cmd/compile/internal/big/arith_decl.go
>
> ./cmd/compile/internal/big/roundingmode_string.go
>
> ./cmd/dist/sys_default.go
>
> ./cmd/dist/util_gc.go
>
> ./cmd/dist/util_gccgo.go
>
> ./cmd/link/internal/ld/dwarf_defs.go
>
> ./cmd/link/internal/ld/macho_combine_dwarf.go
>
> ./cmd/pprof/internal/profile/legacy_profile.go
>
> ./cmd/pprof/internal/report/source_html.go
>
> ./cmd/vet/testdata/buildtag_bad.go
>
> ./cmd/vet/testdata/copylock_func.go
>
> ./cmd/vet/testdata/copylock_range.go
>
> ./compress/bzip2/bit_reader.go
>
> ./compress/bzip2/move_to_front.go
>
> ./compress/flate/huffman_bit_writer.go
>
> ./compress/flate/huffman_code.go
>
> ./compress/flate/reverse_bits.go
>
> ./crypto/aes/cipher_asm.go
>
> ./crypto/aes/cipher_generic.go
>
> ./crypto/md5/md5block_decl.go
>
> ./crypto/md5/md5block_generic.go
>
> ./crypto/rc4/rc4_asm.go
>
> ./crypto/rc4/rc4_ref.go
>
> ./crypto/sha1/sha1block_decl.go
>
> ./crypto/sha1/sha1block_generic.go
>
> ./crypto/sha256/sha256block_decl.go
>
> ./crypto/sha512/sha512block_decl.go
>
> ./crypto/subtle/constant_time.go
>
> ./crypto/tls/cipher_suites.go
>
> ./crypto/tls/generate_cert.go
>
> ./crypto/tls/handshake_client.go
>
> ./crypto/tls/handshake_messages.go
>
> ./crypto/tls/handshake_server.go
>
> ./crypto/tls/key_agreement.go
>
> ./crypto/x509/cert_pool.go
>
> ./crypto/x509/pem_decrypt.go
>
> ./crypto/x509/x509_test_import.go
>
> ./debug/dwarf/class_string.go
>
> ./encoding/gob/dec_helpers.go
>
> ./encoding/gob/enc_helpers.go
>
> ./go/build/testdata/multi/file_appengine.go
>
> ./hash/crc32/crc32_generic.go
>
> ./math/big/accuracy_string.go
>
> ./math/big/arith_decl.go
>
> ./math/big/arith_decl_pure.go
>
> ./math/big/roundingmode_string.go
>
> ./net/cgo_resnew.go
>
> ./net/cgo_resold.go
>
> ./net/cgo_socknew.go
>
> ./net/cgo_sockold.go
>
> ./net/cgo_stub.go
>
> ./net/conf_netcgo.go
>
> ./net/dial_gen.go
>
> ./net/fd_mutex.go
>
> ./net/fd_poll_runtime.go
>
> ./net/file_stub.go
>
> ./net/hook_cloexec.go
>
> ./net/interface_stub.go
>
> ./net/internal/socktest/switch_stub.go
>
> ./net/internal/socktest/sys_cloexec.go
>
> ./net/lookup_stub.go
>
> ./net/sendfile_stub.go
>
> ./net/sockoptip_stub.go
>
> ./net/sockopt_stub.go
>
> ./net/sock_cloexec.go
>
> ./net/sock_stub.go
>
> ./net/sys_cloexec.go
>
> ./net/tcpsockopt_stub.go
>
> ./os/types_notwin.go
>
> ./os/user/lookup_stubs.go
>
> ./regexp/syntax/perl_groups.go
>
> ./runtime/atomic_pointer.go
>
> ./runtime/hashmap_fast.go
>
> ./runtime/lfstack_32bit.go
>
> ./runtime/lock_futex.go
>
> ./runtime/lock_sema.go
>
> ./runtime/netpoll_epoll.go
>
> ./runtime/netpoll_kqueue.go
>
> ./runtime/netpoll_stub.go
>
> ./runtime/print1_write.go
>
> ./runtime/race/testdata/cgo_test_main.go
>
> ./runtime/sys_x86.go
>
> ./runtime/vdso_none.go
>
> ./runtime/wbfat_gen.go
>
> ./strings/strings_decl.go
>
> ./syscall/syscall_no_getwd.go
>
> ./time/zoneinfo_read.go

