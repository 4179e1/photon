#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void *child(void *arg)
{
    printf("child begin\n");
    pause();
    printf("child end\n");
    return NULL;
}

void sig_alarm(int signo)
{
    printf("alarm\n");
}


int main(int argc, char *argv[])
{
    signal(SIGALRM, sig_alarm);
    printf("parent: begin\n");
    pthread_t p;
    pthread_create(&p, NULL, child, NULL);
    sleep(1);
    kill (getpid(), SIGALRM);
    printf("alarm sent\n");
    sleep(1);
    printf("parent: end\n");
}