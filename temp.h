for (int i = 1; i <= n; i++)
    {

        
   char fileName[50];
   sprintf(fileName,"Dataset/Store%d/Home/%d.txt",store,mypathes257[i-1]);
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
   
    extract_substring_with_length(line4,entity,8,2);
   
double pricenum= string_to_double(price);


double oldscore = string_to_double(sc);


double entitymum =  string_to_double(entity);

double newscore = (oldscore   + user_scores_2_goods[i])/2;
updateFile(fileName,entitymum,items[i-1],pricenum,newscore);
    }

}
