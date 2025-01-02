#include <stdio.h>
#include <pthread.h>

void* callback(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("Subthread: %d\n", i + 1);
    }
    printf("Subthread: %p\n", pthread_self());

    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, callback, NULL);

    pthread_detach(tid);

    for (int i = 0; i < 5; ++i) {
        printf("Main thread: %d\n", i + 1);
    }
    printf("Main thread: %p\n", pthread_self());

    pthread_exit(NULL);

    return 0;
}