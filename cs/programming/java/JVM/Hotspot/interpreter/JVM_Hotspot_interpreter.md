# general



* CPP interpreter

* Template interpreter

  > The VM creates an assembler "template" for each bytecode

  * the default and the only officially supported interpreter
  * much faster than the CPP interpreter
  * generated at VM startup



Template interpreter



0. BCP -> iadd -> 0x0123000


1. `iadd`
   1. 0x0123000 -> `mov ...`
   2. ...
2. `inc %r13` : BCP -> dup -> `0x012303e4`
3. `dup`
   1. `movabs …`
   2. ...



![](pics/template_interpreter_1.jpg)



![](pics/template_interpreter_2.jpg)





## type hierarchy



* `AllStatic`
  * `AbstractInterpreter`
    * `TemplateInterpreter`
      * `Interpreter`
    * `CppInterpreter` (`#ifdef CC_INTERP`)
      * `Interpreter`



* `StackObj`
  * `BytecodeIntepreter`(`#ifdef CC_INTERP`)





## 流程





### 启动



* `JNI_CreateJavaVM` - `jni.cpp`

* `Threads::create_vm` - `thread.cpp`

* `jint init_globals()` - `init.cpp`

  会init解释器，此外还会做各种init …


* `interpreter_init()` - `interpreter.cpp`

* `Interpreter::initialize()` - `Interpreter`本身在`interpreter.hpp`

  实际为集成的父类的该方法，根据选用的…，为：

  * `TemplateInterpreter::initialize()` - `templateInterpreter.cpp`

    * `AbstractInterpreter::initialize()`

      > 下面不重复。

      重置各种计数器 = =

    * `TemplateTable::initialize()`

      1. `AbstractInterpreter::initialize()`

      2. `TemplateTable::initialize()`

      3. `InterpreterGenerator g(_code)`

         ```cpp
             _code = new StubQueue(new InterpreterCodeletInterface, code_size, NULL,
                                   "Interpreter");
             InterpreterGenerator g(_code);
         ```

         ​

         ```cpp
         // templateInterpreter_x86_64.cpp

         InterpreterGenerator::InterpreterGenerator(StubQueue* code)
           : TemplateInterpreterGenerator(code) {
            generate_all(); // down here so it can be "virtual"
         }
         ```

      4.  `  _active_table = _normal_table;  // initialize dispatch table`
         ​

  * `CppInterpreter::initialize()` - `cppInterpreter.cpp`

    * `AbstractInterpreter::initialize()`

    * `BytecodeInterpreter::initialize`

      ```cpp
        // Allow c++ interpreter to do one initialization now that switches are set, etc.
        BytecodeInterpreter start_msg(BytecodeInterpreter::initialize);
        if (JvmtiExport::can_post_interpreter_events())
          BytecodeInterpreter::runWithChecks(&start_msg);
        else
          BytecodeInterpreter::run(&start_msg);
      ```

      ​







# cpp interpreter





# template interpreter



## “产生”解释器

Ref:

