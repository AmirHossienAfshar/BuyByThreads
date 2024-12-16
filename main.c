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

sem_t rw_mutex[3][624];  
sem_t mutex[3][624];    
int read_count[3][624] ={0}; 




int pipefds[9][2];

struct ThreadArgs  {
    int input1;   
    char category[100];
    char itemName[50];
    int num;
    int store;
    } ;

   struct ThreadReturn  {
    int foundItem;   
    
    double score;
   } ;

   void updateFile(const char *fileName, int newEntity,char *name,double price,double score) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
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



   double string_to_double(const char *str) {
    double result = 0.0;
    double fraction = 0.0;
    int is_negative = 0;
    int divisor = 1;

 
    if (*str == '-') {
        is_negative = 1;
        str++;
    }

 
    while (*str && isdigit(*str)) {
        result = result * 10 + (*str - '0');
        str++;
    }

   
    if (*str == '.') {
        str++;
        while (*str && isdigit(*str)) {
            fraction = fraction * 10 + (*str - '0');
            divisor *= 10;
            str++;
        }
        result += fraction / divisor;
    }

  
    if (is_negative) {
        result = -result;
    }

    return result;
}

void extract_substring_with_length(const char *source, char *destination, int start, int length) {
    strncpy(destination, source + start, length);
    destination[length] = '\0';  
}
void* thread_function(void* arg ) {
    struct ThreadArgs* data = (struct ThreadArgs*) arg;
    
      struct ThreadReturn res;
     int num=data->input1;
    
     char fileNum[10];
     
     sprintf(fileNum, "%d", num);
     
     char *fileName,score[10],entity[10],price[10];
      char *endptr1;
      char *endptr2;
      char *endptr3;
    
     double scorenum,entitymum,pricenum;
   
     fileName= data->category;
    
     strcat(fileName,fileNum);
     
     char *temp= ".txt";
     strcat(fileName,temp);
     char line1[100];
     char line2[100];
     char line3[100];
     char line4[100];
     char line5[100];
     char line6[100];
    
  
  sem_wait( &(mutex[data->store][data->input1])   ); 
     read_count[data->store][data->input1]++;
     if (read_count[data->store][data->input1] == 1) {
        sem_wait(&rw_mutex[data->store][data->input1]); 
     }
     sem_post(&mutex[data->store][data->input1]);  
     //reading
     FILE  *file = fopen(fileName, "r"); 
     fgets(line1, sizeof(line1), file);
     fgets(line2, sizeof(line1), file);
     fgets(line3, sizeof(line1), file);
     fgets(line4, sizeof(line1), file);
     fgets(line5, sizeof(line1), file);
     fgets(line6, sizeof(line1), file);
     fclose(file);
     //end of reading
      sem_wait(&mutex[data->store][data->input1]);   
     read_count[data->store][data->input1]--;
     if (read_count[data->store][data->input1] == 0) {
        sem_post(&rw_mutex[data->store][data->input1]); 
     }
    sem_post(&mutex[data->store][data->input1]); 

      
   
    
    extract_substring_with_length(line2,price,7,6);
    extract_substring_with_length(line3,score,7,3);
    extract_substring_with_length(line4,entity,8,2);
   
pricenum= string_to_double(price);
//strtod(price, &endptr1);

scorenum = string_to_double(score);
//strtod(score, &endptr2);

entitymum =  string_to_double(entity);
///strtod(entity, &endptr3);



    char *tempchar= data->itemName;
    int n=strlen(data->itemName)+7;
    if( strstr(line1,data->itemName)==NULL || strlen(line1)!=n){
     res.foundItem=0;
    }else{  res.foundItem=1;}
    if(entitymum<data->num){
         res.foundItem=0;
    }


     if(res.foundItem==1){
       double sc= scorenum*10.1 / pricenum;
       res.score= sc;
       
      
     }
    
     
       /////////////////////////////////////////////////////////////
        // send to front
if(res.foundItem==1){
    if(data->store==0){
        char buff[50];
     
    
       sprintf(buff, "%.5f", res.score); 
       write(pipefds[0][1], buff, 20); 
       write(pipefds[1][1], score, 10); 
       write(pipefds[2][1], price, 10); 
    }
     if(data->store==1){
        char buff[50];

      
       sprintf(buff, "%.5f", res.score); 
       write(pipefds[3][1], buff, 20); 
       write(pipefds[4][1], score, 10); 
       write(pipefds[5][1], price, 10); 
    }
    if(data->store==2){
        char buff[50];

    
       sprintf(buff, "%.5f", res.score); 
       write(pipefds[6][1], buff, 20); 
       write(pipefds[7][1], score, 10); 
       write(pipefds[8][1], price, 10); 
    }
    
     printf("\n  %d kk  %lf **  ",res.foundItem,res.score);
     printf(" %d  \n ",num);

    
     sem_wait(&rw_mutex[data->store][data->input1]);
    //////////////////////////////////////////////////
     
      
      // this part of code will be updated
     if(data->store==1){
        int newEntity= (int)entitymum- data->num;
        updateFile(fileName,newEntity,data->itemName,pricenum,scorenum);
     }


    /////////////////////////////////////////////////////////
     sem_post(&rw_mutex[data->store][data->input1]);
    
    
    
    
}
     
   
   

    
     return 0;
}


   

