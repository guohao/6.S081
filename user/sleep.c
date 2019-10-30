#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc,char* argv[]){
    if(argc < 1){
	    printf("Bad arguments, time is required\n");
	    exit(1);
    }
    int n = atoi(argv[1]);
    if ( n < 1 ){
	    printf("Bad n\n");
	    exit(1);
    }
    sleep(n);
    exit(0);
}



