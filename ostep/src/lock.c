#include <stdio.h>
#include <pthread.h>

pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;

void *child(void *arg)
{
    printf("child done\n");
    pthread_mutex_unlock (&l);
    return NULL;
}

int main (int argc, char *argv[]) {

    pthread_mutex_lock (&l);
    printf ("init\n");

    pthread_t p;
    pthread_create(&p, 0, child, 0);

    pthread_mutex_lock (&l);
    printf ("parent done?\n");
    return 0;
}