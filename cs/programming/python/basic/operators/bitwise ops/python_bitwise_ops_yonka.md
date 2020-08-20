

ref: https://wiki.python.org/moin/BitwiseOperators

* `^` bitwise exclusive or (异或)
* `|` bitwise or
* `&` bitwise and
& `~` bitwise nor(?) / complement 

### 输出数字的二进制

* bin函数

  但这个函数只按实际的number的bit位数来输出
  > 背景： python的number似乎默认情况下bit位数仅为“够用”而不是固定的 8/16/32/64，比如
  >
  > ```python
  > a = 1
  > print(a.bit_length())  # output: 1
  > a = 99999
  > print(a.bit_length())  # output: 17
  > ```
  
  可以的办法：
  
  * `bin(v & int('0b' + '1' * n, 2))` v为输出的数字，n为要输出的位数
  
    这个其实是**改变bit length的方式** 
  
    ```python
  a = -1
    a.bit_length()  # 1
  a = -1 & int('1' * 32, 2)
    a.bit_length()  # 32
    
    a = 1
    a.bit_length()  # 1
    a = 1 & int('1' * 32, 2)
    a.bit_length()  # 1
    ```
    
    
    
    这个 `'0b'` 可以不要
    
    还可以写成 `bin(v & (2 ** n - 1))` or `bin(v & ((1 << n) - 1))`
    
    **囧，对正数好像不行，只对负数有效。。。，正数不知道怎么处理，除了字符串填充。。。**
  
  此外对于负数（或者说最高位为1的数），只会输出 `-0b10`这样的格式，也需要采取上面的做法，如：
  
  ```python
   bin(-1 & ((1 << 32) - 1))
  # output: '0b11111111111111111111111111111111'
  ```
  
  
  
  