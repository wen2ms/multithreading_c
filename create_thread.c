#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* callback(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("Subthread: i = %d\n", i + 1);
    }
    printf("Subthread: %p\n", pthread_self());

    return NULL;
}

int main() {
    pthread_t tid;

    pthread_create(&tid, NULL, callback, NULL);

    for (int i = 0; i < 5; ++i) {
        printf("Main thread: i = %d\n", i + 1);
    }
    printf("Main thread: %p\n", pthread_self());

    sleep(1);

    return 0;
}