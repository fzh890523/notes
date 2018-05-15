

# cases



## 设置了authorized_keys仍然要输入密码的问题





```
debug1: rekey after 134217728 blocks
debug1: SSH2_MSG_NEWKEYS received
debug1: SSH2_MSG_EXT_INFO received
debug1: kex_input_ext_info: server-sig-algs=<rsa-sha2-256,rsa-sha2-512>
debug1: SSH2_MSG_SERVICE_ACCEPT received
debug1: Authentications that can continue: publickey,gssapi-keyex,gssapi-with-mic,password
debug1: Next authentication method: publickey
debug1: Offering RSA public key: /Users/${user}/.ssh/id_rsa
debug1: Authentications that can continue: publickey,gssapi-keyex,gssapi-with-mic,password
debug1: Trying private key: /Users/${user}/.ssh/id_dsa
debug1: Trying private key: /Users/${user}/.ssh/id_ecdsa
debug1: Trying private key: /Users/${user}/.ssh/id_ed25519
debug1: Next authentication method: password
admin@${ip}'s password:
```







```
tail -f /var/log/secure
Apr 24 13:34:55 ${host} su: pam_unix(su-l:session): session opened for user admin by root(uid=0)
Apr 24 13:35:33 ${host} sshd[12029]: Authentication refused: bad ownership or modes for file /home/admin/.ssh/authorized_keys
Apr 24 13:35:36 ${host} sshd[12029]: Connection closed by ${ip} port 32818 [preauth]
Apr 24 13:35:47 ${host} su: pam_unix(su-l:session): session closed for user admin
Apr 24 13:36:52 ${host} sshd[12040]: Authentication refused: bad ownership or modes for file /home/admin/.ssh/authorized_keys
Apr 24 13:38:54 ${host} su: pam_unix(su-l:session): session opened for user admin by root(uid=0)
Apr 24 13:40:18 ${host} sshd[12092]: Authentication refused: bad ownership or modes for directory /home/admin/.ssh
Apr 24 13:40:32 ${host} su: pam_unix(su-l:session): session closed for user admin
Apr 24 13:41:03 ${host} sshd[12092]: Connection closed by ${ip} port 53524 [preauth]
Apr 24 13:41:44 ${host} sshd[12101]: Connection closed by ${ip} port 44473 [preauth]
Apr 24 13:42:50 ${host} sshd[12107]: Connection closed by ${ip} port 57466 [preauth]
Apr 24 13:42:55 ${host} sshd[12123]: Accepted publickey for admin from ${ip} port 34185 ssh2: RSA SHA256:xxx
Apr 24 13:42:55 ${host} sshd[12123]: pam_unix(sshd:session): session opened for user admin by (uid=0)
Apr 24 13:42:55 ${host} sshd[12123]: pam_unix(sshd:session): session opened for user admin by (uid=0)
Apr 24 13:42:58 ${host} sshd[12125]: Received disconnect from ${ip} port 34185:11: disconnected by user
Apr 24 13:42:58 ${host} sshd[12125]: Disconnected from ${ip} port 34185
Apr 24 13:42:58 ${host} sshd[12123]: pam_unix(sshd:session): session closed for user admin
```



`~/.ssh`和`~/.ssh/authorized_keys`需要时`700`（for dir）和`600`（for file）



