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

sem_t semEast, semWest, semConsume;
pthread_mutex_t car_count_mutex;

unsigned int car_count = 0;
int *buffer[MAX_CARS_ON_STREET], side_count[2];

void *pass_east(void *ptr)
{ /* Producer Type 1: */
    int car_id = ((int *)ptr)[CAR_ID_INDEX];
    sleep(1);
    printf("Car %d: Waiting to go to East.\n", car_id);
    sem_wait(&semEast);

    pthread_mutex_lock(&car_count_mutex);
    printf("Car %d: Passing to East. Cars on the street: %d\n", car_id, car_count + 1);
    if (car_count < MAX_CARS_ON_STREET)
        buffer[car_count++] = (int *)ptr;
    else
    {
        printf("Street crashed.\n");
        exit(4);
    }
    if (car_count == MAX_CARS_ON_STREET || car_count == side_count[EAST])
        sem_post(&semConsume);

    pthread_mutex_unlock(&car_count_mutex);

    return NULL;
}

void *pass_west(void *ptr)
{
    int car_id = ((int *)ptr)[CAR_ID_INDEX];
    sleep(1);
    printf("Car %d: Waiting to go to West.\n", car_id);
    sem_wait(&semWest);

    pthread_mutex_lock(&car_count_mutex);
    printf("Car %d: Passing to West. Cars on the street: %d\n", car_id, car_count + 1);
    if (car_count < MAX_CARS_ON_STREET)
        buffer[car_count++] = (int *)ptr;
    else
    {
        printf("Street crashed.\n");
        exit(4);
    }
    if (car_count == MAX_CARS_ON_STREET || car_count == side_count[WEST])
        sem_post(&semConsume);

    pthread_mutex_unlock(&car_count_mutex);

    return NULL;
}

void *consume(void *ptr)
{
    int car_dir;
    int car_id;
    while (side_count[EAST] != 0 && side_count[WEST] != 0)
    {
        sem_wait(&semConsume);
        sleep(1);
        pthread_mutex_lock(&car_count_mutex);
        car_dir = buffer[car_count - 1][CAR_DIR_INDEX];
        while (car_count != 0)
        {
            if (car_dir != buffer[--car_count][CAR_DIR_INDEX])
            {
                printf("Opposite cars were about to cross together and got stuck. Street froze.\n");
                exit(5);
            }
            car_id = buffer[car_count][CAR_ID_INDEX];
            printf("Car %d: Reached to %s. Cars on the street: %d\n", car_id, CAR_DIR_STR(car_dir), car_count);
            side_count[car_dir]--;
        }
        pthread_mutex_unlock(&car_count_mutex);
        if (car_dir == EAST)
        {
            sem_post(&semWest);
            sem_post(&semWest);
            sem_post(&semWest);
        }
        else
        {
            sem_post(&semEast);
            sem_post(&semEast);
            sem_post(&semEast);
        }
    }

    return NULL;
}

void *(*passing_func[])(void *ptr) = {pass_east, pass_west};

int main(int argc, char *argv[])
{
    pthread_t *th;
    int i, *a, threads;
    threads = atoi(argv[THREAD_NUM_POS]);
    srand(time(NULL));
    th = (pthread_t *)malloc((threads + 1) * sizeof(pthread_t));
    a = (int *)malloc(2 * threads * sizeof(int));

    if (sem_init(&semEast, 0, MAX_CARS_ON_STREET) != 0)
    {
        perror("Failed to init semaphore.");
        exit(0);
    }
    if (sem_init(&semWest, 0, 0) != 0)
    {
        perror("Failed to init semaphore.");
        exit(0);
    }
    if (sem_init(&semConsume, 0, 0) != 0)
    {
        perror("Failed to init semaphore.");
        exit(0);
    }
    if (pthread_mutex_init(&car_count_mutex, NULL) != 0)
    {
        perror("Failed to init mutex.");
        exit(3);
    }
    for (i = 0; i < threads; i++)
    {
        a[2 * i] = rand() % 2;
        a[2 * i + 1] = i;
        side_count[a[2 * i]]++;
        if (pthread_create(&th[i], NULL, passing_func[a[2 * i]], &a[2 * i]) != 0)
        {
            perror("Failed to create thread.");
            exit(1);
        }
    }
    if (pthread_create(&th[i], NULL, consume, NULL) != 0)
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
    free(a);
    sem_destroy(&semEast);
    sem_destroy(&semWest);
    sem_destroy(&semConsume);
    pthread_mutex_destroy(&car_count_mutex);
    return 0;
}
