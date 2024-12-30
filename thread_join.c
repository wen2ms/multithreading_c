#include <stdio.h>
#include <pthread.h>

struct Dog {
    char* breed;
    int age; 
};

void* callback_using_static(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("Subthread: %d\n", i + 1);
    }
    printf("Subthread: %p\n", pthread_self());

    static struct Dog dog = {"Labrado", 1};

    pthread_exit(&dog);

    return NULL;
}

void* callback_using_main(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("Subthread: %d\n", i + 1);
    }
    printf("Subthread: %p\n", pthread_self());

    struct Dog* dog = (struct Dog*)arg;
    dog->breed = "Dobermann";
    dog->age = 10;

    pthread_exit(dog);

    return NULL;
}

int main() {
    pthread_t tid;

    struct Dog dog;

    pthread_create(&tid, NULL, callback_using_main, &dog);
    // pthread_create(&tid, NULL, callback_using_static, NULL);

    void* ptr;
    pthread_join(tid, &ptr);

    for (int i = 0; i < 5; ++i) {
        printf("Main thread: %d\n", i + 1);
    }
    printf("Main thread: %p\n", pthread_self());

    struct Dog* dog_ptr = (struct Dog*)ptr;
    printf("breed: %s, age: %d\n", dog_ptr->breed, dog_ptr->age); 

    return 0;
}