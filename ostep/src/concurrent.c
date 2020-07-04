#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static volatile int counter = 0;

void *mythread(void *arg)
{
    int i;
    int n = (int)arg;
    for (i = 0; i < n; i++)
    {
        counter += 1;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t p1, p2;
    int n = atoi(argv[1]);

    pthread_create(&p1, NULL, mythread, (void *)n);
    pthread_create(&p2, NULL, mythread, (void *)n);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Counter: %d\n", counter);
    return 0;
}
