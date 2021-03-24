#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
    int firstFork, secondFork;

    firstFork = fork();
    if (firstFork < 0)
    {
        printf("Error with first fork().\n");
        return 1;
    }
    if (firstFork == 0)
    {
        // First child process
        printf("This is first child. It will become a zombie in 2 seconds.\n");
        sleep(2);
        exit(EXIT_SUCCESS);
    }

    secondFork = fork();
    if (secondFork < 0)
    {
        printf("Error with second fork().\n");
        return 1;
    }
    if (secondFork == 0)
    {
        // Second child process
        printf("This is second child. It will become a zombie in 6 seconds.\n");
        sleep(6);
        exit(EXIT_SUCCESS);
    }

    // Main process
    sleep(4);
    int status;
    waitpid(firstFork, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("First child has exited. Main process gathered death certificate.\n");
        printf("Process #%d will not become a zombie with /etc/init as parent.\n", firstFork);
    }
    printf("Main process will end working without waiting for the second child.\n");
    printf("Process #%d will become a zombie with /etc/init as a parent process.\n", secondFork);
    return 0;
}