- [Sun Hotspot JVM Part 1: The Interpreter](http://markmail.org/download.xqy?id=vn5olfue3yiwldjh&number=1)



- by `InterpreterMacroAssembler`
  - `interp_masm_<arch>.{hpp,cpp}`
  - 继承自： `MacroAssembler` （`macroAssembler_<arch>.{hpp,cpp}`） - `Assembler`



### sth

#### 分派表



* 分派表/table

  大致理解为 {code: entry}，虽然实际是以code为index的数组

* entry

  大致为 {tos: address}

  tos有：

  ```cpp
  enum TosState {         // describes the tos cache contents
    btos = 0,             // byte, bool tos cached
    ctos = 1,             // char tos cached
    stos = 2,             // short tos cached
    itos = 3,             // int tos cached
    ltos = 4,             // long tos cached
    ftos = 5,             // float tos cached
    dtos = 6,             // double tos cached
    atos = 7,             // object cached
    vtos = 8,             // tos not cached
    number_of_states,
    ilgl                  // illegal state: should not occur
  };
  ```

  ​





#### 两套分派表（dispatch table）

- 正常
- safepoint版本



```cpp
// templateInterpreter.cpp

EntryPoint TemplateInterpreter::_return_entry[TemplateInterpreter::number_of_return_entries];
EntryPoint TemplateInterpreter::_earlyret_entry;
EntryPoint TemplateInterpreter::_deopt_entry [TemplateInterpreter::number_of_deopt_entries ];
EntryPoint TemplateInterpreter::_continuation_entry;
EntryPoint TemplateInterpreter::_safept_entry;

address TemplateInterpreter::_invoke_return_entry[TemplateInterpreter::number_of_return_addrs];
address TemplateInterpreter::_invokeinterface_return_entry[TemplateInterpreter::number_of_return_addrs];
address TemplateInterpreter::_invokedynamic_return_entry[TemplateInterpreter::number_of_return_addrs];

DispatchTable TemplateInterpreter::_active_table;
DispatchTable TemplateInterpreter::_normal_table;
DispatchTable TemplateInterpreter::_safept_table;

void TemplateInterpreterGenerator::generate_all() {
  AbstractInterpreterGenerator::generate_all();
  
  // ...
  
  { CodeletMark cm(_masm, "safepoint entry points");
    Interpreter::_safept_entry =
      EntryPoint(
        generate_safept_entry_for(btos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(ctos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(stos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(atos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(itos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(ltos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(ftos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(dtos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(vtos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint))
      );
  }  
  
  // ...
  
  // Bytecodes
  set_entry_points_for_all_bytes();
  set_safepoints_for_all_bytes();  
}

void TemplateInterpreterGenerator::set_entry_points_for_all_bytes() {
  for (int i = 0; i < DispatchTable::length; i++) {
    Bytecodes::Code code = (Bytecodes::Code)i;
    if (Bytecodes::is_defined(code)) {
      set_entry_points(code);
    } else {
      set_unimplemented(i);
    }
  }
}

void TemplateInterpreterGenerator::set_entry_points(Bytecodes::Code code) {
  CodeletMark cm(_masm, Bytecodes::name(code), code);
  // initialize entry points
  assert(_unimplemented_bytecode    != NULL, "should have been generated before");
  assert(_illegal_bytecode_sequence != NULL, "should have been generated before");
  address bep = _illegal_bytecode_sequence;
  address cep = _illegal_bytecode_sequence;
  address sep = _illegal_bytecode_sequence;
  address aep = _illegal_bytecode_sequence;
  address iep = _illegal_bytecode_sequence;
  address lep = _illegal_bytecode_sequence;
  address fep = _illegal_bytecode_sequence;
  address dep = _illegal_bytecode_sequence;
  address vep = _unimplemented_bytecode;
  address wep = _unimplemented_bytecode;
  // code for short & wide version of bytecode
  if (Bytecodes::is_defined(code)) {
    Template* t = TemplateTable::template_for(code);
    assert(t->is_valid(), "just checking");
    set_short_entry_points(t, bep, cep, sep, aep, iep, lep, fep, dep, vep);
  }
  if (Bytecodes::wide_is_defined(code)) {
    Template* t = TemplateTable::template_for_wide(code);
    assert(t->is_valid(), "just checking");
    set_wide_entry_point(t, wep);
  }
  // set entry points
  EntryPoint entry(bep, cep, sep, aep, iep, lep, fep, dep, vep);
  Interpreter::_normal_table.set_entry(code, entry);
  Interpreter::_wentry_point[code] = wep;
}
// 正常的table是逐code找出对应的template来...得到entry然后set

void TemplateInterpreterGenerator::set_safepoints_for_all_bytes() {
  for (int i = 0; i < DispatchTable::length; i++) {
    Bytecodes::Code code = (Bytecodes::Code)i;
    if (Bytecodes::is_defined(code)) Interpreter::_safept_table.set_entry(code, Interpreter::_safept_entry);
  }
}
// safepoint的table是全部set到同一个entry： Interpreter::_safept_entry 
// 做完safepoint的...后然后再转交给normal table来dispatch


void TemplateInterpreter::notice_safepoints() {
  if (!_notice_safepoints) {
    // switch to safepoint dispatch table
    _notice_safepoints = true;
    copy_table((address*)&_safept_table, (address*)&_active_table, sizeof(_active_table) / sizeof(address));  // 替换了“入口”，不过逐byte copy的，显然需要外部同步，不然访问一个“中间态”的对象，结果是不可预知的
  }
}

static inline void copy_table(address* from, address* to, int size) {
  // Copy non-overlapping tables. The copy has to occur word wise for MT safety.
  while (size-- > 0) *to++ = *from++;
}

// SafepointSynchronize::end 中会调这个
// switch from the dispatch table which notices safepoints back to the
// normal dispatch table.  So that we can notice single stepping points,
// keep the safepoint dispatch table if we are single stepping in JVMTI.
// Note that the should_post_single_step test is exactly as fast as the
// JvmtiExport::_enabled test and covers both cases.
void TemplateInterpreter::ignore_safepoints() {
  if (_notice_safepoints) {
    if (!JvmtiExport::should_post_single_step()) {
      // switch to normal dispatch table
      _notice_safepoints = false;
      copy_table((address*)&_normal_table, (address*)&_active_table, sizeof(_active_table) / sizeof(address));
    }
  }
}
```



```cpp
// templateInterpreter_x86_64.cpp

  { CodeletMark cm(_masm, "safepoint entry points");
    Interpreter::_safept_entry =
      EntryPoint(
        generate_safept_entry_for(btos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(ctos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(stos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(atos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(itos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(ltos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(ftos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(dtos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint)),
        generate_safept_entry_for(vtos, CAST_FROM_FN_PTR(address, InterpreterRuntime::at_safepoint))
      );
  }

address TemplateInterpreterGenerator::generate_safept_entry_for(
        TosState state,
        address runtime_entry) {
  address entry = __ pc();
  __ push(state);
  __ call_VM(noreg, runtime_entry);
  __ dispatch_via(vtos, Interpreter::_normal_table.table_for(vtos));
  return entry;
}
```



看起来很“粗浅”，但问题在于多线程间的同步，详见 `《jvm_safePoint.d》 `的 `状态检查执行时机` 节。



* 解释执行代码

  通过把dispatch table换成safepoint版本来...

  > TODO 但没说safepoint版本的dispatch table是如何做check的
  >
  > TODO 替换的“原子性”

* 执行native代码（JNI）

  从JNI return到java code时做check，这时候： `safepoint_state` 为critical region - java线程要看到VM线程设置的该状态

* 执行编译代码

  通过读`global (safepoint polling) page`来...

  > TODO 编译器这块代码不是很好读，到底怎么个read polling page还没看到，有时间仔细翻翻

  > 这个“读”的操作，由（JIT）编译器在特定的场景插入
  >
  > > 查看`address get_polling_page()`的callGraph可以看出来一些：
  > >
  > > * `address get_polling_page`
  > >
  > >   * `void Parse::add_safepoint()`(`pase1.cpp`)
  > >
  > >     * `do_lookupswitch`
  > >
  > >     * `do_tableswitch`
  > >
  > >     * `maybe_add_safepoint`
  > >
  > >       * `do_lookupswitch`
  > >
  > >       * `do_tableswitch`
  > >
  > >       * `void Parse::do_one_bytecode()`
  > >
  > >         * `case Bytecodes::_lcmp:`
  > >
  > >           ```cpp
  > >               // Safepoints are now inserted _before_ branches.  The long-compare
  > >               // bytecode painfully produces a 3-way value (-1,0,+1) which requires a
  > >               // slew of control flow.  These are usually followed by a CmpI vs zero and
  > >               // a branch; this pattern then optimizes to the obvious long-compare and
  > >               // branch.  However, if the branch is backwards there's a Safepoint
  > >               // inserted.  The inserted Safepoint captures the JVM state at the
  > >               // pre-branch point, i.e. it captures the 3-way value.  Thus if a
  > >               // long-compare is used to control a loop the debug info will force
  > >               // computation of the 3-way value, even though the generated code uses a
  > >               // long-compare and branch.  We try to rectify the situation by inserting
  > >               // a SafePoint here and have it dominate and kill the safepoint added at a
  > >               // following backwards branch.  At this point the JVM state merely holds 2
  > >               // longs but not the 3-way value.
  > >               if( UseLoopSafepoints ) {
  > >                 switch( iter().next_bc() ) {
  > >                 case Bytecodes::_ifgt:
  > >                 case Bytecodes::_iflt:
  > >                 case Bytecodes::_ifge:
  > >                 case Bytecodes::_ifle:
  > >                 case Bytecodes::_ifne:
  > >                 case Bytecodes::_ifeq:
  > >                   // If this is a backwards branch in the bytecodes, add Safepoint
  > >                   maybe_add_safepoint(iter().next_get_dest());
  > >                 }
  > >               }
  > >           ```
  > >
  > >         * `case Bytecodes::_goto_w`
  > >
  > >         * `case Bytecodes::_ifnull`、`case Bytecodes::_ifnonnull`
  > >
  > >         * `case Bytecodes::_if_acmpeq`、`case Bytecodes::_if_acmpne`
  > >
  > >         * if大家族
  > >
  > >           * `case Bytecodes::_ifeq`
  > >           * `case Bytecodes::_ifne`
  > >           * `case Bytecodes::_iflt`
  > >           * `case Bytecodes::_ifle`
  > >           * `case Bytecodes::_ifgt`
  > >           * `case Bytecodes::_ifge`
  > >
  > >         * if_icmp大家族
  > >
  > >           - `case Bytecodes::_if_icmpeq`
  > >           - ...
  > >
  > >   * `int LIR_Assembler::safepoint_poll(LIR_Opr tmp, CodeEmitInfo* info)`(`c1_LIRAssembler_x86.cpp`)
  > >
  > >     * `void LIR_Assembler::emit_op1(LIR_Op1* op)`(`c1_LIRAssembler.cpp`)
  > >
  > >       ```cpp
  > >           case lir_safepoint:
  > >             if (compilation()->debug_info_recorder()->last_pc_offset() == code_offset()) {
  > >               _masm->nop();
  > >             }
  > >             safepoint_poll(op->in_opr(), op->info());
  > >             break;
  > >       ```
  > >
  > >       * `void LIR_Op1::emit_code(LIR_Assembler* masm)`(`c1_LIR.cpp`)
  > >
  > >   * ...

* blocked

  return（to non-block state）之前检查，需要等safepoint结束才能返回。

  > block在jvm层面而不是native层面
  >
  > TODO 应该是在一些

* In VM 或者 在状态间迁移





#### why运行时产生

- 可以根据CPU类型进行优化
- TOS可以存在RAX，或者栈中



### how运行时产生

- 每种bytecode都有对应的generator函数

- 主要都在 `TemplateTable`(`templateTable.{hpp,cpp}`、`templateTable_<arch>.{hpp,cpp}`)

  - hpp中定义class、方法声明；cpp中实现

  - 文件中还定义了`Template`类

  -  `templateTable.cpp`中主要是initialize、generate等

    如：

    ```cpp
    // templateTable.hpp

      // Templates
      static Template* template_for     (Bytecodes::Code code)  { Bytecodes::check     (code); return &_template_table     [code]; }
      static Template* template_for_wide(Bytecodes::Code code)  { Bytecodes::wide_check(code); return &_template_table_wide[code]; }

      // ...
    ```

    ​

    ```cpp
    // templateTable.cpp

    InterpreterMacroAssembler* TemplateTable::_masm;

    void Template::generate(InterpreterMacroAssembler* masm) {
      // parameter passing
      TemplateTable::_desc = this;
      TemplateTable::_masm = masm;
      // code generation
      _gen(_arg);
      masm->flush();
    }

    // ...

    void TemplateTable::initialize() {
      if (_is_initialized) return;

      // Initialize table
      TraceTime timer("TemplateTable initialization", TraceStartupTime);

      _bs = Universe::heap()->barrier_set();

      // For better readability
      const char _    = ' ';
      const int  ____ = 0;
      const int  ubcp = 1 << Template::uses_bcp_bit;
      const int  disp = 1 << Template::does_dispatch_bit;
      const int  clvm = 1 << Template::calls_vm_bit;
      const int  iswd = 1 << Template::wide_bit;
      //                                    interpr. templates
      // Java spec bytecodes                ubcp|disp|clvm|iswd  in    out   generator             argument
      def(Bytecodes::_nop                 , ____|____|____|____, vtos, vtos, nop                 ,  _           );
      def(Bytecodes::_aconst_null         , ____|____|____|____, vtos, atos, aconst_null         ,  _           );
      
      // ...
      
      def(Bytecodes::_invokehandle        , ubcp|disp|clvm|____, vtos, vtos, invokehandle        , f1_byte      );

      def(Bytecodes::_shouldnotreachhere   , ____|____|____|____, vtos, vtos, shouldnotreachhere ,  _           );
      // platform specific bytecodes
      pd_initialize();

      _is_initialized = true;
    }

    void TemplateTable::def(Bytecodes::Code code, int flags, TosState in, TosState out, void (*gen)(int arg), int arg) {
      // should factor out these constants
      const int ubcp = 1 << Template::uses_bcp_bit;
      const int disp = 1 << Template::does_dispatch_bit;
      const int clvm = 1 << Template::calls_vm_bit;
      const int iswd = 1 << Template::wide_bit;
      // determine which table to use
      bool is_wide = (flags & iswd) != 0;
      // make sure that wide instructions have a vtos entry point
      // (since they are executed extremely rarely, it doesn't pay out to have an
      // extra set of 5 dispatch tables for the wide instructions - for simplicity
      // they all go with one table)
      assert(in == vtos || !is_wide, "wide instructions have vtos entry point only");
      Template* t = is_wide ? template_for_wide(code) : template_for(code);
      // setup entry
      t->initialize(flags, in, out, gen, arg);  // 把实际的该code对应的generator实现“装入”
      assert(t->bytecode() == code, "just checkin'");
    }

    void TemplateTable::transition(TosState tos_in, TosState tos_out) {
      assert(_desc->tos_in()  == tos_in , "inconsistent tos_in  information");
      assert(_desc->tos_out() == tos_out, "inconsistent tos_out information");
    }
    ```

    ​

  - 实际的代码生成主要在`*<arch>.cpp`中（平台相关）

    如：

    ```cpp
    // templateTable_x86_64.cpp

    #define __ _masm->

    void TemplateTable::iconst(int value) {
      transition(vtos, itos);
      if (value == 0) {
        __ xorl(rax, rax);
      } else {
        __ movl(rax, value);
      }
    }
    ```

    ​

    ```cpp
    // assembler_x86.cpp

    void Assembler::xorl(Register dst, Register src) {
      (void) prefix_and_encode(dst->encoding(), src->encoding());
      emit_arith(0x33, 0xC0, dst, src);
    }

    void Assembler::movl(Register dst, int32_t imm32) {
      int encode = prefix_and_encode(dst->encoding());
      emit_int8((unsigned char)(0xB8 | encode));
      emit_int32(imm32);
    }

    // ...
    ```

    ​

    ```cpp
    // register_definitions_x86.cpp

    REGISTER_DEFINITION(Register, noreg);
    REGISTER_DEFINITION(Register, rax);
    REGISTER_DEFINITION(Register, rcx);
    REGISTER_DEFINITION(Register, rdx);
    REGISTER_DEFINITION(Register, rbx);
    REGISTER_DEFINITION(Register, rsp);
    REGISTER_DEFINITION(Register, rbp);
    REGISTER_DEFINITION(Register, rsi);
    REGISTER_DEFINITION(Register, rdi);
    #ifdef AMD64
    REGISTER_DEFINITION(Register, r8);
    REGISTER_DEFINITION(Register, r9);
    // ...

    // from register.hpp
    #define REGISTER_DEFINITION(type, name) \
    const type name = ((type)name##_##type##EnumValue)
    ```

- 具体流程

  见下面的启动流程，在`TemplateInterpreter::initialize`中会执行`TemplateTable::initialize`。



### 细节



- code generator函数使用 `MASM` 来写code到code buffer，CodeletMark标记一个字节码（的代码）的开始/结束
- dispatch过程是根据 `#BC` 和 `#TosStats` 二维条件进行的，不合法的组合（认为unimplemented）会通过专门的handler处理，其他的会做正常的dispatch
- ​







### 实例

#### invokevirtual

以`x86_64`平台为例：



```cpp
// templateTable_x86_64.cpp

void TemplateTable::invokevirtual(int byte_no) {
  transition(vtos, vtos);
  assert(byte_no == f2_byte, "use this argument");
  prepare_invoke(byte_no,
                 rbx,    // method or vtable index
                 noreg,  // unused itable index
                 rcx, rdx); // recv, flags

  // rbx: index
  // rcx: receiver
  // rdx: flags

  invokevirtual_helper(rbx, rcx, rdx);
}

void TemplateTable::prepare_invoke(int byte_no,
                                   Register method,  // linked method (or i-klass)
                                   Register index,   // itable index, MethodType, etc.
                                   Register recv,    // if caller wants to see it
                                   Register flags    // if caller wants to test it
                                   ) {
  // ...
  
  // save 'interpreter return address'
  __ save_bcp();
  
  load_invoke_cp_cache_entry(byte_no, method, index, flags, is_invokevirtual, false, is_invokedynamic);

  // maybe push appendix to arguments (just before return address)
  if (is_invokedynamic || is_invokehandle) {
    // ...
  }
  
  // load receiver if needed (after appendix is pushed so parameter size is correct)
  // Note: no return address pushed yet
  if (load_receiver) {
    // ...
  }
  
  if (save_flags) {
    __ movl(r13, flags);
  }

  // compute return type
  __ shrl(flags, ConstantPoolCacheEntry::tos_state_shift);
  // Make sure we don't need to mask flags after the above shift
  ConstantPoolCacheEntry::verify_tos_state_shift();
  // load return address
  {
    // ...
  }

  // push return address
  __ push(flags);

  // Restore flags value from the constant pool cache, and restore rsi
  // for later null checks.  r13 is the bytecode pointer
  if (save_flags) {
    __ movl(flags, r13);
    __ restore_bcp();
  }  
}  
  
void TemplateTable::invokevirtual_helper(Register index,
                                         Register recv,
                                         Register flags) {
  // Uses temporary registers rax, rdx
  assert_different_registers(index, recv, rax, rdx);
  assert(index == rbx, "");
  assert(recv  == rcx, "");

  // Test for an invoke of a final method
  Label notFinal;
  __ movl(rax, flags);
  __ andl(rax, (1 << ConstantPoolCacheEntry::is_vfinal_shift));
  __ jcc(Assembler::zero, notFinal);

  const Register method = index;  // method must be rbx
  assert(method == rbx,
         "Method* must be rbx for interpreter calling convention");

  // do the call - the index is actually the method to call
  // that is, f2 is a vtable index if !is_vfinal, else f2 is a Method*

  // It's final, need a null check here!
  __ null_check(recv);

  // profile this call
  __ profile_final_call(rax);
  __ profile_arguments_type(rax, method, r13, true);

  __ jump_from_interpreted(method, rax);

  __ bind(notFinal);

  // get receiver klass
  __ null_check(recv, oopDesc::klass_offset_in_bytes());
  __ load_klass(rax, recv);

  // profile this call
  __ profile_virtual_call(rax, r14, rdx);

  // get target Method* & entry point
  __ lookup_virtual_method(rax, index, method);
  __ profile_arguments_type(rdx, method, r13, true);
  __ jump_from_interpreted(method, rdx);
}
```



以 `lookup_virtual_method` 为例：

```cpp
// macroAssembler_x86.cpp

// virtual method calling
void MacroAssembler::lookup_virtual_method(Register recv_klass,
                                           RegisterOrConstant vtable_index,
                                           Register method_result) {
  const int base = InstanceKlass::vtable_start_offset() * wordSize;
  assert(vtableEntry::size() * wordSize == wordSize, "else adjust the scaling in the code below");
  Address vtable_entry_addr(recv_klass,
                            vtable_index, Address::times_ptr,
                            base + vtableEntry::method_offset_in_bytes());
  movptr(method_result, vtable_entry_addr);
}

void MacroAssembler::movptr(Register dst, Address src) {
  LP64_ONLY(movq(dst, src)) NOT_LP64(movl(dst, src));
}
```



```cpp
// assembler_x86.cpp

void Assembler::movq(Register dst, Register src) {
  int encode = prefixq_and_encode(dst->encoding(), src->encoding());
  emit_int8((unsigned char)0x8B);
  emit_int8((unsigned char)(0xC0 | encode));
}
```



```cpp
// assembler.hpp

// class AbstractAssembler
void emit_int8(   int8_t  x) { code_section()->emit_int8(   x); }
```



```cpp
// codeBuffer.hpp

// class CodeSection
void emit_int8 ( int8_t  x)  { *((int8_t*)  end()) = x; set_end(end() + sizeof(int8_t)); }
```

























