//
//  StudyMaterial.cpp
//  BuildingShell
//
//  Created by Fatema on 9/30/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#include <stdio.h>


//Epoch
// echo $SHELL    to see which shell

// Old shell like Bourne Shell is using Canonical Sehll
// Bash and newer shell is using non-nanocial mode
// most of the shells are using non-canocial mode

#include <iostream>
#include <string.h>
#include <stdlib.h> //what is that?
#include <unistd.h>  //what is that?
#include <sys/types.h> //a header file which contains definition for ///many ID's and counts etc
#include <sys/wait.h> //what is this?

#include <signal.h>  // should ignore signal() function bcz of its unknwn behvr. instd use sigaction
#include <termios.h> // for terminali/o like canonical read or non-canonicl read. write is similar in all case. just to print
// tcgetattr(int flides, )

#include <errno.h>

extern int errno;








/////////////// make another header file for this //////////////
#include <sys/stat.h> //defines the structure of the data returned by funtions fstat(), lstat(), stat()
#include <fcntl.h> // header in the C POSIX library for the C programming language that contains constructs that refer to file control, opening a file, retrieving and changing the file permissions
#include <errno.h>




///////////////// Pthread ///////////
//each thread has a signal mask to block signal
// but all the thread of a process has only one signal handler
//All threads share data via global variables of the proceess.. as you are in the same process. it odsnt need IPC mechanism





extern int errno;

#define MAX_INPUT_LENGTH 1000
#define MAX_NO_Of_COMMAND 100
#define clear() printf("\033[<L>;<C>H")  //not_working



void init_shell() {
    clear();
    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
           "***********************");
    char *username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}


void printDirectory() {
    char currentWorkingDirectory[1024];
    getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory)); // it seems currentworkingdirectory is set in the sent argument
    printf("\nDir: %s", currentWorkingDirectory);
}





// Message Que

//some command // ipcs | less

















//int parsePipe(char *str, char **strPiped) {
//    int i;
//    for (int i = 0; i < 2; i++) {
//        strPiped[i] = strsep(&str, "|");
//        if (strPiped[i] == NULL) {
//            break;
//        }
//        if (strPiped[1] == NULL) {
//            return 0; //return zero if no pipe is found
//        } else {
//            return 1;
//        }
//    }
//}
//
//int processString(char *string, char **arguments, char **argumentsPiped) {
//    char *strPiped[2];
//    int piped = 0;
//
//    piped = parsePipe(str, strPiped);
//}













void printSuccessFailureDesc(int fd) {
    
    if (fd == -1) {
        printf("Error Number %d\n", errno);
        perror(" ( program )\n");
        exit(1);
    } else {
        printf("opended the fd = %d File descriptor number is %d \n",fd, fd);
    }
    //    if (close(fd) < 0) {
    //        perror("");
    //        exit(1);
    //    }
    //    printf("closed the fd.\n");
    
}





void testFileDescriptor() {
    int fd1 = open("foo.txt", O_RDWR, 0);
    
    int fd2 = open("fooo.txt", O_RDONLY, 0);
    printSuccessFailureDesc(fd1);
    printSuccessFailureDesc(fd2);
}

void redrawBuffer() {
    
}

///job control and process relationship. like forgrnd and bckgrnd process. how they are implemnted and what r the consqences
// to get the grop id of a process call pid_t getpgrp(void)
// semicolon to include much more command on same line on shell  exmple: $ cmd1 ; cmd2
// cmd1 } cmd2
// as a process you can join other grp. or create a new grp..
// int setpgid(pid_t pid, pid_t pgid)//a process can set gro ID of itself or its children. if pgid downt exist or a grp dsnt exist cret a grp
//if u creat a new session u cannot be the grpup leader
// the foreground process will have control terminal. if u input smthing and multiple proecess is open only the control terminal will get it.
// A session is one or more process groups proc1 | proc2 & proc3 | proc4 | proc5
// A login shell is a session in general

//To establish a new session. login to ssh liza@linprog is an example of pseudo-terminal. it is a emulated terminal
// only have one foreground group. many backgrnd groups
//pid_t setsid(void);
// A process can open file /dev/tty to talk to controlling terminal regardless of how std IO are riderct
//foreground grp can use fd independently
// a way to by pass I/O redirection

void signalAndTerminal() {
    
    //the termios structure is beloww
    //    struct termios {
    //        tcflag_t c_flag;
    //    }
    //Two signals cannot be ignores sigkill and sigstop
    //default signal is ignoring
    
    char ch;
    struct termios save_termios;
    save_termios.c_cc[VMIN] = 0;
    save_termios.c_cc[VTIME] = 0;
    save_termios.c_cflag &= ~(ECHO | ICANON);
    if (tcgetattr(STDIN_FILENO, &save_termios) < 0) {
        perror("setaatr");
    }
    int cur_pos = 0;
    while (1) {
        printf("."); fflush(0);sleep(1);
        if (read(STDIN_FILENO, &ch, 1) > 0) {
            if (ch == '\n') {
                putchar(ch);
                fflush(0);
                //buff
            }
            printf("!!%c", ch);
        };
    }
}

void signalAndTerminal2() {
    //    if (fd = fopen("dev/tty", "r+") == 0) {
    //        printf("Cannot open dev/tty\n");
    //        exit(0);
    //    }
    //pid_t tcgetpgrp(int filedes); // gives process group id of foregorund grp
}



void fork_test() {
    pid_t child_pid = fork();
    pid_t wait_result;
    int stat_loc;
    if (child_pid == 0) {
        printf("### child ###\nCurrent PID: %d and Child PID: %d\n", getpid(), child_pid);
        sleep(1);
    } else {
        wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
        printf("### Parent ###\nCurrent PID: %d and Child PID: %d\n",getpid(), child_pid);
    }
}




void test_char_pointer() {
    char label[] = "Single";
    char label2[10] = "Married";
    char *labelPtr = label;
    //Difference between character pointer nd constant charecter pointer and pointer constant
}



int main(int argc, const char * argv[]) {
    //char inputString[MAX_INPUT_LENGTH], *parsedArguments[MAX_NO_Of_COMMAND];
    //char *parsedArgumentsPiped[MAX_NO_Of_COMMAND];
    //int execFlag = 0;
    //init_shell();
    //testFileDescriptor();
    //signalAndTerminal();
    //fork_test();
    //exec_test(); // doesnt work
    
    char str[100] = "ls -l -h -a";
    const char s[2] = " ";
    char *token = strtok(str, s);
    while ( token != NULL) {
        printf(" token %s \n",token);
        token = strtok(NULL, s);
        printf(" token 2 %s \n",token);
    }
    
    //    FILE *fp;
    //    fp = fopen("file.txt", "r");
    //    if (fp == NULL) {
    //        fprintf(stderr, "Value of errno: %d\n", errno);
    //        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    //    } else {
    //        printf("ugfgef\n");
    //        fclose(fp);
    //    }
    
    return 0;
}
