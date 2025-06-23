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

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_ID: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_PRINT: return "PRINT";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_LESS: return "LESS";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

char* tokenize_string(const char* source) {
    Lexer* lexer = init_lexer(strdup(source));
    
    // Estimate buffer size (generous allocation)
    size_t buffer_size = strlen(source) * 10 + 1000;
    char* json_buffer = malloc(buffer_size);
    strcpy(json_buffer, "[");
    
    Token* token;
    int first = 1;
    
    while ((token = get_next_token(lexer))->type != TOKEN_EOF) {
        if (!first) {
            strcat(json_buffer, ",");
        }
        first = 0;
        
        // Add token object to JSON
        char token_json[500];
        if (token->value) {
            // Escape quotes in value
            char escaped_value[200];
            int j = 0;
            for (int i = 0; token->value[i] && j < 198; i++) {
                if (token->value[i] == '"' || token->value[i] == '\\') {
                    escaped_value[j++] = '\\';
                }
                escaped_value[j++] = token->value[i];
            }
            escaped_value[j] = '\0';
            
            snprintf(token_json, sizeof(token_json), 
                "{\"type\":\"%s\",\"value\":\"%s\"}", 
                token_type_to_string(token->type), escaped_value);
        } else {
            snprintf(token_json, sizeof(token_json), 
                "{\"type\":\"%s\",\"value\":null}", 
                token_type_to_string(token->type));
        }
        
        strcat(json_buffer, token_json);
        
        // Free token
        if (token->value) free(token->value);
        free(token);
    }
    
    // Handle EOF token
    if (!first) {
        strcat(json_buffer, ",");
    }
    strcat(json_buffer, "{\"type\":\"EOF\",\"value\":null}");
    strcat(json_buffer, "]");
    
    // Free EOF token
    if (token->value) free(token->value);
    free(token);
    
    free_lexer(lexer);
    
    return json_buffer;
}

char* parse_to_ast(const char* source) {
    Lexer* lexer = init_lexer(strdup(source));
    Parser* parser = init_parser(lexer);
    
    ASTNode* ast = parse(parser);
    char* json = ast_to_json(ast);
    
    free_ast(ast);
    free_parser(parser);
    free_lexer(lexer);
    
    return json;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
char* compile(const char* source) {
    return compile_string(source);
}

EMSCRIPTEN_KEEPALIVE
char* tokenize(const char* source) {
    return tokenize_string(source);
}

EMSCRIPTEN_KEEPALIVE
char* parse_ast(const char* source) {
    return parse_to_ast(source);
}

EMSCRIPTEN_KEEPALIVE
void free_result(char* result) {
    free_code(result);
}

EMSCRIPTEN_KEEPALIVE
void free_tokens(char* tokens) {
    free(tokens);
}

EMSCRIPTEN_KEEPALIVE
void free_ast_json(char* ast_json) {
    free(ast_json);
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
