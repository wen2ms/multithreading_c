#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int number = 0;

const int kMax = 50;

pthread_rwlock_t rwlock;

void* read_number(void* arg) {
    for (int i = 0; i < kMax; ++i) {
        pthread_rwlock_rdlock(&rwlock);

        printf("Read thread, id = %p, number = %d\n", pthread_self(), number);

        pthread_rwlock_unlock(&rwlock);

        usleep(rand() % 5);
    }

    return NULL;
}

void* write_number(void* arg) {
    for (int i = 0; i < kMax; ++i) {
        pthread_rwlock_wrlock(&rwlock);

        int cursor = number;

        ++cursor;

        usleep(10);

        number = cursor;

        printf("Write thread, id = %p, number = %d\n", pthread_self(), number);

        pthread_rwlock_unlock(&rwlock);
    }

    return NULL;
}

int main() {
    pthread_t read_threads[5], write_threads[3];
    pthread_rwlock_init(&rwlock, NULL);

    for (int i = 0; i < 5; ++i) {
        pthread_create(&read_threads[i], NULL, read_number, NULL);
    }

    for (int i = 0; i < 3; ++i) {
        pthread_create(&write_threads[i], NULL, write_number, NULL);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(read_threads[i], NULL);
    }

    for (int i = 0; i < 3; ++i) {
        pthread_join(write_threads[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    return 0;
}