#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    while (1){
        char buff[100]; // 100 char limit. stack!

    printf("Enter a Command: ");
    fgets(buff, sizeof(buff), stdin);
    buff[strcspn(buff, "\n")] = '\0';

    if (strcmp(buff, "exit") == 0){ 
        break;
    }

     // Pipe check for 2 concatenated commands
    if (strchr(buff, '|') != NULL){          // Check strings for a '|'. 
        char *cmd1 = strtok(buff, "|");      // parse the first part of the command before |  
        char *cmd2 = strtok(NULL, "|");      // parse from where you left is off after |
        
        char *args1[20];  
        int j = 0;
        char *token1 = strtok(cmd1, " "); 

        while (token1 != NULL){       // Max limit of 20 words, start at slot 0. Add them in slots that iterate
            args1[j] = token1;
            j++;
            token1 = strtok(NULL, " ");
        }
        args1[j] = NULL;

        char *args2[20];
        int x = 0;
        char *token2 = strtok(cmd2, " ");

        while(token2 != NULL){ 
            args2[x] = token2; 
            x++;
            token2 =strtok(NULL, " ");
        }
        args2[x] = NULL;

        int pipefd[2]; // [0] = read end, [1] = write end
        pipe(pipefd);
        pid_t pid1 = fork(); // fork the first child.

        if (pid1 == 0){ 
            dup2(pipefd[1], STDOUT_FILENO); //swap door 1 with the pipes end. when the first command is done printing, send it to the pipe
            close(pipefd[0]); 
            close(pipefd[1]);
            execvp(args1[0], args1); // clone eliminated.
        } 

        pid_t pid2 = fork(); // fork the rest.

        if (pid2 == 0){ 
            dup2(pipefd[0], STDIN_FILENO); //swap door 0 (keyboard input) with the pipes read end. the 2nd command reads from the pipe
            close(pipefd[0]);
            close(pipefd[1]); 
            execvp(args2[0], args2); // clone eliminated.
        } 

        close(pipefd[0]);
        close(pipefd[1]); 
        wait(NULL); // child one
        wait(NULL); // child 2 

        continue;
    } 

    char *args[20]; // safeguarding it against more than 20 words in a single line of input
    int i = 0; // slots for parsing
    char *token = strtok(buff, " "); // Token points strtok to where the word is in the buff and " " explains it to skip whitespaces

    while (token != NULL){
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    } 
    args[i] = NULL;
    if (strcmp(args[0], "cd") == 0){   // check if the first command is cd, if yes than chdir to whatever the next command is. Clone's change dir and die but parent dir still is in the same dir. This bypasses.
        chdir(args[1]);
        continue;
    } 

    pid_t pid = fork(); // pid_t = process ID (specific INT for macOS) and fork() = clone of the current program

    if (pid == 0){             // Clone has PID = 0. If it's =0 then take in the 1st command that is stored in args[0] and being from there.
        execvp(args[0], args);
    } else { 
        wait(NULL);
    }


}
    

    return 0;
}
