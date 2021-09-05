

![img](_pics/protocol_TLS_yonka/1191499-20170907142446460-1617706947.png)

![enter image description here](_pics\protocol_TLS_yonka/I0612.jpg)



![enter image description here](_pics\protocol_TLS_yonka/SGyYa.png)



![img](_pics/protocol_TLS_yonka/bg2014020502.png)

# 安全问题



## 无法抵御中间人攻击



### 应对： HSTS

* 验证server cert的CA需要为权威CA
* server cert的域名与当前域名吻合



依赖：

* 浏览器预置CA信息（pub key）
* 浏览器可能需要维持一个动态更新机制以应对权威CA故障（信息泄露等变得不可信）