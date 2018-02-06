

from [欢迎来到高级语言虚拟机圈子](http://hllvm.group.iteye.com/group/topic/17147)



关注各种高级语言虚拟机（high-level language virtual machine，HLL 
VM）的设计与实现，泛化至各种高级语言的运行时的设计与实现，也会涉及动态编译、GC、动态语言的编译等话题。总之大方向是高级语言的语义的实现方式就对了

 `>_<`

我会开一些帖子作为资料堆积用，例如书籍、论文、教程、文章及相关链接收集。请不要以讨论方式回复这些资料堆积帖，以便于保持资料的“整洁”；不过可以另外开对应的讨论帖放讨论。

也欢迎大家开讨论帖对HLL VM的话题热烈讨论。请大家告诉我你们都对HLL VM的什么方面感兴趣 ^_^

也可以发娱乐帖，各种稀奇古怪的与HLL VM相关的代码之类的可以放在这个分类下。

需要圈子管理员权限的同学请PM我，有兴趣帮我一起收集和整理HLL VM相关资料那是最好不过了。



讨论范围包括但不限于：

●[Java Virtual Machine](http://java.sun.com/docs/books/jvms/)

- [Oracle(Sun) HotSpot VM](http://java.sun.com/javase/technologies/hotspot/index.jsp) ([OpenJDK](http://openjdk.java.net/groups/hotspot/): GPLv2+CE)

-- [Da Vinci Machine](http://openjdk.java.net/projects/mlvm/)

-- [JSR 292 Backport](http://code.google.com/p/jvm-language-runtime/)

-- [UltraViolet™ Virtual Machine](https://www.reservoir.com/ultraviolet)

- [Azul HotSpot VM](http://www.managedruntime.org/)

- [IBM J9](http://www.ibm.com/developerworks/java/jdk/)

- [Oracle JRockit](http://www.oracle.com/technology/products/jrockit/index.html)

- [Apache Harmony](http://harmony.apache.org/) (Apache License v2)

- [Jikes RVM](http://jikesrvm.org/) (EPLv1)

- [Kaffe](http://www.kaffe.org/) (GPL)

- [SableVM](http://www.sablevm.org/) (LGPLv2.1)

- [Oracle(Sun) CLDC HotSpot Implementation](https://phoneme.dev.java.net/) (GPLv2)

  - [Oracle(Sun) Project Monty virtual machine](http://java.sun.com/products/cldc/wp/ProjectMontyWhitePaper.pdf)

  - [PSPKVM](http://sourceforge.net/projects/pspkvm/) (GPL)

- [Oracle(Sun) Squawk VM](https://squawk.dev.java.net/) (GPLv2)

- [Oracle(Sun) JavaCard VM](http://java.sun.com/javacard/)

- [Oracle(Sun) Maxine VM](http://research.sun.com/projects/maxine/) (GPLv2)

- [Oracle(Sun) Mackinac](http://labs.oracle.com/projects/mackinac/)

- [Metacircular Research Platform (MRP)](http://mrp.codehaus.org/) (Apache License, EPLv1, GPLv2+CE)

- [Moxie JVM](http://moxie.sourceforge.net/) (Apache License v2)

- [JamVM](http://jamvm.sourceforge.net/) (GPL) ([Sourceforge Project Page](http://sourceforge.net/projects/jamvm/))

- [JamaicaVM](http://www.aicas.com/)

- [cacaovm](http://www.cacaovm.org/) (GPL)

- [Jelatine JVM](http://jelatine.sourceforge.net/) (GPLv3)

- [NanoVM](http://www.harbaum.org/till/nanovm/index.shtml) (GPL)

- [Open Mika](http://www.k-embedded-java.com/index.php?option=com_content&view=article&id=25:openmika&catid=2:products&Itemid=14) (BSD)

- Wonka VM

- [Aegis VM](http://aegisvm.sourceforge.net/) (LGPL)

- [joeq](http://joeq.sourceforge.net/) (GPL)

- [kissme](http://sourceforge.net/projects/kissme/) (GPL)

- [VMKit](http://vmkit.llvm.org/) (University of Illinois Open Source License)

- [Jupiter](http://www.eecg.toronto.edu/jupiter/)

- [K virtual machine](http://java.sun.com/products/cldc/wp/)

- [NSIcom CrE-ME](http://www.nsicom.com/)

- [LaTTe](http://web.archive.org/web/20071022042409/http://latte.snu.ac.kr/) (BSD)

- Esmertec JBed

- Symantec JIT

- TowerJ DynaFlex

- Microsoft Java Virtual Machine / jview

- HP Chai VM

- Oracle OJVM

- [Oracle JVM](http://download.oracle.com/docs/cd/B28359_01/java.111/b31225/chone.htm#BABJCDIE) （考据中，不知道这个跟OJVM是什么关系）

- [Mozilla ElectricalFire](http://www-archive.mozilla.org/catalog/oprojects/ef/) (Netscape PL)

- MachJ

- [JanosVM](http://www.cs.utah.edu/flux/janos/janosvm.html)

- [Xam](http://www.xerxys.info/index.php?title=Category:Xam) (GPLv3)

- E-bunny

- Jeode EVM

- [shuJIT](http://www.shudo.net/jit/)

- Inprise "javacomp" JIT

- [OpenJIT](http://www.openjit.org/) (BSD)

- TYA

- [Open Runtime Platform (ORP)](http://orp.sourceforge.net/) (Intel Open Source License)

- [Jaos](http://www-old.oberon.ethz.ch/jaos/overview.html)

- [AromaVM](http://www.ihmc.us/research/projects/Nomads/)

- Sun Exact VM

- [Sun JavaInJava](http://research.sun.com/techrep/1998/abstract-64.html)

- [IKVM.NET](http://www.ikvm.net/) (custom BSD-like)

- [TinyVM](http://tinyvm.sourceforge.net/) (MPLv1)

- [Ovm](http://www.cs.purdue.edu/homes/jv/soft/ovm/index.html) [（简介）](http://www.cs.purdue.edu/homes/jv/pubs/scp04.pdf) (New BSD)

- [GCJ](http://gcc.gnu.org/java/) (GPL)

- [Excelsior JET](http://www.excelsior-usa.com/jet.html)

- [RoboVM](http://www.robovm.org/) ([license](http://www.robovm.org/license.html))

- [JC Virtual Machine](http://jcvm.sourceforge.net/) (LGPL)

- [MiniJavaVM](http://fm.zju.edu.cn/~wengkai/paper/ZhuHuaiyi.html)

- [ArmJVM](http://fm.zju.edu.cn/~wengkai/paper/CaiHualin.html)

- [Rava](http://github.com/koichiro/rava) (Ruby License)

- [RUVA](http://ruva.rubyforge.org/) (Ruby License)

- [ Japhar](http://www.hungry.com/old-hungry/products/japhar/)

- [Joker-vm](http://sourceforge.net/projects/joker-vm/) (GPL)

- [Jato](http://jatovm.sourceforge.net/) (GPLv2+LE)

- [Hornet](https://github.com/penberg/hornet) (BSD)

- [Another Open JavaCard Virtual Machine](http://sourceforge.net/projects/openjcvm/) (GPL)

- [Sun Spotless JVM](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.63.1116)

- Nemesis JVM

- [nanokit](http://www.nanokit.org/trac/wiki/WikiStart)

- [Aplix JBlend](http://www.aplixcorp.com/en/products/jb_series/overview.html)

- Orto JVM

- [Avian](http://oss.readytalk.com/avian/) (ISC License) [Github链接](https://github.com/ReadyTalk/avian)

- [JnJVM](http://dl.acm.org/citation.cfm?id=1462066)

- [Doppio](http://int3.github.io/doppio/about.html) (MIT License) Java on CoffeeScript

- [Mysaifu JVM](http://www2s.biglobe.ne.jp/~dat/java/project/jvm/)

- [node-jvm](https://github.com/YaroslavGaponov/node-jvm)

- [Fiji](http://fiji-systems.com/)

<http://rtjava.blogspot.com/2011/07/fijivm-real-time-java-vm-overview.html>



●[Google Dalvik VM](http://android.git.kernel.org/?p=platform/dalvik.git;a=summary) (Apache License v2)



●[NestedVM](http://nestedvm.ibex.org/) (Apache License v2)



●[Harissa](http://www.usenix.org/publications/library/proceedings/coots97/full_papers/muller/muller_html/node3.html)



●[DynamicJava](http://old.koalateam.com/djava/) (BSD)

这是个Java实现的Java源码解释器，而不是JVM规范定义的那类JVM



●[FIJI ForthIsh Java Interpreter](http://sourceforge.net/projects/fiji/) (GPL)



●[Eve Virtual Machine](http://www.ewesoft.com/eve/index.html)



●[Parrot VM](http://parrot.org/) (Artistic License 2.0)



●[Common Language Infrastructure (CLI)](http://www.ecma-international.org/publications/standards/Ecma-335.htm)

- Common Language Runtime (CLR) (.NET Framework) [open sourcing!](http://news.microsoft.com/2014/11/12/microsoft-takes-net-open-source-and-cross-platform-adds-new-development-capabilities-with-visual-studio-2015-net-2015-and-visual-studio-online/)

- .NET Compact Framework

- [.NET Micro Framework](http://www.netmf.com/) (Apache License v2)

- [Mono](http://www.mono-project.com/Main_Page) (LGPL/GPL/X11)

- [DotGNU Portable .NET](http://www.gnu.org/software/dotgnu/pnet.html) (GPL)

- [Shared Source Common Language Infrastructure (SSCLI)](http://www.microsoft.com/downloads/details.aspx?FamilyID=8c09fd61-3f26-4555-ae17-3121b4f51d4d&displaylang=en) (Microsoft Shared Source Common Language Infrastructure license)

- Open Runtime Platform (ORP) (Intel Open Source License)

- StarJIT

- [ILDJIT](http://sourceforge.net/apps/mediawiki/ildjit/index.php?title=Main_Page) (GPL)



●[Dynamic Language Runtime (DLR)](http://www.codeplex.com/dlr) (Ms-PL)



●[Dis Virtual Machine](http://www.vitanuova.com/inferno/papers/dis.html)



●Python

- [Python (CPython)](http://python.org/) (PSFL)

- [PyPy](http://codespeak.net/pypy/dist/pypy/doc/index.html) (MIT)

- [Pyston](https://github.com/dropbox/pyston) (Apache License v2)

- [Jython](http://www.jython.org/) (PSFLv2)

- [IronPython](http://ironpython.net/) (Ms-PL)

- [Unladen Swallow](http://code.google.com/p/unladen-swallow/) (Apache License v2)

- [Pynie](http://bitbucket.org/allison/pynie)

- [Psyco](http://psyco.sourceforge.net/) (MIT)

- [Stackless Python](http://www.stackless.com/) (PSFL)

- [tinypy](http://www.tinypy.org/) (MIT)

- [python-on-a-chip (p14p)](http://code.google.com/p/python-on-a-chip/) (GPLv2)



●Ruby

- [Ruby 1.8.x (MRI)](http://www.ruby-lang.org/) (Ruby License)

- [Ruby 1.9](http://www.ruby-lang.org/) ([YARV](http://www.atdot.net/yarv/)) (Ruby License)

- [Rubinius](http://rubini.us/) (BSD)

- [MacRuby](http://www.macruby.org/) (Ruby License)

- [JRuby](http://jruby.org/) (CPL/GPL/LGPL)

- [IronRuby](http://www.ironruby.net/) (Ms-PL)

- [MagLev](http://maglev.gemstone.com/) (MIT)

- [Ruby Enterprise Edition](http://www.rubyenterpriseedition.com/)

- [BlueRuby](https://wiki.sdn.sap.com/wiki/display/Research/BlueRuby)

- [The Ludicrous JIT Compiler](http://rubystuff.org/ludicrous/) (BSD)

- [XRuby](http://code.google.com/p/xruby/) (Apache License v2)

- [Cardinal](http://cardinal2.rubyforge.org/)

- [Ruby.NET](http://rubydotnet.googlegroups.com/web/Home.htm) (new BSD)

- [HotRuby](http://hotruby.yukoba.jp/) (Ruby License)

- [tinyrb](http://code.macournoyer.com/tinyrb/) (Ruby License)

- [mirc](http://mirb.heroku.com/)



●JavaScript

- [Google V8](http://code.google.com/p/v8/) (new BSD)

- Mozilla [ JägerMonkey](https://wiki.mozilla.org/JaegerMonkey)/[TraceMonkey](https://wiki.mozilla.org/JavaScript:TraceMonkey)/[Tamarin](http://www.mozilla.org/projects/tamarin/)/[SpiderMonkey](http://www.mozilla.org/js/spidermonkey/) (MPL 1.1/GPL 2.0/LGPL 2.1)

- Apple [Nitro (SquirrelFish Extreme)](http://webkit.org/blog/214/introducing-squirrelfish-extreme/)/[SquirrelFish](http://webkit.org/blog/189/announcing-squirrelfish/)/[JavaScriptCore](http://webkit.org/projects/javascript/index.html) (LGPL)

- [KDE KJS](http://api.kde.org/4.0-api/kdelibs-apidocs/kjs/html/index.html) (LGPLv2)

- [Opera Carakan](http://my.opera.com/core/blog/2009/02/04/carakan)

- [DMDScript](http://www.digitalmars.com/dscript/)

- [Managed JScript](http://blogs.msdn.com/b/jscript/archive/2007/05/04/managed-jscript-announced.aspx)

- JScript

- JScript 9 "Chakra"

- [Narcissus](http://mxr.mozilla.org/mozilla/source/js/narcissus/)

- [BESEN](http://besen.sourceforge.net/) (OwnLicense+AGPLv3)

- [Jurassic](http://jurassic.codeplex.com/) (Ms-PL)

- [IronJS](https://github.com/fholm/IronJS) (Apache License v2)

- [lv5](https://github.com/Constellation/iv)

- [Tachyon](https://github.com/Tachyon-Team/Tachyon/tree/master/source)

- [Ejscript](http://ejscript.org/) ([Embedthis Open Source Software License](https://github.com/embedthis/ejs-2/blob/master/LICENSE.md))

- Qt v4

- [MuscalietJS (mcjs)](https://github.com/mcjs/mcjs): An Extensible Portable Layered JavaScript Engine



●ActionScript 3

- [Adobe AVM2](http://www.adobe.com/devnet/actionscript/articles/avm2overview.pdf)

- [Lightspark](http://sourceforge.net/apps/trac/lightspark)

- [JITB](http://blog.joa-ebert.com/2010/08/19/introducing-jitb/)



●[Groovy](http://groovy.codehaus.org/) (BSD)



●Perl

- [Perl](http://www.perl.org/)

- [Pugs](http://www.pugscode.org/)

- [Rakudo](http://rakudo.org/)

- [MoarVM](http://moarvm.org/) （Rakudo的新VM）

- Tycho (by Matthew Wilson)



●PHP

- [PHP](http://php.net/index.php) (PHP License v3.01)

- [HipHop VM](https://github.com/facebook/hiphop-php) (PHP and Zend License)

- [HippyVM](http://hippyvm.com/) ([Github](https://github.com/hippyvm/))(MIT License)

- [Phalanger](http://www.codeplex.com/Phalanger) (SS-PL)

- [Zend](http://www.zend.com/en/)

- [Quercus](http://www.caucho.com/resin-3.0/quercus/)



●[Smalltalk](http://www.smalltalk.org/)

- Smalltalk

- [GNU Smalltalk](http://smalltalk.gnu.org/) (GPL)

- [Squeak](http://www.squeak.org/) (SqL) http://www.squeakvm.org/index.html

- [GemStone/S](http://www.gemstone.com/products/gemstone)

- [Pharo](http://pharo-project.org/home) (MIT/Apache Lisence)

- [Strongtalk](http://code.google.com/p/strongtalk/) (new BSD)

- [Cog](http://www.mirandabanda.org/cogblog/) (MIT License)

- [Jtalk](http://nicolaspetton.github.com/jtalk/) (MIT License)



●[Self](http://selflanguage.org/)

- Self

- [Klein Metacircular Virtual Machine Kit](http://kleinvm.sourceforge.net/)



●[Newspeak](http://newspeaklanguage.org/) (Apache License v2/SqL/MIT)



●[Erlang](http://ftp.sunet.se/pub/lang/erlang/) (MPL)

- BEAM

- [Erjang](http://wiki.github.com/krestenkrab/erjang)



●Prolog



●[Mercury](http://ftp.mercury.cs.mu.oz.au/information/motivation.html)



●[Pure](http://code.google.com/p/pure-lang/)



●BASIC



●COBOL



●[Factor](http://www.factorcode.org/) (BSD)



●Neko

- [Neko VM](http://nekovm.org/) (LGPL)



●[Potion](http://github.com/whymirror/potion) (MIT)



●[Forth](http://www.forth.org/)



●Scheme

- [Ikarus](http://ikarus-scheme.org/) (GPLv3)

- [WeScheme](http://wescheme.org/)



●Common Lisp



●[HLVM](http://www.ffconsultancy.com/ocaml/hlvm/) (BSD)



●[Ela](http://code.google.com/p/elalang/)



●[SQLite Virtual Database Engine](http://www.sqlite.org/vdbe.html)



被称为“编译型语言”的解释器实现：



●C

- [Ch](http://www.softintegration.com/)



●C++

- [Cint](http://root.cern.ch/drupal/content/cint)



嗯……[Haskell](http://www.haskell.org/)、[OCaml](http://caml.inria.fr/ocaml/)之类的语言也是有解释器实现的，也算在内。

说来OCaml的解释器是direct-threaded的，这个肯定得拿来说说。在Forth社区以外确实讨论各种threaded技巧的氛围不怎么浓。



还有些有趣的语言也是用解释器实现的。[Fortress](http://projectfortress.sun.com/Projects/Community/)也有[解释器](http://projectfortress.sun.com/Projects/Community/wiki/InterpreterGuide)，有机会也要拿来挖掘挖掘。



常用作嵌入式脚本的：



●Lua

- [Lua](http://www.lua.org/) (MIT)

- [LuaJIT](http://luajit.org/) (MIT/X)



Terra

<http://terralang.org>



●[Squirrel](http://squirrel-lang.org/) (zlib/libpng)



●[AngelScript](http://www.angelcode.com/angelscript/) (zlib)



●[JewelScript](http://www.jewe.org/jewelscript/) (zlib/libpng)



●[Monster Script](http://monsterscript.net/)



一些游戏引擎里的脚本引擎：



●[吉里吉里2](http://kikyou.info/tvp/)

- TJS2 (GPLv2)



●[吉里吉里3](https://sv.kikyou.info/trac/kirikiri/wiki)

- [Risse](https://sv.kikyou.info/trac/kirikiri/wiki/documents/kirikiri3/development/risse_spec)



●RealLive

- RealLive

- [rlvm](http://eglaysher.github.com/rlvm/) (GPLv3)

- [xclannad](http://www.creator.club.ne.jp/~jagarl/xclannad.html)



●AliceSoft

- System 3.x

- System 4.x



●NScripter

- [NScripter](http://www.nscripter.com/)

- [ONScripter](http://ogapee.at.infoseek.co.jp/onscripter.html)



●[YU-RIS](http://yu-ris.net/)



●[Hot Soup Processor (HSP)](http://hsp.tv/)



●Stuff スクリプトエンジン



●[Wintermute Engine (WME)](http://dead-code.org/home/)



●[Unreal](http://udn.epicgames.com/Main/WebHome.html)

- UnrealScript



●[Torque Game Engine](http://www.torquepowered.com/)

- TorqueScript



●[Z Machine](http://www.gnelson.demon.co.uk/zspec/)



有些EL（表达式语言，expression language）的实现也可以拿来讨论：



●[MVEL](http://mvel.codehaus.org/)



●[janino](http://docs.codehaus.org/display/JANINO/Home) (New BSD)



●[Unified Expression Language (JSR 245)](http://jcp.org/en/jsr/detail?id=245)

- [JUEL](http://juel.sourceforge.net/)

- Resin EL



●JSP 2.0 EL

- [Commons EL](http://commons.apache.org/el/)



●[JEXL](http://commons.apache.org/jexl/)

●JEval

●SpEL

●OGNL



●IK Expression

●[Aviator](http://code.google.com/p/aviator/)

●[JSEL](http://code.google.com/p/lite/wiki/JSEL)



一些模板引擎里其实也有树遍历式的解释器



●[Velocity](http://velocity.apache.org/) (Apache License v2)



虽然自身不是HLL VM，但经常被用于HLL VM实现的：



●[Low Level Virtual Machine (LLVM)](http://llvm.org/) (University of Illinois Open Source License)

- [llvm-py](http://www.mdevan.org/llvm-py/index.html)



年代久远的虚拟机：



●SECD



动态编译的先驱：



●Oberon



还有像Half-Life 2的SourceMod插件里也有JIT…SourcePawn？

看这里 <http://www.bailopan.net/blog/?p=73>



各种机器像是游戏机的模拟器里自然也有高效的编译器或者说binary translator的实现

●[Dolphin Emulator](http://www.dolphin-emulator.com/)



一些JIT库：



●[Xbyak](http://homepage1.nifty.com/herumi/soft/xbyak_e.html) (BSD)



●[asmjit](http://code.google.com/p/asmjit/) (MIT)



●[jitasm](http://code.google.com/p/jitasm/) (BSD)



●[libjit](http://freshmeat.net/projects/libjit/) (LGPLv2.1)

- [libjit-linear-scan-register-allocator](http://code.google.com/p/libjit-linear-scan-register-allocator/) (LGPL)



一些GC库：



●[Boehm GC](http://www.hpl.hp.com/personal/Hans_Boehm/gc/)



●[Qish](http://starynkevitch.net/Basile/qishintro.html) (LGPL)



生成解释器的工具：



●[Vmgen](http://www.complang.tuwien.ac.at/anton/vmgen/)



●[Generic Interpreter](http://www.csupomona.edu/~carich/gi/)



下面这个有点离题，但非常强悍：



●[Valgrind](http://valgrind.org/)

虽然被许多人认为是内存访问的检查工具，但实际上是x86-to-x86的动态即时编译器，非常非常强……
​    



