//
//  Header.h
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#ifndef Header_h
#define Header_h

#define PRINT_NEWLINE printf("\n")

#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define FOR_LOOP(len) for(int i = 0; i < len; i++)
#define FOR_LOOP_FROM(start,len) for(int i = start; i < len; i++)


#define WRITE 1
#define READ 0

extern bool should_exit_shell;

/* Definition of a Job Status */
#define FOREGROUND 'F'
#define BACKGROUND 'B'
#define SUSPENDED 'S'
#define WAITING_INPUT 'W'

/* standard file descriptors */
#define STDIN 1
#define STDOUT 2
#define STDERR 3
#define ERROR_CODE -1
#define CHILD_PROCESS 0

/* command delimiter */
#define PIPEDELIMITER "|"
#define REGULARDELIMITER " "
#define MYPATHDELIMITER "="
#define REDIRECT_FROM_FILE_PERSPECTIVE_INPUT "<"
#define REDIRECT_FROM_FILE_PERSPECTIVE_OUTPUT ">"
#define NUMBER_OF_TOKENS_MYLS 100
#define MYLS "myls"

#define MY
#define ERROR -1
#define CHILDFORK 0

static int num_of_active_jobs = 0;



static const char * mypath = "MYPATH";
//static const char *myls_path = "/Volumes/Liza\ Disk/FSU/CPDP/Codes/MylsImplement/MylsImplement/myls";


#endif /* Header_h */
