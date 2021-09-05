
* search: `pacman -Ss <pkg>`
* install: `pacmane -S <pkg>`

* list installed packages: `pacman -Qi`
* list installed package files: `pacman -Ql <pkg>`

* whoprovides等效： `pacman -F <file>`

  `-Fly` 输出包文件

  ```sh
  $ pacman -F make
  warning: database file for 'mingw32' does not exist (use '-Fy' to download)
  warning: database file for 'mingw64' does not exist (use '-Fy' to download)
  warning: database file for 'ucrt64' does not exist (use '-Fy' to download)
  warning: database file for 'clang64' does not exist (use '-Fy' to download)
  warning: database file for 'msys' does not exist (use '-Fy' to download)
  
  $ pacman -Fy make
  :: Synchronizing package databases...
   mingw32                                                   5.7 MiB   951 KiB/s 00:06 [###############################################] 100%
   mingw64                                                   5.8 MiB   538 KiB/s 00:11 [###############################################] 100%
   ucrt64                                                    5.7 MiB   463 KiB/s 00:13 [###############################################] 100%
   clang64                                                   4.7 MiB   232 KiB/s 00:21 [###############################################] 100%
   msys                                                   1293.1 KiB   162 KiB/s 00:08 [###############################################] 100%
  msys/bash-completion 2.11-1 [installed]
      usr/share/bash-completion/completions/make
  msys/bisonc++ 6.04.00-1
      usr/share/doc/bisonc++-doc/demos/bison++Example.NEW/make
  ```

  

  