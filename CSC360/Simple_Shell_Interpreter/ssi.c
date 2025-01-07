
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <dirent.h>
#include <sys/types.h>

#include <sys/wait.h>

#define SIZEPROMPT 256
#define SIZEMAXIMUM 1024

/*
td:
write the readme.txt
go over specification and make sure it works 
*/



// Functions list
char* get_prompt();
void do_command(char *args[]);
void get_args(char* user_input, char* args[]);
void change_dir(char *args[]);
void forking(char* args[]);
void bg_command(char* args[]);
void bg_list();
void addToList(pid_t pid, char **args);
int getLengthList();
void childTerminationCheck();

// struct to keep for background processes, from tutorial slides
typedef struct bg_pro{
    pid_t pid;
    char command[SIZEMAXIMUM];
    struct bg_pro* next;
}
bg_pro;
bg_pro* head =NULL;


/*
Main driving code, base code from git
*/
int main()
{
	int bailout = 0;

	while (!bailout)
	{
        char *prompt = get_prompt();
		char *user_input = readline(prompt);
        char *args[SIZEPROMPT];
        get_args(user_input, args);

       if (args[0]== NULL){
            free(user_input);
            free(prompt);
        }

		else if (!strcmp(user_input, "q") || !strcmp(user_input, "quit") || !strcmp(user_input, "exit")){
			bailout = 1;
		}

		else{
            do_command(args);
		}

    
        childTerminationCheck();

		free(user_input);
        free(prompt);
        
	}
    
	printf("Goodbye!\n");
	return 0;
}


/*
formulates the prompt line that the user inputs on
*/
char* get_prompt() {
    char host_name[SIZEPROMPT];
    char cur_dir[SIZEPROMPT];

    gethostname(host_name, sizeof(host_name));
    getcwd(cur_dir, sizeof(cur_dir));

    char *prompt = malloc(SIZEPROMPT);
    snprintf(prompt, SIZEMAXIMUM, "%s@%s: %s > ", getlogin(), host_name, cur_dir);

    return prompt; 
}


/*
directs to function to handle inputed command
inputs: args, command
*/
void do_command(char *args[]){
    if(strcmp(args[0], "cd") == 0){
        change_dir(args);
    }

    else if(strcmp(args[0], "bg") == 0) {
        bg_command(args);
    }

    else if(strcmp(args[0], "bglist") == 0){
        bg_list(args);
    }

    else{
        forking(args);
    }
    
}

/*
isolates args from user input
input: args array, user input 
*/
void get_args(char* user_input, char* args[]){
    char *token;
    int i = 0;
    token = strtok(user_input, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

/*
changes directory
inputs: args array
*/
void change_dir(char* args[]){
    // return to home directory
    if((args[1] == NULL) || (strcmp(args[1], "~") == 0)){
        chdir(getenv("HOME"));
        return;
    }

    // go back one dir
    else if(strcmp(args[1], "..") == 0){
        if(chdir("..") == -1){
            perror("Can't go back a directory");
        }
        
    }
    // checking and display message if dir doesnt exitst
    else if (args[1] != NULL){
        if (chdir(args[1]) == -1){
            perror("Unable to find the directory");
        }
    }

    else{
       perror("Unable to process command");
    }

}

/*
fork and creates child processes, then waits
input: args array
*/
void forking(char* args[]) {
	pid_t pid = fork();
	if (pid < 0) {
        perror("Unable to fork");
        return;
    }
    
    if (pid == 0) {
        execvp(args[0], args);
    }
    else {
        // Waiting for child
        waitpid(pid, NULL, 0);
    }
}

/*
forks and adds newly created process to bg_pro list to keep track of
*/
void bg_command(char* args[]){
    pid_t pid = fork();
   
    if (pid < 0) {
        perror("Unable to fork");
        return;
    }

    if (pid == 0) {
        execvp(args[1], args+1);
    }

    else {
        addToList(pid, args);
    }
}

/*
adding new bg_pro to linked list to keep track of
inputs: pid, args array

*/
void addToList(pid_t pid, char **args){
    bg_pro *new_bg =(bg_pro*)malloc(sizeof(bg_pro));

    new_bg->pid = pid;
    snprintf(new_bg->command, SIZEMAXIMUM, "%s", args[1]);

    new_bg ->next = head;
    head = new_bg;
}

/*
prints processes in background when bglist is called
- uses linked list 
*/
void bg_list(){
    for(bg_pro* front = head; front != NULL; front = front->next){
        printf("%d: %s\n", front->pid, front->command);
    }
}

/*
returns length of linked list
*/
int getLengthList(void){
    bg_pro *front = head;
    int num = 0;
    while(front != NULL){
        front = front->next;
        num++;
    }
    return num;
}


/*
child termination checker for all bg process in the list - from tutorial slides

*/ 
void childTerminationCheck(void) {
    if (getLengthList() > 0) {
        pid_t ter = waitpid(0, NULL, WNOHANG);
        
        while (ter > 0) {
            bg_pro *temp_head = head;
            if (head->pid == ter) {
                printf("%d: %s has terminated.\n", temp_head->pid, temp_head->command);
                head = head->next;
                free(temp_head);
            } else {
                while (temp_head->next != NULL && temp_head->next->pid != ter) {
                    temp_head = temp_head->next;
                }

                if (temp_head->next != NULL) {
                    printf("%d: %s has terminated.\n", temp_head->next->pid, temp_head->next->command);
                    bg_pro *temporary = temp_head->next;
                    temp_head->next = temporary->next;
                    free(temporary); 
                }
            }
            ter = waitpid(0, NULL, WNOHANG);   
        }
    }
}