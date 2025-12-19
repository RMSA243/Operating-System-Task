#include <stdio.h>
#include <stdlib.h>


#include <unistd.h>
#include <sys/wait.h>

#include <string.h>
#include <ctype.h>


int main(int argc, char *argv[]) 
{


    pid_t pid1;

    pid_t pid2;



    int status;

    pid1 = fork();

   	if (pid1 == -1) 
	{

        	perror("fork");
        	exit(1);

    	}
	else if (pid1 == 0) 
	{

		printf(" 1st child process is created and PID = %d\n", getpid());

	        printf("Original array: %s\n", argv[1]);

        	printf("Array size in bytes: %lu \n", strlen( argv[1] ) * sizeof( char ) );


        
       		char *ptr = argv[1];

       		while (*ptr != '\0') //to remove special characters from the array
		{

            		if ( isalpha(*ptr)||isdigit(*ptr) )
			{

                		++ptr;

            		}
			else 
			{

                		memmove( ptr,ptr + 1,strlen(ptr) );

            		}

        	}


        	printf("New updated array: %s\n", argv[1] );
       		printf("Updated array's size in bytes: %lu\n", sizeof( argv[1]) );


        	
       		char *args[] = {"./count", argv[1], NULL}; //to count array size
        	execv(args[0], args);

        	perror("exec error");
        	exit(1);

    	} 
	else 
	{

       		pid2 = fork();

        	if (pid2 == -1) 
		{
            		perror("fork");
           		exit(1);

        	}
		else if (pid2 == 0) 
		{

            		printf("2nd child process is created and PID = %d\n", getpid());

        	} 
		else 
		{


            		printf("Parent process waiting...\n");

           		waitpid(pid1, &status, 0);
            		waitpid(pid2, &status, 0);

            		printf("all 'child' processes are finished, parent process is exiting. Thank-You.\n");

        }


    return 0;
}
