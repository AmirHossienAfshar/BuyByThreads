# BuyByThreads
### project schema

#### main code:
the fileMonitor.c: this is the main app. when it is runned, a subprocess is made for the tkinter front end, and the tkinter front end pops up. ( this actully is made by execlb of the userInput.py. ) and there the main has a process checks for the new uesers came from the front end. the communication between these is made by the shared memory, which is the shopping_data.txt. any modifes in that file is cathched by the main. 
the format of the input is looking like this:
`user_name, user_id, treashold, is_repeated, good1, count1, good2, count2, ...`
1. and user_id is a uniqe number (the line number in the txt file). 
2. the user_name is the name.
3. the treashold is the treashold.
4. is_repeated is used for the discount offering.
5. good1 is the first good.
6. count1 is the count of the first good.

then after submiting, a new tread for the user is made by the heavy process begins with the function of new_user. it calls the main_function. this is where for each user 3 subprocess are made for each shop and 8 sub process for each sub directory. 
