// Author: Siyam Al Shahriar

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;
int flag;
pid_t pid;
int command_p = -1;

void terminate_prog(int time, int pid){
    sleep(time);
    printf("Timeout foreground process.\n");
    kill(pid, SIGINT);
}
void signal_handler(int signum){
    if (command_p != -1){
        kill(command_p, SIGINT);
    }
}

int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    char wd[400];
    char *cwd;
    char cmd[256];
    signal(SIGINT, signal_handler);
    printf("Welcome to Gyshell - Made my Siyam\n");
    while (true) {
      
        do{ 
            // Print the shell prompt.
            
            fflush(stdout);
            cwd = getcwd(wd, sizeof(wd));
            printf("%s%s", cwd, prompt);
            fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

        arguments[0] = strtok(command_line, delimiters);
        int i = 0;
        while (arguments[i] != NULL){
            i++;
            arguments[i] = strtok(NULL, delimiters);
        }
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }
        
        char* last_arg = arguments[i-1];
        bool background_process = false;
        if (strcmp(last_arg, "&") == 0){
            background_process = true;
            arguments[i-1] = NULL;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
        // 1. Tokenize the command line input (split it on whitespace)
        // 2. Implement Built-In Commands
        // 3. Create a child process which will execute the command line input
        // 4. The parent process should wait for the child to complete unless its a background process
        
        if (strcmp(arguments[0], "cd") == 0){
            chdir(arguments[1]);
        } else if (strcmp(arguments[0], "pwd") == 0){
            printf("%s\n", getcwd(wd, sizeof(wd)));
        } else if (strcmp(arguments[0], "echo") == 0){
            i = 1;
            while (arguments[i] != NULL){
                if (arguments[i][0] == '$'){
                    printf("%s", getenv(arguments[i]+1));
                }
                else{
                    printf("%s", arguments[i]);
                }
                i++;
            }
            printf("\n");
        } else if (strcmp(arguments[0], "env") == 0){
            if (arguments[1] != NULL){
                printf("%s\n", getenv(arguments[1]));
            } else {
                char** env = environ;
                for (; *env; env++){
                    printf("%s\n", *env);
                }
            } 
            
        }
            else if (strcmp(arguments[0], "setenv") == 0){
                char* temp_val[2];
                temp_val[0] = strtok(arguments[1], "=");
                i = 0;
                while(temp_val[i] != NULL || i ==2){
                    i++;
                    temp_val[i] = strtok(NULL, "=");
                }
                setenv(temp_val[0], temp_val[1], 1);
            } else if (strcmp(arguments[0], "exit") == 0){
                exit(0);
            } else {
                pid = fork();
                if (pid == 0){
                    signal(SIGINT, SIG_DFL);
                    execvp(arguments[0], arguments);
                    exit(0);
                } else {
                    if (background_process){
                        command_p = -1;
                    } else {
                        command_p = pid;
                        int pid_1 = fork();
                        if (pid_1 == 0){
                            signal(SIGINT, SIG_DFL);
                            terminate_prog(10000, pid);
                            exit(0);
                        } else {
                            waitpid(pid, NULL, 0);
                            kill(pid_1, SIGINT);
                            waitpid(pid_1, NULL, 0);
                        }
                    }
                }
            }
        

        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}