#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    AST_PROGRAM,
    AST_VARIABLE,
    AST_NUMBER,
    AST_BINARY_OP,
    AST_ASSIGN,
    AST_IF,
    AST_PRINT
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct {
            struct ASTNode** statements;
            size_t statement_count;
        } program;
        
        struct {
            char* name;
        } variable;
        
        struct {
            int value;
        } number;
        
        struct {
            char op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary_op;
        
        struct {
            char* name;
            struct ASTNode* value;
        } assign;
        
        struct {
            struct ASTNode* condition;
            struct ASTNode* if_body;
            struct ASTNode* else_body;
        } if_statement;
        
        struct {
            struct ASTNode* expression;
        } print;
    } data;
} ASTNode;

typedef struct {
    Lexer* lexer;
    Token* current_token;
} Parser;

Parser* init_parser(Lexer* lexer);
void advance_parser(Parser* parser);
void eat(Parser* parser, TokenType type);
ASTNode* parse(Parser* parser);
ASTNode* program(Parser* parser);
ASTNode* statement(Parser* parser);
ASTNode* expression(Parser* parser);
ASTNode* term(Parser* parser);
ASTNode* factor(Parser* parser);
ASTNode* create_ast_node(ASTNodeType type);
void free_ast(ASTNode* node);
void free_parser(Parser* parser);

#endif 