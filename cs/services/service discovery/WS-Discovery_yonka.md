

## 安装

```sh
cd /tmp
wget https://github.com/christgau/wsdd/archive/master.zip
unzip master.zip
sudo mv wsdd-master/src/wsdd.py wsdd-master/src/wsdd
sudo cp wsdd-master/src/wsdd /usr/bin
sudo cp wsdd-master/etc/systemd/wsdd.service /etc/systemd/system

# Open wsdd.service in nano and comment out User=nobody and Group=nobody with a ; semicolon.
sudo nano /etc/systemd/system/wsdd.service

sudo systemctl daemon-reload
sudo systemctl start wsdd
sudo systemctl enable wsdd
sudo service wsdd status
```

卸载

```sh
sudo systemctl stop wsdd
sudo systemctl disable wsdd
sudo rm /usr/bin/wsdd
sudo rm /etc/systemd/system/wsdd.service
```
