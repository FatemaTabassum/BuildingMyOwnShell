//
//  Parser.h
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#ifndef Parser_h
#define Parser_h

#include <stdio.h>
#include <string.h>
#include "Header.h"
#include "ExecuteCommand.h"


extern bool is_pipe_command;
extern bool is_set_env_var_command;
extern bool is_io_redirection_command_output;
extern bool is_io_redirection_command_input;


void tokenize( char *str);
void parse_tokens(char *all_tokens[], int idx);
void reset_all_values_parser();

#endif /* Parser_h */
