#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#define BUFFER 10


int buffer[10];
int get_srand();

sem_t empty1,full1;
sem_t empty2,full2;
sem_t empty3,full3;

pthread_mutex_t mutex;

pthread_t arr[50];

int num_producer=1, num_customer=3;

int sum = 0;
int in=0;
int out1=0;
int out2=0;
int out3=0;
int get_srand(int range)

{
        int ans;
        srand((unsigned)time(NULL));
        ans = rand()%range+1;  //1~range
        return ans;
}
void*  producer(void* arg)

{
    int producer_number=1;
    while(1)
    {


        sem_wait(&empty1); //P
        sem_wait(&empty2); //P
        sem_wait(&empty3); //P
        pthread_mutex_lock(&mutex);

        buffer[in]=producer_number;
        printf("The producer is writing %d\n",producer_number);
        producer_number++;
        in=(in+1)%BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&full3);//V
        sem_post(&full2);//V
        sem_post(&full1);//V

    }
}
void* customer1(void* arg)

{
    while(1)
    {


        sem_wait(&full1);
        pthread_mutex_lock(&mutex);

        printf("The customer1 is reading %d\n",buffer[out1]);
        out1=(out1+1)%BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty1);//V

        sleep(get_srand(2));
    }
}
void* customer2(void* arg)

{
    while(1)
    {


        sem_wait(&full2);
        pthread_mutex_lock(&mutex);

        printf("The customer2 is reading %d\n",buffer[out2]);
        out2=(out2+1)%BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty2);//V

        sleep(get_srand(8));
    }
}
void* customer3(void* arg)

{
    while(1)
    {


        sem_wait(&full3);
        pthread_mutex_lock(&mutex);

        printf("The customer3 is reading %d\n",buffer[out3]);
        out3=(out3+1)%BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty3);//V

        sleep(get_srand(32));
    }
}
int main()
{


    pthread_mutex_init(&mutex, NULL);
    sem_init(&full1, 0, 0);
    sem_init(&empty1, 0, 10);
    sem_init(&full2, 0, 0);
    sem_init(&empty2, 0, 10);
    sem_init(&full3, 0, 0);
    sem_init(&empty3, 0, 10);
    pthread_create(&arr[0], NULL, producer, NULL);
    pthread_create(&arr[1], NULL, customer1, NULL);
    pthread_create(&arr[2], NULL, customer2, NULL);
    pthread_create(&arr[3], NULL, customer3, NULL);
    for(int i = 0; i < num_producer+num_customer; ++i)
    {
            pthread_join(arr[i], NULL);
    }
    return 0;
}

