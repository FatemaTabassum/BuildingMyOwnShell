//
//  Parser.c
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#include "Parser.h"
#include "ExecuteCommand.h"

char *all_tokens[100];





// Need to tokenize first

void tokenize( char *str) {
    
    const char *s = " ";
    char *token = strtok(str, s);
    int idx = 0;
    while ( token != NULL) {
        //printf("%s \n",token);
        all_tokens[idx++] = token;
        token = strtok(NULL, s);
    }
//    for (int i = 0; i < idx; i++ ) {
//        printf("%d { %s }\n", i, all_tokens[i]);
//    }
    parse_tokens(all_tokens, idx);              // call to infer tokens
    return;
}






// parse and infer the meaning of tokens

void parse_tokens(char *all_tokens[], int idx) {
    
        const char * str = all_tokens[0];
        if ( strcmp(str, "pwd") == 0){
            
            print_current_working_directory();
            return;
            
        } else if (strcmp(str, "cd") == 0) {
            if (idx > 1) {
                change_current_directory(all_tokens[1]);
            }
        } else if (strcmp(str, "exit") == 0) {
            exit_shell();                           // exit code with command exit
            if (should_exit_shell == true) {
                return;
            }

        } else if (strcmp(str, "set") == 0) {
            
            set_env_variable(all_tokens[1]);
            
        } else if (strcmp(str, "ls") == 0) {
            
        }
    printf("\n");
}
