#include <stdio.h>
#include <string.h>
#include "Parser.h"
#include "Header.h"

void signal_handler(int signo);
void join_signal(void);
void reset_values(char str[]);


char input_line[1024];


void initialize_const () {
    should_exit_shell = false;
}






int main() {

    initialize_const();
    printf("mybash-1.1$ ");
    while (1) {
    
        
        if(!fgets(input_line,1024,stdin)) {
            printf("exit\n");
            return(0);
        }
        input_line[strcspn(input_line, "\n")] = '\0';
        if(feof(stdin)==true){
            signal_handler(SIGTERM);
            break;
        }
        
        tokenize(input_line);
        if (should_exit_shell == true) {
            return 0;
        }
        reset_values(input_line);
        printf("mybash-1.1$ ");
        
}
    return 0;
}

#pragma mark reset
void reset_values(char str[]) {
    reset_all_values_parser();
    //reset_execute();
//    for (int i = 0; i < 1000; i++) {
//        str[i] = '\0';
//    }
}







void join_signal() {
    if (signal(SIGINT, signal_handler) == SIG_ERR)
        //psignal(SIGINT, "Cannot join SIGINT due to");
    if (signal(SIGTERM, signal_handler) == SIG_ERR)
        //psignal(SIGTERM, "Cannot join SIGTERM due to");
}

void signal_handler(int signo) {
    if (signo == SIGINT) {
        exit(EXIT_SUCCESS);
    }
    if (signo == SIGTERM) {
        exit(EXIT_SUCCESS);
    }
}



