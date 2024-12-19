#ifndef MAINFUNCTION_H
#define MAINFUNCTION_H

#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include "confirmation.h"
#include <string.h>

sem_t rw_mutex[3][624];
sem_t mutex[3][624];
sem_t write_mutex[3];
int read_count[3][624] = {0};

int pipefds[100][10][2];
int pipes[100][2];
int pipescore[100][1000][2];
int pipeitem[100][1000][2];
int pipe1[100][2];
int pipe2[100][2];
int pipe3[100][2];
int pipe4[100][2];
int pipe5[100][2];
int pipe6[100][2];
int pipe7[100][2];
int pipe8[100][2];
int pipe9[100][2];
void format_goods_list(char **goods_list, int num_goods, char *formatted_output, int output_size) {
    // Ensure the buffer is empty
    formatted_output[0] = '\0';

    // Iterate through the goods list and concatenate them
    for (int i = 0; i < num_goods; i++) {
        // Check for buffer overflow before appending
        if (strlen(formatted_output) + strlen(goods_list[i]) + 2 > output_size) {
           
            break;
        }

        strcat(formatted_output, goods_list[i]);  // Append the current good

        // Append a comma if it's not the last element
        if (i < num_goods - 1) {
            strcat(formatted_output, ",");
        }
    }
}

struct ThreadArgs
{
    int input1;
    char category[100];
    char itemName[50];
    int num;
    int store;
    int user;
    int n;
};
struct Threadorderargs{
int user;char **items; int *numitems;int n;int treshhold; int is_repeated; char *user_name;
};

struct ThreadReturn
{
    int foundItem;

    double score;
};

