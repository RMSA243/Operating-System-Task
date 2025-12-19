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

        perror("Sorry,invalid arguments. \n\n");
        exit(1);

    }


    FILE *file;

    sem_t *empty, *full, *mutex;

    key_t key;
    char *shm, buffer[shm_size];

    int shmid, count;

    



    if( (file = fopen(argv[1], "r") ) == NULL)
    {

        perror("Unable to open file. \n\n");
        exit(1);

    }


    if((key = ftok(".", 'a')) == -1)
    {

        perror("Sorry, token error.\n\n");
        exit(1);


    }



    if( (shmid = shmget(key, shm_size, IPC_CREAT|0666) ) == -1 ) 
    {

        perror("Sorry, shmget error. \n\n");
        exit(1);

    }



    if( (shm = shmat(shmid, NULL, 0) ) == (char* ) -1) 
    {

        perror("Sorry, shmat error. \n\n");
        exit(1);

    }



    if( (full =sem_open("full", O_CREAT,0644,0) ) == SEM_FAILED)
    {

        perror("Sorry, Sem_Open error 'full'. \n\n");
        exit(1);

    }


    if((empty = sem_open("empty", O_CREAT, 0644, 1)) == SEM_FAILED)
    {

        perror("Sorry, sem_open error 'empty'. \n\n");
        exit(EXIT_FAILURE);


    }


    if((mutex = sem_open("mutex", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }


    while(!feof(file))
    {

        sem_wait(empty);
        sem_wait(mutex);


        if((count = fread(buffer, sizeof(char), shm_size, file)) > 0)
  	{

            memcpy(shm, buffer, count);
            printf("producer : %s \n", buffer);

        }


        sem_post(mutex);
        sem_post(full);


        memset(buffer, 0, shm_size);

    }


    sem_wait(empty);
    sem_wait(mutex);


    *shm = '$';
    printf("Producer : Writing to shared memory is done. \n\n\n");


    sem_post(mutex);
    sem_post(full);


    fclose(file);
    shmdt(shm);


    shmctl(shmid,IPC_RMID, NULL);
    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutex");


    return 0;

}