int main(){

    for(int i=0; i<3;i++){
       for(int j=0;j<624;j++){

             sem_init(&rw_mutex[i][j], 0, 1);
           sem_init(&mutex[i][j], 0, 1);
        }
        
    }
    
    
    pid_t pid1, pid2, pid3;
    char *item= "Jeans";
    int number=5;
    
    for(int i=0;i<9;i++)
    if (pipe(pipefds[i]) == -1) {
    perror("Pipe creation failed");
    exit(EXIT_FAILURE);
}

   char buffer[100];
    pid1 = fork();
    if (pid1 == 0) {
       int s=0;
       pid_t p1, p2, p3,p4,p5,p6,p7,p8;
       p1= fork();
       if(p1==0){                
         // Write your code here /////////////////////////////////////////////////////////////
       
        pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Apparel/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=160+i+1;
        args[i].store=0;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }


/////////////////////////////////////////////////////////////////////////////////////
               return 0;}
        p2= fork();
       if(p2==0){                
         // Write your code here ///////////////////////////////////////////////////


           pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Beauty/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=503+i+1;
        args[i].store=0;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
   
////////////////////////////////////////////////////////////////////////

               return 0;}
        p3= fork();
       if(p3==0){                 
        // Write your code here ////////////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Digital/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=i+1;
        args[i].store=0;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }

        /////////////////////////////////////////////////////////////////////////
               return 0;}
        p4= fork();
       if(p4==0){                
         // Write your code here ////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Food/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=239+i+1;
        args[i].store=0;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
         ////////////////////////////////////////////////////////////////
               return 0;}
         p5= fork();
       if(p5==0){              
           // Write your code here /////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Home/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=78+i+1;
        args[i].store=0;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
           /////////////////////////////////////////////////////////////////
               return 0;}
        p6= fork();
       if(p6==0){             
            // Write your code here /////////////////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Market/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=331+i+1;
        args[i].store=0;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
            ////////////////////////////////////////////////////////////////////////////
               return 0;}
        p7= fork();
       if(p7==0){          
               // Write your code here /////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Sports/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].store=0;
        args[i].input1=575+i+1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
               ///////////////////////////////////////////////////////////////
               return 0;}
        p8= fork();
       if(p8==0){                
         // Write your code here ////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store1/Toys/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=424+i+1;
        args[i].store=0;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
         ///////////////////////////////////////////////////////
               return 0;}
            
printf("I am the first child. My PID: %d, Parent PID: %d\n", getpid(), getppid());


        return 0; 
    }




    pid2 = fork();
    if (pid2 == 0) {
        
         pid_t p1, p2, p3,p4,p5,p6,p7,p8;
       p1= fork();
       if(p1==0){                
         // Write your code here /////////////////////////////////////////////////////////////
       
        pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Apparel/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=160+i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }


/////////////////////////////////////////////////////////////////////////////////////
               return 0;}
        p2= fork();
       if(p2==0){                
         // Write your code here ///////////////////////////////////////////////////


           pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Beauty/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=503+i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
   
////////////////////////////////////////////////////////////////////////

               return 0;}
        p3= fork();
       if(p3==0){                 
        // Write your code here ////////////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Digital/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }

        /////////////////////////////////////////////////////////////////////////
               return 0;}
        p4= fork();
       if(p4==0){                
         // Write your code here ////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Food/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=239+i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
         ////////////////////////////////////////////////////////////////
               return 0;}
         p5= fork();
       if(p5==0){              
           // Write your code here /////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Home/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=78+i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
           /////////////////////////////////////////////////////////////////
               return 0;}
        p6= fork();
       if(p6==0){             
            // Write your code here /////////////////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Market/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=331+i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
            ////////////////////////////////////////////////////////////////////////////
               return 0;}
        p7= fork();
       if(p7==0){          
               // Write your code here /////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Sports/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=575+i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
               ///////////////////////////////////////////////////////////////
               return 0;}
        p8= fork();
       if(p8==0){                
         // Write your code here ////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store2/Toys/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=424+i+1;
        args[i].store=1;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
         ///////////////////////////////////////////////////////
               return 0;}
            


        printf("I am the second child. My PID: %d, Parent PID: %d\n", getpid(), getppid());
        return 0; 
    }

