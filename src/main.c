#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = malloc(file_size + 1);
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';
    
    fclose(file);
    return buffer;
}

char* compile_string(const char* source) {
    Lexer* lexer = init_lexer(strdup(source));
    Parser* parser = init_parser(lexer);
    
    ASTNode* ast = parse(parser);
    char* output = generate_code(ast);
    
    free_ast(ast);
    free_parser(parser);
    free_lexer(lexer);
    
    return output;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
char* compile(const char* source) {
    return compile_string(source);
}

EMSCRIPTEN_KEEPALIVE
void free_result(char* result) {
    free_code(result);
}
#endif

int main(int argc, char** argv) {
#ifndef __EMSCRIPTEN__
    if (argc < 2) {
        printf("Usage: %s <input_file> [output_file]\n", argv[0]);
        return 1;
    }
    
    char* source = read_file(argv[1]);
    if (!source) {
        return 1;
    }
    
    char* output = compile_string(source);
    free(source);
    
    if (argc >= 3) {
        FILE* file = fopen(argv[2], "w");
        if (!file) {
            fprintf(stderr, "Error: Could not open output file %s\n", argv[2]);
            free_code(output);
            return 1;
        }
        
        fprintf(file, "%s", output);
        fclose(file);
    } else {
        printf("%s\n", output);
    }
    
    free_code(output);
#endif
    
    return 0;
} 