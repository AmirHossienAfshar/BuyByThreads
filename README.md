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

then after submiting a new user, it is cathced by the front end, and a new process is made for the user is made for doing the heavy process, as it begins with the function of `new_user`. it calls the `main_function`. this is where for each user 3 subprocess are made to take care of each shop and 8 sub process for each sub directory of those processes as well. ( total of 24 processes for each user ). 

there will be arround 80 threads, each for a txt file to check for the shopping list. as the data is being checked, there will be pipes to send the recived data to theier parent procesess ( the one that made those threads) and those 3 main processes check for the best shopping list, according to the desired 'score'. The best score is sent to the tkinter_app by as a new instance, by a process that is being execlb, and a confirmation is recived from the user. if the user confirmed, ( or if not) those values are sent to the final and score thread (and the values such as `last_modified` and `entity` are updated); this time if the user confiremed ( the shop was done), a rating shop user gui will pop up for gathering around the user scores for the shop. agian that score process gives its value to the final thread and it updates the scores.
