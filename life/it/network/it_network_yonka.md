

## name resolve

#### 家庭环境自定义dns解析

* 最方便的方式自然是路由器上指定了

  想usg这种，可以登上去修改配置，满足自定义的需求。 但操作相对不便，路由器（GUI）原生支持最好

* 前一条不可行的情况下（比如unifi统一管理usg的话，因为unifi没这个功能，而unifi配置又会覆盖usg配置，会使得自行配置随后一会儿就失效）： 在unifi/路由器那配置secondary dns指向自建dns server（如dnsmasq），然后在自建dns中配置

  unifi在： `NETWORKS - Local Networks - ... - DHCP Controls - DHCP Name Server` 中配置，auto改为manual然后填入。 第一个填路由器ip，第二个填。。。

  如LEDE自带一个gui，操作还算方便。

  此时，LEDE那边应该对该接口指定只使用路由ip这一个dns server，否则可能发生循环（-> primary解析失败 -> secondary也即本地 -> ...）？  但实测没异常。
  这个可以在LEDE的网页的interfaces里配置。

  > **注意**： 不同系统（或者说dns client）的解析策略有差异，尤其在secondary dns server上。 似乎windows和macos都是把secondary当作primary的冷备份，只有当服务器不可用（解析请求超时等，甚至可能要多次超时。。）才使用，而不是解析失败无结果再尝试secondary。 
  >
  > 所以表现为该方案在win/macos上无效， 但对于linux有效。。。
  >
  > ref：
  >
  > * https://support.microsoft.com/en-us/help/2834226/net-dns-dns-client-resolution-timeouts
  > * https://www.cnet.com/news/os-x-10-6-3-and-dns-server-priority-changes/

* 其实如果电信光猫支持的话，还可以在最外的光猫那配置，但看了下，没有。。。







