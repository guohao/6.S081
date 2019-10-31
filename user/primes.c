#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define INT_SIZE sizeof(int)

/**
 * Version 0: Allocated an int array of len 34. Pipe 2 can only receive first ten numbers because 'write' command's 3rd argument is size of bytes rather than length of array.
 * Version 1: Allocated an int array of size 34, out of resources when calculate 7. Misunderstood the hint that "write 32-bit intS" meaning.
 * Version 2: Use an 1-len int array. Out of resource after printed 31. Allocate pipe array nerver the regardless of it is unused.
 * Final version: Read twice in child process in avoid of allocating unused pipe array. Passed and can calculate 100.
 */
void do_pipeline(int p_p[]){
    close(p_p[1]);
    int buf[1];
    int n=read(p_p[0],buf,INT_SIZE);
    int prime=buf[0];
    printf("prime %d\n",prime);
    n=read(p_p[0],buf,INT_SIZE);
    if(n<INT_SIZE){
        close(p_p[0]);
        exit(0);
    }
    int p[2];
    pipe(p);
    if(fork()==0){
        close(p_p[0]);
        do_pipeline(p);
    }else{
        close(p[0]);
        write(p[1],buf,INT_SIZE);
        while((n=read(p_p[0],buf,INT_SIZE))==INT_SIZE){
            if(buf[0]%prime!=0){
                write(p[1],buf,INT_SIZE);
            }
        }
        close(p_p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}

int main(){
    int p[2];
    pipe(p);
    if(fork()==0){
        do_pipeline(p);
    }else{
        close(p[0]);
        int buf[1];
        for(int i=2;i<100;i++){
            buf[0]=i;
            write(p[1],buf,INT_SIZE);
        }
        close(p[1]);
        wait(0);
    }
    return 0;
}
