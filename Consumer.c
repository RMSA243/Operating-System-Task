#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<semaphore.h>


#define shm_size 20



int main(int argc, char* argv[])
{

    if(argc != 2) 
    {

        perror("Sorry, invalid arguments.\n\n");
        exit(1);

    }


    sem_t *empty, *full, *mutex;

    key_t key;
    char *shm, buffer[shm_size];

    int shmid, count;



    if((key = ftok(".", 'a')) == -1)
    {

        perror("Sorry, token error.\n\n");
        exit(1);


    }



    if( (shmid = shmget(key, shm_size, IPC_CREAT|0666) ) == -1 ) 
    {

        perror("Sorry, shmget error.\n\n");
        exit(1);

    }



    if( (shm = shmat(shmid, NULL, 0) ) == (char* ) -1) 
    {

        perror("Sorry, shmat error.\n\n");
        exit(1);

    }



    if( (full =sem_open("full", 0) ) == SEM_FAILED)
    {

        perror("Sorry, Sem_Open error 'full'.\n\n");
        exit(1);

    }


    if((empty = sem_open("empty", 0)) == SEM_FAILED)
    {

        perror("Sorry, sem_open error 'empty'.\n\n");
        exit(EXIT_FAILURE);


    }


    if((mutex = sem_open("mutex", 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }



    while(1)
    {

        sem_wait(full);
        sem_wait(mutex);


        memcpy(buffer, shm, shm_size);
        printf("Consumer : %s\n", buffer);


        if(*shm == '$') 
        {

            printf("Consumer : Reading from shared memory is done.\n\n\n");
            break;

        }


        sem_post(mutex);
        sem_post(empty);


        memset(buffer, 0, shm_size);

        getchar();

    }


    sem_post(mutex);//transfering control
    sem_post(empty);//transfering control


    shmdt(shm);


    sem_close(full);
    sem_close(empty);
    sem_close(mutex);


    return 0;

}
