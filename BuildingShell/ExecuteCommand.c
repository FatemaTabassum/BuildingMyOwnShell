//
//  ExecuteCommand.c
//  BuildingShell
//
//  Created by Fatema on 10/1/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#include "ExecuteCommand.h"

#define MAX_ARGUMENTS 10
#define MAX_PATH_SIZE 1024

void simple_i_o_redicrect(char* all_commands[], int command_count);


static char *arguments_v[MAX_ARGUMENTS];
static int arguments_cnt;
static char curr_working_dir[MAX_PATH_SIZE];


void reset_all_values_Execute() {
    FOR_LOOP(MAX_PATH_SIZE) {
        curr_working_dir[i] = '\0';
    }
}


void get_current_working_directory(void) {
    char * cur = getcwd(curr_working_dir, sizeof(curr_working_dir)); // it seems currentworkingdirectory is set in the sent argument
    if (cur == NULL) {
        perror("cur directory");
        //exit(EXIT_FAILURE);
    }
    return ;
}


void get_all_arguments_of_commands(char * str) {
    // set initial values. needed when already there is value
    arguments_cnt = 0;
    FOR_LOOP(MAX_ARGUMENTS) {
        arguments_v[i] = "\0";
    }
    const char * s = " ";
    char *token = strtok(str, s);
    int idx = 0;
    while ( token != NULL) {
//        FOR_LOOP(strlen(token)) {
//            printf("%c", token[i]);
//        } printf("\n");
        arguments_v[idx] = token;
        arguments_v[idx][strlen(arguments_v[idx])] = '\0';
        idx++;
        token = strtok(NULL, s);
    }
    arguments_v[idx] = NULL;
    arguments_cnt = idx;
    return ;
}

