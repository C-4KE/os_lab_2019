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

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
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
                printf("Seed must be a positive number.");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (!array_size)
            {
                printf("Size of an array must be a positive number.");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (!pnum)
            {
                printf("pnum must be a positive number.");
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
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
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int i;
  int pipefd[2];
  if (pipe(pipefd) == -1)
  {
      printf("Error with creating pipe.\n");
      return 1;
  }
  pid_t main_pid;
  for (i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (active_child_processes == 1)
      {
          main_pid = child_pid;
      }
      if (child_pid == 0) {
        if (active_child_processes == 1)
        {
            // child process

        // parallel somehow
        int j;
        int min = INT_MAX;
        for (j = 0; j < array_size; j++)
        {
            if (array[j] < min)
            {
                min = array[j];
            }
        }
        if (with_files) {
          // use files here
          close(pipefd[0]);
          close(pipefd[1]);
          FILE* file;
          if ((file = fopen("min_value.txt", "w")) != NULL)
          {
            char buff[30];
            sprintf(buff, "%d", min);
            fputs(buff, file);
            fclose(file);
          }
          else
          {
              printf("Error with writing value to the file.\n");
              return 1;
          }
        } else {

            // use pipe here
            close(pipefd[0]);
            char buff[30];
            sprintf(buff, "%d", min);
            write(pipefd[1], buff, strlen(buff));
            close(pipefd[1]);
        }
        return 0;
        }
        else
        {
            close(pipefd[0]);
            close(pipefd[1]);
            return 0;
        }
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  bool is_max_found = false;
  int parent_max = INT_MIN;
  while (active_child_processes > 0) {
    // your code here
    if (!is_max_found)
    {
        int i;
        for (i = 0; i < array_size; i++)
        {
            if (array[i] > parent_max)
            {
                parent_max = array[i];
            }
        }
        is_max_found = true;
    }
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  //for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    //int max = INT_MIN;
    int max = parent_max;

    if (with_files) {
        // read from files
        waitpid(main_pid, NULL, 0);
        FILE* file;
        if ((file = fopen("min_value.txt", "r")) != NULL)
        {
            char buff[30];
            fgets(buff, 29, file);
            min = atoi(buff);
            fclose(file);
        }
        else
        {
            printf("Error with reading file.\n");
            return 1;
        }
    } else {
      // read from pipes
        char number[30];
        char buff;
        close(pipefd[1]);
        int i = 0;
        while(read(pipefd[0], &buff, 1) > 0)
        {
            number[i] = buff;
            i++;
        }
        min = atoi(number);
        close(pipefd[0]);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  //}

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
