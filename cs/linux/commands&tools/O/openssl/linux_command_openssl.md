

### 生成密钥、签名证书

```sh
openssl req -x509 -sha256 -nodes -days 365 -newkey rsa:2048 -subj '/O=com/CN=com' -keyout com.key -out com.crt  # Public Key Algorithm: rsaEncryption, Signature Algorithm: sha256WithRSAEncryption
openssl req -out svr1.csr -newkey rsa:2048 -nodes -keyout srv1.key -subj "/CN=*.com/O=svr1"  # Public Key Algorithm: rsaEncryption
openssl x509 -req -days 365 -CA com.crt -CAkey com.key -set_serial 0 -in svr1.csr -out svr1.crt  # Signature Algorithm: sha256WithRSAEncryption
# 以root为ca
cp com.crt svr1-ca.crt
```

```sh
openssl ecparam -list_curves | grep -i secp256
openssl req -x509 -nodes -days 365 -newkey ec -pkeyopt ec_paramgen_curve:prime256v1 -subj '/O=com/CN=com' -keyout com-ec.key -out com-ec.crt  # Public Key Algorithm: id-ecPublicKey
# secp256r1 == prime256v1
openssl req -out svr2-ec.csr -newkey ec -pkeyopt ec_paramgen_curve:prime256v1 -nodes -keyout srv2-ec.key -subj "/CN=*.com/O=svr1"
openssl x509 -req -days 365 -CA com-ec.crt -CAkey com-ec.key -set_serial 0 -in svr2-ec.csr -out svr2-ec.crt  # Signature Algorithm: ecdsa-with-SHA256
# 这里如果用 rsa那组ca来签名也可以，此时： Signature Algorithm: sha256WithRSAEncryption
```

> 相关算法分为：
>
> * 加密算法
> * 签名算法





```sh
openssl req -x509 -sha256 -nodes -days 365 -newkey rsa:2048 -subj '/O=com/CN=com' -keyout com.key -out com.crt

# >= 1.1.1 可以用这个
openssl req -out svr2.csr -newkey rsa:2048 -nodes -keyout svr2.key -subj "/CN=*.com/O=svr2" -addext "subjectAltName = DNS:*.com"

# 老版本做法
openssl req -out svr2.csr -newkey rsa:2048 -nodes -keyout srv2.key -subj "/CN=*.com/O=svr2" -addext "subjectAltName = DNS:*.com"     -reqexts SAN \
    -config <(cat /etc/ssl/openssl.cnf \
        <(printf "\n[SAN]\nsubjectAltName=DNS:*.com")) 

openssl x509 -req -days 365 -CA com.crt -CAkey com.key -set_serial 0 -in svr2.csr -out svr2.crt -extensions v3_ca -extfile ./ssl-extensions-x509.cnf
# cnf文件如下


openssl req -x509 -nodes -days 365 -newkey ec -pkeyopt ec_paramgen_curve:prime256v1 -subj '/O=com/CN=com' -keyout com-ec.key -out com-ec.crt
openssl req -out svr2-ec.csr -newkey ec -pkeyopt ec_paramgen_curve:prime256v1 -nodes -keyout svr2-ec.key -subj "/CN=*.com/O=svr1" -addext "subjectAltName = DNS:*.com"
openssl x509 -req -days 365 -CA com-ec.crt -CAkey com-ec.key -set_serial 0 -in svr2-ec.csr -out svr2-ec.crt -extensions v3_ca -extfile ./ssl-extensions-x509.cnf



```



```properties
[v3_ca]
subjectAltName = DNS:*.com
```





### 输出证书内容

* `openssl x509 -in bundle.crt -text -noout`
  
  > 据说如果文件里有多个cert (cert chain)，只会打印root - 验证了确实
  
  不带`-in`参数或者`-in /dev/stdin`可以从stdin读取，确认可行
  
  `k get secrets istio-ca-secret -o json | jq '.data."ca-cert.pem"' | base64 -d | openssl x509 -text -noout` 比如解析k8s secret
  
* `openssl crl2pkcs7 -nocrl -certfile CHAINED.pem | openssl pkcs7 -print_certs -text -noout`
  据说可以打印bundle文件中全部certs - 是的

  不带`-certfile`参数好像可以read from stdin - 不太行，输出不对
  
* `openssl req -in svr2.csr -noout -text` 输出csr内容



### 输出密钥内容

* ec key

  ```sh
  openssl ec -in svr2-ec.key -noout -text
  read EC key
  Private-Key: (256 bit)
  priv:
      11:5a:...:
      be:3b:...:
      d4:aa
  pub:
      04:...
      ...
  ASN1 OID: prime256v1
  NIST CURVE: P-256
  ```

* rsa key

  ```sh
  openssl rsa -in svr2.key -noout -text
  RSA Private-Key: (2048 bit, 2 primes)
  modulus:
      00:c5...
      ...
  publicExponent: 65537 (0x10001)
  privateExponent:
      00:97:...
      ...
  ...    
  ```

  

### 校验证书、密钥

* rsa

  ```sh
  openssl rsa -modulus -noout -in myserver.key | openssl md5
  openssl x509 -modulus -noout -in myserver.crt | openssl md5
  # 输出应该一致
  
  openssl rsa -check -noout -in myserver.key
  # If it doesn't say 'RSA key ok', it isn't OK!
  ```

* ec

  ```sh
  openssl ec -check -noout -in myserver.key
  ```

  