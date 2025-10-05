#include <stdio.h>
#include <stdlib.h>

main() {
    for(int i=0;i<100000;++i){char *str = (char *)0xdeadbeef;
printf("%s", str);  // 非法地址访问
 
    }
    return 0;
}