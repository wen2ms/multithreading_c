#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

const int kMax = 50;

int number = 0;

void* counter_a(void* arg) {
    for (int i = 0; i < kMax; ++i) {
        int cursor = number;

        ++cursor;
        usleep(10);

        number = cursor;

        printf("Thread A, id = %p, number = %d\n", pthread_self(), number);
    }
    
    return NULL;
}

void* counter_b(void* arg) {
    for (int i = 0; i < kMax; ++i) {
        int cursor = number;
        
        number = ++cursor;

        printf("Thread B, id = %p, number = %d\n", pthread_self(), number);
        
        usleep(5);
    }

    return NULL;
}

int main() {
    pthread_t thread_a, thread_b;

    pthread_create(&thread_a, NULL, counter_a, NULL);
    pthread_create(&thread_b, NULL, counter_b, NULL);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    return 0;
}

