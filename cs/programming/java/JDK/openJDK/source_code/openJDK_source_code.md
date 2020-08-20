

# 代码

主要来自： [jeremy jsinger的《OpenJDKArchitecture》演讲稿](http://www.dcs.gla.ac.uk/~jsinger/pdfs/sicsa_openjdk/OpenJDKArchitecture.pdf)





## 代码结构



![](pics/source_code_structure.jpg)



* jdk
  * mostly jre classes (bootstrap, system, libraries)
  * a few sdk class (jvmti support ...)
  * os-specific subclasses
* langtools
  * only sdk classes
* corba, jaxp, jaxws
  * EE rather than SE





## 代码分类



### JDK <-> hotspot接口

* API基本上都声明为 `JVM_ENTRY`

  * 一般命名为 `JVM_Xxx`

    如 `JVM_StartThread`

  * 可以通过native方法方式从JDK里调用

  如：

  ```cpp
  JVM_ENTRY(jobject, JVM_CurrentThread(JNIEnv* env, jclass threadClass))
    JVMWrapper("JVM_CurrentThread");
    oop jthread = thread->threadObj();
    assert (thread != NULL, "no current thread!");
    return JNIHandles::make_local(env, jthread);
  JVM_END
  ```

* jnienv method RegisterNatives

  没懂 = =

  * `native method fastpath to JVM_ENTRY function`
  * 在类静态初始化时调用（？）
    * Thread, Compiler, Object, Class, System, ClassLoader etc

  不知道是不是这个意思：`JVM_ENTRY`暴露API，实际方法在jnienv的里。

  ```cpp
  jobject JNIHandles::make_local(JNIEnv* env, oop obj) {
    if (obj == NULL) {
      return NULL;                // ignore null handles
    } else {
      JavaThread* thread = JavaThread::thread_from_jni_environment(env);
      assert(Universe::heap()->is_in_reserved(obj), "sanity check");
      return thread->active_handles()->allocate_handle(obj);
    }
  }
  ```

  ​

### jdk launcher <-> hotspot interface

* JDK提供的launcher

  * 在`src/share/bin/java.[h/c]`
  * 被java、javac等使用

  如：

  ```cpp
  /*
   * static void SetJvmEnvironment(int argc, char **argv);
   *   Is called just before the JVM is loaded.  We can set env variables
   *   that are consumed by the JVM.  This function is non-destructive,
   *   leaving the arg list intact.  The first use is for the JVM flag
   *   -XX:NativeMemoryTracking=value.
   */
  static void
  SetJvmEnvironment(int argc, char **argv) {
  	// ...
  }
  ```

  ​

* libjvm提供的小的启用API

  * 在`src/share/vm/prims/jni.[h/cpp]`

    * 如： `JNI_CreateJavaVM`

      ```cpp
      _JNI_IMPORT_OR_EXPORT_ jint JNICALL JNI_CreateJavaVM(JavaVM **vm, void **penv, void *args) {
        // ...
      }
      ```

  * launcher也会通过以下数据结构中的回调来访问VM函数

    * `struct JavaVM`

      ```cpp
      #ifdef __cplusplus
      typedef JavaVM_ JavaVM;
      #else
      typedef const struct JNIInvokeInterface_ *JavaVM;
      #endif

      struct JNIInvokeInterface_ {
          void *reserved0;
          void *reserved1;
          void *reserved2;

          jint (JNICALL *DestroyJavaVM)(JavaVM *vm);

          jint (JNICALL *AttachCurrentThread)(JavaVM *vm, void **penv, void *args);

          jint (JNICALL *DetachCurrentThread)(JavaVM *vm);

          jint (JNICALL *GetEnv)(JavaVM *vm, void **penv, jint version);

          jint (JNICALL *AttachCurrentThreadAsDaemon)(JavaVM *vm, void **penv, void *args);
      };

      struct JavaVM_ {
          const struct JNIInvokeInterface_ *functions;
      #ifdef __cplusplus

          jint DestroyJavaVM() {
              return functions->DestroyJavaVM(this);
          }
          jint AttachCurrentThread(void **penv, void *args) {
              return functions->AttachCurrentThread(this, penv, args);
          }
          jint DetachCurrentThread() {
              return functions->DetachCurrentThread(this);
          }

          jint GetEnv(void **penv, jint version) {
              return functions->GetEnv(this, penv, version);
          }
          jint AttachCurrentThreadAsDaemon(void **penv, void *args) {
              return functions->AttachCurrentThreadAsDaemon(this, penv, args);
          }
      #endif
      };
      ```

      ​

    * `struct JNIEnv`

      ```cpp
      #ifdef __cplusplus
      typedef JNIEnv_ JNIEnv;
      #else
      typedef const struct JNINativeInterface_ *JNIEnv;
      #endif

      struct JNINativeInterface_ {
          void *reserved0;
          void *reserved1;
          void *reserved2;

          void *reserved3;
          jint (JNICALL *GetVersion)(JNIEnv *env);

          jclass (JNICALL *DefineClass)
            (JNIEnv *env, const char *name, jobject loader, const jbyte *buf,
             jsize len);
          jclass (JNICALL *FindClass)
            (JNIEnv *env, const char *name);

          jmethodID (JNICALL *FromReflectedMethod)
            (JNIEnv *env, jobject method);
          jfieldID (JNICALL *FromReflectedField)
            (JNIEnv *env, jobject field);
          // ...
      }

      /*
       * We use inlined functions for C++ so that programmers can write:
       *
       *    env->FindClass("java/lang/String")
       *
       * in C++ rather than:
       *
       *    (*env)->FindClass(env, "java/lang/String")
       *
       * in C.
       */

      struct JNIEnv_ {
          const struct JNINativeInterface_ *functions;
      #ifdef __cplusplus

          jint GetVersion() {
              return functions->GetVersion(this);
          }
          jclass DefineClass(const char *name, jobject loader, const jbyte *buf,
                             jsize len) {
              return functions->DefineClass(this, name, loader, buf, len);
          }
          // ...
      }
      ```

      ​

## openJDK和Hotspot

openJDK基本上等于Hotspot

* just single hotspot sub-repo

  ？啥意思？

  * 几乎都是c++代码

* ~90%是泛化（generic）（代码）（平台中性 - arch-neutral）

  * `src/share/vm/<function>`
    * each functional subdir is a src tree and include root
  * `src/share/tools/<tools>`
    * not part of JVM per se
      * libhsdis.so uses binutils to disassemble code



### hotspot



* code factored by os and/or cpu
  * `src/os/<os>/vm`,
  * `src/cpu/<cpu>/vm`,
  * `src/os_cpu/<os>_<cpu>/vm`
    * all are both src trees and include roots
* os includes
  * Windows, Linux, Solaris, AIX, BSD, OSX
* cpu includes
  * x86(_32/64), AArch64, PPC, Sparc, zero**
* os_cpu inlcudes a sparse cross--product



### hotspot shared code

#### utility code

* many utility classes
  * general purpose in separate dirs – libadt, utilities
  * more specialized with client code – runtime/timer
* n.b. src/share/vm/utilities/debug.[hpp/cpp]
  * call these functions from gdb
    * find method for pc
    * print stack
    * dump threads, etc



#### oops - java data & metadata



* see `src/share/vm/oops/oops.* oopsHierarchy.*`

  * oopDesc == C++ overlay for any Java object

    ```cpp
    class oopDesc {
    	markOop _mark;
    	Klass* _klass;
    }
    ```

  * oop == [C++ accessor for] Java reference

    * typedef class oopDesc* oop

  * debug builds override operations via methods

    ```cpp
    class oop {
      oopDesc *o;
      bool operator == (void *) . . .
      operator oopDesc*() . . .
    }
    ```

    ​

#### oops hierarchy

* oop & oopDesc have a hierarchy of subclasses

  ```cpp
  oop
  	instanceOop
  	arrayOop
  		objArrayOop
  		typeArrayOop
  typedef xxxOopDesc* xxxOop
  ```

* also a couple of related types

  markOop

  * header element overlay for GC and lock operations

  narrowOop

  * special for when -XX:+UseCompressedOops
  * expands 32 bit oop into 64 bit object address



#### oops - metadata Klass hierarchy

* Klass -- models Java class as C++ type

  ```cpp
  Klass
  	InstanceKlass
  		InstanceClassLoaderKlass
  		InstanceMirrorKlass
  			● (for java.lang.Class instances)
  		InstanceRefKlass
  	ArrayKlass
  		ObjArrayKlass
  		TypeArrayKlass
  narrowKlass
  ```

  ​

#### runtime support layer

* in `src/share/vm/runtime`
* global configuration
  * i.e. -XX:[+/-]GlobalConfigVar[=value]
    * in `globals.hpp`
* execution support functions/types
  * locks, Java/VM threads, stack frames
  * handles (== GC-visible oop slot)
  * generic management of JITted stubs
    * see esp. sharedruntime.[hpp/cpp]
    * Java -> C++, Java --> Java link routines
    * C++ ineffables (e.g. cache flush)



#### memory management

* utility classes and API definitions
  * in `src/share/vm/memory & gc_interface`
  * regions, chunks, free lists, barriers, card tables
  * reference processing
* specific implementations
  * under src/share/vm/gc_implementation
  * shared subdir
    * spaces & buffers, timers & counters, GC threads/policies
  * CMS, G1, Parallel, ParNew



#### GC implementations CMS

* Concurrent Mark Sweep
  * Genarational GC
    * ParNew Young Gen
      * Eden + Pair of Survivor Spaces
    * Mark Sweep Old Gen
      * mostly concurrent
      * sweep to free lists
  * Fragmentation a problem
    * falls back to stop-the world serial compaction
  * Card Table a Problem
    * tracks Old -> Young Gen references
    * card mark can introduce cache contention



#### GC implementations G1

* Garbage First
  * Generational
    * ParNew Young Gen
  * Region Based Old Gen Management
    * evacuate from most empty regions
    * compacts as it relocates
  * Large objects an issue
    * need to evacuate contiguous regions
  * Remembered sets a problem
    * remembered sets track inter-region refs
    * can be very large and can introduce cache contention



####  interpreter(s)

* in `src/share/vm/interpreter`
* C++ Interpreter
  * conventional inner loop case switch interpreter
  * slow but easy to port
* Template Interpreter
  * dispatch table of 'per-bytecode' generated asm
    * Java stack <== machine stack
    * generated asm manipulates stack and/or VM state
    * dedicated machine registers for method & bytecode pointer
    * asm epilog increments bytecode and dispatches
  * '10x' faster than C++ interpreter



#### runtime machine code generation

* in `src/share/vm/asm & code`
  * generic register & assembler classes
    * Register
      * cpu-dependent code defines actual register set
    * AbstractAssembler
      * cpu-dependent subclasses, Assembler, MacroAssembler etc
  * instruction patching
    * needed for dynamic call resolution & deopt
  * code management
    * buffers, blobs,
    * relocs, debug info
    * stub methods, compiled methods



#### compiler interface

* in `src/share/vm/compiler`
  * compilation driver
    * API to queue requests
    * dedicated compiler threads
* in `src/share/vm/ci`
  * compiler <--> vm abstraction layer
    * limits compiler's knowledge of vm



#### compilers C1

* client compiler
  * traditional optimizing compiler
    * good code
    * fast compilation
* good for desktop client apps
  * hardcore optimizing JIT would be JTL (Just Too Late)
* also used for -XX:+TieredCompilation
  * interpret (gather profile info) ==>
  * c1 compile (gather profile info) ==>
  * c2 compile



#### compilers C2



* in `src/share/vm/opto`
* server compiler
  * highly performant code
  * slower but still o(n log(n)) time for n bytecodes
* parses bytecode to ideal graph
  * most optimization at ideal level
    * main optimization scheme based on GCM/GVN (Click 95)
      * GVN provides highly efficient SSA data representation
      * combines control, dataflow, io and memory dependencies
      * type lattice supports very aggressive optimizations
    * some ad hoc graph rewriting



#### compilers C2 back end

* in `src/share/vm/adlc`
* architecture description language compiler
  * lowering, scheduling, code generation, peephole optimization
* each per cpu back end provides ad file
  * register model
    * drives generic register allocator
  * lowering rules
    * matcher translates ideal node/subgraph --> insn (sequence)
  * insns linked to cost & pipeline model
    * scheduler tries to minimise cost & delays



##### C2 Compiler Algorithms


* Global Code Motion / Global Value Numbering, Cliff Click. ACM PLDI 95
* A Fast Algorithm for Finding Dominators in a Flowgraph, Thomas Lengauer and Robert Tarjan, TOPLAS 79
* Register Allocation & Spilling via Graph Coloring, G J Chaitin, SIGPLAN 82
* Escape Analysis for Java, Jong Deok-Choi, Manish Gupta et al, OOPSLA 99



### hotspot os_cpu-dependent



#### exmaples

* os-specific global configuration
  * e.g. -XX:+UseTransparentHugePages
* signal handling
* mutexes & threads
* scheduling
* page & stack management
* timers & clocks
* thread_local storage
* atomic load/store/xchg
* byte swap & copy
* thread stack management
* some signal handling (register 'fixing')



### hotspot cpu-dependent



#### register model

* n.b. all cpu-dependent code in `src/cpu/<arch>/vm`
* register model
  * `register_definitions_<arch>.*`, `register_<arch>.*`
    * generic register declarations/definitions
  * `vmreg_<arch>.*`
    * cpu-specific register implementation



#### code assembly

* `assembler_<arch>.*`

  `openjdk/hotspot/src/cpu/<arch>/vm/assembler_*`

  * encode cpu instruction set

* `macroassembler_<arch>.*`

  `openjdk/hotspot/src/cpu/<arch>/vm/macroassembler_*`

  * encode logical ops as insn sequence

* `interp_masm_<arch>.*`

  `openjdk/hotspot/src/cpu/<arch>/vm/interp_masm_*`

  * extend masm with extra ops for interpreter only

* `nativeInst_<arch>.*`

  `openjdk/hotspot/src/cpu/<arch>/vm/nativeInst_*`

  * implement insn patching



#### runtime

* `sharedRuntime_<arch>.*`

  `openjdk/hotspot/src/cpu/ppc/vm/sharedRuntime_*`

  * generate Java --> C++ transition stubs
    * argument marshalling
    * register save/restore
    * native wrapper code
  * generate Java -> Java transition stubs
    * i2c/c2i stubs
    * exception_blob & handler_blob
    * deopt_blob & uncommon_path_blob
    * resolve_blob


* `stubGenerator_<arch>.*`

  `openjdk/hotspot/src/cpu/ppc/vm/stubGenerator_*`

  * generates . . .
    * call stub (C++ --> Java)
    * catch unhandled excpn (C++ <-- Java)
    * forward_exception (Java <-- C++)
    * housekeeping stubs
      * atomic_xchg, atomic_cmpxchg, atomic_add
      * fences & memory barriers
      * stack walking
      * special case math code
      * inline copy



#### template interpreter

* `templateTable_<arch>.*`

  `openjdk/hotspot/src/cpu/x86/vm/templateTable_*`

  * methods to generate templates

    * one method per bytecode insn

      ```cpp
      void TemplateTable::dup() {
      	// stack ... a
      	_masm.load_ptr(0, rax); // plant stack load
      	_masp.push_ptr(rax); // plant stack push
      } // stack: ..., a, a
      ```

  * methods to generate inline auxiliary code

    * e.g. resolve class or member, initialize classpool constant
      * prepare_invoke()
      * load_field_cp_cache_entry

* `templateInterpreterGenerator_<arch>.*`

  `openjdk/hotspot/src/cpu/x86/vm/templateInterpreterGenerator_*`

  * methods to generate interpreter-specific stubs
    * normal call frame setup
    * native call frame setup
    * exception handling
    * exception throwing
      * including special exception throw cases
        * array bounds
        * class cast . . .
  * used where templates require special case handling
    * plant load and jump to stub



#### c1 implementation

* whole host of c1_Xxx files including

  ```
  openjdk/hotspot/src/cpu/<arch>/vm/
  	c1_CodeStubs_<arch>.cpp
  	c1_Defs_<arch>.hpp
  	c1_FpuStackSim_<arch>.{cpp,hpp}
  	c1_FrameMap_<arch>.{cpp,hpp}
  	c1_LIRAssembler_<arch>.{cpp,hpp}
  	c1_LIRGenerator_<arch>.cpp
  	c1_LinearScan_<arch>.{cpp,hpp}
  	c1_MacroAssembler_<arch>.{cpp,hpp}
  	c1_Runtime1_<arch>.cpp
  	c1_globals_<arch>.hpp
  openjdk/hotspot/src/os/<os>/vm/
  	c1_globals_<os>.hpp
  openjdk/hotspot/src/share/vm/c1/
  	c1_CFGPrinter.{cpp,hpp}
  	c1_Canonicalizer.{cpp,hpp}
  	c1_CodeStubs.hpp
  	c1_Compilation.{cpp,hpp}
  	c1_Compiler.{cpp,hpp}
  	c1_Defs.{cpp,hpp}
  	c1_FpuStackSim.hpp
  	c1_FrameMap.{cpp,hpp}
  	c1_GraphBuilder.{cpp,hpp}
  	c1_IR.{cpp,hpp}
  	c1_Instruction.{cpp,hpp}
  	c1_InstructionPrinter.{cpp,hpp}
  	c1_LIR.{cpp,hpp}
  	c1_LIRAssembler.{cpp,hpp}
  	c1_LIRGenerator.{cpp,hpp}
  	c1_LinearScan.{cpp,hpp}
  	c1_MacroAssembler.hpp
  	c1_Optimizer.{cpp,hpp}
  	c1_RangeCheckElimination.{cpp,hpp}
  	c1_Runtime1.{cpp,hpp}
  	c1_ValueMap.{cpp,hpp}
  	c1_ValueSet.{cpp,hpp}
  	c1_ValueStack.{cpp,hpp}
  	c1_ValueType.{cpp,hpp}
  	c1_globals.{cpp,hpp}
  ```

  * global config
    * `c1_globals_<arch>.hpp`
  * its own LIR and LIR optimizer
    * `c1_LIRGenerator_<arch>.cpp`
    * `c1_LIRAssembler_<arch>.cpp`
  * register allocator
    * `c1_LinearScan_<arch>.cpp`
  * assembler and runtime support
    * `c1_MacroAssembler_<arch>.cpp`
    * `c1_Runtime_<arch>.cpp`
    * `c1_CodeStubs_<arch>.cpp`



#### c2 implementation



* very few files – code mostly generated by adlc
  * global config
    * `c2_globals_<arch>.hpp`
  * declarative architecture description (very large)
    * `<arch>.ad`
      * registers & register classes
      * encodings
      * frame layout & calling convention
      * processor pipeline model
      * operand and instruction matching rules
      * peephole optimization matching rules
      * inline code
    * useful docn in ad files – helps to compare across ports





# 环境、工具



[【hello,world 也打脸】记storm-starter在某知名IDE下的悲催调试经历](http://www.cnblogs.com/foreach-break/p/dig_NoClassDefFound_Exception_inside_JVM.html) 中<del>应该</del>是用netBeans调试jvm，看起来效果不错。



## 查找工具



### 查找特定的文件，去掉适配平台/OS的部分



如下：

```shell
find ./ -name "c1_*" | cut -c '4-' | gsed -e 's/x86\|sparc/<arch>/g' | gsed -e 's/bsd\|linux\|windows\|solaris/<os>/g' | sort | uniq
```





### 查找特定的文件，合并cpp和hpp





```shell
BEGIN{FS="."; f1=""; f2=""; f0=""}
{
	if(f1 != ""){
      if(f1 == $1){
      	printf "%s.{%s,%s}\n", f1, f2, $2;      
      	f1 = "";
      	f2 = "";
      	f0 = "";
      } else {
      	print f0
      	f1 = $1;
      	f2 = $2;
      	f0 = $0;        
      }
	} else {
      f1 = $1;
      f2 = $2;
      f0 = $0;
	}
}
END {
  if(f1 != "") {
    print f0;
  }
}
```

















