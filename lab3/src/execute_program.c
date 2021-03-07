#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) 
{
    //argv[0] = "sequential_min_max";
    pid_t child_pid = fork();
    if (child_pid >= 0)
    {
        if (child_pid == 0)
        {
            // Child process
            int er = execvp("./sequential_min_max", argv);
            if (er == -1)
            {
                printf("Error with executing sequential_min_max\n");
            }
            return 0;
        }
    }
    else 
    {
        printf("Fork failed.\n");
    }
    return 0;
}