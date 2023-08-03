#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define ITERATIONS 5
#define LED_PIN 13

int sharedVariable = 0; 
pthread_mutex_t mutex;
sem_t semaphore;

void* workerThread(void* arg) {
    int threadId = *((int*)arg);

    for (int i = 0; i < ITERATIONS; ++i) {
       
        pthread_mutex_lock(&mutex);
        sharedVariable++; 
        printf("Wątek %d: Wartość wspólnej zmiennej: %d\n", threadId, sharedVariable);
        pthread_mutex_unlock(&mutex);

       
        digitalWrite(LED_PIN, HIGH);
        delay(200);

        digitalWrite(LED_PIN, LOW);
        delay(200);

       
        sem_wait(&semaphore);
        printf("Wątek %d: Zapis do współdzielonego pliku\n", threadId);
        sem_post(&semaphore);
    }

    return NULL;
}

int main() {
 
    pinMode(LED_PIN, OUTPUT);

    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semaphore, 0, 1); 

    
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, workerThread, (void*)&threadIds[i]);
    }

  
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

   
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);

    return 0;
}
