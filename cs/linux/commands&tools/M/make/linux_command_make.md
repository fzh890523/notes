



## debug



Use `make -d` or `make --debug[=flags]` [options](http://www.gnu.org/software/make/manual/make.html#Options-Summary):

> ```
> ‘-d’
> ```
>
> Print debugging information in addition to normal processing. **The debugging information says which files are being considered for remaking, which file-times are being compared and with what results, which files actually need to be remade,** which implicit rules are considered and which are applied—everything interesting about how make decides what to do. The `-d` option is equivalent to `‘--debug=a’` (see below).
>
> ```
> ‘--debug[=options]’
> ```
>
> Print debugging information in addition to normal processing. Various levels and types of output can be chosen. With no arguments, print the “basic” level of debugging. Possible arguments are below; only the first character is considered, and values must be comma- or space-separated.
>
> `a` (all) All types of debugging output are enabled. This is equivalent to using ‘-d’.
>
> `b` (basic) **Basic debugging prints each target that was found to be out-of-date, and whether the build was successful or not.**
>
> `v` (verbose) A level above `‘basic’`; includes messages about which makefiles were parsed, prerequisites that did not need to be rebuilt, etc. This option also enables `‘basic’` messages.
>
> `i` (implicit) Prints messages describing the implicit rule searches for each target. This option also enables `‘basic’` messages.
>
> `j` (jobs) Prints messages giving details on the invocation of specific subcommands.
>
> `m` (makefile) By default, the above messages are not enabled while trying to remake the makefiles. This option enables messages while rebuilding makefiles, too. Note that the `‘all’` option does enable this option. This option also enables `‘basic’` messages.

Another option is to use [remake](http://bashdb.sourceforge.net/remake/) - a patched version of GNU Make that adds improved error reporting, the ability to trace execution, and a debugger.



> `--debug=v`一般够用且不至于太啰嗦



