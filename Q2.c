#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#define MAX_SIZE 100

int main()
{
    int arr[MAX_SIZE], n, i;
    int fd1[2], fd2[2]; // file descriptors for pipes
    pid_t pid; // process id
    int sum = 0;

    printf("Enter the size of the array: ");
    scanf("%d", &n);

    printf("Enter the elements of the array:\n");

    for(i=0;i<n;i++)
    {


        scanf("%d",&arr[i]);

    }



    if ( pipe(fd1) < 0 || pipe(fd2) < 0 )   // creating pipes and checking if they are created correctly
    {

        perror("pipe not working");

        exit(1);


    }


    pid = fork(); // create child process

    if (pid < 0) //if fork fail
    {

	perror("unable to properly fork the commands");
        exit(1);


    }
    else if (pid == 0) // child process
    {
        close(fd1[1]); // closing unused write end of pipe 1
        close(fd2[0]); // close unused read end of pipe 2

        int size = sizeof(int) * n;
        int arr_recv[n];

        read(fd1[0], arr_recv, size); // read from pipe 1

        
        for (i = 0; i < n; i++) // calculate sum of array
        {

            sum += arr_recv[i];


        }


        write(fd2[1], &sum, sizeof(sum)); // write sum to pipe 2
        close(fd2[1]); // close write end of pipe 2


        exit(0);


    }
    else //In parent process
    {
        close(fd1[0]); // close unused read end of pipe 1
        close(fd2[1]); // closing the write end of the pipe 2

        int size = sizeof(int) * n;

        write(fd1[1], arr, size); // write array to pipe 1

        int result;
        read(fd2[0], &result, sizeof(result)); // read sum from pipe 2



        printf("The sum of the array is %d\n", result);

        close(fd2[0]); // closing the read end of pipe 2


    }


    return 0;

}
