#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define NA -1
#define NUM 35

void logic(int p, int q, int prime, int end)
{
    if(p==NA){
        for(int i = 2; i <= end; i++) {
            write(q, &i, 1);
        }
        return;
    }
    int x;
    while(read(p, &x, 1) > 0){
        if(x % prime != 0) {
            write(q, &x, 1);
        }    
    }
}

void demo2(int p, int end)
{
    int prime;
    if(read(p, &prime, 1) <= 0) {
        close(p);
        return;
    }
    fprintf(1, "prime %d\n", prime);
    int q[2];
    pipe(q);
    int f = fork();
    if(f>0){
        close(q[0]);
        logic(p,q[1],prime,end);
        close(p);
        close(q[1]);
        f = wait((int *) 0);
        exit(0);
    }
    if(f==0){
        close(p);
        close(q[1]);
        demo2(q[0], end);
        close(q[0]);
        exit(0);
    }
}


void demo(int end)
{
    int p[2];
    pipe(p);
    int f = fork();
    if(f>0)
    {
        close(p[0]);
        logic(NA, p[1],0, end);
        close(p[1]);
        f = wait((int *) 0);
        exit(0);
    }
    if(f==0)
    {
        close(p[1]);
        demo2(p[0], end);
        close(p[0]);
        exit(0);
    }
}


int
main(int argc, char *argv[])
{
  demo(NUM);
  exit(0);
}
