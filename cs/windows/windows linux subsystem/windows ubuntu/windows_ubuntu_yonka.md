

## 设置

```cmd
PS C:\Users\xx> ubuntu /?                                                                                             Launches or configures a Linux distribution.

Usage:
    <no args>
        Launches the user's default shell in the user's home directory.

    install [--root]
        Install the distribuiton and do not launch the shell when complete.
          --root
              Do not create a user account and leave the default user set to root.

    run <command line>
        Run the provided command line in the current working directory. If no
        command line is provided, the default shell is launched.

    config [setting [value]]
        Configure settings for this distribution.
        Settings:
          --default-user <username>
              Sets the default user to <username>. This must be an existing user.

    help
        Print usage information.
```

### 默认用户

```sh
ubuntu.exe config --default-user ${user}
```

