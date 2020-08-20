

**得到汇编代码方式**

> go tool 6g -S file.go > file.s
>
> 可以用-N来指定禁用优化
>
> go tool compile -S file.go
>
> \# 也可以用 -N
>
> 得到 .o 文件

> > 类似如下：
> >
> > ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/f1e09bd3-0ed6-4ac4-9601-85bf0b757eea/index_files/a3947167-3cea-4ead-8a18-3053382b64ef.png)
> >
> > ..
> >
> > ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/f1e09bd3-0ed6-4ac4-9601-85bf0b757eea/index_files/a6455eda-dd4c-42f3-82bb-30432d306252.png)
> >
> > ...

> 还可以用gccgo
>
> gccgo -S -O0 -masm=intel file.go
>
> 上面的方式都是得到编译后的...，但go linker可能会改变最终写到最终...的内容，所以如果要查看最终执行的内容的话，可以用 objdump
>
> \# x86/amd64
>
> objdump -d executable > disassembly
>
> \# ARM
>
> objdump -M reg-names-raw -d executable > disassembly
>
> 得到文件内容类似如下：

> > 00000000004018f0 <main.main>:
> >
> >   4018f0:	65 48 8b 0c 25 28 00 	mov    %gs:0x28,%rcx
> >
> >   4018f7:	00 00 
> >
> >   4018f9:	48 8b 89 00 00 00 00 	mov    0x0(%rcx),%rcx
> >
> >   401900:	48 3b 61 10          	cmp    0x10(%rcx),%rsp
> >
> >   401904:	76 06                	jbe    40190c <main.main+0x1c>
> >
> >   401906:	e8 15 fb ff ff       	callq  401420 <main.TestNilInterface>
> >
> >   40190b:	c3                   	retq   
> >
> >   40190c:	e8 ef 19 05 00       	callq  453300 <runtime.morestack_noctxt>
> >
> >   401911:	eb dd                	jmp    4018f0 <main.main>
> >
> > ​	...
> >
> > 0000000000401420 <main.TestNilInterface>:
> >
> >   401420:	65 48 8b 0c 25 28 00 	mov    %gs:0x28,%rcx
> >
> >   401427:	00 00 
> >
> >   401429:	48 8b 89 00 00 00 00 	mov    0x0(%rcx),%rcx
> >
> >   401430:	48 8d 44 24 c8       	lea    -0x38(%rsp),%rax
> >
> >   401435:	48 3b 41 10          	cmp    0x10(%rcx),%rax
> >
> >   401439:	0f 86 99 04 00 00    	jbe    4018d8 <main.TestNilInterface+0x4b8>
> >
> >   40143f:	48 81 ec b8 00 00 00 	sub    $0xb8,%rsp
> >
> >   401446:	31 c0                	xor    %eax,%eax
> >
> >   401448:	48 8d 7c 24 50       	lea    0x50(%rsp),%rdi
> >
> >   40144d:	e8 68 46 05 00       	callq  455aba <runtime.duffzero+0x23a>
> >
> >   401452:	31 db                	xor    %ebx,%ebx
> >
> >   401454:	48 89 5c 24 30       	mov    %rbx,0x30(%rsp)
> >
> >   401459:	48 89 5c 24 38       	mov    %rbx,0x38(%rsp)
> >
> >   40145e:	c6 04 24 01          	movb   $0x1,(%rsp)
> >
> >   401462:	e8 09 fc ff ff       	callq  401070 <main.getNilSTest>
> >
> >   401467:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
> >
> >   40146c:	48 8b 5c 24 10       	mov    0x10(%rsp),%rbx
> >
> >   401471:	48 89 5c 24 38       	mov    %rbx,0x38(%rsp)
> >
> >   401476:	48 89 44 24 30       	mov    %rax,0x30(%rsp)
> >
> >   40147b:	48 83 f8 00          	cmp    $0x0,%rax
> >
> >   40147f:	0f 85 e0 00 00 00    	jne    401565 <main.TestNilInterface+0x145>
> >
> >   401485:	48 8d 1d 34 86 14 00 	lea    0x148634(%rip),%rbx        # 549ac0 <go.string.*+0x8400>
> >
> >   40148c:	48 89 9c 24 90 00 00 	mov    %rbx,0x90(%rsp)
> >
> >   401493:	00 
> >
> >   401494:	48 c7 84 24 98 00 00 	movq   $0x9,0x98(%rsp)
> >
> >   40149b:	00 09 00 00 00 
> >
> >   4014a0:	31 db                	xor    %ebx,%ebx
> >
> >   4014a2:	48 89 9c 24 80 00 00 	mov    %rbx,0x80(%rsp)
> >
> >   4014a9:	00 
> >
> >   4014aa:	48 89 9c 24 88 00 00 	mov    %rbx,0x88(%rsp)
> >
> >   4014b1:	00 
> >
> >   4014b2:	48 8d 9c 24 80 00 00 	lea    0x80(%rsp),%rbx
> >
> >   4014b9:	00 
> >
> >   4014ba:	48 83 fb 00          	cmp    $0x0,%rbx
> >
> >   4014be:	0f 84 0d 04 00 00    	je     4018d1 <main.TestNilInterface+0x4b1>
> >
> >   4014c4:	48 c7 84 24 a8 00 00 	movq   $0x1,0xa8(%rsp)
> >
> >   4014cb:	00 01 00 00 00 
> >
> >   4014d0:	48 c7 84 24 b0 00 00 	movq   $0x1,0xb0(%rsp)
> >
> >   4014d7:	00 01 00 00 00 
> >
> >   4014dc:	48 89 9c 24 a0 00 00 	mov    %rbx,0xa0(%rsp)
> >
> >   4014e3:	00 
> >
> >   4014e4:	48 8d 1d 35 d9 0e 00 	lea    0xed935(%rip),%rbx        # 4eee20 <runtime.etext+0x11d80>
> >
> >   4014eb:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4014ef:	48 8d 9c 24 90 00 00 	lea    0x90(%rsp),%rbx
> >
> >   4014f6:	00 
> >
> >   4014f7:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   4014fc:	48 c7 44 24 10 00 00 	movq   $0x0,0x10(%rsp)
> >
> >   401503:	00 00 
> >
> >   401505:	e8 06 b2 00 00       	callq  40c710 <runtime.convT2E>
> >
> >   40150a:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
> >
> >   40150f:	48 8b 44 24 20       	mov    0x20(%rsp),%rax
> >
> >   401514:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   40151b:	00 
> >
> >   40151c:	48 89 4c 24 70       	mov    %rcx,0x70(%rsp)
> >
> >   401521:	48 89 0b             	mov    %rcx,(%rbx)
> >
> >   401524:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
> >
> >   401529:	80 3d 8e 98 21 00 00 	cmpb   $0x0,0x21988e(%rip)        # 61adbe <runtime.writeBarrierEnabled>
> >
> >   401530:	0f 85 84 03 00 00    	jne    4018ba <main.TestNilInterface+0x49a>
> >
> >   401536:	48 89 43 08          	mov    %rax,0x8(%rbx)
> >
> >   40153a:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   401541:	00 
> >
> >   401542:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   401546:	48 8b 9c 24 a8 00 00 	mov    0xa8(%rsp),%rbx
> >
> >   40154d:	00 
> >
> >   40154e:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   401553:	48 8b 9c 24 b0 00 00 	mov    0xb0(%rsp),%rbx
> >
> >   40155a:	00 
> >
> >   40155b:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   401560:	e8 1b 89 06 00       	callq  469e80 <fmt.Println>
> >
> >   401565:	c6 04 24 02          	movb   $0x2,(%rsp)
> >
> >   401569:	e8 02 fb ff ff       	callq  401070 <main.getNilSTest>
> >
> >   40156e:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
> >
> >   401573:	48 8b 5c 24 10       	mov    0x10(%rsp),%rbx
> >
> >   401578:	48 89 5c 24 38       	mov    %rbx,0x38(%rsp)
> >
> >   40157d:	48 89 44 24 30       	mov    %rax,0x30(%rsp)
> >
> >   401582:	48 83 f8 00          	cmp    $0x0,%rax
> >
> >   401586:	0f 85 d7 00 00 00    	jne    401663 <main.TestNilInterface+0x243>
> >
> >   40158c:	48 8d 1d 3d 85 14 00 	lea    0x14853d(%rip),%rbx        # 549ad0 <go.string.*+0x8410>
> >
> >   401593:	48 89 9c 24 90 00 00 	mov    %rbx,0x90(%rsp)
> >
> >   40159a:	00 
> >
> >   40159b:	48 c7 84 24 98 00 00 	movq   $0x9,0x98(%rsp)
> >
> >   4015a2:	00 09 00 00 00 
> >
> >   4015a7:	31 db                	xor    %ebx,%ebx
> >
> >   4015a9:	48 89 5c 24 60       	mov    %rbx,0x60(%rsp)
> >
> >   4015ae:	48 89 5c 24 68       	mov    %rbx,0x68(%rsp)
> >
> >   4015b3:	48 8d 5c 24 60       	lea    0x60(%rsp),%rbx
> >
> >   4015b8:	48 83 fb 00          	cmp    $0x0,%rbx
> >
> >   4015bc:	0f 84 f1 02 00 00    	je     4018b3 <main.TestNilInterface+0x493>
> >
> >   4015c2:	48 c7 84 24 a8 00 00 	movq   $0x1,0xa8(%rsp)
> >
> >   4015c9:	00 01 00 00 00 
> >
> >   4015ce:	48 c7 84 24 b0 00 00 	movq   $0x1,0xb0(%rsp)
> >
> >   4015d5:	00 01 00 00 00 
> >
> >   4015da:	48 89 9c 24 a0 00 00 	mov    %rbx,0xa0(%rsp)
> >
> >   4015e1:	00 
> >
> >   4015e2:	48 8d 1d 37 d8 0e 00 	lea    0xed837(%rip),%rbx        # 4eee20 <runtime.etext+0x11d80>
> >
> >   4015e9:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4015ed:	48 8d 9c 24 90 00 00 	lea    0x90(%rsp),%rbx
> >
> >   4015f4:	00 
> >
> >   4015f5:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   4015fa:	48 c7 44 24 10 00 00 	movq   $0x0,0x10(%rsp)
> >
> >   401601:	00 00 
> >
> >   401603:	e8 08 b1 00 00       	callq  40c710 <runtime.convT2E>
> >
> >   401608:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
> >
> >   40160d:	48 8b 44 24 20       	mov    0x20(%rsp),%rax
> >
> >   401612:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   401619:	00 
> >
> >   40161a:	48 89 4c 24 70       	mov    %rcx,0x70(%rsp)
> >
> >   40161f:	48 89 0b             	mov    %rcx,(%rbx)
> >
> >   401622:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
> >
> >   401627:	80 3d 90 97 21 00 00 	cmpb   $0x0,0x219790(%rip)        # 61adbe <runtime.writeBarrierEnabled>
> >
> >   40162e:	0f 85 68 02 00 00    	jne    40189c <main.TestNilInterface+0x47c>
> >
> >   401634:	48 89 43 08          	mov    %rax,0x8(%rbx)
> >
> >   401638:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   40163f:	00 
> >
> >   401640:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   401644:	48 8b 9c 24 a8 00 00 	mov    0xa8(%rsp),%rbx
> >
> >   40164b:	00 
> >
> >   40164c:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   401651:	48 8b 9c 24 b0 00 00 	mov    0xb0(%rsp),%rbx
> >
> >   401658:	00 
> >
> >   401659:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   40165e:	e8 1d 88 06 00       	callq  469e80 <fmt.Println>
> >
> >   401663:	c6 04 24 03          	movb   $0x3,(%rsp)
> >
> >   401667:	e8 04 fa ff ff       	callq  401070 <main.getNilSTest>
> >
> >   40166c:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
> >
> >   401671:	48 8b 5c 24 10       	mov    0x10(%rsp),%rbx
> >
> >   401676:	48 89 5c 24 38       	mov    %rbx,0x38(%rsp)
> >
> >   40167b:	48 89 44 24 30       	mov    %rax,0x30(%rsp)
> >
> >   401680:	48 83 f8 00          	cmp    $0x0,%rax
> >
> >   401684:	0f 85 d7 00 00 00    	jne    401761 <main.TestNilInterface+0x341>
> >
> >   40168a:	48 8d 1d 4f 84 14 00 	lea    0x14844f(%rip),%rbx        # 549ae0 <go.string.*+0x8420>
> >
> >   401691:	48 89 9c 24 90 00 00 	mov    %rbx,0x90(%rsp)
> >
> >   401698:	00 
> >
> >   401699:	48 c7 84 24 98 00 00 	movq   $0x9,0x98(%rsp)
> >
> >   4016a0:	00 09 00 00 00 
> >
> >   4016a5:	31 db                	xor    %ebx,%ebx
> >
> >   4016a7:	48 89 5c 24 50       	mov    %rbx,0x50(%rsp)
> >
> >   4016ac:	48 89 5c 24 58       	mov    %rbx,0x58(%rsp)
> >
> >   4016b1:	48 8d 5c 24 50       	lea    0x50(%rsp),%rbx
> >
> >   4016b6:	48 83 fb 00          	cmp    $0x0,%rbx
> >
> >   4016ba:	0f 84 d5 01 00 00    	je     401895 <main.TestNilInterface+0x475>
> >
> >   4016c0:	48 c7 84 24 a8 00 00 	movq   $0x1,0xa8(%rsp)
> >
> >   4016c7:	00 01 00 00 00 
> >
> >   4016cc:	48 c7 84 24 b0 00 00 	movq   $0x1,0xb0(%rsp)
> >
> >   4016d3:	00 01 00 00 00 
> >
> >   4016d8:	48 89 9c 24 a0 00 00 	mov    %rbx,0xa0(%rsp)
> >
> >   4016df:	00 
> >
> >   4016e0:	48 8d 1d 39 d7 0e 00 	lea    0xed739(%rip),%rbx        # 4eee20 <runtime.etext+0x11d80>
> >
> >   4016e7:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4016eb:	48 8d 9c 24 90 00 00 	lea    0x90(%rsp),%rbx
> >
> >   4016f2:	00 
> >
> >   4016f3:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   4016f8:	48 c7 44 24 10 00 00 	movq   $0x0,0x10(%rsp)
> >
> >   4016ff:	00 00 
> >
> >   401701:	e8 0a b0 00 00       	callq  40c710 <runtime.convT2E>
> >
> >   401706:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
> >
> >   40170b:	48 8b 44 24 20       	mov    0x20(%rsp),%rax
> >
> >   401710:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   401717:	00 
> >
> >   401718:	48 89 4c 24 70       	mov    %rcx,0x70(%rsp)
> >
> >   40171d:	48 89 0b             	mov    %rcx,(%rbx)
> >
> >   401720:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
> >
> >   401725:	80 3d 92 96 21 00 00 	cmpb   $0x0,0x219692(%rip)        # 61adbe <runtime.writeBarrierEnabled>
> >
> >   40172c:	0f 85 4c 01 00 00    	jne    40187e <main.TestNilInterface+0x45e>
> >
> >   401732:	48 89 43 08          	mov    %rax,0x8(%rbx)
> >
> >   401736:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   40173d:	00 
> >
> >   40173e:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   401742:	48 8b 9c 24 a8 00 00 	mov    0xa8(%rsp),%rbx
> >
> >   401749:	00 
> >
> >   40174a:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   40174f:	48 8b 9c 24 b0 00 00 	mov    0xb0(%rsp),%rbx
> >
> >   401756:	00 
> >
> >   401757:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   40175c:	e8 1f 87 06 00       	callq  469e80 <fmt.Println>
> >
> >   401761:	c6 04 24 04          	movb   $0x4,(%rsp)
> >
> >   401765:	e8 06 f9 ff ff       	callq  401070 <main.getNilSTest>
> >
> >   40176a:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
> >
> >   40176f:	48 8b 5c 24 10       	mov    0x10(%rsp),%rbx
> >
> >   401774:	48 89 5c 24 38       	mov    %rbx,0x38(%rsp)
> >
> >   401779:	48 89 44 24 30       	mov    %rax,0x30(%rsp)
> >
> >   40177e:	48 83 f8 00          	cmp    $0x0,%rax
> >
> >   401782:	0f 85 d3 00 00 00    	jne    40185b <main.TestNilInterface+0x43b>
> >
> >   401788:	48 8d 1d 61 83 14 00 	lea    0x148361(%rip),%rbx        # 549af0 <go.string.*+0x8430>
> >
> >   40178f:	48 89 9c 24 90 00 00 	mov    %rbx,0x90(%rsp)
> >
> >   401796:	00 
> >
> >   401797:	48 c7 84 24 98 00 00 	movq   $0x9,0x98(%rsp)
> >
> >   40179e:	00 09 00 00 00 
> >
> >   4017a3:	31 db                	xor    %ebx,%ebx
> >
> >   4017a5:	48 89 5c 24 40       	mov    %rbx,0x40(%rsp)
> >
> >   4017aa:	48 89 5c 24 48       	mov    %rbx,0x48(%rsp)
> >
> >   4017af:	48 8d 5c 24 40       	lea    0x40(%rsp),%rbx
> >
> >   4017b4:	48 83 fb 00          	cmp    $0x0,%rbx
> >
> >   4017b8:	0f 84 b9 00 00 00    	je     401877 <main.TestNilInterface+0x457>
> >
> >   4017be:	48 c7 84 24 a8 00 00 	movq   $0x1,0xa8(%rsp)
> >
> >   4017c5:	00 01 00 00 00 
> >
> >   4017ca:	48 c7 84 24 b0 00 00 	movq   $0x1,0xb0(%rsp)
> >
> >   4017d1:	00 01 00 00 00 
> >
> >   4017d6:	48 89 9c 24 a0 00 00 	mov    %rbx,0xa0(%rsp)
> >
> >   4017dd:	00 
> >
> >   4017de:	48 8d 1d 3b d6 0e 00 	lea    0xed63b(%rip),%rbx        # 4eee20 <runtime.etext+0x11d80>
> >
> >   4017e5:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4017e9:	48 8d 9c 24 90 00 00 	lea    0x90(%rsp),%rbx
> >
> >   4017f0:	00 
> >
> >   4017f1:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   4017f6:	48 c7 44 24 10 00 00 	movq   $0x0,0x10(%rsp)
> >
> >   4017fd:	00 00 
> >
> >   4017ff:	e8 0c af 00 00       	callq  40c710 <runtime.convT2E>
> >
> >   401804:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
> >
> >   401809:	48 8b 44 24 20       	mov    0x20(%rsp),%rax
> >
> >   40180e:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   401815:	00 
> >
> >   401816:	48 89 4c 24 70       	mov    %rcx,0x70(%rsp)
> >
> >   40181b:	48 89 0b             	mov    %rcx,(%rbx)
> >
> >   40181e:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
> >
> >   401823:	80 3d 94 95 21 00 00 	cmpb   $0x0,0x219594(%rip)        # 61adbe <runtime.writeBarrierEnabled>
> >
> >   40182a:	75 37                	jne    401863 <main.TestNilInterface+0x443>
> >
> >   40182c:	48 89 43 08          	mov    %rax,0x8(%rbx)
> >
> >   401830:	48 8b 9c 24 a0 00 00 	mov    0xa0(%rsp),%rbx
> >
> >   401837:	00 
> >
> >   401838:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   40183c:	48 8b 9c 24 a8 00 00 	mov    0xa8(%rsp),%rbx
> >
> >   401843:	00 
> >
> >   401844:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   401849:	48 8b 9c 24 b0 00 00 	mov    0xb0(%rsp),%rbx
> >
> >   401850:	00 
> >
> >   401851:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   401856:	e8 25 86 06 00       	callq  469e80 <fmt.Println>
> >
> >   40185b:	48 81 c4 b8 00 00 00 	add    $0xb8,%rsp
> >
> >   401862:	c3                   	retq   
> >
> >   401863:	4c 8d 43 08          	lea    0x8(%rbx),%r8
> >
> >   401867:	4c 89 04 24          	mov    %r8,(%rsp)
> >
> >   40186b:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
> >
> >   401870:	e8 9b e7 00 00       	callq  410010 <runtime.writebarrierptr>
> >
> >   401875:	eb b9                	jmp    401830 <main.TestNilInterface+0x410>
> >
> >   401877:	89 03                	mov    %eax,(%rbx)
> >
> >   401879:	e9 40 ff ff ff       	jmpq   4017be <main.TestNilInterface+0x39e>
> >
> >   40187e:	4c 8d 43 08          	lea    0x8(%rbx),%r8
> >
> >   401882:	4c 89 04 24          	mov    %r8,(%rsp)
> >
> >   401886:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
> >
> >   40188b:	e8 80 e7 00 00       	callq  410010 <runtime.writebarrierptr>
> >
> >   401890:	e9 a1 fe ff ff       	jmpq   401736 <main.TestNilInterface+0x316>
> >
> >   401895:	89 03                	mov    %eax,(%rbx)
> >
> >   401897:	e9 24 fe ff ff       	jmpq   4016c0 <main.TestNilInterface+0x2a0>
> >
> >   40189c:	4c 8d 43 08          	lea    0x8(%rbx),%r8
> >
> >   4018a0:	4c 89 04 24          	mov    %r8,(%rsp)
> >
> >   4018a4:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
> >
> >   4018a9:	e8 62 e7 00 00       	callq  410010 <runtime.writebarrierptr>
> >
> >   4018ae:	e9 85 fd ff ff       	jmpq   401638 <main.TestNilInterface+0x218>
> >
> >   4018b3:	89 03                	mov    %eax,(%rbx)
> >
> >   4018b5:	e9 08 fd ff ff       	jmpq   4015c2 <main.TestNilInterface+0x1a2>
> >
> >   4018ba:	4c 8d 43 08          	lea    0x8(%rbx),%r8
> >
> >   4018be:	4c 89 04 24          	mov    %r8,(%rsp)
> >
> >   4018c2:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
> >
> >   4018c7:	e8 44 e7 00 00       	callq  410010 <runtime.writebarrierptr>
> >
> >   4018cc:	e9 69 fc ff ff       	jmpq   40153a <main.TestNilInterface+0x11a>
> >
> >   4018d1:	89 03                	mov    %eax,(%rbx)
> >
> >   4018d3:	e9 ec fb ff ff       	jmpq   4014c4 <main.TestNilInterface+0xa4>
> >
> >   4018d8:	e8 23 1a 05 00       	callq  453300 <runtime.morestack_noctxt>
> >
> >   4018dd:	e9 3e fb ff ff       	jmpq   401420 <main.TestNilInterface>
> >
> > ​	...
> >
> > 0000000000401070 <main.getNilSTest>:
> >
> >   401070:	65 48 8b 0c 25 28 00 	mov    %gs:0x28,%rcx
> >
> >   401077:	00 00 
> >
> >   401079:	48 8b 89 00 00 00 00 	mov    0x0(%rcx),%rcx
> >
> >   401080:	48 8d 44 24 e0       	lea    -0x20(%rsp),%rax
> >
> >   401085:	48 3b 41 10          	cmp    0x10(%rcx),%rax
> >
> >   401089:	0f 86 84 03 00 00    	jbe    401413 <main.getNilSTest+0x3a3>
> >
> >   40108f:	48 81 ec a0 00 00 00 	sub    $0xa0,%rsp
> >
> >   401096:	31 c0                	xor    %eax,%eax
> >
> >   401098:	48 89 44 24 68       	mov    %rax,0x68(%rsp)
> >
> >   40109d:	48 89 44 24 70       	mov    %rax,0x70(%rsp)
> >
> >   4010a2:	31 c0                	xor    %eax,%eax
> >
> >   4010a4:	48 89 84 24 b0 00 00 	mov    %rax,0xb0(%rsp)
> >
> >   4010ab:	00 
> >
> >   4010ac:	48 89 84 24 b8 00 00 	mov    %rax,0xb8(%rsp)
> >
> >   4010b3:	00 
> >
> >   4010b4:	0f b6 84 24 a8 00 00 	movzbl 0xa8(%rsp),%eax
> >
> >   4010bb:	00 
> >
> >   4010bc:	3c 01                	cmp    $0x1,%al
> >
> >   4010be:	0f 85 65 01 00 00    	jne    401229 <main.getNilSTest+0x1b9>
> >
> >   4010c4:	31 c0                	xor    %eax,%eax
> >
> >   4010c6:	48 89 44 24 40       	mov    %rax,0x40(%rsp)
> >
> >   4010cb:	31 ed                	xor    %ebp,%ebp
> >
> >   4010cd:	48 39 e8             	cmp    %rbp,%rax
> >
> >   4010d0:	0f 85 d6 00 00 00    	jne    4011ac <main.getNilSTest+0x13c>
> >
> >   4010d6:	48 8d 1d d3 d9 15 00 	lea    0x15d9d3(%rip),%rbx        # 55eab0 <go.string.*+0x1d3f0>
> >
> >   4010dd:	48 89 5c 24 78       	mov    %rbx,0x78(%rsp)
> >
> >   4010e2:	48 c7 84 24 80 00 00 	movq   $0x10,0x80(%rsp)
> >
> >   4010e9:	00 10 00 00 00 
> >
> >   4010ee:	31 db                	xor    %ebx,%ebx
> >
> >   4010f0:	48 89 5c 24 68       	mov    %rbx,0x68(%rsp)
> >
> >   4010f5:	48 89 5c 24 70       	mov    %rbx,0x70(%rsp)
> >
> >   4010fa:	48 8d 5c 24 68       	lea    0x68(%rsp),%rbx
> >
> >   4010ff:	48 83 fb 00          	cmp    $0x0,%rbx
> >
> >   401103:	0f 84 19 01 00 00    	je     401222 <main.getNilSTest+0x1b2>
> >
> >   401109:	48 c7 84 24 90 00 00 	movq   $0x1,0x90(%rsp)
> >
> >   401110:	00 01 00 00 00 
> >
> >   401115:	48 c7 84 24 98 00 00 	movq   $0x1,0x98(%rsp)
> >
> >   40111c:	00 01 00 00 00 
> >
> >   401121:	48 89 9c 24 88 00 00 	mov    %rbx,0x88(%rsp)
> >
> >   401128:	00 
> >
> >   401129:	48 8d 1d f0 dc 0e 00 	lea    0xedcf0(%rip),%rbx        # 4eee20 <runtime.etext+0x11d80>
> >
> >   401130:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   401134:	48 8d 5c 24 78       	lea    0x78(%rsp),%rbx
> >
> >   401139:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   40113e:	48 c7 44 24 10 00 00 	movq   $0x0,0x10(%rsp)
> >
> >   401145:	00 00 
> >
> >   401147:	e8 c4 b5 00 00       	callq  40c710 <runtime.convT2E>
> >
> >   40114c:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
> >
> >   401151:	48 8b 44 24 20       	mov    0x20(%rsp),%rax
> >
> >   401156:	48 8b 9c 24 88 00 00 	mov    0x88(%rsp),%rbx
> >
> >   40115d:	00 
> >
> >   40115e:	48 89 4c 24 58       	mov    %rcx,0x58(%rsp)
> >
> >   401163:	48 89 0b             	mov    %rcx,(%rbx)
> >
> >   401166:	48 89 44 24 60       	mov    %rax,0x60(%rsp)
> >
> >   40116b:	80 3d 4c 9c 21 00 00 	cmpb   $0x0,0x219c4c(%rip)        # 61adbe <runtime.writeBarrierEnabled>
> >
> >   401172:	0f 85 93 00 00 00    	jne    40120b <main.getNilSTest+0x19b>
> >
> >   401178:	48 89 43 08          	mov    %rax,0x8(%rbx)
> >
> >   40117c:	48 8b 9c 24 88 00 00 	mov    0x88(%rsp),%rbx
> >
> >   401183:	00 
> >
> >   401184:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   401188:	48 8b 9c 24 90 00 00 	mov    0x90(%rsp),%rbx
> >
> >   40118f:	00 
> >
> >   401190:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   401195:	48 8b 9c 24 98 00 00 	mov    0x98(%rsp),%rbx
> >
> >   40119c:	00 
> >
> >   40119d:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   4011a2:	e8 d9 8c 06 00       	callq  469e80 <fmt.Println>
> >
> >   4011a7:	48 8b 44 24 40       	mov    0x40(%rsp),%rax
> >
> >   4011ac:	48 89 44 24 50       	mov    %rax,0x50(%rsp)
> >
> >   4011b1:	48 8b 05 a8 9c 21 00 	mov    0x219ca8(%rip),%rax        # 61ae60 <go.itab.*main.STest1.main.ITest>
> >
> >   4011b8:	31 ed                	xor    %ebp,%ebp
> >
> >   4011ba:	48 39 e8             	cmp    %rbp,%rax
> >
> >   4011bd:	74 1d                	je     4011dc <main.getNilSTest+0x16c>
> >
> >   4011bf:	48 8b 5c 24 50       	mov    0x50(%rsp),%rbx
> >
> >   4011c4:	48 89 9c 24 b8 00 00 	mov    %rbx,0xb8(%rsp)
> >
> >   4011cb:	00 
> >
> >   4011cc:	48 89 84 24 b0 00 00 	mov    %rax,0xb0(%rsp)
> >
> >   4011d3:	00 
> >
> >   4011d4:	48 81 c4 a0 00 00 00 	add    $0xa0,%rsp
> >
> >   4011db:	c3                   	retq   
> >
> >   4011dc:	48 8d 1d 1d 47 11 00 	lea    0x11471d(%rip),%rbx        # 515900 <runtime.etext+0x38860>
> >
> >   4011e3:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4011e7:	48 8d 1d 72 34 11 00 	lea    0x113472(%rip),%rbx        # 514660 <runtime.etext+0x375c0>
> >
> >   4011ee:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   4011f3:	48 8d 1d 66 9c 21 00 	lea    0x219c66(%rip),%rbx        # 61ae60 <go.itab.*main.STest1.main.ITest>
> >
> >   4011fa:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   4011ff:	e8 9c b4 00 00       	callq  40c6a0 <runtime.typ2Itab>
> >
> >   401204:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
> >
> >   401209:	eb b4                	jmp    4011bf <main.getNilSTest+0x14f>
> >
> >   40120b:	4c 8d 43 08          	lea    0x8(%rbx),%r8
> >
> >   40120f:	4c 89 04 24          	mov    %r8,(%rsp)
> >
> >   401213:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
> >
> >   401218:	e8 f3 ed 00 00       	callq  410010 <runtime.writebarrierptr>
> >
> >   40121d:	e9 5a ff ff ff       	jmpq   40117c <main.getNilSTest+0x10c>
> >
> >   401222:	89 03                	mov    %eax,(%rbx)
> >
> >   401224:	e9 e0 fe ff ff       	jmpq   401109 <main.getNilSTest+0x99>
> >
> >   401229:	3c 02                	cmp    $0x2,%al
> >
> >   40122b:	0f 85 65 01 00 00    	jne    401396 <main.getNilSTest+0x326>
> >
> >   401231:	31 c0                	xor    %eax,%eax
> >
> >   401233:	48 89 44 24 38       	mov    %rax,0x38(%rsp)
> >
> >   401238:	31 ed                	xor    %ebp,%ebp
> >
> >   40123a:	48 39 e8             	cmp    %rbp,%rax
> >
> >   40123d:	0f 85 d6 00 00 00    	jne    401319 <main.getNilSTest+0x2a9>
> >
> >   401243:	48 8d 1d 86 d8 15 00 	lea    0x15d886(%rip),%rbx        # 55ead0 <go.string.*+0x1d410>
> >
> >   40124a:	48 89 5c 24 78       	mov    %rbx,0x78(%rsp)
> >
> >   40124f:	48 c7 84 24 80 00 00 	movq   $0x10,0x80(%rsp)
> >
> >   401256:	00 10 00 00 00 
> >
> >   40125b:	31 db                	xor    %ebx,%ebx
> >
> >   40125d:	48 89 5c 24 68       	mov    %rbx,0x68(%rsp)
> >
> >   401262:	48 89 5c 24 70       	mov    %rbx,0x70(%rsp)
> >
> >   401267:	48 8d 5c 24 68       	lea    0x68(%rsp),%rbx
> >
> >   40126c:	48 83 fb 00          	cmp    $0x0,%rbx
> >
> >   401270:	0f 84 19 01 00 00    	je     40138f <main.getNilSTest+0x31f>
> >
> >   401276:	48 c7 84 24 90 00 00 	movq   $0x1,0x90(%rsp)
> >
> >   40127d:	00 01 00 00 00 
> >
> >   401282:	48 c7 84 24 98 00 00 	movq   $0x1,0x98(%rsp)
> >
> >   401289:	00 01 00 00 00 
> >
> >   40128e:	48 89 9c 24 88 00 00 	mov    %rbx,0x88(%rsp)
> >
> >   401295:	00 
> >
> >   401296:	48 8d 1d 83 db 0e 00 	lea    0xedb83(%rip),%rbx        # 4eee20 <runtime.etext+0x11d80>
> >
> >   40129d:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4012a1:	48 8d 5c 24 78       	lea    0x78(%rsp),%rbx
> >
> >   4012a6:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   4012ab:	48 c7 44 24 10 00 00 	movq   $0x0,0x10(%rsp)
> >
> >   4012b2:	00 00 
> >
> >   4012b4:	e8 57 b4 00 00       	callq  40c710 <runtime.convT2E>
> >
> >   4012b9:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
> >
> >   4012be:	48 8b 44 24 20       	mov    0x20(%rsp),%rax
> >
> >   4012c3:	48 8b 9c 24 88 00 00 	mov    0x88(%rsp),%rbx
> >
> >   4012ca:	00 
> >
> >   4012cb:	48 89 4c 24 58       	mov    %rcx,0x58(%rsp)
> >
> >   4012d0:	48 89 0b             	mov    %rcx,(%rbx)
> >
> >   4012d3:	48 89 44 24 60       	mov    %rax,0x60(%rsp)
> >
> >   4012d8:	80 3d df 9a 21 00 00 	cmpb   $0x0,0x219adf(%rip)        # 61adbe <runtime.writeBarrierEnabled>
> >
> >   4012df:	0f 85 93 00 00 00    	jne    401378 <main.getNilSTest+0x308>
> >
> >   4012e5:	48 89 43 08          	mov    %rax,0x8(%rbx)
> >
> >   4012e9:	48 8b 9c 24 88 00 00 	mov    0x88(%rsp),%rbx
> >
> >   4012f0:	00 
> >
> >   4012f1:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4012f5:	48 8b 9c 24 90 00 00 	mov    0x90(%rsp),%rbx
> >
> >   4012fc:	00 
> >
> >   4012fd:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   401302:	48 8b 9c 24 98 00 00 	mov    0x98(%rsp),%rbx
> >
> >   401309:	00 
> >
> >   40130a:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   40130f:	e8 6c 8b 06 00       	callq  469e80 <fmt.Println>
> >
> >   401314:	48 8b 44 24 38       	mov    0x38(%rsp),%rax
> >
> >   401319:	48 89 44 24 48       	mov    %rax,0x48(%rsp)
> >
> >   40131e:	48 8b 05 43 9b 21 00 	mov    0x219b43(%rip),%rax        # 61ae68 <go.itab.*main.STest2.main.ITest>
> >
> >   401325:	31 ed                	xor    %ebp,%ebp
> >
> >   401327:	48 39 e8             	cmp    %rbp,%rax
> >
> >   40132a:	74 1d                	je     401349 <main.getNilSTest+0x2d9>
> >
> >   40132c:	48 8b 5c 24 48       	mov    0x48(%rsp),%rbx
> >
> >   401331:	48 89 9c 24 b8 00 00 	mov    %rbx,0xb8(%rsp)
> >
> >   401338:	00 
> >
> >   401339:	48 89 84 24 b0 00 00 	mov    %rax,0xb0(%rsp)
> >
> >   401340:	00 
> >
> >   401341:	48 81 c4 a0 00 00 00 	add    $0xa0,%rsp
> >
> >   401348:	c3                   	retq   
> >
> >   401349:	48 8d 1d 70 46 11 00 	lea    0x114670(%rip),%rbx        # 5159c0 <runtime.etext+0x38920>
> >
> >   401350:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   401354:	48 8d 1d 05 33 11 00 	lea    0x113305(%rip),%rbx        # 514660 <runtime.etext+0x375c0>
> >
> >   40135b:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   401360:	48 8d 1d 01 9b 21 00 	lea    0x219b01(%rip),%rbx        # 61ae68 <go.itab.*main.STest2.main.ITest>
> >
> >   401367:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   40136c:	e8 2f b3 00 00       	callq  40c6a0 <runtime.typ2Itab>
> >
> >   401371:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
> >
> >   401376:	eb b4                	jmp    40132c <main.getNilSTest+0x2bc>
> >
> >   401378:	4c 8d 43 08          	lea    0x8(%rbx),%r8
> >
> >   40137c:	4c 89 04 24          	mov    %r8,(%rsp)
> >
> >   401380:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
> >
> >   401385:	e8 86 ec 00 00       	callq  410010 <runtime.writebarrierptr>
> >
> >   40138a:	e9 5a ff ff ff       	jmpq   4012e9 <main.getNilSTest+0x279>
> >
> >   40138f:	89 03                	mov    %eax,(%rbx)
> >
> >   401391:	e9 e0 fe ff ff       	jmpq   401276 <main.getNilSTest+0x206>
> >
> >   401396:	3c 03                	cmp    $0x3,%al
> >
> >   401398:	75 5f                	jne    4013f9 <main.getNilSTest+0x389>
> >
> >   40139a:	31 db                	xor    %ebx,%ebx
> >
> >   40139c:	48 8d 1d 9d a1 11 00 	lea    0x11a19d(%rip),%rbx        # 51b540 <runtime.etext+0x3e4a0>
> >
> >   4013a3:	48 89 1c 24          	mov    %rbx,(%rsp)
> >
> >   4013a7:	48 8d 1d b2 32 11 00 	lea    0x1132b2(%rip),%rbx        # 514660 <runtime.etext+0x375c0>
> >
> >   4013ae:	48 89 5c 24 08       	mov    %rbx,0x8(%rsp)
> >
> >   4013b3:	48 8d 1d 1e 9b 21 00 	lea    0x219b1e(%rip),%rbx        # 61aed8 <go.itab.main.STest3.main.ITest>
> >
> >   4013ba:	48 89 5c 24 10       	mov    %rbx,0x10(%rsp)
> >
> >   4013bf:	48 8d 5c 24 38       	lea    0x38(%rsp),%rbx
> >
> >   4013c4:	48 89 5c 24 18       	mov    %rbx,0x18(%rsp)
> >
> >   4013c9:	48 c7 44 24 20 00 00 	movq   $0x0,0x20(%rsp)
> >
> >   4013d0:	00 00 
> >
> >   4013d2:	e8 a9 b4 00 00       	callq  40c880 <runtime.convT2I>
> >
> >   4013d7:	48 8b 5c 24 28       	mov    0x28(%rsp),%rbx
> >
> >   4013dc:	48 89 9c 24 b0 00 00 	mov    %rbx,0xb0(%rsp)
> >
> >   4013e3:	00 
> >
> >   4013e4:	48 8b 5c 24 30       	mov    0x30(%rsp),%rbx
> >
> >   4013e9:	48 89 9c 24 b8 00 00 	mov    %rbx,0xb8(%rsp)
> >
> >   4013f0:	00 
> >
> >   4013f1:	48 81 c4 a0 00 00 00 	add    $0xa0,%rsp
> >
> >   4013f8:	c3                   	retq   
> >
> >   4013f9:	31 db                	xor    %ebx,%ebx
> >
> >   4013fb:	48 89 9c 24 b0 00 00 	mov    %rbx,0xb0(%rsp)
> >
> >   401402:	00 
> >
> >   401403:	48 89 9c 24 b8 00 00 	mov    %rbx,0xb8(%rsp)
> >
> >   40140a:	00 
> >
> >   40140b:	48 81 c4 a0 00 00 00 	add    $0xa0,%rsp
> >
> >   401412:	c3                   	retq   
> >
> >   401413:	e8 e8 1e 05 00       	callq  453300 <runtime.morestack_noctxt>
> >
> >   401418:	e9 53 fc ff ff       	jmpq   401070 <main.getNilSTest>
> >
> >   40141d:	00 00                	add    %al,(%rax)
> >
> > ​	...

------

