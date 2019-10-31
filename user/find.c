#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/**
 * Version 0: strcmp didn't judge return code 
 * Version 1: use 'return' instead of 'exit()' in main , which caused error. 'Return' is a language primitive however 'exit' is a system call.
 */
void find(char *p,char *f){
    char buf[512],*i;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd=open(p,0))<0){
        printf("Failed to open dir=%s \n",p);
        exit(1);
    }
    if((fstat(fd,&st))<0){
        printf("Failed to get dir stat=%s \n",p);
        close(fd);
        exit(1);
    }
    if (st.type!=T_DIR){
        close(fd);
        return ;
    }
    strcpy(buf,p);
    i=buf+strlen(buf);
    *i++='/';
    while(read(fd,&de,sizeof de)==sizeof de){
        if(de.inum==0 || strcmp(".",de.name) ==0 || strcmp("..",de.name)==0){
            continue;
        }
        memmove(i,de.name,DIRSIZ);
        i[DIRSIZ]=0;
        if(stat(buf,&st)<0){
            printf("Failed to stat file=%s",buf);
            exit(1);
        }
        if(st.type==T_FILE){
            if(strcmp(de.name,f)==0){
                printf("%s\n",buf);
            }
        }else if(st.type==T_DIR){
            find(buf,f);
        }
    }
    close(fd);
}
int main(int argc,char* argv[]){
    if(argc!=3){
        printf("Usage: <path> <regex>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}


