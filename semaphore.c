#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dispatch/dispatch.h>
#include <unistd.h>

dispatch_semaphore_t semaphore_producer;
dispatch_semaphore_t semaphore_consumer;

struct Node {
    int number;
    struct Node* next;
};

struct Node* head = NULL;

void* produce(void* arg) {
    while (1) {
        dispatch_semaphore_wait(semaphore_producer, DISPATCH_TIME_FOREVER);

        struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

        new_node->number = rand() % 100;
        new_node->next = head;
        head = new_node;

        printf("Producer, id: %p, number: %d\n", pthread_self(), new_node->number);

        dispatch_semaphore_signal(semaphore_consumer);

        sleep(rand() % 3);
    }

    return NULL;
}

void* consume(void* arg) {
    while (1) {
        dispatch_semaphore_wait(semaphore_consumer, DISPATCH_TIME_FOREVER);

        struct Node* node = head;

        printf("Consumer, id: %p, number: %d\n", pthread_self(), node->number);

        head = head->next;
        free(node);

        dispatch_semaphore_signal(semaphore_producer);

        sleep(rand() % 3);
    }

    return NULL;
}

int main() {
    semaphore_producer = dispatch_semaphore_create(1);
    semaphore_consumer = dispatch_semaphore_create(0);

    pthread_t producers[5], consumers[5];

    for (int i = 0; i < 5; ++i) {
        pthread_create(&producers[i], NULL, produce, NULL);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_create(&consumers[i], NULL, consume, NULL);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    dispatch_release(semaphore_producer);
    dispatch_release(semaphore_consumer);

    return 0;
}