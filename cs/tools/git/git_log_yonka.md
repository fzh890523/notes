

# 

`git log --help` for ref



## 好用的的模板

* `hs`

  ```sh
  git config --global alias.hs "log --pretty='%C(yellow)%h %C(cyan)%cd %Cblue%aN%C(auto)%d %Creset%s %C(bold normal)%S' --graph --date=short --date-order"
  # --date=short: only date in format YYYY-MM-DD
  # %cd use format from --date
  # %S like --source
  git hs
  
  # like: d45fdb06xxx 2021-09-03 author1 Add .cicd
  ```

  



## 支持直接指定多个目标

如： `git log master v1.0.0`



## 查看所有（分支）

* `--all`

  > Instead of `--all` you may want to use `--branches`, since `--all` also includes `refs/tags` and `refs/remotes`.

* `--branches`

  > 这个参数比较特殊，因为支持的是shell glob，所以如果没有配置任何通配，比如`--branches=master`，会处理为`--branches=master/*`
  >
  > ```sh
  > --branches[=<pattern>]
  >  Pretend as if all the refs in refs/heads are listed on
  >  the command line as <commit>. If <pattern> is given, 
  >  limit branches to ones matching given shell glob. If 
  >  pattern lacks ?, *, or [, /* at the end is implied.
  > ```
  >
  > 所以这个case，需要处理为 `--branches=maste[r]`。 比较丑。 其实一个分支的话，直接`git log master`即可

  * `=maste[r]`
  * `={master,release-7*}`
  * `--branches=maste[r] --branches=v7.0*` 可以这样指定多个，效果应该符合预期



## `--pretty`参数

详见： https://git-scm.com/docs/pretty-formats





* `--pretty=oneline`

  ```
  * 9dae7ff088531b39e1076xxxxx (tag: v7.0.1, 7.0.xx-yy, 7.0.xx) Fix: interface conversion error
  ```

  一行显示一个commit，默认log的话是分行显示各个信息的（commit msg等）

  据说约等于 `--pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit`



### 格式化参数

#### 颜色相关



*%Cred*

switch color to red

*%Cgreen*

switch color to green

*%Cblue*

switch color to blue

*%Creset*

reset color

*%C(…)*

