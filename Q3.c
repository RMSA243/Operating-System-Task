#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) 
{
    int fd[2];
    pid_t pid;

    char buffer[BUFFER_SIZE];
    int bytes_read, bytes_written;

    int input_fd, output_fd;


    if (argc != 3)
    {
        printf("Please, send at least 2 files as parameters. \n\n");
        exit(1);


    }


    if (pipe(fd) < 0)
    {
        perror("error in pipe creation");
        exit(1);


    }


    pid = fork();

    if (pid < 0) 
    {
        perror("error in process creation");
        exit(1);

    }

    if (pid == 0) // In Child process
    {

        
        close(fd[1]);  // Closing the write end of the pipe

        output_fd = open(argv[2], O_WRONLY | O_CREAT );


        if (output_fd < 0)
	{
            perror("error in opening output file");
            exit(1);


        }

        while ((bytes_read = read(fd[0], buffer, BUFFER_SIZE)) > 0)
	{


            bytes_written = write(output_fd, buffer, bytes_read);


            if (bytes_written != bytes_read)
	    {

                perror("writing error in output file");
                exit(1);


            }



        }


        close(output_fd);
        exit(EXIT_SUCCESS);


    } 
    else // Parent process
    {
        
        close(fd[0]);  // Closing read end of the pipe

        input_fd = open(argv[1], O_RDONLY);

        if (input_fd == -1) 
	{
            perror("error in the opening input file");

            exit(EXIT_FAILURE);

        }
        while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) 
	{


            if (write(fd[1], &bytes_read, sizeof(int)) != sizeof(int))
	    {

                perror("write pipe size");
                exit(1); //exit failure


            }


            if (write(fd[1], buffer, bytes_read) != bytes_read)
	    {
                perror("error in writing pipe data");

                exit(1);

            }

        }


        close(input_fd); //file closing
        close(fd[1]); //write end closing


        wait(NULL);  // Waiting for the child process to finish
        exit(0);

    }



    return 0;

}
