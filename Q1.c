#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int fd[2]; // for pipe
    pid_t pid; // for process id

    char message[] = "Hi There";
    char buffer[100]; //for string transfer

    if (pipe(fd) < 0) // creating pipe
    {
       	perror("pipe not working");
        exit(1);
    }


    pid = fork(); // creating process



    if (pid < 0) // if fork failed
    {

        perror("unable to properly fork the commands");
        exit(1);

    }
    else if (pid == 0) // In child process
    {
        close(fd[1]); // closing unused write end of the pipe

        read(fd[0], buffer, sizeof(buffer)); // reading from the pipe

       
        for (int i = 0; buffer[i] != '\0'; i++)  // reverse case of each character in the message
        {

            if (buffer[i] >= 'a' && buffer[i] <= 'z')
            {
                buffer[i] -= 32;
            }
            else if (buffer[i] >= 'A' && buffer[i] <= 'Z')
            {

                buffer[i] += 32;

            }


        }


        printf("%s\n", buffer); // printing changed message


    }
    else // parent process
    {
        close(fd[0]); // closing unused read end

        write(fd[1], message, sizeof(message)); // write to pipe
    }

    return 0;


}
