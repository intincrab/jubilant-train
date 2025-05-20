#include "parser.h"
#include <stdio.h>

Parser* init_parser(Lexer* lexer) {
    Parser* parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    return parser;
}

void advance_parser(Parser* parser) {
    parser->current_token = get_next_token(parser->lexer);
}

void eat(Parser* parser, TokenType type) {
    if (parser->current_token->type == type) {
        advance_parser(parser);
    } else {
        fprintf(stderr, "Syntax error: Expected token type %d, got %d\n", 
                type, parser->current_token->type);
        exit(1);
    }
}

ASTNode* create_ast_node(ASTNodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode* factor(Parser* parser) {
    Token* token = parser->current_token;
    
    if (token->type == TOKEN_NUMBER) {
        eat(parser, TOKEN_NUMBER);
        ASTNode* node = create_ast_node(AST_NUMBER);
        node->data.number.value = atoi(token->value);
        free(token->value);
        free(token);
        return node;
    } else if (token->type == TOKEN_LPAREN) {
        eat(parser, TOKEN_LPAREN);
        ASTNode* node = expression(parser);
        eat(parser, TOKEN_RPAREN);
        return node;
    } else if (token->type == TOKEN_ID) {
        eat(parser, TOKEN_ID);
        ASTNode* node = create_ast_node(AST_VARIABLE);
        node->data.variable.name = token->value;
        free(token);
        return node;
    }
    
    fprintf(stderr, "Syntax error: Unexpected token in factor\n");
    exit(1);
}

ASTNode* term(Parser* parser) {
    ASTNode* node = factor(parser);
    
    while (parser->current_token->type == TOKEN_MULTIPLY || 
           parser->current_token->type == TOKEN_DIVIDE) {
        Token* token = parser->current_token;
        
        if (token->type == TOKEN_MULTIPLY) {
            eat(parser, TOKEN_MULTIPLY);
        } else if (token->type == TOKEN_DIVIDE) {
            eat(parser, TOKEN_DIVIDE);
        }
        
        ASTNode* binary_op = create_ast_node(AST_BINARY_OP);
        binary_op->data.binary_op.op = token->value[0];
        binary_op->data.binary_op.left = node;
        binary_op->data.binary_op.right = factor(parser);
        
        free(token->value);
        free(token);
        
        node = binary_op;
    }
    
    return node;
}

ASTNode* arithmetic_expr(Parser* parser) {
    ASTNode* node = term(parser);
    
    while (parser->current_token->type == TOKEN_PLUS || 
           parser->current_token->type == TOKEN_MINUS) {
        Token* token = parser->current_token;
        
        if (token->type == TOKEN_PLUS) {
            eat(parser, TOKEN_PLUS);
        } else if (token->type == TOKEN_MINUS) {
            eat(parser, TOKEN_MINUS);
        }
        
        ASTNode* binary_op = create_ast_node(AST_BINARY_OP);
        binary_op->data.binary_op.op = token->value[0];
        binary_op->data.binary_op.left = node;
        binary_op->data.binary_op.right = term(parser);
        
        free(token->value);
        free(token);
        
        node = binary_op;
    }
    
    return node;
}

ASTNode* comparison_expr(Parser* parser) {
    ASTNode* node = arithmetic_expr(parser);
    
    // Handle comparison operators
    if (parser->current_token->type == TOKEN_GREATER ||
        parser->current_token->type == TOKEN_LESS ||
        parser->current_token->type == TOKEN_EQUAL ||
        parser->current_token->type == TOKEN_NOT_EQUAL ||
        parser->current_token->type == TOKEN_GREATER_EQUAL ||
        parser->current_token->type == TOKEN_LESS_EQUAL) {
        
        Token* token = parser->current_token;
        char op_char;
        
        if (token->type == TOKEN_GREATER) {
            op_char = '>';
            eat(parser, TOKEN_GREATER);
        } else if (token->type == TOKEN_LESS) {
            op_char = '<';
            eat(parser, TOKEN_LESS);
        } else if (token->type == TOKEN_EQUAL) {
            op_char = '=';
            eat(parser, TOKEN_EQUAL);
        } else if (token->type == TOKEN_NOT_EQUAL) {
            op_char = '!';
            eat(parser, TOKEN_NOT_EQUAL);
        } else if (token->type == TOKEN_GREATER_EQUAL) {
            op_char = 'G'; // Special marker for >=
            eat(parser, TOKEN_GREATER_EQUAL);
        } else if (token->type == TOKEN_LESS_EQUAL) {
            op_char = 'L'; // Special marker for <=
            eat(parser, TOKEN_LESS_EQUAL);
        }
        
        ASTNode* binary_op = create_ast_node(AST_BINARY_OP);
        binary_op->data.binary_op.op = op_char;
        binary_op->data.binary_op.left = node;
        binary_op->data.binary_op.right = arithmetic_expr(parser);
        
        free(token->value);
        free(token);
        
        node = binary_op;
    }
    
    return node;
}

ASTNode* expression(Parser* parser) {
    return comparison_expr(parser);
}

ASTNode* statement(Parser* parser) {
    if (parser->current_token->type == TOKEN_ID) {
        char* var_name = strdup(parser->current_token->value);
        eat(parser, TOKEN_ID);
        
        if (parser->current_token->type == TOKEN_ASSIGN) {
            eat(parser, TOKEN_ASSIGN);
            ASTNode* node = create_ast_node(AST_ASSIGN);
            node->data.assign.name = var_name;
            node->data.assign.value = expression(parser);
            eat(parser, TOKEN_SEMICOLON);
            return node;
        } else {
            fprintf(stderr, "Syntax error: Expected assignment operator\n");
            exit(1);
        }
    } else if (parser->current_token->type == TOKEN_IF) {
        eat(parser, TOKEN_IF);
        eat(parser, TOKEN_LPAREN);
        ASTNode* condition = expression(parser);
        eat(parser, TOKEN_RPAREN);
        
        eat(parser, TOKEN_LBRACE);
        ASTNode* if_body = program(parser);
        eat(parser, TOKEN_RBRACE);
        
        ASTNode* else_body = NULL;
        if (parser->current_token->type == TOKEN_ELSE) {
            eat(parser, TOKEN_ELSE);
            eat(parser, TOKEN_LBRACE);
            else_body = program(parser);
            eat(parser, TOKEN_RBRACE);
        }
        
        ASTNode* node = create_ast_node(AST_IF);
        node->data.if_statement.condition = condition;
        node->data.if_statement.if_body = if_body;
        node->data.if_statement.else_body = else_body;
        
        return node;
    } else if (parser->current_token->type == TOKEN_PRINT) {
        eat(parser, TOKEN_PRINT);
        eat(parser, TOKEN_LPAREN);
        ASTNode* expr = expression(parser);
        eat(parser, TOKEN_RPAREN);
        eat(parser, TOKEN_SEMICOLON);
        
        ASTNode* node = create_ast_node(AST_PRINT);
        node->data.print.expression = expr;
        
        return node;
    } else {
        fprintf(stderr, "Syntax error: Invalid statement\n");
        exit(1);
    }
}

ASTNode* program(Parser* parser) {
    ASTNode* node = create_ast_node(AST_PROGRAM);
    node->data.program.statements = malloc(sizeof(ASTNode*) * 10); // Start with space for 10 statements
    node->data.program.statement_count = 0;
    size_t capacity = 10;
    
    while (parser->current_token->type != TOKEN_EOF && 
           parser->current_token->type != TOKEN_RBRACE) {
        
        if (node->data.program.statement_count >= capacity) {
            capacity *= 2;
            node->data.program.statements = realloc(
                node->data.program.statements, 
                sizeof(ASTNode*) * capacity
            );
        }
        
        node->data.program.statements[node->data.program.statement_count++] = statement(parser);
    }
    
    return node;
}

ASTNode* parse(Parser* parser) {
    return program(parser);
}

void free_ast(ASTNode* node) {
    if (node == NULL) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            for (size_t i = 0; i < node->data.program.statement_count; i++) {
                free_ast(node->data.program.statements[i]);
            }
            free(node->data.program.statements);
            break;
            
        case AST_VARIABLE:
            free(node->data.variable.name);
            break;
            
        case AST_BINARY_OP:
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            break;
            
        case AST_ASSIGN:
            free(node->data.assign.name);
            free_ast(node->data.assign.value);
            break;
            
        case AST_IF:
            free_ast(node->data.if_statement.condition);
            free_ast(node->data.if_statement.if_body);
            if (node->data.if_statement.else_body) {
                free_ast(node->data.if_statement.else_body);
            }
            break;
            
        case AST_PRINT:
            free_ast(node->data.print.expression);
            break;
            
        default:
            break;
    }
    
    free(node);
}

void free_parser(Parser* parser) {
    free(parser);
}