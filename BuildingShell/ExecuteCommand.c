//
//  ExecuteCommand.c
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#include "ExecuteCommand.h"



void fork_test(void) {
    pid_t child_pid = fork();
    pid_t wait_result;
    int stat_loc;
//    if (child_pid == 0) {
//        printf("### child ###\nCurrent PID: %d and Child PID: %d\n", getpid(), child_pid);
//        sleep(1);
//    } else {
//        wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
//        printf("### Parent ###\nCurrent PID: %d and Child PID: %d\n",getpid(), child_pid);
//    }
}


void print_current_working_directory(void) {
    char curr_working_dir[1024];
    getcwd(curr_working_dir, sizeof(curr_working_dir)); // it seems currentworkingdirectory is set in the sent argument
    printf("\n%s", curr_working_dir);
    PRINT_NEWLINE;
    PRINT_NEWLINE;
    return;
}

void exit_shell(void) {
    should_exit_shell = true;
    PRINT_NEWLINE;
    //check_all_the_processes_to_kill
    return;
}

void set_shell(void) {
    
}


char * change_to_home_directory(const char * path) {

    char * home_path = getenv("HOME");
    int len_home = strlen(home_path);
    int len_path_ = strlen(path);
    char * new_full_path
    = malloc(len_home + len_path_ + 5);
    strcpy(new_full_path, home_path);
    for (int i = 1; i < len_path_; i++) {
        new_full_path[len_home + i -1 ] = path[i];
    }
    new_full_path[len_home + len_path_] = '\0';
    
    //printf(" ****************** %s\n", new_full_path);
    return new_full_path;
    
}

void change_current_directory(const char * dir_path) {
    char curr_working_dir[1024];
    //printf("dir path = %s %c\n", dir_path, dir_path[0]);
    if (dir_path[0] == '~') {
        dir_path = change_to_home_directory(dir_path);
    }

    //printf("%s\n", getcwd(curr_working_dir, sizeof(curr_working_dir)));
    int ch_dr_flg = chdir(dir_path);
    //printf("%s\n", getcwd(curr_working_dir, sizeof(curr_working_dir)));
    if (ch_dr_flg != 0) {
        char yourstring[100];
        snprintf(yourstring, sizeof yourstring, "cd: %s", dir_path);
        perror(yourstring);
        //fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
    }
}
