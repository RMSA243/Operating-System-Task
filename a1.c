#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) 
{

    if (argc != 2) 
    {
        printf("Please enter one integer value without any space.\n\n");

        exit(1);
    }


    int number = atoi(argv[1]); // converting to integer

    if (number < 6) 
    {
        printf("Please enter an integer which should be greater than or equal to 6. Your entered number is not a perfect number.\n\n");

        exit(1);

    }


    int f_sum1 = 0, f_sum2 = 0; // receiving sums

    int fd1[2], fd2[2]; //for two pipes

    pid_t pid1, pid2; // for process




    if ( pipe(fd1) < 0 || pipe(fd2) < 0 )   // creating pipes and checking if they are created correctly
    {

        perror("pipe not working");
        exit(1);

    }

    if ( (pid1 = fork() ) < 0 || ( pid2 = fork()) < 0) // creating child processes and checking if they are created correctly
    {  

        perror("unable to properly fork the commands");
        exit(1);

    }


    	if ( pid1 == 0 )   // child 1
	{

        	close(fd1[0]);  // close read end of pipe 1 which is not used

        	int sum1 = 0;

        	for (int i = 1; i <= number/4; i++) // from 1 to n/4 inclusive
		{

           		if (number % i == 0) 
			{

                		sum1 += i;

            		}

        	}


        	write(fd1[1], &sum1, sizeof(sum1));  // sending the result to parent process through write end of pipe 1

        	close(fd1[1]);  // closing write end of pipe 1

        	exit(0);


    	}
	else if (pid2 == 0) // 2nd child
	{  
        	close(fd2[0]);  // closing the read end of pipe 2 which is not used

        	int sum2 = 0;

        	for (int i = number/4 + 1; i <= number/2; i++) // from n/4+1 to n/2 inclusive
		{
            		if (number % i == 0) 
			{
                		sum2 += i;

            		}


        	}

        	write(fd2[1], &sum2, sizeof(sum2) );  // sending the result to parent process through write end of pipe 2

        	close(fd2[1]);  // closing write end of pipe 2
        	exit(0);


    	} 
	else // parent process
	{


        	close(fd1[1]);  // closing not used write end of pipe 1
        	close(fd2[1]);  // closing not used write end of pipe 2

        	wait(NULL);  // waiting for 1st child process to finish
        	wait(NULL);  // waiting for 2nd child process to finish

        	read( fd1[0] , &f_sum1, sizeof(f_sum1) );  // getting the result from 1st child process in f_sum1
        	read( fd2[0] , &f_sum2, sizeof(f_sum2 ) );  // getting the result from 2nd child process in f_sum2

        	close(fd1[0]);  // closing read end of pipe 1 after getting values
        	close(fd2[0]);  // closing read end of pipe 2 after getting values


        	int f_total_sum = f_sum1 + f_sum2;

        	if (f_total_sum == number) 
		{

            		printf("True, given number is a perfect number.\n\n\n");

        	}
		else 
		{
            		printf("False, given number is not a perfect number.\n\n\n");

        	}


    	}



    	return 0;

}
