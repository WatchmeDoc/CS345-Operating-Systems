/**
 * @file ask1.c
 * @author George Manos (csd4333@csd.uoc.gr)
 * @brief Narrow road assignment for cs-345 Operating Systems course
 * @date 2021-11-13
 *
 * 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#define MAX_CARS_ON_STREET 3
#define THREAD_NUM_POS 2
#define CAR_ID_INDEX 1
#define CAR_DIR_INDEX 0
#define CAR_DIR_STR(car_dir) car_dir == EAST ? "East" : "West"
#define EAST 0
#define WEST 1
#define CONSUME 2

sem_t sem[3];
pthread_mutex_t car_count_mutex;

unsigned int car_count = 0;
int *buffer[MAX_CARS_ON_STREET], side_count[2];

void *pass(void *ptr)
{ /* Producer: Depending on car's DIR(ection), it will wait or pass */
    int car_dir = ((int *)ptr)[CAR_DIR_INDEX];
    int car_id = ((int *)ptr)[CAR_ID_INDEX];
    sleep(1);
    printf("Car %d: Waiting to go to %s.\n", car_id, CAR_DIR_STR(car_dir));
    sem_wait(&sem[car_dir]);

    pthread_mutex_lock(&car_count_mutex);
    printf("Car %d: Passing to %s. Cars on the street: %d\n", car_id, CAR_DIR_STR(car_dir), car_count + 1);
    if (car_count < MAX_CARS_ON_STREET)
        buffer[car_count++] = (int *)ptr;
    else
    {
        printf("Street crashed.\n");
        exit(4);
    }
    assert(car_count <= side_count[car_dir]);
    if (car_count == MAX_CARS_ON_STREET || car_count == side_count[car_dir]) /* If its the 3rd car in a row or the queue is empty, they all pass together. */
        sem_post(&sem[CONSUME]);

    pthread_mutex_unlock(&car_count_mutex);

    return NULL;
}

void *consume(void *ptr)
{
    int car_dir;
    int car_id, i;
    while (side_count[EAST] != 0 || side_count[WEST] != 0)
    {
        sem_wait(&sem[CONSUME]);
        sleep(1);

        pthread_mutex_lock(&car_count_mutex);           /* Modify car_count variables etc */
        car_dir = buffer[car_count - 1][CAR_DIR_INDEX]; /* Check what way are the cars going */
        while (car_count > 0)
        {
            if (car_dir != buffer[--car_count][CAR_DIR_INDEX])
            { /* If opossite dir cars were detected, the street freezes. */
                printf("Opposite cars were about to cross together and got stuck. Street froze.\n");
                exit(5);
            }
            car_id = buffer[car_count][CAR_ID_INDEX];
            printf("Car %d: Reached to %s. Cars on the street: %d\n", car_id, CAR_DIR_STR(car_dir), car_count);
            side_count[car_dir]--;
            assert(side_count[car_dir] >= 0);
        }

        pthread_mutex_unlock(&car_count_mutex);
        if (side_count[1 - car_dir] == 0) /* Even though we are modifying side_count array, it */
            car_dir = 1 - car_dir;
        for (i = 0; i < side_count[1 - car_dir] && i < MAX_CARS_ON_STREET; i++)
            sem_post(&sem[1 - car_dir]);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t *th;
    int i, *a, threads, args[] = {0, 0, 0};
    /* Data init, mallocs etc */
    threads = atoi(argv[THREAD_NUM_POS]);
    srand(time(NULL));
    th = (pthread_t *)malloc((threads + 1) * sizeof(pthread_t));
    a = (int *)malloc(2 * threads * sizeof(int));

    /* Create producer threads */
    for (i = 0; i < threads; i++)
    {
        a[2 * i] = rand() % 2;
        a[2 * i + 1] = i;
        side_count[a[2 * i]]++;
        if (i == 0)
            args[a[0]] = MAX_CARS_ON_STREET; /* 1st Priority street. */

        if (pthread_create(&th[i], NULL, pass, &a[2 * i]) != 0)
        {
            perror("Failed to create thread.");
            exit(1);
        }
    }
    /* Create an extra thread (consumer) */
    if (pthread_create(&th[i], NULL, consume, NULL) != 0)
    {
        perror("Failed to create thread.");
        exit(1);
    }
    /* Init semaphores */
    for (i = 0; i < 3; i++)
    {
        if (sem_init(&sem[i], 0, args[i]) != 0)
        {
            perror("Failed to init semaphore.");
            exit(0);
        }
    }
    /* Init mutex */
    if (pthread_mutex_init(&car_count_mutex, NULL) != 0)
    {
        perror("Failed to init mutex.");
        exit(3);
    }
    /* For all threads, join! */
    for (i = 0; i < threads + 1; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread.");
            exit(2);
        }
    }
    /* Free/Destroy allocated memory */
    free(th);
    free(a);
    sem_destroy(&sem[EAST]);
    sem_destroy(&sem[WEST]);
    sem_destroy(&sem[CONSUME]);
    pthread_mutex_destroy(&car_count_mutex);
    return 0;
}
