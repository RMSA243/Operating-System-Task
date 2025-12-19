#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#define FILE_SIZE 100

void* replace_integers_first_half(void* arg)
{

    int* map = (int*) arg;

    int i = 0;
    for (i = 0; i < FILE_SIZE / 2; i++)
    {

        if (isdigit((char) map[i]))
  	{
            map[i] = ' ';


        }

    }


    return NULL;
}

void* replace_integers_second_half(void* arg)
{

    int* map = (int*) arg;

    int i = 0;

    for (i = FILE_SIZE / 2; i < FILE_SIZE; i++)
    {
        if (isdigit((char) map[i]))
	{
            map[i] = ' ';

        }
    }

    return NULL;

}



int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        printf("Sorry, Invalid arguments.\n\n");
        return 1;

    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) 
    {

        perror("open");
        return 1;
    }



    int* map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) 
    {

        perror("mmap");
        return 1;

    }



    pthread_t threads[2];
    if (pthread_create(&threads[0], NULL, replace_integers_first_half, map) != 0)
    {


        perror("pthread_create");
        return 1;

    }


    if (pthread_create(&threads[1], NULL, replace_integers_second_half, map + (FILE_SIZE / 2)) != 0)
    {

        perror("Error: Pthread_create");
        return 1;

    }

    if (pthread_join(threads[0], NULL) != 0)
    {

        perror("pthread_join");
        return 1;

    }

    if (pthread_join(threads[1], NULL) != 0)
    {


        perror("pthread_join");
        return 1;

    }


    if (munmap(map, FILE_SIZE) != 0)
    {

        perror("munmap");
        exit(1);
    }

    close(fd);

    return 0;
}
