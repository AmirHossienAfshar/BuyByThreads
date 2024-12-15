
#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct ThreadArgs  {
    int input1;   
    char category[100];
    char itemName[50];
    int num;
    } ;

   struct ThreadReturn  {
    int foundItem;   
    double score;
   } ;


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
    
     FILE  *file = fopen(fileName, "r"); 
     
     char line1[100];
     char line2[100];
     char line3[100];
     char line4[100];
     char line5[100];
     char line6[100];
     fgets(line1, sizeof(line1), file);
     fgets(line2, sizeof(line1), file);
     fgets(line3, sizeof(line1), file);
     fgets(line4, sizeof(line1), file);
     fgets(line5, sizeof(line1), file);
     fgets(line6, sizeof(line1), file);
      
   
    
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
       double sc= scorenum*100.01 / pricenum;
       res.score= sc;
      
     }
    
     
       /////////////////////////////////////////////////////////////
        // send to front
if(res.foundItem==1){
    
    printf("\n  %d kk  %lf **  ",res.foundItem,res.score);
    printf(" %d  \n ",num);
    
    
}
          

    //////////////////////////////////////////////////////////////////////////////
  
    //////////////////////////////////////////////////

      // we should update file here

    /////////////////////////////////////////////////////////
    
     
   
   

    
     return 0;
}


   

int main(){


    pid_t pid1, pid2, pid3;
    char *item= "Bookshelf";
    int number=31;
    
    
 
    pid1 = fork();
    if (pid1 == 0) {
        
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





      
        printf("I am the third child. My PID: %d, Parent PID: %d\n", getpid(), getppid());
       
return 0;}

return 0;
}
