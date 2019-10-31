#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int p0[2];
    int p1[2];
    pipe(p0);	
    pipe(p1);	
    if(fork()==0){
        // child
        int pid=getpid();
        // p0[0] for read, p1[1] for write
        close(p0[1]);
        close(p1[0]);

        char w_buf[]="ping";
        write(p1[1],w_buf,5);
        int n;
        char r_buf[5];
        for(;;){
            n=read(p0[0],r_buf,5);
            if(n!=5){
                printf("%d bad read size:%d,exit\n",pid,n);
                break;
            }
            printf("%d: %s\n",pid,r_buf);
            write(p1[1],w_buf,5);
        }
        close(p0[0]);
        close(p1[1]);
        exit(1);
    }else{
        // child
        int pid=getpid();
        // p1[0] for read, p0[1] for write
        close(p0[0]);
        close(p1[1]);

        char w_buf[]="pong";
        int n;
        char r_buf[5];
        for(;;){
            n=read(p1[0],r_buf,5);
            if(n!=5){
                printf("%d: bad read size:%d,exit\n",pid,n);
                break;
            }
            printf("%d: %s\n",pid,r_buf);
            write(p0[1],w_buf,5);
        }
        close(p1[0]);
        close(p0[1]);
        exit(1);
    }
}
