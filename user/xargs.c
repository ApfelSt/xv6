#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define SIZE 1024

void
xargs(char *args[], int argc)
{
    char *argv[MAXARG];
    int i, n;
    n = 0;
    while(n < MAXARG){
        char *p = argv[n] = malloc(SIZE);
        while(read(0, p, 1) > 0){
            if (*p == '\n') {
                *p = '\0';
                break;
            }
            p++;
            i++;
        }
        if (i == 0) {
            free(argv[n]);
            break; // EOF
        }
        n++;
        i = 0;
    }
    for(i=0;i<n;i++){
        char *argvs[argc + 2];
        for (int j = 0; j < argc; j++) {
            argvs[j] = args[j];
        }
        argvs[argc] = argv[i];
        argvs[argc + 1] = '\0';
        int f = fork();
        if (f < 0) {
            fprintf(2, "xargs: fork failed\n");
            exit(1);
        }
        if(f==0){
            if (exec(args[0], argvs) < 0) {
                fprintf(2, "xargs: exec failed\n");
                exit(1);
            }
        }
        wait(&f);
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    
    
    xargs(argv + 1, argc - 1);
    
    exit(0);
}
