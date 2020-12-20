### select in Go's runtime

26 September 2013

by [skoo](http://skoo.me/skoo.html)

select可以用来管理多个channel的读写，以及实现channel读写timeout等。select并不是以库的形式提供，而是语言级支持的语法特性，因此select的实现主要由编译器和runtime共同完成，本文将重点关注runtime部分。

![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/ed16bbde-62af-475f-be0f-a39aa8495f0c/index_files/0364df99-2352-4615-9ba1-e0c8c79dde56.png)

select语句的执行主要由4个阶段组成，依次是创建select对象，注册所有的case条件，执行select语句，最后释放select对象。这里提到的select对象是底层runtime维护的一个Select结构，这个对象对Go程序来说基本是透明的。后面的内容中，我将称这个select对象为**选择器**。

#### 选择器内存模型(Select)

这里内存模型主要是描述的选择器在内存是如何布局的，是什么样的数据结构来维护的。源码位于runtime/chan.c中，描述内存模型的函数主要是`newselect`。newselect就是在内存上创建一个选择器。

描述选择器内存模型最重要的两个结构体定义如下：

```
struct	Scase
{
	SudoG	sg;			// must be first member (cast to Scase)
	Hchan*	chan;		// chan
	byte*	pc;			// return pc
	uint16	kind;
	uint16	so;			// vararg of selected bool
	bool*	receivedp;	// pointer to received bool (recv2)
};

struct	Select	
{
	uint16	tcase;			// total count of scase[]
	uint16	ncase;			// currently filled scase[]
	uint16*	pollorder;		// case poll order
	Hchan**	lockorder;		// channel lock order
	Scase	scase[1];		// one per case (in order of appearance)
};

```

`Scase`描述Go程序select语句中定义的case条件，也就是说Go程序中的一个case在runtime中就是用Scase这个结构来维护的。可以看到`Scase`中有一个`Hchan *chan`字段，这个显然就是每个case条件上操作的channel了。

`Select`就是定义“选择器”的核心结构了，每个字段当然都很重要，不过可以重点关注pollorder、lockorder、scase三个字段。这里先看一下`Scase scase[1]`这个字段的定义，可以猜到scase字段就是用来存储所有case条件的，但这里却只是定义了一个只有一个元素的数组，这怎么够存储多余1个case的情况呢？？？

![img]()

此图就是整个选择器的内存模型了，这一整块内存结构其实也是由`头部结构`+`数据结构`组成，头部就是Select那一部分，对应上面提到的`struct Select`，数据结构部分都是由数组构成。

- `scase`就是一个数组，数组元素为Scase类型，存储每个case条件。
- `lockorder`指针指向的也是一个数组，元素为`Hchan *`类型，存储每个case条件中操作channel。
- `pollorder`是一个uint16的数组.

从头部开始这一整块内存是由一次类malloc（为什么是类malloc，因为Go有自己的内存管理接口，不是采用的普通malloc）调用分配的，然后再将Select头部结构中的lockorder和pollorder两个指针分别指向正确的位置即可。当然，在一口气分配这块内存前，是事先算好了所有需要的内存的大小的。这里特别强调**一次malloc**分配所有需要的内存，就是想表达除了C/C++外还有哪门语言有这么强的内存控制能力？其他语言（包括Go）在处理这种情况，手法应该差不多都是先New一个主要的对象，然后New这个主要对象字段中需要的对象。当然，你可能会告诉我，这门语言有很好的内存管理系统，不在乎这样的对象创建…呵呵。拥有内存的完全控制能力，也是系统软件大量采用C/C++编写的原因，也是其他语言的实现基本采用C/C++的原因吧。这个问题不能继续扯下去了。

scase字段被定义为1个元素的数组的问题还没有解决。上图展示的是一个有6个case条件的选择器内存模型，可以看到lockorder、pollorder以及scase(**黑色部分**)都是6个单元的数组。**注意**，黑色部分的scase的第一个单元位于Select头部结构内存空间中，这个单元就是`struct Select`中定义的那个只有一个元素的scase数组了，在malloc分配这块内存的时候，scase就只需要少分配一个单元就可以了，所以上图中可以看出只是多加了5个scase的存储单元。这样一来，scase字段和lockorder、pollorder就没有什么两样了，殊途同归。其实，`Scase scase[1]`字段完全可以定义为`Scase *scase`嘛，但这样要多浪费一个指针的内存空间。我还是很倾向这种扣字节式的实现方式。

在newselect函数中创建好这块内存空间后，就再也找不到填充scase、lockorder和pollorder三个数组的过程了，也就是创建好内存模型就结束了，还没填数据呢，这是怎么回事？填充选择器其实就是注册case的过程。

到这里，选择器就被创建好了，剩下的就是选择器如何工作了。

#### 注册case条件

了解了选择器的内存布局，也就是创建好了一个选择器，再看如何把所有case条件数据注册到选择器中，重点看一下两个函数吧:

```
static void
selectsend(Select *sel, Hchan *c, void *pc, void *elem, int32 so)
{
	i = sel->ncase;
	……………..
	sel->ncase = i+1;
	cas = &sel->scase[i];

	cas->pc = pc;
	cas->chan = c;
	cas->so = so;
	cas->kind = CaseSend;
	cas->sg.elem = elem;
}

```

这个selectsend函数就是在碰到case条件是写数据到channel的时候会调用。它会将Go程序中此case上的数据以及channel等信息传给选择器，填充在具体的Scase结构中，完成写channel的case注册。

```
static void
selectrecv(Select *sel, Hchan *c, void *pc, void *elem, bool *received, int32 so)
{
	i = sel->ncase;
	sel->ncase = i+1;
	cas = &sel->scase[i];
	cas->pc = pc;
	cas->chan = c;

	cas->so = so;
	cas->kind = CaseRecv;
	cas->sg.elem = elem;
	cas->receivedp = received;
}

```

selectrecv和selectsend很像，它是在碰到case条件是从一个channel读取数据的时候会被调用，以完成对读channel的case注册。同样，也是事先将channel以及存放数据的内存传递给选择器，填充在一个Scase中。这里由于是等待读取数据，所以是把存储数据的内存地址交给选择器，然后选择器在从channel取到数据后，将数据拷贝到这个内存里。

case条件的注册过程特别简单，没什么复杂的内容，但这部分其实和编译器很相关，比如只有一个case的select可以优化成直接操作channel等。

#### 执行选择器

这部分是select的核心，主要包含选择器是如何管理case条件，如何读写对应的channel。

选择器和channel的交互是由`selectgo()`这个函数实现的，这个函数有一点小长，不过过程其实很简单的。下面贴一个此函数的代码骨架。

```
static void*
selectgo(Select **selp)
{
	sel = *selp;

	// 这里是一个很重要的地方，pollorder数组依次填上每个case的编号[0, n]，然后第二个for就
	// 是一个洗牌操作了，将pollorder数组中的编号给随机打乱。目的当然就是为了case条件执行的
	// 随机性。
	for(i=0; i<sel->ncase; i++)
		sel->pollorder[i] = i;
	for(i=1; i<sel->ncase; i++) {
		……….
	}

	// 这里又来两个遍历所有case的循环，好蛋疼啊。这次做的事情就是将lockorder中的元素给排序
	// 一下。注意，lockorder数组中的元素是每个case对应的channel的地址。
	for(i=0; i<sel->ncase; i++) {
		…………..
	}
	for(i=sel->ncase; i-->0; ) {
		………
	}

	// sellock就是遍历lockorder数组，然后将数组中的每个channel给加上锁，因为后面不知道将
	// 操作哪个channel，干脆就全部给加上好了？？？真暴力啊。上面对lockorder排序的目的也出来
	// 了，就是方便此处加锁的时候，对lockorder中的channel去重。因为两个case完全可能同时操
	// 作同一个channel，所以lockorder中可能存储重复的channel了。
	sellock(sel);

	// 走到这里，总算把准备工作给干完了，将开始真正干活了。
loop:
	// 这个for循环就是按pollorder的顺序去遍历所有case，碰到一个可以执行的case后就中断循
	// 环。pollorder中的编号在初始化阶段就已经被洗牌了，所以是随机挑了一个可以执行的case。
	for(i=0; i<sel->ncase; i++) {
		o = sel->pollorder[i];
		cas = &sel->scase[o];
		…..

		switch(cas->kind) {
		case CaseRecv:
			……..

		case CaseSend:
			…….
		case CaseDefault:
			……		
		}
	}

	// 没有找到可以执行的case，但有default条件，这个if里就会直接退出了。
	if(dfl != nil) {
		…..
	}

	// 到这里，就是没有找到可以执行的case，也没有default条件的情况了。
	
	// 把当前goroutine给入队到每个case对应的channel的等待队列中去。channel的等待队列在
	// channel实现中已经详细介绍了。
	for(i=0; i<sel->ncase; i++) {
		………..
		switch(cas->kind) {
		case CaseRecv:
			enqueue(&c->recvq, sg);
			break;

		case CaseSend:
			enqueue(&c->sendq, sg);
			break;
		}
	}
	// 入队完后，就把当前goroutine给挂起等待发生一个可以执行的case为止。这里同时也把所有
	// channel上的加锁给解开了。
	runtime·park((void(*)(Lock*))selunlock, (Lock*)sel, "select");

	// 当前goroutine被唤醒开始执行了，再次把所有channel加锁。还是暴力。
	sellock(sel);

	// 这一个遍历case的for循环，很有意思。这里就是本次select不会执行的那些case对应的
	// channel给出队当前goroutine。就是不管它们了，已经找到了一个执行的目标case了。
	for(i=0; i<sel->ncase; i++) {
		cas = &sel->scase[i];
		if(cas != (Scase*)sg) {
			c = cas->chan;
			if(cas->kind == CaseSend)
				dequeueg(&c->sendq);
			else
				dequeueg(&c->recvq);
		}
	}

	// 还是没找到case，重新循环执行一遍。这种情况应该是goroutine被其他一些因素给唤醒了。
	if(sg == nil)
		goto loop;

	………..

	// 解锁退出，完成了select的执行了。
	selunlock(sel);
	goto retc;


// 这些goto的tag，都是针对每个case具体操作channel的过程。和channel的实现中介绍的差不多。
asyncrecv:
	………
	goto retc;

asyncsend:
	…………….
	goto retc;

syncrecv:
	………………..
	goto retc;

syncsend:
	……………..

// select执行完退出的时候，不光是释放选择器对象，还会返回pc。这个pc就是本次select执行的case
// 的地址。只有把这个栈地址返回，才能继续执行case条件中的语句。
retc:
	pc = cas->pc;
	runtime·free(sel);
	return pc;
}

```

虽然只是一个代码骨架，也挺长的，估计也只有对照源码才能更好的理解了。总之，select的执行逻辑还是有一点小复杂的。初看的时候，不是特别好理解。再总结一下我认为的几个应该知道的地方：

- select语句的执行将会对涉及的所有channel加锁，并不是只加锁需要操作的channel。
- 对所有channel加锁之前，存在一个对涉及到的所有channel进行堆排序的过程，目的就是为了去重。
- select并不是直接随机选择一个可执行的case，而是事先将所有case洗牌，再从头到尾选择第一个可执行的case。
- 如果select语句是放置在for循环中，长期执行，会不会每次循环都经历选择器的创建到释放的4个阶段？？？我可以明确的告诉你，目前必然是这样子的，所以select的使用是有代价的，还不低。

select的实现核心部分其实就完了，优化的空间应该还是挺多的。

#### 编译器优化

虽然不懂编译器，但还是大概扫了一眼编译器中对select的实现部分。大概看一下cmd/gc/select.c代码中的注释，就可以了解到，编译器其实对select有一定的优化。比如你写的select没有任何的case条件，那还创建选择器干嘛呢；再或者你只有一个case条件，这显然可以不用select嘛；即使有一个case+default，也是可以优化为非阻塞的直接操作channel啊。

这部分编译器相关的优化可以详细看`walkselect()`函数。

注：本文是基于go1.1.2版本代码。

- **
- [go 17](http://skoo.me/categories.html#go-ref)


- **

- [Go 17](http://skoo.me/tags.html#Go-ref)

- [runtime 11](http://skoo.me/tags.html#runtime-ref)

- ​

- 来源： <<http://skoo.me/go/2013/09/26/go-runtime-select/>>