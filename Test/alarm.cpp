#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
void sigalrm_fn(int sig)

{
    printf("alarm!\n");
    alarm(2);
    return;
}

int main(void)

{
    signal(SIGALRM, sigalrm_fn);  //后面的函数必须是带int参数的
    alarm(1);
    while(1)
    pause();

}