pid3 = fork();
    if (pid3 == 0) {

     pid_t p1, p2, p3,p4,p5,p6,p7,p8;
       p1= fork();
       if(p1==0){                
         // Write your code here /////////////////////////////////////////////////////////////
       
        pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Apparel/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=160+i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }


/////////////////////////////////////////////////////////////////////////////////////
               return 0;}
        p2= fork();
       if(p2==0){                
         // Write your code here ///////////////////////////////////////////////////


           pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Beauty/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=503+i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
   
////////////////////////////////////////////////////////////////////////

               return 0;}
        p3= fork();
       if(p3==0){                 
        // Write your code here ////////////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Digital/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }

        /////////////////////////////////////////////////////////////////////////
               return 0;}
        p4= fork();
       if(p4==0){                
         // Write your code here ////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Food/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=239+i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
         ////////////////////////////////////////////////////////////////
               return 0;}
         p5= fork();
       if(p5==0){              
           // Write your code here /////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Home/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=78+i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
           /////////////////////////////////////////////////////////////////
               return 0;}
        p6= fork();
       if(p6==0){             
            // Write your code here /////////////////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Market/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=331+i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
            ////////////////////////////////////////////////////////////////////////////
               return 0;}
        p7= fork();
       if(p7==0){          
               // Write your code here /////////////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Sports/");
strcpy(args[i].itemName, item);

        args[i].num=number;
        args[i].input1=575+i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
               ///////////////////////////////////////////////////////////////
               return 0;}
        p8= fork();
       if(p8==0){                
         // Write your code here ////////////////////////////////
   pthread_t threads[78]; 
    int thread_args[78];   
    struct ThreadArgs args[78] ;
    int i;

    for( i=0;i<78;i++){
        strcpy(args[i].category, "Dataset/Store3/Toys/");
strcpy(args[i].itemName, item);
        
        args[i].num=number;
        args[i].input1=424+i+1;
        args[i].store=2;
    }
    

   
    for (i = 0; i < 78; i++) {
        thread_args[i] = i + 1; 
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 78; i++) {
        pthread_join(threads[i], NULL);
    }
         ///////////////////////////////////////////////////////
               return 0;}
            





      
        printf("I am the third child. My PID: %d, Parent PID: %d\n", getpid(), getppid());
       
return 0;}
double price[3];
double score[3];
double scoreitem[3];
int notFound[3];
  struct pollfd pfds[3];
    pfds[0].fd = pipefds[2][0];
    pfds[0].events = POLLIN; 
    pfds[1].fd = pipefds[5][0];
    pfds[1].events = POLLIN; 
    pfds[2].fd = pipefds[8][0];
    pfds[2].events = POLLIN; 

 struct pollfd pfd= pfds[0];
int pollResult = poll(&pfd, 1, 10000); 
    if (pollResult > 0 ){
read(pipefds[0][0], buffer, sizeof(buffer));  
score[0]=string_to_double(buffer);
read(pipefds[1][0], buffer, sizeof(buffer));  
scoreitem[0]=string_to_double(buffer);
read(pipefds[2][0], buffer, sizeof(buffer));  
price[0]=string_to_double(buffer);

    }else{
        score[0]=-1;
    }


pfd= pfds[1];
 pollResult = poll(&pfd, 1, 10000); 
    if (pollResult > 0 ){
read(pipefds[3][0], buffer, sizeof(buffer));  
score[1]=string_to_double(buffer);
read(pipefds[4][0], buffer, sizeof(buffer));  
scoreitem[1]=string_to_double(buffer);
read(pipefds[5][0], buffer, sizeof(buffer));  
price[1]=string_to_double(buffer);
}else{
        score[1]=-1;
    }

pfd= pfds[2];
 pollResult = poll(&pfd, 1, 10000); 
    if (pollResult > 0 ){
read(pipefds[6][0], buffer, sizeof(buffer));  
score[2]=string_to_double(buffer);
read(pipefds[7][0], buffer, sizeof(buffer));  
scoreitem[2]=string_to_double(buffer);
read(pipefds[8][0], buffer, sizeof(buffer));  
price[2]=string_to_double(buffer);
}else{
        score[2]=-1;
    }

double max;
int maxId;
max=-1; maxId=0;


for(int i=0;i<3;i++){
  if(score[i]>max)  {
   
    max= score[i];
    maxId=i;
  }
}
int store= maxId+1;

if(score[maxId]==-1)printf("Not Found");else
printf("the Best choise: score: %lf scoreitem :%lf  price:%lf  store:%d ", score[maxId],scoreitem[maxId],price[maxId],store);
for(int i=0;i<9;i++){

    close(pipefds[i][0]);
close(pipefds[i][1]); 
 for(int i=0; i<3;i++){
       for(int j=0;j<624;j++){

             sem_destroy(&rw_mutex[i][j]);
           sem_destroy(&mutex[i][j]);
        }
        
    }
    

}

return 0;
}