void updateFile(const char *fileName, int newEntity, char *name, double price, double score)
{
    FILE *file = fopen(fileName, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char lastModified[30];
    snprintf(lastModified, sizeof(lastModified), "%04d-%02d-%02d %02d:%02d:%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);

    fprintf(file, "Name: %s\n", name);
    fprintf(file, "Price: %.2f\n", price);
    fprintf(file, "Score: %.1f\n", score);
    fprintf(file, "Entity: %d\n", newEntity);
    fprintf(file, "\nLast Modified: %s\n", lastModified);

    fclose(file);

    printf("File updated successfully.\n");
}

double string_to_double(const char *str)
{
    double result = 0.0;
    double fraction = 0.0;
    int is_negative = 0;
    int divisor = 1;

    if (*str == '-')
    {
        is_negative = 1;
        str++;
    }

    while (*str && isdigit(*str))
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    if (*str == '.')
    {
        str++;
        while (*str && isdigit(*str))
        {
            fraction = fraction * 10 + (*str - '0');
            divisor *= 10;
            str++;
        }
        result += fraction / divisor;
    }

    if (is_negative)
    {
        result = -result;
    }

    return result;
}

int starts_with_ignore_case(const char *str1, const char *str2)
{
    size_t len2 = strlen(str2);

    if (strlen(str1) < len2)
    {
        return 0;
    }

    for (size_t i = 0; i < len2; i++)
    {
        if (str1[i] != str2[i])
        {
            return 0;
        }
    }

    return 1;
}

void extract_substring_with_length(const char *source, char *destination, int start, int length)
{
    strncpy(destination, source + start, length);
    destination[length] = '\0';
}
void *thread_function(void *arg)
{
    struct ThreadArgs *data = (struct ThreadArgs *)arg;

    struct ThreadReturn res;
    int num = data->input1;
//printf(" PID: %d create thred  PID: %d\n",  getpid(),pthread_self());
sem_wait(&write_mutex[data->store-1]);
    char logfilename[100];
    strcpy(logfilename, data->category);
    char userlog[100];
    sprintf(userlog, "USER%d_order.log", data->user);
    strcat(logfilename, userlog);

    FILE *filelog = fopen(logfilename, "a");
    fprintf(filelog, "thread  %d read file %d.txt proccess:%d  \n", data->input1, data->input1, getpid());
    fclose(filelog);
sem_post(&write_mutex[data->store-1]);
    char fileNum[10];

    sprintf(fileNum, "%d", num);

    char *fileName, score[10], entity[10], price[10];char name[20];
    char *endptr1;
    char *endptr2;
    char *endptr3;

    double scorenum, entitymum, pricenum;

    fileName = data->category;

    strcat(fileName, fileNum);

    char *temp = ".txt";
    strcat(fileName, temp);
    char line1[100];
    char line2[100];
    char line3[100];
    char line4[100];
    char line5[100];
    char line6[100];

    sem_wait(&(mutex[data->store][data->input1]));
    read_count[data->store][data->input1]++;
    if (read_count[data->store][data->input1] == 1)
    {
        sem_wait(&rw_mutex[data->store][data->input1]);
    }
    sem_post(&mutex[data->store][data->input1]);
    // reading
    FILE *file = fopen(fileName, "r");
    fgets(line1, sizeof(line1), file);
    fgets(line2, sizeof(line1), file);
    fgets(line3, sizeof(line1), file);
    fgets(line4, sizeof(line1), file);
    fgets(line5, sizeof(line1), file);
    fgets(line6, sizeof(line1), file);
    fclose(file);
    // end of reading
    sem_wait(&mutex[data->store][data->input1]);
    read_count[data->store][data->input1]--;
    if (read_count[data->store][data->input1] == 0)
    {
        sem_post(&rw_mutex[data->store][data->input1]);
    }
    sem_post(&mutex[data->store][data->input1]);
    extract_substring_with_length(line1,name,6,strlen(line1)-6);
    extract_substring_with_length(line2, price, 7, 6);
    extract_substring_with_length(line3, score, 7, 3);
    extract_substring_with_length(line4, entity, 8, 2);

    pricenum = string_to_double(price);
    scorenum = string_to_double(score);
    entitymum = string_to_double(entity);

    char *tempchar = data->itemName;
    int n = strlen(data->itemName) + 7;
    if (strstr(line1, data->itemName) == NULL || strlen(line1) != n)
   {
       
        res.foundItem = 0;
    }
    else
    {
          sem_wait(&write_mutex[data->store-1]);
    FILE  *filelog = fopen(logfilename, "a"); 
    fprintf(filelog, "thread  %d found truefile proccess:%d  \n",data->input1,getpid());
    fclose(filelog);
    sem_post(&write_mutex[data->store-1]);
        res.foundItem = 1;
    }
    if (entitymum < data->num)
    {
        res.foundItem = 0;
    }

    if (res.foundItem == 1)
    {
        double sc = scorenum * 10.1 / pricenum;
        res.score = sc;
    }

    /////////////////////////////////////////////////////////////
    // send to front
    if (res.foundItem == 1)
    {
        if (data->store == 0)
        {
            char buff[50];

            sprintf(buff, "%.5f", res.score);
            write(pipefds[data->user][0][1], buff, 20);
            write(pipefds[data->user][1][1], score, 10);
            write(pipefds[data->user][2][1], price, 10);
        }
        if (data->store == 1)
        {
            char buff[50];

            sprintf(buff, "%.5f", res.score);
            write(pipefds[data->user][3][1], buff, 20);
            write(pipefds[data->user][4][1], score, 10);
            write(pipefds[data->user][5][1], price, 10);
        }
        if (data->store == 2)
        {
            char buff[50];

            sprintf(buff, "%.5f", res.score);
            write(pipefds[data->user][6][1], buff, 20);
            write(pipefds[data->user][7][1], score, 10);
            write(pipefds[data->user][8][1], price, 10);
        }

        char whichstore[50];
        sprintf(whichstore, "%d", data->input1);
        write(pipefds[data->user][9][1], whichstore, sizeof(whichstore));


        char check[2];
        read(pipes[data->user][0], check, sizeof(check));

        write(pipes[data->user][1], check, sizeof(check));
       if (data->store + 1 == atoi(check))
        {
             FILE *file = fopen(fileName, "r");
    fgets(line1, sizeof(line1), file);
    fgets(line2, sizeof(line1), file);
    fgets(line3, sizeof(line1), file);
    fclose(file);
    extract_substring_with_length(line3, score, 7, 3);
    scorenum = string_to_double(score);
 
            int newEntity = (int)entitymum - data->num;
            updateFile(fileName, newEntity, data->itemName, pricenum, scorenum);
        }

        
        sem_post(&rw_mutex[data->store][data->input1]);
    }

    return 0;
}

void *odrder_thread(void *arg){
    struct Threadorderargs *data = (struct Threadorderargs *)arg;
    int user=data->user; char **items =data->items; int *numitems=data->numitems; int n= data->n; int treshhold=data->treshhold;int is_repeated=data->treshhold; char *user_name=data->user_name;
 float off = 1.0;
    if (is_repeated == 1)
    {
        off = 0.5;
    }
     for (int i = 0; i < n; i++)
    {
        printf("Item[%d]: %s, Count: %d\n", i, items[i], numitems[i]);
    }

    
    double scores[3] = {0};
    double prices[3] = {0};
    double scoreitems[3] = {0};
    int mypathes257[n];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 624; j++)
        {

            sem_init(&rw_mutex[i][j], 0, 1);
            sem_init(&mutex[i][j], 0, 1);
        }
    }
    if (pipe(pipes[user]) == -1)
    {
        perror("Pipe creation failed");
        // exit(EXIT_FAILURE);
    }


    for (int k = 0; k < n; k++)
    {
      
        pid_t pid1, pid2, pid3;
        char *item = items[k];
        int number = numitems[k];

        for (int i = 0; i < 10; i++)
        {
            if (pipe(pipefds[user][i]) == -1)
            {
                perror("Pipe creation failed");
                // exit(EXIT_FAILURE);
            }
            int flags = fcntl(pipefds[user][i][0], F_GETFL, 0);
            fcntl(pipefds[user][i][0], F_SETFL, flags | O_NONBLOCK);
        }

        for (int i = 0; i < 654; i++)
        {
            if (pipe(pipeitem[user][i]) == -1)
            {
                perror("Pipe creation failed");
            }
            int flags = fcntl(pipeitem[user][i][0], F_GETFL, 0);
            fcntl(pipeitem[user][i][0], F_SETFL, flags | O_NONBLOCK);

            if (pipe(pipescore[user][i]) == -1)
            {
                perror("Pipe creation failed");
            }
            flags = fcntl(pipescore[user][i][0], F_GETFL, 0);
            fcntl(pipescore[user][i][0], F_SETFL, flags | O_NONBLOCK);
        }
        char buffer[100];
        pid1 = fork();
        if (pid1 == 0)
        {
            int s = 0;
              printf("I am the first child for store1. My PID: %d, Parent PID: %d\n", getpid(), getppid());
            pid_t p1, p2, p3, p4, p5, p6, p7, p8;
            p1 = fork();
            if (p1 == 0)
            {
               printf(" PID: %d create child for Apparel PID: %d\n",  getppid(),getpid());
                pthread_t threads[79];
                int thread_args[79];
                struct ThreadArgs args[79];
                int i;

                for (i = 0; i < 79; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Apparel/");
                    strcpy(args[i].itemName, item);
                    args[i].n = n;
                    args[i].num = number;
                    args[i].input1 = 160 + i + 1;
                    args[i].store = 0;
                    args[i].user = user;
                }

                for (i = 0; i < 79; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 79; i++)
                {
                   
                    pthread_join(threads[i], NULL);
                }

                /////////////////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p2 = fork();
            if (p2 == 0)
            {
                printf(" PID: %d create child for Beauty PID: %d\n",  getppid(),getpid());

                pthread_t threads[72];
                int thread_args[72];
                struct ThreadArgs args[72];
                int i;

                for (i = 0; i < 72; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Beauty/");
                    strcpy(args[i].itemName, item);
                    args[i].n = n;
                    args[i].num = number;
                    args[i].input1 = 503 + i + 1;
                    args[i].store = 0;
                    args[i].user = user;
                }

                for (i = 0; i < 72; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 72; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                ////////////////////////////////////////////////////////////////////////

                return 0;
            }
            p3 = fork();
            if (p3 == 0)
            {
                printf(" PID: %d create child for Digital PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Digital/");
                    strcpy(args[i].itemName, item);
                    args[i].n = n;
                    args[i].num = number;
                    args[i].input1 = i + 1;
                    args[i].store = 0;
                    args[i].user = user;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                /////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p4 = fork();
            if (p4 == 0)
            {
               printf(" PID: %d create child for Food PID: %d\n",  getppid(),getpid());
                pthread_t threads[92];
                int thread_args[92];
                struct ThreadArgs args[92];
                int i;

                for (i = 0; i < 92; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Food/");
                    strcpy(args[i].itemName, item);
                    args[i].n = n;
                    args[i].num = number;
                    args[i].input1 = 239 + i + 1;
                    args[i].store = 0;
                    args[i].user = user;
                }

                for (i = 0; i < 92; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 92; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ////////////////////////////////////////////////////////////////
                return 0;
            }
            p5 = fork();
            if (p5 == 0)
            {
                printf(" PID: %d create child for Home PID: %d\n",  getppid(),getpid());
                pthread_t threads[82];
                int thread_args[82];
                struct ThreadArgs args[82];
                int i;

                for (i = 0; i < 82; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Home/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 78 + i + 1;
                    args[i].store = 0;
                    args[i].n = n;
                }

                for (i = 0; i < 82; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 82; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                /////////////////////////////////////////////////////////////////
                return 0;
            }
            p6 = fork();
            if (p6 == 0)
            {
                printf(" PID: %d create child for Market PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Market/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 331 + i + 1;
                    args[i].store = 0;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ////////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p7 = fork();
            if (p7 == 0)
            {
                printf(" PID: %d create child for Sports PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Sports/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].store = 0;
                    args[i].input1 = 575 + i + 1;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                
                return 0;
            }
            p8 = fork();
            if (p8 == 0)
            {
                printf(" PID: %d create child for Toys PID: %d\n",  getppid(),getpid());
                pthread_t threads[79];
                int thread_args[79];
                struct ThreadArgs args[79];
                int i;

                for (i = 0; i < 79; i++)
                {
                    strcpy(args[i].category, "Dataset/Store1/Toys/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 424 + i + 1;
                    args[i].store = 0;
                    args[i].n = n;
                }

                for (i = 0; i < 79; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 79; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ///////////////////////////////////////////////////////
                return 0;
            }

          

            return 0;
        }

        pid2 = fork();
        if (pid2 == 0)
        {    
               printf("I am the second child for store2. My PID: %d, Parent PID: %d\n", getpid(), getppid());

            pid_t p1, p2, p3, p4, p5, p6, p7, p8;
            p1 = fork();
            if (p1 == 0)
            {
                printf(" PID: %d create child for Apparel PID: %d\n",  getppid(),getpid());
                pthread_t threads[79];
                int thread_args[79];
                struct ThreadArgs args[79];
                int i;

                for (i = 0; i < 79; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Apparel/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 160 + i + 1;
                    args[i].store = 1;
                    args[i].n = n;
                }

                for (i = 0; i < 79; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 79; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                /////////////////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p2 = fork();
            if (p2 == 0)
            {
                
                printf(" PID: %d create child for Beauty PID: %d\n",  getppid(),getpid());
                pthread_t threads[72];
                int thread_args[72];
                struct ThreadArgs args[72];
                int i;

                for (i = 0; i < 72; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Beauty/");
                    strcpy(args[i].itemName, item);

                    args[i].num = number;
                    args[i].input1 = 503 + i + 1;
                    args[i].store = 1;
                    args[i].user = user;
                    args[i].n = n;
                }

                for (i = 0; i < 72; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 72; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                ////////////////////////////////////////////////////////////////////////

                return 0;
            }
            p3 = fork();
            if (p3 == 0)
            {
                printf(" PID: %d create child for Digital PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Digital/");
                    strcpy(args[i].itemName, item);

                    args[i].num = number;
                    args[i].input1 = i + 1;
                    args[i].store = 1;
                    args[i].user = user;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                /////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p4 = fork();
            if (p4 == 0)
            {
                printf(" PID: %d create child for Food PID: %d\n",  getppid(),getpid());
                pthread_t threads[92];
                int thread_args[92];
                struct ThreadArgs args[92];
                int i;

                for (i = 0; i < 92; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Food/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 239 + i + 1;
                    args[i].store = 1;
                    args[i].n = n;
                }

                for (i = 0; i < 92; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 92; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ////////////////////////////////////////////////////////////////
                return 0;
            }
            p5 = fork();
            if (p5 == 0)
            {
                printf(" PID: %d create child for Home PID: %d\n",  getppid(),getpid());
                pthread_t threads[82];
                int thread_args[82];
                struct ThreadArgs args[82];
                int i;

                for (i = 0; i < 82; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Home/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 78 + i + 1;
                    args[i].store = 1;
                    args[i].n = n;
                }

                for (i = 0; i < 82; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 82; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                
                return 0;
            }
            p6 = fork();
            if (p6 == 0)
            {
                printf(" PID: %d create child for Market PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Market/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 331 + i + 1;
                    args[i].store = 1;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                
                return 0;
            }
            p7 = fork();
            if (p7 == 0)
            {
                printf(" PID: %d create child for Sports PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Sports/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 575 + i + 1;
                    args[i].store = 1;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                
                return 0;
            }
            p8 = fork();
            if (p8 == 0)
            {
                printf(" PID: %d create child for Toys PID: %d\n",  getppid(),getpid());
                pthread_t threads[79];
                int thread_args[79];
                struct ThreadArgs args[79];
                int i;

                for (i = 0; i < 79; i++)
                {
                    strcpy(args[i].category, "Dataset/Store2/Toys/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 424 + i + 1;
                    args[i].store = 1;
                    args[i].n = n;
                }

                for (i = 0; i < 79; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 79; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ///////////////////////////////////////////////////////
                return 0;
            }

         
            return 0;
        }

        pid3 = fork();
        if (pid3 == 0)
        {
             printf("I am the third child for store3. My PID: %d, Parent PID: %d\n", getpid(), getppid());
            pid_t p1, p2, p3, p4, p5, p6, p7, p8;
            p1 = fork();
            if (p1 == 0)
            {
                printf(" PID: %d create child for Apparel PID: %d\n",  getppid(),getpid());

                pthread_t threads[79];
                int thread_args[79];
                struct ThreadArgs args[79];
                int i;

                for (i = 0; i < 79; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Apparel/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 160 + i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 79; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 79; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                /////////////////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p2 = fork();
            if (p2 == 0)
            {
                 printf(" PID: %d create child for Beauty PID: %d\n",  getppid(),getpid());

                pthread_t threads[72];
                int thread_args[72];
                struct ThreadArgs args[72];
                int i;

                for (i = 0; i < 72; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Beauty/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 503 + i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 72; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 72; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                ////////////////////////////////////////////////////////////////////////

                return 0;
            }
            p3 = fork();
            if (p3 == 0)
            {
                 printf(" PID: %d create child for Digital PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Digital/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                /////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p4 = fork();
            if (p4 == 0)
            {
                 printf(" PID: %d create child for Food PID: %d\n",  getppid(),getpid());
                pthread_t threads[92];
                int thread_args[92];
                struct ThreadArgs args[92];
                int i;

                for (i = 0; i < 92; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Food/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 239 + i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 92; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 92; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ////////////////////////////////////////////////////////////////
                return 0;
            }
            p5 = fork();
            if (p5 == 0)
            {
                 printf(" PID: %d create child for Home PID: %d\n",  getppid(),getpid());
                pthread_t threads[82];
                int thread_args[82];
                struct ThreadArgs args[82];
                int i;

                for (i = 0; i < 82; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Home/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 78 + i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 82; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 82; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                /////////////////////////////////////////////////////////////////
                return 0;
            }
            p6 = fork();
            if (p6 == 0)
            {
                 printf(" PID: %d create child for Market PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Market/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 331 + i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ////////////////////////////////////////////////////////////////////////////
                return 0;
            }
            p7 = fork();
            if (p7 == 0)
            {
                printf(" PID: %d create child for Sports PID: %d\n",  getppid(),getpid());
                pthread_t threads[78];
                int thread_args[78];
                struct ThreadArgs args[78];
                int i;

                for (i = 0; i < 78; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Sports/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 575 + i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 78; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 78; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ///////////////////////////////////////////////////////////////
                return 0;
            }
            p8 = fork();
            if (p8 == 0)
            {
                printf(" PID: %d create child for Toys PID: %d\n",  getppid(),getpid());
                pthread_t threads[79];
                int thread_args[79];
                struct ThreadArgs args[79];
                int i;

                for (i = 0; i < 79; i++)
                {
                    strcpy(args[i].category, "Dataset/Store3/Toys/");
                    strcpy(args[i].itemName, item);
                    args[i].user = user;
                    args[i].num = number;
                    args[i].input1 = 424 + i + 1;
                    args[i].store = 2;
                    args[i].n = n;
                }

                for (i = 0; i < 79; i++)
                {
                    thread_args[i] = i + 1;
                    if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0)
                    {
                        perror("Failed to create thread");
                        return 0;
                    }
                }

                for (i = 0; i < 79; i++)
                {
                    pthread_join(threads[i], NULL);
                }
                ///////////////////////////////////////////////////////
                return 0;
            }

           

            return 0;
        }
        double price[3];
        double score[3];
        double scoreitem[3];
        int notFound[3];
        struct pollfd pfds[3];
        pfds[0].fd = pipefds[user][2][0];
        pfds[0].events = POLLIN;
        pfds[1].fd = pipefds[user][5][0];
        pfds[1].events = POLLIN;
        pfds[2].fd = pipefds[user][8][0];
        pfds[2].events = POLLIN;

        struct pollfd pfd = pfds[0];
        int pollResult = poll(&pfd, 1, 1000);
        if (pollResult > 0)
        {
            read(pipefds[user][0][0], buffer, sizeof(buffer));
            score[0] = string_to_double(buffer);
            read(pipefds[user][1][0], buffer, sizeof(buffer));
            scoreitem[0] = string_to_double(buffer);
            read(pipefds[user][2][0], buffer, sizeof(buffer));
            price[0] = string_to_double(buffer);
        }
        else
        {
            score[0] = -1000;
        }

        pfd = pfds[1];
        pollResult = poll(&pfd, 1, 1000);
        if (pollResult > 0)
        {
            read(pipefds[user][3][0], buffer, sizeof(buffer));
            score[1] = string_to_double(buffer);
            read(pipefds[user][4][0], buffer, sizeof(buffer));
            scoreitem[1] = string_to_double(buffer);
            read(pipefds[user][5][0], buffer, sizeof(buffer));
            price[1] = string_to_double(buffer);
        }
        else
        {
            score[1] = -1000;
        }

        pfd = pfds[2];
        pollResult = poll(&pfd, 1, 1000);
        if (pollResult > 0)
        {
            read(pipefds[user][6][0], buffer, sizeof(buffer));
            score[2] = string_to_double(buffer);
            read(pipefds[user][7][0], buffer, sizeof(buffer));
            scoreitem[2] = string_to_double(buffer);
            read(pipefds[user][8][0], buffer, sizeof(buffer));
            price[2] = string_to_double(buffer);
        }
        else
        {
            score[2] = -1000;
        }
        scores[0] = scores[0] + score[0];
        scores[1] = scores[1] + score[1];
        scores[2] = scores[2] + score[2];
        prices[0] = prices[0] + price[0] * numitems[k] * off;
        prices[1] = prices[1] + price[1] * numitems[k]* off;
        prices[2] = prices[2] + price[2] * numitems[k]* off;
        if (prices[0] > treshhold)
        {
            scores[0] = -1000;
            
        }

        if (prices[1] > treshhold)
        {
            scores[1] = -1000;
            
        }
        if (prices[2] > treshhold)
        {
            scores[2] = -1000;
            
        }
        scoreitems[0] = scoreitems[0] + scoreitem[0];
        scoreitems[1] = scoreitems[0] + scoreitem[1];
        scoreitems[2] = scoreitems[0] + scoreitem[2];

        char pathitemm[50];
        read(pipefds[user][9][0], pathitemm, sizeof(pathitemm));
        mypathes257[k] = atoi(pathitemm);

        for (int i = 0; i < 9; i++)
        {

            close(pipefds[user][i][0]);
            close(pipefds[user][i][1]);
        }
    }

    double max;
    int maxId;
    max = -1;
    maxId = 0;

    for (int i = 0; i < 3; i++)
    {
        if (scores[i] > max)
        {

            max = scores[i];
            maxId = i;
        }
    }
    int store = maxId + 1;
    char message[100];
    if (scores[maxId] <= -1)
    {
       
        strcpy(message, "Not Found");
    }
    else
    {
        sprintf(message, "the Best choise: score: %lf scoreitem :%lf  price:%lf  store:%d ", scores[maxId], scoreitems[maxId], prices[maxId], store);
    }

    int *user_scores_2_goods;
    char formatted_output[256];
    format_goods_list(items, n, formatted_output, sizeof(formatted_output));
    user_scores_2_goods = confirm_function(message, user, n, items, user_name);
    if (user_scores_2_goods == NULL)
    {
        fprintf(stderr, "Error: No scores received.\n");
        exit(EXIT_FAILURE);
    }

    // Debugging Tool: Print the received array
 
   

     write(pipe1[user][1],&maxId,sizeof(int));
     write(pipe2[user][1],scores,sizeof(double)*3);
     write(pipe3[user][1],user_scores_2_goods,sizeof(int)*(n+1));
     write(pipe4[user][1],&store,sizeof(int));
     write(pipe5[user][1],&maxId,sizeof(int));
     write(pipe6[user][1],scores,sizeof(double)*3);
     write(pipe7[user][1],user_scores_2_goods,sizeof(int)*(n+1));
     write(pipe8[user][1],mypathes257,sizeof(int)*n);
     write(pipe9[user][1],&store,sizeof(int));
     
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void main_function(user, treshhold, items, numitems, n /*tedad cala ha*/){

// }

// int main(){
void *final_thread(void *args){
 struct Threadorderargs *data = (struct Threadorderargs *)args;
    int user=data->user; char **items =data->items; int *numitems=data->numitems; int n=data->n; int treshhold=data->treshhold;int is_repeated=data->treshhold; char *user_name=data->user_name;
 int maxId;
 double scores[3];
 int user_scores_2_goods[n+1];
 int store;
 
 read(pipe1[user][0],&maxId,sizeof(int));
     
     read(pipe2[user][0],scores,sizeof(scores));
         
     read(pipe3[user][0],user_scores_2_goods,sizeof(user_scores_2_goods));
        
     read(pipe4[user][0],&store,sizeof(int));
    
  if (scores[maxId] > -1 && user_scores_2_goods[0])
    {

        char *st;
        if (store == 1)
        {
            st = "1";
        }
        if (store == 2)
        {
            st = "2";
        }
        if (store == 3)
        {    
            st = "3";
        }
       //  char *st = store == 1 ? "1" : (store == 2 ? "2" : "3");
        if (write(pipes[user][1], st, 2) == -1)
        {
            if (errno == EPIPE)
            {
                fprintf(stderr, "Error: Pipe closed unexpectedly (EPIPE) for user %d.\n", user);
                exit(EXIT_FAILURE); // or handle gracefully
            }
        }

    }

}

void *thread_scores(void *arg){
struct Threadorderargs *data = (struct Threadorderargs *)arg;
    int user=data->user; char **items =data->items; int *numitems=data->numitems; int n=data->n; int treshhold=data->treshhold;int is_repeated=data->treshhold; char *user_name=data->user_name;
 int maxId;
 double scores[3];
 int user_scores_2_goods[n+1];
 int mypathes257[n];
 int store;
      read(pipe5[user][0],&maxId,sizeof(int));
      
     read(pipe6[user][0],scores,sizeof(scores));
   
     read(pipe7[user][0],user_scores_2_goods,sizeof(user_scores_2_goods));
    
     read(pipe8[user][0],mypathes257,sizeof(mypathes257));
      
     read(pipe9[user][0],&store,sizeof(int));
     
    if (scores[maxId] > -1 && user_scores_2_goods[0])
    {

        
        for (int i = 1; i <= n; i++)
        {       
                     
            char fileName[50];
            char category[50];
            if(mypathes257[i-1]>=161&&mypathes257[i-1]<=239){strcpy(category,"Apparel");}//79
            if(mypathes257[i-1]>=504&&mypathes257[i-1]<=575){strcpy(category,"Beauty");}
            if(mypathes257[i-1]>=1&&mypathes257[i-1]<=78){strcpy(category,"Digital");}
            if(mypathes257[i-1]>=240&&mypathes257[i-1]<=331){strcpy(category,"Food");}//92
            if(mypathes257[i-1]>=79&&mypathes257[i-1]<=160){strcpy(category,"Home");}//82
            if(mypathes257[i-1]>=332&&mypathes257[i-1]<=424){strcpy(category,"Market");}//93
            if(mypathes257[i-1]>=576&&mypathes257[i-1]<=653){strcpy(category,"Sports");}
            if(mypathes257[i-1]>=425&&mypathes257[i-1]<=503){strcpy(category,"Toys");}//79


            sprintf(fileName,"Dataset/Store%d/%s/%d.txt",store,category,mypathes257[i-1]);
            char line1[100];
                char line2[100];
                char line3[100];
                char line4[100];
                char line5[100];
                char line6[100];
                
            
            sem_wait( &(mutex[user][mypathes257[i-1]])   ); 
                read_count[store][mypathes257[i-1]]++;
                if (read_count[store][mypathes257[i-1]] == 1) {
                    sem_wait(&rw_mutex[store][mypathes257[i-1]]); 
                }
                sem_post(&mutex[store][mypathes257[i-1]]);  
                //reading
                printf("%s",fileName);
                FILE  *file = fopen(fileName, "r"); 
                fgets(line1, sizeof(line1), file);
                fgets(line2, sizeof(line1), file);
                fgets(line3, sizeof(line1), file);
                fgets(line4, sizeof(line1), file);
                fgets(line5, sizeof(line1), file);
                fgets(line6, sizeof(line1), file);
                fclose(file);
                //end of reading
                sem_wait(&mutex[store][mypathes257[i-1]]);   
                read_count[store][mypathes257[i-1]]--;
                if (read_count[store][mypathes257[i-1]] == 0) {
                    sem_post(&rw_mutex[store][mypathes257[i-1]]); 
                }
                sem_post(&mutex[store][mypathes257[i-1]]); 
                char sc[10];char entity[10]; char price[10];
            extract_substring_with_length(line3,sc,7,3);
            extract_substring_with_length(line2,price,7,6);
         //     printf("\nentity:%s\n",entity);
                extract_substring_with_length(line4,entity,8,2);
        //    printf("\nprice:%s\n",price);
            double pricenum= string_to_double(price);
            
             double oldscore = string_to_double(sc);


             int entitymum =  atoi(entity);
             int entitynum= (int)entitymum;
          //   printf("\n%d\n",user_scores_2_goods[i]);
            // printf("%.1f",oldscore);
             int check=1;
             if(oldscore==0||pricenum==0){check=0;}
             if(entitymum==0){entitymum=10;}
             if(pricenum==0){pricenum=105.12;}
             double newscore = (oldscore   +(double) user_scores_2_goods[i])/2;
             // printf("\n%.1f\n",newscore);
              if(check==1){
               sem_wait(&rw_mutex[store][mypathes257[i-1]]);
            updateFile(fileName,entitymum,items[i-1],pricenum,newscore);
              sem_post(&rw_mutex[store][mypathes257[i-1]]);
              }
         }
    }

}
int main_function(int user, char **items, int *numitems, int n, int treshhold, int is_repeated, char *user_name)
{

   
    if (pipe(pipe1[user]) == -1)
            {
                perror("Pipe creation failed");
            }
     if (pipe(pipe2[user]) == -1)
            {
                perror("Pipe creation failed");
             }
        if (pipe(pipe3[user]) == -1)
            {
                perror("Pipe creation failed");
            }
            if (pipe(pipe4[user]) == -1)
            {
                perror("Pipe creation failed");
           }
            if (pipe(pipe5[user]) == -1)
            {
                perror("Pipe creation failed");
           
            }
           if (pipe(pipe6[user]) == -1)
            {
                perror("Pipe creation failed");
            }
            if (pipe(pipe7[user]) == -1)
            {
                perror("Pipe creation failed");
            }
            if (pipe(pipe8[user]) == -1)
            {
                perror("Pipe creation failed");
            }
            if (pipe(pipe9[user]) == -1)
            {
                perror("Pipe creation failed");
            }
           

    pthread_t threads[3];

    struct Threadorderargs arg1;
    struct Threadorderargs arg2;
    struct Threadorderargs arg3;
    arg1.user=user;arg1.user_name=user_name;arg1.treshhold=treshhold;arg1.is_repeated=is_repeated;arg1.items=items;arg1.n=n;arg1.numitems=numitems;
    arg2.user=user;arg2.user_name=user_name;arg2.treshhold=treshhold;arg2.is_repeated=is_repeated;arg2.items=items;arg2.n=n;arg2.numitems=numitems;
    arg3.user=user;arg3.user_name=user_name;arg3.treshhold=treshhold;arg3.is_repeated=is_repeated;arg3.items=items;arg3.n=n;arg3.numitems=numitems;


pthread_create(&threads[0], NULL, odrder_thread, &arg1);
pthread_create(&threads[1], NULL, thread_scores, &arg2);
pthread_create(&threads[2], NULL, final_thread, &arg3);
printf("main process : %lu create thread for order: %lu \n",threads[0]);
printf("main process : %lu create thread for scores: %lu  \n",threads[1]);
printf("main process : %lu create thread for final: %lu   \n",threads[2]);

pthread_join(threads[0], NULL);
pthread_join(threads[1], NULL);
pthread_join(threads[2], NULL);

    
    
    for (int i = 0; i < 9; i++) {
        if (pipefds[user][i][0] > 0) {
            if (close(pipefds[user][i][0]) == 0) {
                pipefds[user][i][0] = -1;  
            } else {
            }
        } else {
         }
       if (pipefds[user][i][1] > 0) {
            if (close(pipefds[user][i][1]) == 0) {
                pipefds[user][i][1] = -1;  
            } else {
            }
        } else {
        }
    }

    for(int i=0;i<9;i++){

    close(pipefds[user][i][0]);
    close(pipefds[user][i][1]);
    }
   

    printf("the whole app is now done.");
     return 0;
}

#endif
