#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

void signal_handler(int signum)
{
    FILE* file;
    if ((file = fopen("pids.txt", "r")) != NULL)
    {
        int i, pnum, status;
        char buff[30];
        fgets(buff, 29, file);
        pnum = atoi(buff);
        for (i = 0; i < pnum; i++)
        {
            char buff[30];
            fgets(buff, 29, file);
            int p_id = atoi(buff);
            waitpid(p_id, &status, WNOHANG);
            if (WIFEXITED(status)) // Если процесс не завершился
            {
                kill(p_id, SIGKILL);
            }
        }
        fclose(file);
    }
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = 0;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if (!seed)
            {
                printf("Seed must be a positive number.\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (!array_size)
            {
                printf("Size of an array must be a positive number.\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (!pnum)
            {
                printf("pnum must be a positive number.\n");
                return 1;
            }
            int amount_of_children;
            if (array_size % 2 == 0)
            {
                amount_of_children = array_size / 2;
            }
            else
            {
                amount_of_children = (array_size / 2) + 1;
            }
            if (pnum > amount_of_children)
            {
                printf("Number of processes is too big. It must not exceed %d for array with size %d.\n", amount_of_children, array_size);
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          case 4:
            timeout = atoi(optarg);
            if (timeout <= 0)
            {
                printf("timeout must be positive number.\n");
                return 1;
            }
            break;

          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\"\n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  signal(SIGALRM, signal_handler);

  if (timeout)
  {
        alarm(timeout);
  }

  FILE* file; // For children' pids
    if ((file = fopen("pids.txt", "w")) != NULL)
    {
        char buff[30];
        sprintf(buff, "%d", pnum);
        fputs(buff, file);
        fputc('\n', file);
        fclose(file);
    }
    else
    {
        printf("Error while creating a file.\n");
        return 1;
    }

  int i;
  int** pipefd_min;
  int** pipefd_max;
  if (!with_files)
  {
      pipefd_min = (int**)malloc(sizeof(int*) * pnum);
      pipefd_max = (int**)malloc(sizeof(int*) * pnum);
  }
  for (i = 0; i < pnum; i++) {
    
    if(!with_files)
    {
        pipefd_min[i] = (int*)malloc(sizeof(int) * 2);
        pipefd_max[i] = (int*)malloc(sizeof(int) * 2);
        if (pipe(pipefd_min[i]) < 0)
        {
            printf("Error when creating a pipe.");
            return 1;
        }
        if (pipe(pipefd_max[i]) < 0)
        {
            printf("Error when creating a pipe.");
            return 1;
        }
    }
    int step;
    if (array_size > pnum)
    {
        step = array_size / pnum;
        if ((array_size % pnum != 0))
        {
        step += 1;
        }
    }
    else 
    {
        step = 2;
    }
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid > 0)
      {
          FILE* file;
          if ((file = fopen("pids.txt", "a")) != NULL)
            {
                char buff[30];
                sprintf(buff, "%d", child_pid);
                fputs(buff, file);
                fputc('\n', file);
                fclose(file);
            }
            else
            {
                printf("Error with writing pid to the file.\n");
                return 1;
            }
      }
      if (child_pid == 0) 
        {
            // child process

        // parallel somehow
            int j;
            int min = INT_MAX;
            int max = INT_MIN;
            int start;
            int end;
            if ((active_child_processes - 1) * step + step < array_size)
                {
                    end = (active_child_processes - 1) * step + step;
                }
                else 
                {
                    end = array_size;
                }
                start = (active_child_processes - 1) * step;
            for (j = start; j < end; j++)
            {
                if (array[j] < min)
                {
                    min = array[j];
                }
                if (array[j] > max)
                {
                    max = array[j];
                }
            }
            if (with_files) {
            // use files here
            FILE* file;
            if ((file = fopen("min_value.txt", "a")) != NULL)
            {
                char buff[30];
                sprintf(buff, "%d", min);
                fputs(buff, file);
                fputc('\n', file);
                fclose(file);
            }
            else
            {
                printf("Error with writing value to the file.\n");
                return 1;
            }
            if ((file = fopen("max_value.txt", "a")) != NULL)
            {
                char buff[30];
                sprintf(buff, "%d", max);
                fputs(buff, file);
                fputc('\n', file);
                fclose(file);
            }
            else
            {
                printf("Error with writing value to the file.\n");
                return 1;
            }
            } else {

                // use pipe here
                write(pipefd_min[i][1], &min, sizeof(int));
                write(pipefd_max[i][1], &max, sizeof(int));
            }
            return 0;
        }
    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  
  while (active_child_processes > 0) {
    // your code here
    waitpid(0, NULL, 0);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  FILE* min_file;
  FILE* max_file;

  if (with_files)
  {
    if ((min_file = fopen("min_value.txt", "r")) == NULL)
    {
        printf("Error with reading file.\n");
        return 1;
    }

    if ((max_file = fopen("max_value.txt", "r")) == NULL)
    {
        printf("Error with reading file.\n");
        return 1;
    }
  }

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
        // read from files
        char buff[30];
        fgets(buff, 29, min_file);
        int temp = atoi(buff);
        if (temp < min)
        {
            min = temp;
        }
        fgets(buff, 29, max_file);
        temp = atoi(buff);
        if (temp > max)
        {
            max = temp;
        }
    } else {
      // read from pipes
        int temp_min;
        int temp_max;
        read(pipefd_min[i][0], &temp_min, sizeof(int));
        read(pipefd_max[i][0], &temp_max, sizeof(int));
        if (temp_min < min)
        {
            min = temp_min;
        }
        if (temp_max > max)
        {
            max = temp_max;
        }
        close(pipefd_min[i][0]);
        close(pipefd_min[i][1]);
        free(pipefd_min[i]);
        close(pipefd_max[i][0]);
        close(pipefd_max[i][1]);
        free(pipefd_max[i]);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  if (with_files)
  {
    fclose(min_file);
    fclose(max_file);
  }
  else
  {
      free(pipefd_min);
      free(pipefd_max);
  }

  remove("min_value.txt");
  remove("max_value.txt");
  remove("pids.txt");

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}