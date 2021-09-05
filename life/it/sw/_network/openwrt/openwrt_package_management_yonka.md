
ref: https://gist.github.com/tienthanh2509/ffc8fb96653ae4971e71c1248345b854

一些常见的包
```sh
opkg update

# Web interface
opkg install luci
# https for luci web
opkg install luci-ssl-openssl libuhttpd-openssl

# OpenVPN
opkg install luci-app-openvpn openvpn

# Other tools
opkg install bash ca-certificates libustream-openssl curl wget binutils aria2 fdisk

# Wireguard VPN
opkg install wireguard luci-app-wireguard

# Full source-routing
opkg install ip
```
