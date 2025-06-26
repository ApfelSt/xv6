#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define CHAR 'a'

void pingpong()
{
    int p[2], c[2];
    pipe(p);
    pipe(c);
    int f = fork();

    if (f == 0)
    {
        close(p[1]);
        close(c[0]);
        char buf[1];
        read(p[0], buf, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(c[1], buf, 1);
        close(p[0]);
        close(c[1]);
        exit(0);
    }
    else
    {
        close(p[0]);
        close(c[1]);
        char buf[1] = {CHAR};
        write(p[1], buf, 1);
        read(c[0], buf, 1);
        fprintf(1, "%d: received pong\n", getpid());
        close(p[1]);
        close(c[0]);
        wait(0);
    }
}

int
main(int argc, char *argv[])
{
    pingpong();
  exit(0);
}
