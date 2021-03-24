#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>
#include <getopt.h>
#include <sys/time.h>

#include "find_min_max.h"
#include "utils.h"
#include "evaluate_sum.h"

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"threads_num", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0)
            {
                printf("Seed must be a positive number.\n");
                return 1;
            }
            break;

          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0)
            {
                printf("Size of an array must be a positive number.\n");
                return 1;
            }
            break;
            
          case 2:
            threads_num = atoi(optarg);
            if (threads_num <= 0)
            {
                printf("Number of threads must be a positive number.\n");
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
            if (threads_num > amount_of_children)
            {
                printf("Number of threads is too big. It must not exceed %d for array with size %d.\n", amount_of_children, array_size);
                return 1;
            }
            break;

          default:
            printf("Index %d is out of options\n", option_index);
        }
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

  if (seed == 0 || array_size == 0 || threads_num == 0) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --threads_num \"num\"\n",
           argv[0]);
    return 1;
  }

  pthread_t threads[threads_num];

  /*
   * TODO:
   * your code here
   * Generate array here
   */

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

  // Step to divide array
  int step;
    if (array_size > threads_num)
    {
        step = array_size / threads_num;
        if ((array_size % threads_num) != 0)
        {
            step++;
        }
    }
    else 
    {
        step = 2;
    }

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  struct SumArgs args[threads_num];
  for (uint32_t i = 0; i < threads_num; i++) {
      if (i * step + step < array_size)
                {
                    args[i].end = i * step + step;
                }
                else 
                {
                    args[i].end = array_size;
                }
                args[i].begin = i * step;
        args[i].array = array;
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n", elapsed_time);
  return 0;
}