#pragma mark PIPE
void execute_pipe(char *all_commands[],  int num_of_commnds) {
    
    int first_command = 0;
    int last_command = num_of_commnds - 1;
    
    int prev_pipe_fd[2];
    int new_pipe_fd[2];
    
    for (int i = 0; i < num_of_commnds; i++) {
        
        //printf("commands = %s\n", all_commands[i]);
        get_all_arguments_of_commands(all_commands[i]);
        int pid;
        if (i < last_command) {
            if (pipe(new_pipe_fd) == ERROR_CODE) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        pid = fork();
        if (pid == ERROR_CODE)  {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == CHILD_PROCESS) {
            /* Child Process */
            if (i == first_command) {
                
            close(new_pipe_fd[READ]);
            dup2(new_pipe_fd[WRITE], WRITE);
            close(new_pipe_fd[WRITE]);
                
        } else if (i == last_command) {
            
            dup2(prev_pipe_fd[READ], READ);
            close(prev_pipe_fd[READ]);
    
        } else if( i > first_command && i < last_command){
            
            dup2(prev_pipe_fd[READ], READ);
            close(prev_pipe_fd[READ]);

            //close(prev_pipe_fd[WRITE]);

            close(new_pipe_fd[READ]);
            dup2(new_pipe_fd[WRITE], WRITE);
            close(new_pipe_fd[WRITE]);
        }
            if (!strcmp(arguments_v[0], MYLS)) {
                execute_myls(arguments_v, arguments_cnt);
            } else if(execvp(arguments_v[0],arguments_v ) == ERROR_CODE) {
                fprintf(stderr, "%s: command not found",arguments_v[0]);
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
            
        } else {
            /* Parent Process */
            int status;
            waitpid(pid, &status, 0);
            if (i == first_command) {
                close(new_pipe_fd[WRITE]);
                prev_pipe_fd[READ] = new_pipe_fd[READ];
            }
            else if (i > first_command && i < last_command) {
                
                close(prev_pipe_fd[READ]);
                prev_pipe_fd[READ] = new_pipe_fd[READ];
                close(new_pipe_fd[WRITE]);
                
            }
            
            else if (i == last_command) {
                close(prev_pipe_fd[READ]);
            }
        }
    }
}



#pragma mark I/O Redirection

void execute_io_redirection_input(char* all_commands[], int command_count) {
    
    get_current_working_directory();
    get_all_arguments_of_commands(all_commands[0]);
    char input_redirection_file[1024];
    pid_t pid ;
    
    
    
    if (all_commands[1][0] != '/') {
        strcpy(input_redirection_file, curr_working_dir);
        if (input_redirection_file[strlen(input_redirection_file) - 1] != '/') {
            strcat(input_redirection_file, "/");
        }
        strcat(input_redirection_file, (char *)all_commands[1]);
        input_redirection_file[strlen(input_redirection_file)] = '\0';
    } else {
        strcpy(input_redirection_file, all_commands[1]);
        input_redirection_file[strlen(input_redirection_file)] = '\0';
    }
    
    if ((pid = fork()) == ERROR_CODE) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == CHILD_PROCESS) {
        
//        strcpy(input_redirection_file, curr_working_dir);
//        strcat(input_redirection_file, "/");
//        strcat(input_redirection_file, (char *)all_commands[1]);
//        input_redirection_file[strlen(input_redirection_file)] = '\0';
        
        
        
        int file_d = open(input_redirection_file, O_RDONLY, 0000);
        if (file_d == ERROR_CODE) {
            fprintf(stderr, "Failed to open %s for reading\n", input_redirection_file); // print perfect format
            exit(EXIT_FAILURE);
        }
        //char *arguments[10] = {"ls"};
        dup2(file_d, READ);
        close(file_d);
        if (execvp(arguments_v[0], arguments_v) < 0) {
            fprintf(stderr,"%s: command not found\n", arguments_v[0]);
        }
        exit(EXIT_SUCCESS);
    } else {
        
        int wait_status;
        waitpid(pid, &wait_status, 0);
    }
    
}

void execute_io_redirection_output(char* all_commands[], int command_count) {

    get_current_working_directory();
    get_all_arguments_of_commands(all_commands[0]);
    char output_redirection_file[1024];
    pid_t pid;
    
    
    if (all_commands[1][0] != '/') {
        strcpy(output_redirection_file, curr_working_dir);
        if (output_redirection_file[strlen(output_redirection_file) - 1] != '/') {
            strcat(output_redirection_file, "/");
        }
        strcat(output_redirection_file, (char *)all_commands[1]);
        output_redirection_file[strlen(output_redirection_file)] = '\0';
    } else {
        strcpy(output_redirection_file, all_commands[1]);
        output_redirection_file[strlen(output_redirection_file)] = '\0';
    }
    
    if ((pid = fork()) == ERROR_CODE) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == CHILD_PROCESS) {
        
//        strcpy(output_redirection_file, curr_working_dir);
//        strcat(output_redirection_file, "/");
//        strcat(output_redirection_file, (char *)all_commands[1]);
//        output_redirection_file[strlen(output_redirection_file)] = '\0';
        
        if( access( output_redirection_file, F_OK ) != -1 ) {
            
            remove(output_redirection_file);
        } else {
            // file doesn't exist
        }
        
        int file_d = open(output_redirection_file, O_CREAT | O_RDWR, 0777);
        if (file_d == ERROR_CODE) {
            fprintf(stderr, "Failed to open %s for writing\n", output_redirection_file);
            exit(EXIT_FAILURE);
        }
        
        
        if(dup2(file_d, WRITE) == ERROR_CODE) {
            fprintf(stderr, "dup error write %s", strerror(errno));
        }
        close(file_d);
        if (execvp(arguments_v[0], arguments_v) < 0) {
            fprintf(stderr,"%s: command not found\n", arguments_v[0]);
        }
        exit(EXIT_SUCCESS);
    } else {
        
        int wait_status;
        waitpid(pid, &wait_status, 0);
        
    }
    
}

void execute_io_redirection_input_output(char* all_commands[], int command_count) {
    
   
    get_current_working_directory();
    get_all_arguments_of_commands(all_commands[0]);
    char input_redirection_file[1024];
    char output_redirection_file[1024];
    
    pid_t pid ;
    
    
    if (all_commands[1][0] != '/') {
        strcpy(input_redirection_file, curr_working_dir);
        if (input_redirection_file[strlen(input_redirection_file) - 1] != '/') {
            strcat(input_redirection_file, "/");
        }
        strcat(input_redirection_file, (char *)all_commands[1]);
        input_redirection_file[strlen(input_redirection_file)] = '\0';
    } else {
        strcpy(input_redirection_file, all_commands[1]);
        input_redirection_file[strlen(input_redirection_file)] = '\0';
    }
    
//    strcpy(input_redirection_file, curr_working_dir);
//    strcat(input_redirection_file, "/");
//    strcat(input_redirection_file, (char *)all_commands[1]);
//    input_redirection_file[strlen(input_redirection_file)] = '\0';
    
    
    
    
    if (all_commands[2][0] != '/') {
        strcpy(output_redirection_file, curr_working_dir);
        if (output_redirection_file[strlen(output_redirection_file) - 1] != '/') {
            strcat(output_redirection_file, "/");
        }
        strcat(output_redirection_file, (char *)all_commands[2]);
        output_redirection_file[strlen(output_redirection_file)] = '\0';
    } else {
        strcpy(output_redirection_file, all_commands[2]);
        output_redirection_file[strlen(output_redirection_file)] = '\0';
    }
    
    
//    strcpy(output_redirection_file, curr_working_dir);
//    strcat(output_redirection_file, "/");
//    strcat(output_redirection_file, (char*)all_commands[2]);
//    output_redirection_file[strlen(output_redirection_file)] = '\0';
    
    if ((pid = fork()) == ERROR_CODE) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == CHILD_PROCESS) {
        
        int file_in_d = open(input_redirection_file, O_RDONLY, 0000);
        if (file_in_d == ERROR_CODE) {
            fprintf(stderr, "Failed to open %s for reading\n", input_redirection_file); // print perfect format
            exit(EXIT_FAILURE);
        }
        dup2(file_in_d, READ);
        close(file_in_d);
        
        if( access( output_redirection_file, F_OK ) != -1 ) {
            
            remove(output_redirection_file);
        } else {
            // file doesn't exist
        }
        
        
        int file_out_d = open(output_redirection_file, O_CREAT | O_RDWR | O_TRUNC, 0777);
        if (file_out_d == ERROR_CODE) {
            fprintf(stderr, "Failed to open %s for reading\n", output_redirection_file);
            exit(EXIT_FAILURE);
        }
        dup2(file_out_d, WRITE);
        close(file_out_d);
        
        if (execvp(arguments_v[0], arguments_v) < 0) {
            fprintf(stderr,"%s: command not found\n", arguments_v[0]);
        }
        exit(EXIT_SUCCESS);
    } else {
        
        int wait_status;
        waitpid(pid, &wait_status, 0);
    }
    
}









