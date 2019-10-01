//
//  ExecuteCommand.c
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#include "ExecuteCommand.h"


void print_current_working_directory() {
    char curr_working_dir[1024];
    getcwd(curr_working_dir, sizeof(curr_working_dir)); // it seems currentworkingdirectory is set in the sent argument
    printf("\nDir: %s", curr_working_dir);
    PRINT_NEWLINE;
    PRINT_NEWLINE;
    return;
}

void exit_shell() {
    should_exit_shell = true;
    PRINT_NEWLINE;
    return;
}

void set_shell() {
    
}

void go_to_directory() {
    
}