color specification, as described under Values in the "CONFIGURATION FILE" section of [git-config[1\]](https://git-scm.com/docs/git-config). By default, colors are shown only when enabled for log output (by `color.diff`, `color.ui`, or `--color`, and respecting the `auto` settings of the former if we are going to a terminal). `%C(auto,...)` is accepted as a historical synonym for the default (e.g., `%C(auto,red)`). Specifying `%C(always,...)` will show the colors even when color is not otherwise enabled (though consider just using `--color=always` to enable color for the whole output, including this format and anything else git might color). `auto` alone (i.e. `%C(auto)`) will turn on auto coloring on the next placeholders until the color is switched again.

> 详细的color见git config



#### 时间相关

*%ad*

author date (format respects --date= option)

*%aD*

author date, RFC2822 style

*%ar*

author date, relative

*%at*

author date, UNIX timestamp

*%ai*

author date, ISO 8601-like format

*%aI*

author date, strict ISO 8601 format

*%as*

author date, short format (`YYYY-MM-DD`)

*%ah*

author date, human style (like the `--date=human` option of [git-rev-list[1\]](https://git-scm.com/docs/git-rev-list))

*%cd*

committer date (format respects --date= option)

*%cD*

committer date, RFC2822 style

*%cr*

committer date, relative

*%ct*

committer date, UNIX timestamp

*%ci*

committer date, ISO 8601-like format

*%cI*

committer date, strict ISO 8601 format

*%cs*

committer date, short format (`YYYY-MM-DD`)

*%ch*

committer date, human style (like the `--date=human` option of [git-rev-list[1\]](https://git-scm.com/docs/git-rev-list))



## 树形显示

* `git log --pretty=oneline --graph`
* `git log --all --source --pretty=oneline --graph`



## 显示内容



### `--date`

--date=<format>

Only takes effect for dates shown in human-readable format, such as when using `--pretty`. `log.date` config variable sets a default value for the log command’s `--date` option. By default, dates are shown in the original time zone (either committer’s or author’s). If `-local` is appended to the format (e.g., `iso-local`), the user’s local time zone is used instead.

`--date=relative` shows dates relative to the current time, e.g. “2 hours ago”. The `-local` option has no effect for `--date=relative`.

`--date=local` is an alias for `--date=default-local`.

`--date=iso` (or `--date=iso8601`) shows timestamps in a ISO 8601-like format. The differences to the strict ISO 8601 format are:

- a space instead of the `T` date/time delimiter
- a space between time and time zone
- no colon between hours and minutes of the time zone

`--date=iso-strict` (or `--date=iso8601-strict`) shows timestamps in strict ISO 8601 format.

`--date=rfc` (or `--date=rfc2822`) shows timestamps in RFC 2822 format, often found in email messages.

`--date=short` shows only the date, but not the time, in `YYYY-MM-DD` format.

`--date=raw` shows the date as seconds since the epoch (1970-01-01 00:00:00 UTC), followed by a space, and then the timezone as an offset from UTC (a `+` or `-` with four digits; the first two are hours, and the second two are minutes). I.e., as if the timestamp were formatted with `strftime("%s %z")`). Note that the `-local` option does not affect the seconds-since-epoch value (which is always measured in UTC), but does switch the accompanying timezone value.

`--date=human` shows the timezone if the timezone does not match the current time-zone, and doesn’t print the whole date if that matches (ie skip printing year for dates that are "this year", but also skip the whole date itself if it’s in the last few days and we can just say what weekday it was). For older dates the hour and minute is also omitted.

`--date=unix` shows the date as a Unix epoch timestamp (seconds since 1970). As with `--raw`, this is always in UTC and therefore `-local` has no effect.

`--date=format:...` feeds the format `...` to your system `strftime`, except for %z and %Z, which are handled internally. Use `--date=format:%c` to show the date in your system locale’s preferred format. See the `strftime` manual for a complete list of format placeholders. When using `-local`, the correct syntax is `--date=format-local:...`.

`--date=default` is the default format, and is similar to `--date=rfc2822`, with a few exceptions:

- there is no comma after the day-of-week
- the time zone is omitted when the local time zone is used



### `--source`： display commit source

```sh
--source
Print out the ref name given on the command line by which each commit was reached.
```

大约意思是多一个字段用于显示 **从哪reach到该commit的**，一般就是该commit所在的分支名。

如： `* 27182f095943147060d7510886eexxx      7.0.xxx Remove Dockerfile`中的`7.0.xxx`就是





> # Git log formats
>
> By [Jeff Kreeftmeijer](https://jeffkreeftmeijer.com/) on 2021-07-17
>
> Given a repository with a single commit:
>
> ```
> git init
> touch "file.txt"
> git add file.txt
> git commit -m "Subject" -m "First paragraph" -m "Second paragraph"
> ```
>
> ## `git log --format=medium`
>
> The “medium” format is the default when passing no `--format` option:
>
> ```
> git log
> commit f142f6d3327fbc17ff3f9a7f6c4157d70da7083f
> Author: Alice <alice@example.com>
> Date:   Thu Jul 22 07:45:25 2021 +0200
> 
>     Subject
> 
>     First paragraph
> 
>     Second paragraph
> ```
>
> ## `git log --format=oneline`
>
> ```
> git log --format=oneline
> f142f6d3327fbc17ff3f9a7f6c4157d70da7083f Subject
> ```
>
> ## `git log --format=short`
>
> ```
> git log --format=short
> commit f142f6d3327fbc17ff3f9a7f6c4157d70da7083f
> Author: Alice <alice@example.com>
> 
>     Subject
> ```
>
> ## `git log --format=full`
>
> ```
> git log --format=full
> commit f142f6d3327fbc17ff3f9a7f6c4157d70da7083f
> Author: Alice <alice@example.com>
> Commit: Bob <bob@example.com>
> 
>     Subject
> 
>     First paragraph
> 
>     Second paragraph
> ```
>
> ## `git log --format=fuller`
>
> ```
> git log --format=fuller
> commit f142f6d3327fbc17ff3f9a7f6c4157d70da7083f
> Author:     Alice <alice@example.com>
> AuthorDate: Thu Jul 22 07:45:25 2021 +0200
> Commit:     Bob <bob@example.com>
> CommitDate: Thu Jul 22 07:45:25 2021 +0200
> 
>     Subject
> 
>     First paragraph
> 
>     Second paragraph
> ```
>
> ## `git log --format=email`
>
> ```
> git log --format=email
> From f142f6d3327fbc17ff3f9a7f6c4157d70da7083f Mon Sep 17 00:00:00 2001
> From: Alice <alice@example.com>
> Date: Thu, 22 Jul 2021 07:45:25 +0200
> Subject: [PATCH] Subject
> 
> First paragraph
> 
> Second paragraph
> ```
>
> ## `git log --format=raw`
>
> ```
> git log --format=raw
> commit f142f6d3327fbc17ff3f9a7f6c4157d70da7083f
> tree bdd68b0120ca91384c1606468b4ca81b8f67c728
> author Alice <alice@example.com> 1626932725 +0200
> committer Bob <bob@example.com> 1626932725 +0200
> 
>     Subject
> 
>     First paragraph
> 
>     Second paragraph
> ```