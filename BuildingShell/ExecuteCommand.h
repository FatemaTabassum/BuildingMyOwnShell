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

void print_current_working_directory(void);

void exit_shell(void);

void set_shell(void);

void go_to_directory(void);

void change_current_directory(const char *);

#endif /* ExecuteCommand_h */
