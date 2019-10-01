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


void tokenize( char *str);
void parse_tokens(char *all_tokens[], int idx);

#endif /* Parser_h */
