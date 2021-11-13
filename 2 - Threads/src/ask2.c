#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#define THREAD_NUM_POS 2
#define MAX_CARS_ON_FERRY 20
#define MAX_WAITING_SECONDS 10
#define CAR_ID_INDEX 1
#define CAR_SLEEP_INDEX 0
#define TRIP_LENGTH 1

sem_t semEmpty, semFull;
int buffer[MAX_CARS_ON_FERRY];
int count = 0;
int total_cars;

pthread_mutex_t mutexBuffer;

void *producer(void *args)
{ /* Cars boarding on the ferry! */

    int x = ((int *)args)[CAR_SLEEP_INDEX], id = ((int *)args)[CAR_ID_INDEX];
    sleep(x);
    sem_wait(&semEmpty); /* 20 cars at a time are allowed to embark. */
    pthread_mutex_lock(&mutexBuffer);

    if (count < MAX_CARS_ON_FERRY && count < total_cars)
    { /* Unnecessary checking here since with our semaphore this if statement won't fail. */
        printf("Car %d embarking on the ferry. Total cars boarded: %d\n", id, count + 1);
        buffer[count++] = id;
    }
    else
    {
        printf("Error! More than %d cars attempted to embark on the ferry. Ferry sank!\n", MAX_CARS_ON_FERRY);
        printf("(Total cars attempted to embark: %d)\n", count);
        exit(4);
    }
    if (count == MAX_CARS_ON_FERRY || count == total_cars) /* If 20 cars were embarked or there aren't any other waiting cars left, the ferry departs. */
        sem_post(&semFull);
    pthread_mutex_unlock(&mutexBuffer);

    return NULL;
}

void *consumer(void *args)
{ /* Ferry step */
    int i;
    while (total_cars != 0)
    { /* While there are cars waiting, bring a ferry. */
        printf("The ferry is waiting for the cars to board.\n");
        sem_wait(&semFull); /* Wait for the cars to embark */
        printf("All on board. The ferry departs.\n");
        sleep(TRIP_LENGTH); /* Trip length :) */

        pthread_mutex_lock(&mutexBuffer);
        if (count > 0)
        { /* Cars must now disembark */
            total_cars -= count;
            while (--count > 0)
                printf("Car %d disembarking from the ferry.\n", buffer[count]);
            printf("Car %d disembarking from the ferry.\n", buffer[count]);
        }
        else
        { /* Probably won't happen but error checking :) */
            printf("Error: Empty ferry started!\n");
            exit(3);
        }
        for (i = 0; i < MAX_CARS_ON_FERRY && i < total_cars; i++) /* Bring 20 or less cars, depending on how many are left. */
            sem_post(&semEmpty);
        pthread_mutex_unlock(&mutexBuffer);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t *th;
    int i, threads, *a;
    /* Initializations */
    threads = atoi(argv[THREAD_NUM_POS]);
    th = (pthread_t *)malloc((threads + 1) * sizeof(pthread_t));
    a = (int *)malloc(2 * threads * sizeof(int));
    srand(time(NULL));
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, MAX_CARS_ON_FERRY);
    sem_init(&semFull, 0, 0);
    total_cars = threads;
    for (i = 0; i < threads; i++)
    { /* Create the required threads-cars */
        a[2 * i] = rand() % MAX_WAITING_SECONDS + 1;
        a[2 * i + 1] = i;
        if (pthread_create(&th[i], NULL, &producer, &a[2 * i]) != 0)
        {
            perror("Failed to create thread.");
            exit(0);
        }
    } /* Extra thread for the ferry */
    if (pthread_create(&th[i], NULL, &consumer, NULL) != 0)
    {
        perror("Failed to create thread.");
        exit(1);
    }
    for (i = 0; i < threads + 1; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread.");
            exit(2);
        }
    }
    free(th);
    pthread_mutex_destroy(&mutexBuffer);
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    return 0;
}