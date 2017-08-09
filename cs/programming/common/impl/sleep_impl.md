# 实现



之前`man 3 sleep`看到：

```
BUGS
       sleep() may be implemented using SIGALRM; mixing calls to alarm(2) and sleep() is a bad idea.

       Using longjmp(3) from a signal handler or modifying the handling of SIGALRM while sleeping will cause undefined results.
```

就一直记着sleep实现是 timer+SLGALRM。



刚翻了下好像已经不是这么实现了。

搜了下：



from [Implementing sleep() using SIGALRM](https://stackoverflow.com/questions/20881020/implementing-sleep-using-sigalrm)

The SIGALARM implementation is seriously legacy, and won't conform modern standards (not least, because if you call it from a library you'll mess with any alarms the application may be using). So it needs another syscall.

Possible choices:

- Use `nanosleep`. It's a syscall on most kernels - it's just a high resolution version of `sleep`, so does the job for you. Note that `sleep` has some junky behaviour if SIGCHLD is ignored, so the glibc implementation blocks and unblocks the signals in its `nanosleep`-wrapping implementation of `sleep` to try and get the legacy behaviour!
- Use `select`. It's a perfectly good choice - use empty FD_SETs and pass in your timeout. Very portable and there's nothing wrong with it.



看了下，最新的glibc 2.25 和 老的2.0.1 都是新的实现； 1.09.1则是老的SIGALRM实现。

`man -k 2 sleep`（好像参数2没生效）也可以看到，syscall里只有`nanosleep`和`clock_nanosleep`。



## glibc-2.25:sysdeps/nacl/nanosleep.c

不清楚nacl是哪个平台，不过只找到nacl和mach的。



最后调的是

```c
/* Pause execution for a number of nanoseconds.  */
int
 (const struct timespec *requested_time,
                  struct timespec *remaining)
{
  return NACL_CALL (__nacl_irt_basic.nanosleep (requested_time, remaining), 0);
}

weak_alias (, __nanosleep)
libc_hidden_def (__nanosleep)
weak_alias (, nanosleep)
```

看起来这里就是调syscall了。



## glibc-1.09.1:sysdeps/posix/sleep.c



```c
  if (remaining > 0 && remaining < seconds)
    {
      /* The user's alarm will expire before our own would.
	 Restore the user's signal action state and let his alarm happen.  */
      (void) sigaction (SIGALRM, &oact, (struct sigaction *) NULL);
      alarm (remaining);	/* Restore sooner alarm.  */
      sigsuspend (&oset);	/* Wait for it to go off.  */
      after = time ((time_t *) NULL);
    }
  else
    {
      /* Atomically restore the old signal mask
	 (which had better not block SIGALRM),
	 and wait for a signal to arrive.  */
      sigsuspend (&oset);

      after = time ((time_t *) NULL);

      /* Restore the old signal action state.  */
      (void) sigaction (SIGALRM, &oact, (struct sigaction *) NULL);
    }
```



alarm + sigaction + sigsuspend



