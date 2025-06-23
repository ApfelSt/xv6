#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int nextPrime(int p)
{
  int next = p + 1;
  while (1) {
    int isPrime = 1;
    for (int i = 2; i * i <= next; i++) {
      if (next % i == 0) {
        isPrime = 0;
        break;
      }
    }
    if (isPrime) {
      return next;
    }
    next++;
  }
}



void logic(int p, int q, int end, int n)
{
    if(p==-1){
        for(int i = 2; i < end; i++) {
            write(q, &i, 1);
        }
    }
    int x;
    while(read(p, &x, 1) > 0){
        if(x%n != 0){
            write(q, &x, 1);
        }
        if(x==n){
            fprintf(1, "prime %d\n", x, n);
        }
    }
}

void demo2(int p, int n, int end)
{
    if (n*n > end){
        while(read(p, &n, 1) > 0) {
            fprintf(1, "prime %d\n", n);
        }
        close(p);
        exit(0);
    }
    int q[2];
    pipe(q);
    int f = fork();
    if(f>0){
        close(q[0]);
        logic(p,q[1],end, n);
        close(p);
        close(q[1]);
        f = wait((int *) 0);
        exit(0);
    }
    if(f==0){
        close(p);
        close(q[1]);
        n = nextPrime(n);
        demo2(q[0], n, end);
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
        logic(-1, p[1], end, 0);
        close(p[1]);
        f = wait((int *) 0);
        exit(0);
    }
    if(f==0)
    {
        close(p[1]);
        demo2(p[0],2, end);
        close(p[0]);
        exit(0);
    }
}


int
main(int argc, char *argv[])
{
  //primes(35);
  demo(35);
  exit(0);
}
