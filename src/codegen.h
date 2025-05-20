#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

char* generate_code(ASTNode* node);
void free_code(char* code);

#endif 