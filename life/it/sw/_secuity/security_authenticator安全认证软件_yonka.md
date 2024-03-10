

## OTP app



* google authenticator

  页面简洁，功能简陋

  

  **bug多**

  * 导出二维码其他端无法识别

  * 同步问题

    多端登陆时，眼睁睁看着OTP一份变两份/三份... 而且删除一个其他的“分身”也没了，真是日了狗了 ggl家竟然写出这样的垃圾代码

  

  **注意** 导出方式只有二维码，解析出来是个类似URL格式，并且**其他app读取失败**（偶尔一次还成功了，囧）

  这个二维码扫出来数据格式如： `otpauth-migration://offline?data=CjEKCkhlbGxvId6tvu8SGEV4YW1wbGU6YWxpY2VAZ29vZ2xlLmNvbRoHRXhhbXBsZTAC`

  

  有一些开源项目可以反解出数据：

  * https://github.com/digitalduke/otpauth-migration-decoder
  * https://github.com/krissrex/google-authenticator-exporter

  

  也有一个现成的web服务可以完成“一条龙”： https://www.googleauthenticator.dev/

  > 它把流程分步了，所以可以看它来参考
  >
  > 1. 扫描qrcode，得到URL
  >
  > 2. 人工提取出URL中的`data` query，内容填到第二步（解析protobuf数据）
  >
  >    数据结构为（样例如下）：
  >
  >    | Export #1 |                            |
  >    | --------- | -------------------------- |
  >    | Algorithm | ``                         |
  >    | Counter   | ``                         |
  >    | Digits    | ``                         |
  >    | Issuer    | `Example`                  |
  >    | Name      | `Example:alice@google.com` |
  >    | Secret    | `SGVsbG8h3q2+7w==`         |
  >    | Type      | `OTP_TYPE_TOTP`            |
  >
  > 3. 从`Secret`得到`Shared Secret`
  >
  >    | Shared Secret | `JBSWY3DPEHPK3PXP` |
  >    | ------------- | ------------------ |
  >    |               |                    |
  >    
  >    **实际导入端真正需要的也是这个**
  >    
  > 4. 通过`Shared Secret`得到OTP码，也即用来做验证的码
  >
  >    | TOTP | `729764` |
  >    | ---- | -------- |
  >    |      |          |
  >
  > 

* microsoft authenticator

  页面较丰富，集成了一些其他功能比如MS账号管理， 密码管理等。

  但就OTP本身，还是不太好用

* **2FAS authenticator**

  交互不错； 支持导出为二维码 或 **文件**（内容为json，可选加密）； 支持基于google drive的同步