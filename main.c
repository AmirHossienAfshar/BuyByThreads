
#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 

int main(){


    pid_t pid1, pid2, pid3;

 
    pid1 = fork();
    if (pid1 == 0) {

       pid_t p1, p2, p3,p4,p5,p6,p7,p8;
       p1= fork();
       if(p1==0){                 // Write your code here 
               return 0;}
        p2= fork();
       if(p2==0){                 // Write your code here 
               return 0;}
        p3= fork();
       if(p3==0){                 // Write your code here 
               return 0;}
        p4= fork();
       if(p4==0){                 // Write your code here 
               return 0;}
         p5= fork();
       if(p5==0){                 // Write your code here 
               return 0;}
        p6= fork();
       if(p6==0){                 // Write your code here 
               return 0;}
        p7= fork();
       if(p7==0){                 // Write your code here 
               return 0;}
        p8= fork();
       if(p8==0){                 // Write your code here 
               return 0;}
            
printf("I am the first child. My PID: %d, Parent PID: %d\n", getpid(), getppid());
        return 0; 
    }

    // ایجاد فرزند دوم
    pid2 = fork();
    if (pid2 == 0) {
        
 pid_t p1, p2, p3,p4,p5,p6,p7,p8;
       p1= fork();
       if(p1==0){                 // Write your code here 
               return 0;}
        p2= fork();
       if(p2==0){                 // Write your code here 
               return 0;}
        p3= fork();
       if(p3==0){                 // Write your code here 
               return 0;}
        p4= fork();
       if(p4==0){                 // Write your code here 
               return 0;}
         p5= fork();
       if(p5==0){                 // Write your code here 
               return 0;}
        p6= fork();
       if(p6==0){                 // Write your code here 
               return 0;}
        p7= fork();
       if(p7==0){                 // Write your code here 
               return 0;}
        p8= fork();
       if(p8==0){                 // Write your code here 
               return 0;}





        printf("I am the second child. My PID: %d, Parent PID: %d\n", getpid(), getppid());
        return 0; 
    }

    // ایجاد فرزند سوم
    pid3 = fork();
    if (pid3 == 0) {


         pid_t p1, p2, p3,p4,p5,p6,p7,p8;
       p1= fork();
       if(p1==0){                 // Write your code here 
               return 0;}
        p2= fork();
       if(p2==0){                 // Write your code here 
               return 0;}
        p3= fork();
       if(p3==0){                 // Write your code here 
               return 0;}
        p4= fork();
       if(p4==0){                 // Write your code here 
               return 0;}
         p5= fork();
       if(p5==0){                 // Write your code here 
               return 0;}
        p6= fork();
       if(p6==0){                 // Write your code here 
               return 0;}
        p7= fork();
       if(p7==0){                 // Write your code here 
               return 0;}
        p8= fork();
       if(p8==0){                 // Write your code here 
               return 0;}





      
        printf("I am the third child. My PID: %d, Parent PID: %d\n", getpid(), getppid());
       









    return 0;}
}

