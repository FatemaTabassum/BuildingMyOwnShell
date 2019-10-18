#include <stdio.h>
#include <string.h>
#include "Parser.h"
#include "Header.h"

#define BUFFER_SIZE 1024

void signal_handler(int signo);
void join_signal(void);
void reset_values(char str[]);


char input_line[BUFFER_SIZE];
bool from_file = false;
FILE *file;
FILE *in_file;

void initialize_const () {
    should_exit_shell = false;
    //FILE *f = fopen(argv[1], "r");
}


int main(int argc, char *argv[]) {
    
    join_signal();
    initialize_const();
    printf("mybash-1.1$ ");
    if (argc > 1) {
        file = fopen(argv[1], "r");
        from_file = true;
        in_file = file;
    } else {
        in_file = stdin;
    }
    while (1) {
        
        
        if(fgets(input_line,BUFFER_SIZE,in_file) == NULL) {
            return(0);
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        if(feof(stdin)==true){
            signal_handler(SIGTERM);
            break;
        }
        
        if (from_file) {
            printf("%s\n", input_line);
        }
        
        tokenize(input_line);
        if (should_exit_shell == true) {
            return 0;
        }
        reset_values(input_line);
        printf("\nmybash-1.1$ ");
        
    }
    fclose(file);
    return 0;
}

void reset_values(char str[]) {
    reset_all_values_parser();
}

void join_signal() {
    if (signal(SIGINT, signal_handler) == SIG_ERR){
        //psignal(SIGINT, "Cannot join SIGINT due to");
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR){
        //psignal(SIGTERM, "Cannot join SIGTERM due to");
    }
}

void signal_handler(int signo) {
    if (signo == SIGINT) {
        exit(EXIT_SUCCESS);
    }
    if (signo == SIGTERM) {
        exit(EXIT_SUCCESS);
    }
}



