#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(void) {
    while (1){
        char buff[100]; // 100 char limit. stack!

    printf("Enter a Command: ");
    fgets(buff, sizeof(buff), stdin);
    buff[strcspn(buff, "\n")] = '\0';

    if (strcmp(buff, "exit") == 0){ 
        break;
    }

    // < = read from,
    if (strchr(buff, '<') != NULL){ 
        char *cmd = strtok(buff, "<");
        char *filename = strtok(NULL, "<");
        char *args3[20];
        int y = 0;
        char *token3 = strtok(cmd, " ");

        while(token3 != NULL){ 
            args3[y] = token3;
            y++; 
            token3 = strtok(NULL, " ");
        }
        args3[y] = NULL;    
        filename = strtok(filename, " ");

        pid_t pid3 = fork();

        if (pid3 == 0){ 
            int fd = open(filename, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
            execvp(args3[0], args3);
        } else { 
            wait(NULL);
        }
        continue;
    }  

       // > = Write to
       if (strchr(buff, '>')!= NULL){ 
        char *cmd4 = strtok(buff, ">");
        char *filename1 = strtok(NULL, ">");
        filename1 = strtok(filename1 , " ");
        char *args4[20];
        int w = 0;
        char *token4 = strtok(cmd4, " ");

        while(token4 != NULL){ 
            args4[w] = token4;
            w++;
            token4 = strtok(NULL, " ");
        }
        args4[w] = NULL;

        pid_t pid4 = fork();

        if (pid4 == 0){ 
            int fd1 = open(filename1, O_WRONLY | O_CREAT | O_TRUNC, 0644); // 0644 = 6: owner, 4: group (read only). Std permission for a regular file. Unix Permission system
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
            execvp(args4[0], args4);
        } else{ 
            wait(NULL);
        }
    
         continue;
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
    char *token = strtok(buff, " "); // Token points strtok to where the word is in the buff and explains it to skip whitespaces ( " ")

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
