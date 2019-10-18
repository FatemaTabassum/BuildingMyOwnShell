//
//  ExecuteCommand.h
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#ifndef ExecuteCommand_h
#define ExecuteCommand_h

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Header.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include "Parser.h"





void reset_all_values_Execute(void);

void print_current_working_directory(void);

void exit_shell(void);

void set_shell(void);

void go_to_directory(void);

void change_current_directory(const char *);

void set_env_variable(char *);

void execute_pipe(char *all_commands[], int command_count);

void execute_io_redirection_input(char* all_commands[], int command_count);

void execute_io_redirection_output(char* all_commands[], int command_count);

void execute_io_redirection_input_output(char* all_commands[], int command_count);

void execute_myls(char* all_commands[], int command_count);

void execute_io_redirection(char* all_commands[], int command_count);

void simple_i_o_redicrect(char* all_commands[], int command_count);

void execute_internal_external_commands(char* all_commands[], int command_count);

#endif /* ExecuteCommand_h */