#pragma mark Print Current working Directory
void print_current_working_directory(void) {
    char curr_working_dir[1024];
    if(getcwd(curr_working_dir, sizeof(curr_working_dir)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    printf("%s", curr_working_dir);
    PRINT_NEWLINE;
    return;
}

#pragma mark Shell Exit
void exit_shell(void) {
    should_exit_shell = true;
    PRINT_NEWLINE;
    //check_all_the_processes_to_kill
    return;
}

#pragma mark Set Shell
void set_shell(void) {
    
}

bool check_if_path_exist(const char *path) {
    const char* folder;
    //folder = "C:\\Users\\SaMaN\\Desktop\\Ppln";
    folder = path;
    struct stat sb;

    if (stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        //printf("YES\n");
        return true;
    } else {
        //printf("NO\n");
        return false;
    }
}



#pragma mark Set Environment Variable
void set_env_variable(char * env_var_path) {
    
    char s[2] = "\\";
    char *all_str = strtok(env_var_path, s);
    char *all_path[100];
    int idx = 0;
    while (all_str != NULL) {
        all_path[idx] = all_str;
        idx++;
        all_str = strtok(NULL, s);
    }
    char newpath[200];
    int ind = 0;
    for (int i = 0; i < idx; i++) {
        for (int j = 0; j < strlen(all_path[i]); j++) {
            newpath[ind] = (char)all_path[i][j];
            ind++;
        }
    }
    newpath[ind] = '\0';
    //set MYPATH=/Volumes/Liza\ Disk/FSU/CPDP/Codes/MylsImplement/MylsImplement/myls
    if ((setenv(mypath, newpath, 1)) == ERROR) {
        fprintf(stderr, "Error set Environment %s\n", strerror(errno));
        return;
    }
    //printf("getenv_var = %s\n", getenv(mypath));
}


#pragma mark Change Directory
//need to recheck
char * change_to_home_directory(const char * path) {
    char * home_path = getenv("HOME");
    int len_home  = (int)strlen(home_path);
    int len_path_ = (int)strlen(path);
    char * new_full_path
    = malloc((len_home + len_path_ + 5) * sizeof(char));
    strcpy(new_full_path, home_path);
    for (int i = 1; i < len_path_; i++) {
        new_full_path[len_home + i -1 ] = path[i];
    }
    new_full_path[len_home + len_path_] = '\0';
    
    //printf(" ****************** %s\n", new_full_path);
    return new_full_path;
    
}

void change_current_directory(const char * dir_path) {
    if (dir_path[0] == '~' || (dir_path[0]=='~' && dir_path[1]=='/')) {
        dir_path = change_to_home_directory(dir_path);
    }
    int ch_dr_flg = chdir(dir_path);
    if (ch_dr_flg != 0) {
        char yourstring[100];
        snprintf(yourstring, sizeof yourstring, "cd: %s", dir_path);
        perror(yourstring);
    }
}

#pragma mark Myls
char *full_path_name;

void go_to_path(char *path) {
    full_path_name = (char*) malloc(strlen((path) + 3) * sizeof(char));
    strcpy(full_path_name,path);
    if (full_path_name[strlen(full_path_name)-1] != '/') {
        strcat(full_path_name, "/");
    }
    strcat(full_path_name, "myls");
    strcat(full_path_name, "\0");
}


char ** generate_commands_myls_from(char * all_commands[], int command_cnt) {
    
    //input = (char**)malloc(numberOfTokens+1*sizeof(char*));
    //input[numberOfTokens] = (char *)malloc((strlen(temp)+1)*sizeof(char));
    
    char **argv = (char **)malloc(NUMBER_OF_TOKENS_MYLS * sizeof(char*));
    int indx = 0;
    argv[indx] = (char *) malloc(20 * sizeof(char));
    strcpy(argv[indx], "/myls");
    FOR_LOOP_FROM(1, command_cnt+1) {
        argv[i] = (char *) malloc(strlen((all_commands[i-1]) + 5) * sizeof(char));
        argv[i] = all_commands[i-1];
    }
    argv[command_cnt+1] = NULL;
    return argv;
}


void execute_myls(char* all_commands[], int command_count) {
    all_commands[0] = "ls";
    all_commands[command_count] = NULL;
    //set_env_variable(myls_path);
    char *myls_path =  getenv(mypath);
    if (myls_path == NULL) {
        get_current_working_directory();
        go_to_path(curr_working_dir);
        myls_path = strdup(full_path_name);
    } else {
        go_to_path(myls_path);
        myls_path = strdup(full_path_name);
        chmod(myls_path, 0740);
    }
    char **argv = generate_commands_myls_from(all_commands, command_count);
    
    pid_t pid_ls = fork();
    if (pid_ls == ERROR) {
        perror("Fork");
        strerror(errno);
    } else if (pid_ls == CHILDFORK) {
        //check_if_path_exist(myls_path);
        execv(myls_path, argv);
        perror("Return from execvp() not expected");
        exit(EXIT_FAILURE);
    } else {
        // Parent // wait hocchena thik moto. terminate hoye jacche keno 2nd input er khetre
        int wait_status;
        waitpid(pid_ls, &wait_status, 0);
        if (WIFEXITED(wait_status)) {
            //psignal(WTERMSIG(stat), "Child terminated due to");
        }
        else if (WIFSIGNALED(wait_status)) {
            //psignal(WTERMSIG(stat), "Child terminated due to");
        } else if (WTERMSIG(wait_status)) {
            //psignal(WTERMSIG(stat), "Child term due to");
        } else if (WSTOPSIG(wait_status)) {
            //psignal(WTERMSIG(stat), "Child term due to");
        }
        for (int i = 0; i <= command_count; i++) {
            all_commands[i] = NULL;
            
        }
    }
}




void execute_internal_external_commands(char* all_commands[], int command_count) {
    char **argv = (char **) malloc((command_count + 2) * sizeof(char*));
    FOR_LOOP(command_count) {
        argv[i] = (char *) malloc((strlen(all_commands[i]) + 3) * sizeof(char));
        argv[i] = all_commands[i];
    }
    argv[command_count] = NULL;
    pid_t pid = fork();
    if (pid == ERROR_CODE) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == CHILD_PROCESS) {
    
        if(execvp(argv[0], argv) < 0) {
            perror("execvp");
        }
        exit(EXIT_SUCCESS);
    } else { // Parent //
        waitpid(pid, 0, 0);
    }
}


#pragma mark Sample I/O Redirect
void simple_i_o_redicrect(char* all_commands[], int command_count) {
    
    get_current_working_directory();
    get_all_arguments_of_commands(all_commands[0]);
    
    
    //{"grep", "-i", "Unix", "/Users/liza/file2.txt",NULL};
    char *argv[10] = {"grep", "-i", "Unix", "/Users/liza/file2.txt",NULL};
    int fd[2];
    if (pipe(fd) == ERROR_CODE) {
        perror("Pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if (pid == ERROR_CODE) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == CHILD_PROCESS) {
        //char *arguments[10] = {"ls"};
        close(fd[READ]);
        dup2(fd[WRITE], WRITE);
        close(fd[WRITE]);
        if(execvp(argv[0], argv) < 0) {
            perror("execvp");
            
        }
        exit(EXIT_SUCCESS);
    } else {
        
        close(fd[WRITE]);
        dup2(fd[READ], READ);
        close(fd[READ]);
        
        
        
//        char st[1024];
      
        //char *msg[100];
//        int nbytes;
        //scanf("%s", msg);
//        while ((nbytes = read(STDIN_FILENO, msg, sizeof(msg))) > 0) {
//            printf("...............%s........\n", msg);
//            //int nbytes = read(fd[0], msg, sizeof(msg));
//            //write(file_d, msg , sizeof(msg));
//
//        }
        //printf("Output: (%.*s)\n", nbytes, msg);
        
        //close(file_d);
    }
    
}



