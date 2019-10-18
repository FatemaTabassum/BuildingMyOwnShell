//
//  Parser.c
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#include "Parser.h"
#include "ExecuteCommand.h"

char *all_tokens[1025];

bool is_pipe_command = false;
bool is_set_env_var_command = false;
bool is_io_redirection_command_output = false;
bool is_io_redirection_command_input = false;

void tokenize_command_with_delimiter(char *str, const char *s);

void reset_all_values_parser() {
    is_pipe_command = false;
    is_set_env_var_command = false;
    is_io_redirection_command_output = false;
    is_io_redirection_command_input = false;
    is_pipe_command = false;
    is_set_env_var_command = false;
    for (int i = 0; i < 1025; i++) {
        
        all_tokens[i] = "\0";
    }
}

char * truncate_leading_trailing_space(char *str) {
    const char * s = " ";
    char *token = strtok(str, s);
    return token;
}


void tokenize_command_with_redirect_output(char *str) {
    
    const char * s = REDIRECT_FROM_FILE_PERSPECTIVE_OUTPUT;
    char *token = strtok(str, s);
    int idx = 0;
    while ( token != NULL) {
        all_tokens[idx++] = token;
        token = strtok(NULL, s);
    }
    FOR_LOOP_FROM(1, idx) {
        //printf("%d [ %s ]\n", i, all_tokens[i]);
        all_tokens[i] = truncate_leading_trailing_space(all_tokens[i]);
    }
    parse_tokens(all_tokens, idx);
}

void tokenize_command_with_redirect_input(char *str) {
    
    const char * s = REDIRECT_FROM_FILE_PERSPECTIVE_INPUT;
    char *token = strtok(str, s);
    int idx = 0;
    while ( token != NULL) {
        all_tokens[idx++] = token;
        token = strtok(NULL, s);
    }
    FOR_LOOP_FROM(1, idx) {
        //printf("%d [ %s ]\n", i, all_tokens[i]);
        all_tokens[i] = truncate_leading_trailing_space(all_tokens[i]);
    }
    parse_tokens(all_tokens, idx);
    
}

void tokenize_command_with_redirect_input_output(char *str) {
    const char * s = REDIRECT_FROM_FILE_PERSPECTIVE_INPUT;
    char *token = strtok(str, s);
    int idx = 0;
    int idx2 = 0;
    char *new_tokens[100];
    while ( token != NULL) {
        new_tokens[idx++] = token;
        token = strtok(NULL, s);
    }
    all_tokens[idx2++] = new_tokens[0];
    char *new_string = new_tokens[1];
    new_tokens[0] = "\0";
    new_tokens[1] = "\0";
    idx = 0;
    
    s = REDIRECT_FROM_FILE_PERSPECTIVE_OUTPUT;
    token = strtok(new_string, s);
    while (token != NULL) {
        new_tokens[idx] = token;
        idx++;
        token = strtok(NULL, s);
    }
    FOR_LOOP(idx) {
        all_tokens[idx2++] = truncate_leading_trailing_space(new_tokens[i]);
    }
    
    parse_tokens(all_tokens, idx2);
}


void tokenize_command_with_delimiter(char *str, const char *s) {
    char *token = strtok(str, s);
    int idx = 0;
    while ( token != NULL) {
        all_tokens[idx++] = token;
        token = strtok(NULL, s);
    }
        for (int i = 0; i < idx; i++ ) {
            //printf("%d [ %s ]\n", i, all_tokens[i]);
        }
    parse_tokens(all_tokens, idx);
    return;
}




/* Need to tokenize first */
void tokenize( char *str) {
    
    
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '|') {
            is_pipe_command = true;
            break;
        } else if (str[i] == '=') {
            is_set_env_var_command = true;
            break;
        } else if (str[i] == '<') {
            is_io_redirection_command_input = true;
        } else if (str[i] == '>') {
            is_io_redirection_command_output = true;
        }
    }
    if (is_pipe_command) {
        
        tokenize_command_with_delimiter(str, PIPEDELIMITER);
        
    } else if (is_set_env_var_command) {
        
        tokenize_command_with_delimiter(str, MYPATHDELIMITER);
        
    } else if (is_io_redirection_command_output && is_io_redirection_command_input) {
        
        tokenize_command_with_redirect_input_output(str);
        
    } else if (is_io_redirection_command_output) { // <
        
        tokenize_command_with_redirect_output(str);
        
    } else if(is_io_redirection_command_input) { // >
        
        tokenize_command_with_redirect_input(str);
        
    } else {
        
        tokenize_command_with_delimiter(str, REGULARDELIMITER);
    
    }
}





/* parse and infer the meaning of tokens */
void parse_tokens(char *all_tokens[], int idx) {
    
    //FOR_LOOP_FROM(0, idx)
    //printf("%s\n", all_tokens[i]);
    
        const char * str = all_tokens[0];
        if ( strcmp(str, "pwd") == 0 &&
            !is_pipe_command &&
            !is_io_redirection_command_input &&
            !is_io_redirection_command_output ){
            
            print_current_working_directory();
            return;
            
        } else if (strcmp(str, "cd") == 0) {
            if (idx > 1) {
                change_current_directory(all_tokens[1]);
            }
        } else if (strcmp(str, "exit") == 0) {
            printf("exit\n");
            should_exit_shell = true;
            return;

        } else if (strcmp(str, "set MYPATH") == 0) {
            
            set_env_variable(all_tokens[1]);
            
        } else if (strcmp(str, "myls") == 0 &&
                   (!is_pipe_command && !is_io_redirection_command_input)) {
            
            execute_myls(all_tokens, idx);
            
        } else if (is_pipe_command) {
            
            execute_pipe(all_tokens, idx);
            
        } else if (is_io_redirection_command_input && is_io_redirection_command_output) {
            
            execute_io_redirection_input_output (all_tokens, idx);
            
        } else if (is_io_redirection_command_input) {
            
            execute_io_redirection_input (all_tokens, idx);
            
        } else if (is_io_redirection_command_output) {
            
            execute_io_redirection_output (all_tokens, idx);
            
        }  else {
            execute_internal_external_commands(all_tokens, idx);
        }
//        else if (strcmp(str, "grep") == 0) {
//            simple_i_o_redicrect(all_tokens, idx);
//
//        }
    
}
