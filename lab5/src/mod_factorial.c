#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

int evaluate_mod_factorial(int k, int mod, int threads_num); // Функция, организующая потоки для вычисления факториала
void mod_multiply(int** arguments); // Функция, выполняющая шаг вычисления факториала

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv)
{
    int mod = 0;
    int k = -1;
    int threads_num = 0;
     while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"threads_num", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            if (k < 0)
            {
                printf("k must be a positive number.\n");
                return 1;
            }
            break;

          case 1:
            mod = atoi(optarg);
            if (mod <= 0)
            {
                printf("mod must be a positive number.\n");
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

  if (k == -1 || mod == 0 || threads_num == 0) {
    printf("Usage: %s --k \"num\" --mod \"num\" --threads_num \"num\"\n",
           argv[0]);
    return 1;
  }

    int result = evaluate_mod_factorial(k, mod, threads_num);
    printf("Result: %d\n", result);

    return 0;
}

int evaluate_mod_factorial(int k, int mod, int threads_num) // Функция, организующая потоки для вычисления факториала
{
    if (k >= mod) // Если k >= mod, то k! % mod = (((((mod-1)! % mod) * (mod % mod)) % mod) * ... = 0, так как mod % mod == 0
    {
        return 0;
    }
    else
    {
        if (mod == 1) // Любое число % 1 = 0
        {
            return 0;
        }
        else
        {
            if ((k == 0) || (k == 1)) // 1 % mod = 1, где mod > 1
            {
                return 1;
            }
            else
            {
            int result = 1;
            int count = 2;
            int* arguments[3]; // Массив аргументов для функции mod_multply
            arguments[0] = &result;
            arguments[1] = &count;
            arguments[2] = &mod;
            int i;
            int end = ((k - 1) % threads_num) == 0 ? (k - 1) / threads_num : (k - 1) / threads_num + 1; // k - 1 нужное число итераций
            for (i = 0; i < end; i++) 
            {
                int needed_threads;
                pthread_t threads[threads_num];
                if ((i + 1) * threads_num <= (k - 1)) // Если число потоков таково, что на данном шаге они будут задействованы все
                {
                    needed_threads = threads_num;
                }
                else
                {
                    needed_threads = (k - 1) - (i * threads_num); // Число итераций - число, кратное количеству потоков, использованное на предыдущем шаге
                }
                int j;
                for (j = 0; j < needed_threads; j++)
                {
                    if (pthread_create(&threads[j], NULL, (void*)mod_multiply, (void*)arguments) != 0)
                    {
                        printf("Error with creating thread.\n");
                        exit(1);
                    }
                }
                for (j = 0; j < needed_threads; j++)
                {
                    if (pthread_join(threads[j], NULL) != 0)
                    {
                        printf("Error with joining to thread.\n");
                        exit(1);
                    }
                }
            }
            return result;
            }
        }
    }
}

void mod_multiply(int** arguments) // Функция, выполняющая шаг вычисления факториала
{
    pthread_mutex_lock(&mutex);
    *arguments[0] *= (*arguments[1] % *arguments[2]);   // result *= (count % mod)
    *arguments[0] %= *arguments[2];                     // result %= mod
    (*arguments[1])++;                                  // count++
    pthread_mutex_unlock(&mutex);
}