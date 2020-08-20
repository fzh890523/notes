
```
Note: for generators with @contextlib.contextmanager one has to use try ... finally statement to make sure resources are properly freed because exceptions raised inside with block are captured by contextmanager and re-raised inside generator body. I.e. code after the yield statement will not be called if exception is raised inside such a with block.
```


