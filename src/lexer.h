#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_PRINT,
    TOKEN_GREATER,
    TOKEN_LESS,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS_EQUAL,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

typedef struct {
    char* src;
    size_t position;
    size_t length;
    char current_char;
} Lexer;

Lexer* init_lexer(char* src);
void advance(Lexer* lexer);
void skip_whitespace(Lexer* lexer);
Token* get_next_token(Lexer* lexer);
Token* create_token(TokenType type, char* value);
char* read_identifier(Lexer* lexer);
char* read_number(Lexer* lexer);
void free_lexer(Lexer* lexer);

#endif 