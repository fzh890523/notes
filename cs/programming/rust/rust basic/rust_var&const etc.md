

* `let`

* `mut`

* `const` 
  define constants.
  It lives for the entire lifetime of a program but has no fixed address in the memory. 
  > Always use const, instead of static. It’s pretty rare that you actually want a memory location associated with your constant, and using a const allows for optimizations like constant propagation not only in your crate but also in downstream crates.
* `static`
  define a ‘global variable’ type facility.
  There is only one instance for each value, and it’s at a fixed location in memory.
  > Usually, statics are placed at the top of the code file, outside the functions.

