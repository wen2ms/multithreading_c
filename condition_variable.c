#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

struct Node {
    int number;
    struct Node* next;
};

struct Node* head = NULL;

void* produce(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

        new_node->next = head;
        new_node->number = rand() % 1000;

        head = new_node;
        printf("Producer, id: %p, number: %d\n", pthread_self(), new_node->number);

        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);

        sleep(rand() % 3);
    }

    return NULL;
}

void* consume(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (head == NULL) {
            pthread_cond_wait(&cond, &mutex);
        }

        struct Node* node = head;

        printf("Consumer, id: %p, number: %d\n", pthread_self(), node->number);

        head = head->next;
        free(node);

        pthread_mutex_unlock(&mutex);
        
        sleep(rand() % 3);
    }

    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

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

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}