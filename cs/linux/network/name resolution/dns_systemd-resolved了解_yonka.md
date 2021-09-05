ref：

* https://wiki.archlinux.org/index.php/Systemd-resolved
* 



## config



### 配置“global dns”



`/etc/systemd/resolved.conf`

```properties
#  This file is part of systemd.
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.
#
# Entries in this file show the compile time defaults.
# You can change settings by editing this file.
# Defaults can be restored by simply deleting this file.
#
# See resolved.conf(5) for details

[Resolve]
DNS=192.168.1.11
#FallbackDNS=
Domains=~.
#LLMNR=no
#MulticastDNS=no
#DNSSEC=no
#Cache=yes
#DNSStubListener=yes
```

> 但resolved应该是不支持多dns配置的，而且man里提到如果多文件里都对同一值进行配置，会覆盖/只取一个。
>
> 所以要实现： 配置多个dns分别负责不同的zone/domain的话，只能考虑resolved改为dnsmasq等



## troubleshooting

* `resolvectl status`

* `resolvectl query`

  ```sh
  resolvectl query xxx.com
  ubuntu-20042-desktop: 192.168.1.22             -- link: ens160
                        (xxx.localdomain)
  
  -- Information acquired via protocol DNS in 1.3ms.
  -- Data is authenticated: no
  ```

* `journalctl -u systemd-resolved -f`

  ```sh
  sudo systemctl edit systemd-resolved
  ```

  ```properties
  [Service]
  Environment=SYSTEMD_LOG_LEVEL=debug
  ```

  ```sh
  sudo systemctl restart systemd-resolved
  ```

  

