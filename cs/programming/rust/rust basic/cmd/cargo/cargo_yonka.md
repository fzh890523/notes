

## config

### config file

Hierarchical structure
Cargo allows local configuration for a particular package as well as global configuration, like git. Cargo extends this to a hierarchical strategy. If, for example, Cargo were invoked in /projects/foo/bar/baz, then the following configuration files would be probed for and unified in this order:

```
/projects/foo/bar/baz/.cargo/config
/projects/foo/bar/.cargo/config
/projects/foo/.cargo/config
/projects/.cargo/config
/.cargo/config
$CARGO_HOME/config ($CARGO_HOME defaults to $HOME/.cargo)
```

With this structure, you can specify configuration per-package, and even possibly check it into version control. You can also specify personal defaults with a configuration file in your home directory.

### config内容介绍

```toml
# An array of paths to local repositories which are to be used as overrides for
# dependencies. For more information see the Specifying Dependencies guide.
paths = ["/path/to/override"]

[cargo-new]
# This is your name/email to place in the `authors` section of a new Cargo.toml
# that is generated. If not present, then `git` will be probed, and if that is
# not present then `$USER` and `$EMAIL` will be used.
name = "..."
email = "..."

# By default `cargo new` will initialize a new Git repository. This key can
# be set to change the version control system used. Valid values are `git`,
# `hg` (for Mercurial), `pijul`, `fossil`, or `none` to disable this behavior.
vcs = "none"

# For the following sections, $triple refers to any valid target triple, not the
# literal string "$triple", and it will apply whenever that target triple is
# being compiled to. 'cfg(...)' refers to the Rust-like `#[cfg]` syntax for
# conditional compilation.
[target.$triple]
# This is the linker which is passed to rustc (via `-C linker=`) when the `$triple`
# is being compiled for. By default this flag is not passed to the compiler.
linker = ".."
# Same but for the library archiver which is passed to rustc via `-C ar=`.
ar = ".."
# If a runner is provided, compiled targets for the `$triple` will be executed
# by invoking the specified runner executable with actual target as first argument.
# This applies to `cargo run`, `cargo test` and `cargo bench` commands.
# By default compiled targets are executed directly.
runner = ".."
# custom flags to pass to all compiler invocations that target $triple
# this value overrides build.rustflags when both are present
rustflags = ["..", ".."]

[target.'cfg(...)']
# Similar for the $triple configuration, but using the `cfg` syntax.
# If several `cfg` and $triple targets are candidates, then the rustflags
# are concatenated. The `cfg` syntax only applies to rustflags, and not to
# linker.
rustflags = ["..", ".."]
# Similar for the $triple configuration, but using the `cfg` syntax.
# If one or more `cfg`s, and a $triple target are candidates, then the $triple
# will be used
# If several `cfg` are candidates, then the build will error
runner = ".."

# Configuration keys related to the registry
[registry]
index = "..."   # URL of the registry index (defaults to the index of crates.io)
default = "..." # Name of the default registry to use (can be overridden with
                # --registry)

# Configuration keys for registries other than crates.io.
# `$name` should be the name of the registry, which will be used for
# dependencies in `Cargo.toml` files and the `--registry` command-line flag.
# Registry names should only contain alphanumeric characters, `-`, or `_`.
[registries.$name]
index = "..."   # URL of the registry index

[http]
proxy = "host:port" # HTTP proxy to use for HTTP requests (defaults to none)
                    # in libcurl format, e.g., "socks5h://host:port"
timeout = 30        # Timeout for each HTTP request, in seconds
cainfo = "cert.pem" # Path to Certificate Authority (CA) bundle (optional)
check-revoke = true # Indicates whether SSL certs are checked for revocation
low-speed-limit = 5 # Lower threshold for bytes/sec (10 = default, 0 = disabled)
multiplexing = true # whether or not to use HTTP/2 multiplexing where possible

# This setting can be used to help debug what's going on with HTTP requests made
# by Cargo. When set to `true` then Cargo's normal debug logging will be filled
# in with HTTP information, which you can extract with
# `CARGO_LOG=cargo::ops::registry=debug` (and `trace` may print more).
#
# Be wary when posting these logs elsewhere though, it may be the case that a
# header has an authentication token in it you don't want leaked! Be sure to
# briefly review logs before posting them.
debug = false

[build]
jobs = 1                  # number of parallel jobs, defaults to # of CPUs
rustc = "rustc"           # the rust compiler tool
rustdoc = "rustdoc"       # the doc generator tool
target = "triple"         # build for the target triple (ignored by `cargo install`)
target-dir = "target"     # path of where to place all generated artifacts
rustflags = ["..", ".."]  # custom flags to pass to all compiler invocations
rustdocflags = ["..", ".."]  # custom flags to pass to rustdoc
incremental = true        # whether or not to enable incremental compilation
                          # If `incremental` is not set, then the value from
                          # the profile is used.
dep-info-basedir = ".."   # full path for the base directory for targets in depfiles

[term]
verbose = false        # whether cargo provides verbose output
color = 'auto'         # whether cargo colorizes output

# Network configuration
[net]
retry = 2 # number of times a network call will automatically retried
git-fetch-with-cli = false  # if `true` we'll use `git`-the-CLI to fetch git repos
offline = false # do not access the network, but otherwise try to proceed if possible

# Alias cargo commands. The first 4 aliases are built in. If your
# command requires grouped whitespace use the list format.
[alias]
b = "build"
c = "check"
t = "test"
r = "run"
rr = "run --release"
space_example = ["run", "--release", "--", "\"command list\""]
```

### author 

`Aha! I was unaware of GIT_AUTHOR_NAME and GIT_COMMITTER_NAME. Currently we read user.email and user.name from ~/.gitconfig or cargo-new.name and cargo-new.email.`

cargo会从gitconfig获取user信息。

覆盖的话，在config文件里的`cargo-new`块里加入`name`和`email`即可。


## Credentials

`$CARGO_HOME/credentials`

```toml
[registry]
token = "..."   # Access token for crates.io

# `$name` should be a registry name (see above for more information about
# configuring registries).
[registries.$name]
token = "..."   # Access token for the named registry
```

# project layout/structure



This is how [Cargo Docs describes](https://doc.rust-lang.org/cargo/guide/project-layout.html) about the recommended project layout,

```
.
├── Cargo.lock
├── Cargo.toml
├── benches
│   └── large-input.rs
├── examples
│   └── simple.rs
├── src
│   ├── bin
│   │   └── another_executable.rs
│   ├── lib.rs
│   └── main.rs
└── tests
    └── some-integration-tests.rs
```

- The source code goes in the `src` directory.
- The default library file is `src/lib.rs`.
- The default executable file is `src/main.rs`.
- Other executables can be placed in `src/bin/*.rs`.
- Integration tests go in the `tests` directory (unit tests go in each file they’re testing).
- Examples go in the `examples` directory.
- Benchmarks go in the `benches` directory.



