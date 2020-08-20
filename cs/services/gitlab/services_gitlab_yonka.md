

# manual

## 邮件配置

参考： 
https://blog.csdn.net/qq_38966361/article/details/90543377
https://blog.csdn.net/gnail_oug/article/details/96701397

```sh
# vim /etc/gitlab/gitlab.rb
gitlab_rails['smtp_enable'] = true
gitlab_rails['smtp_address'] = "smtp.qq.com"
gitlab_rails['smtp_port'] = 465
gitlab_rails['smtp_user_name'] = "邮箱@qq.com"
gitlab_rails['smtp_password'] = "开通smtp时返回的授权码"
gitlab_rails['smtp_domain'] = "qq.com"
gitlab_rails['smtp_authentication'] = "login"
gitlab_rails['smtp_enable_starttls_auto'] = true
gitlab_rails['smtp_tls'] = true
​
user['git_user_email'] = "邮箱@qq.com"
gitlab_rails['gitlab_email_from'] = '邮箱@qq.com'
gitlab_rails['gitlab_email_display_name'] = 'git server'  ## 显示名字
## from没有正确配置的话，可能发不成功或者被腾讯干掉
```

然后：
```sh
# 使生效
gitlab-ctl reconfigure
## vim /var/opt/gitlab/gitlab-rails/etc/smtp_settings.rb 可以查看生成的配置

# 测试
gitlab-rails console
Notify.test_email('接收方邮件地址','邮件标题','邮件内容').deliver_now  # 参数根据实际情况改一下
```


# 问题处理
