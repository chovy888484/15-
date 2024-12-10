#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include "ls_command.h"
#include <sys/types.h>

#define MAX_LINE 80
#define MAX_ARGS 10

int main()
{
    char input[MAX_LINE];
    char *token;
    char *argv[MAX_ARGS];
    
    while(1){
        //prompt
        printf("myshell> ");

        //read command
        fgets(input, MAX_LINE, stdin);

        //tokenzie input
        token = strtok(input, " \n\t"); //delim 하고싶은 것들 모두 넣기
        int i = 0;
        while(token != NULL){
            argv[i++] = token; //argv[0] 첫번째(명령어) argv[1] 두번째(인수)
            token = strtok(NULL, " \n\t");
        }
        argv[i] = NULL;

        if(argv[0] == NULL){ // empty command
            continue;
        }

        if(strcmp(argv[0], "exit") == 0){
            printf("Goodbye~\n");
            exit(0);
        }else if(strcmp(argv[0], "cd") == 0){
            chdir(argv[1]); // move directory
        }else if(strcmp(argv[0], "pwd") == 0){
            getcwd(input, MAX_LINE);
            printf("%s\n", input);
        }else if(strcmp(argv[0], "ls") == 0){
            my_ls();
        }else if(strcmp(argv[0], "cat") == 0){
            if(argv[1] == NULL){
                fprintf(stderr, "Usage: cat <filename>\n");
                continue;
            }

            FILE *file = fopen(argv[1], "r");
            if(file == NULL){
                perror("Error opening file");
                continue;
            }
            
            char buffer[1024];
            while(fgets(buffer, sizeof(buffer), file) != NULL){
                printf("%s", buffer);
            }

            fclose(file);
        }else{
            if(access(argv[0], X_OK) == 0){
                printf("execute %s\n", argv[0]);
                pid_t pid = fork();
                if(pid == 0){
                    execvp(argv[0], argv);
                    perror("execvp failed");
                    exit(1);
                }else if(pid > 0){
                    wait(NULL);
                }else{
                    perror("fork failed");
                }
            }else{
                printf("command not found: %s\n", argv[0]);
            }
        }
    }
    return 0;
}