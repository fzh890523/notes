



## 



```
# basic pfctl control
# ==
# Related: http://www.OpenBSD.org
# Last update: Tue Dec 28, 2004
# ==
# Note:
# this document is only provided as a basic overview
# for some common pfctl commands and is by no means
# a replacement for the pfctl and pf manual pages.

#### General PFCTL Commands ####
# pfctl -d disable packet-filtering
# pfctl -e enable packet-filtering
# pfctl -q run quiet
# pfctl -v -v run even more verbose

#### Loading PF Rules ####
# pfctl -f /etc/pf.conf load /etc/pf.conf
# pfctl -n -f /etc/pf.conf parse /etc/pf.conf, but dont load it
# pfctl -R -f /etc/pf.conf load only the FILTER rules
# pfctl -N -f /etc/pf.conf load only the NAT rules
# pfctl -O -f /etc/pf.conf load only the OPTION rules

#### Clearing PF Rules & Counters ####
# pfctl -F all flush ALL
# pfctl -F rules flush only the RULES
# pfctl -F queue flush only queue’s
# pfctl -F nat flush only NAT
# pfctl -F info flush all stats that are not part of any rule.
# pfctl -z clear all counters
# note: flushing rules do not touch any existing stateful connections

#### Output PF Information ####
# pfctl -s rules show filter information
# pfctl -v -s rules show filter information for what FILTER rules hit..
# pfctl -vvsr show filter information as above and prepend rule numbers
# pfctl -v -s nat show NAT information, for which NAT rules hit..
# pfctl -s nat -i xl1 show NAT information for interface xl1
# pfctl -s queue show QUEUE information
# pfctl -s label show LABEL information
# pfctl -s state show contents of the STATE table
# pfctl -s info show statistics for state tables and packet normalization
# pfctl -s all show everything

#### Maintaining PF Tables ####
# pfctl -t addvhosts -T show show table addvhosts
# pfctl -vvsTables view global information about all tables
# pfctl -t addvhosts -T add 192.168.1.50 add entry to table addvhosts
# pfctl -t addvhosts -T add 192.168.1.0/16 add a network to table addvhosts
# pfctl -t addvhosts -T delete 192.168.1.0/16 delete nework from table addvhosts
# pfctl -t addvhosts -T flush remove all entries from table addvhosts
# pfctl -t addvhosts -T kill delete table addvhosts entirely
# pfctl -t addvhosts -T replace -f /etc/addvhosts reload table addvhosts on the fly
# pfctl -t addvhosts -T test 192.168.1.40 find ip address 192.168.1.40 in table addvhosts
# pfctl -T load -f /etc/pf.conf load a new table definition
# pfctl -t addvhosts -T show -v output stats for each ip address in table addvhosts
# pfctl -t addvhosts -T zero reset all counters for table addvhosts
```





```
# basic pfctl control
# ==
# Related: http://www.OpenBSD.org
# Last update: Tue Dec 28, 2004
# ==
# Note:
# this document is only provided as a basic overview
# for some common pfctl commands and is by no means
# a replacement for the pfctl and pf manual pages.

#############################
#### General PFCTL Commands ####
#############################

# disable packet-filtering:
# pfctl -d

# enable packet-filtering:
# pfctl -e

# run quiet:
# pfctl -q

# run even more verbose:
# pfctl -v

######################
#### Loading PF Rules ####
######################

# load /etc/pf.conf:
# pfctl -f /etc/pf.conf

# parse /etc/pf.conf, but dont load it:
# pfctl -n -f /etc/pf.conf

# load only the FILTER rules:
# pfctl -R -f /etc/pf.conf

# load only the NAT rules:
# pfctl -N -f /etc/pf.conf

# load only the OPTION rules:
# pfctl -O -f /etc/pf.conf

###############################
#### Clearing PF Rules & Counters ####
###############################

# flush ALL:
# pfctl -F all

# flush only the RULES:
# pfctl -F rules

# flush only queue’s:
# pfctl -F queue

# flush only NAT:
# pfctl -F nat

# flush all stats that are not part of any rule:
# pfctl -F info

# clear all counters:
# pfctl -z

# note: flushing rules do not touch any existing stateful connections

#########################
#### Output PF Information ####
#########################

# show filter information:
# pfctl -s rules

# show filter information for what FILTER rules hit:
# pfctl -v -s rules

# filter information as above and prepend rule numbers:
# pfctl -vvsr show

# show NAT information, for which NAT rules hit:
# pfctl -v -s nat

# show NAT information for interface xl1:
# pfctl -s nat -i xl1

# show QUEUE information:
# pfctl -s queue

# show LABEL information:
# pfctl -s label

$ show contents of the STATE table:
# pfctl -s state

# show statistics for state tables and packet normalization:
# pfctl -s info

# show everything:
# pfctl -s all

#########################
#### Maintaining PF Tables ####
#########################

# show table addvhosts:
# pfctl -t addvhosts -T show

# view global information about all tables:
# pfctl -vvsTables

# add entry to table addvhosts
# pfctl -t addvhosts -T add 192.168.1.50 

# add a network to table addvhosts:
# pfctl -t addvhosts -T add 192.168.1.0/16

# delete nework from table addvhosts:
# pfctl -t addvhosts -T delete 192.168.1.0/16

# remove all entries from table addvhosts:
# pfctl -t addvhosts -T flush

# delete table addvhosts entirely:
# pfctl -t addvhosts -T kill

# reload table addvhosts on the fly:
# pfctl -t addvhosts -T replace -f /etc/addvhosts

# find ip address 192.168.1.40 in table addvhosts:
# pfctl -t addvhosts -T test 192.168.1.40

#load a new table definition:
# pfctl -T load -f /etc/pf.conf 

# output stats for each ip address in table addvhosts:
# pfctl -t addvhosts -T show -v

# reset all counters for table addvhosts:
# pfctl -t addvhosts -T zero
```

