#include <stdio.h>
#include <string.h>
#include "Parser.h"
#include "Header.h"

void initialize_const () {
    should_exit_shell = false;
}

int main() {
//    step1();
//    step2();
//    step3();
    char str[100] = "ls -l -h -a";
    initialize_const();
    printf("$ ");
    while (scanf("%[^\n]%*c", str) != EOF) {
        printf("%s\n", str);
        tokenize(str);
        if (should_exit_shell == true) {
            return 0;
        }
        printf("$ ");
    }
    return 0;
}
