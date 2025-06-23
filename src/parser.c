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

const char* ast_node_type_to_string(ASTNodeType type) {
    switch (type) {
        case AST_PROGRAM: return "PROGRAM";
        case AST_VARIABLE: return "VARIABLE";
        case AST_NUMBER: return "NUMBER";
        case AST_BINARY_OP: return "BINARY_OP";
        case AST_ASSIGN: return "ASSIGN";
        case AST_IF: return "IF";
        case AST_PRINT: return "PRINT";
        default: return "UNKNOWN";
    }
}

char* escape_json_string(const char* str) {
    if (!str) return strdup("null");
    
    size_t len = strlen(str);
    char* escaped = malloc(len * 2 + 3); // Worst case: all chars need escaping + quotes
    strcpy(escaped, "\"");
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '"') {
            strcat(escaped, "\\\"");
        } else if (str[i] == '\\') {
            strcat(escaped, "\\\\");
        } else if (str[i] == '\n') {
            strcat(escaped, "\\n");
        } else if (str[i] == '\r') {
            strcat(escaped, "\\r");
        } else if (str[i] == '\t') {
            strcat(escaped, "\\t");
        } else {
            size_t current_len = strlen(escaped);
            escaped[current_len] = str[i];
            escaped[current_len + 1] = '\0';
        }
    }
    
    strcat(escaped, "\"");
    return escaped;
}

char* ast_to_json_recursive(ASTNode* node, int depth) {
    if (!node) return strdup("null");
    
    // Calculate initial buffer size
    size_t buffer_size = 1024;
    char* json = malloc(buffer_size);
    json[0] = '\0';
    
    // Helper function to ensure buffer size
    #define ENSURE_BUFFER_SIZE(needed) \
        if (strlen(json) + (needed) >= buffer_size) { \
            buffer_size = (strlen(json) + (needed)) * 2; \
            json = realloc(json, buffer_size); \
        }
    
    // Add indentation
    char indent[256] = "";
    for (int i = 0; i < depth * 2; i++) {
        strcat(indent, " ");
    }
    
    ENSURE_BUFFER_SIZE(512);
    sprintf(json, "{\n%s  \"type\": \"%s\",\n%s  \"id\": \"%p\"", 
            indent, ast_node_type_to_string(node->type), indent, (void*)node);
    
    switch (node->type) {
        case AST_PROGRAM: {
            ENSURE_BUFFER_SIZE(256);
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"statement_count\": ");
            
            char count_str[32];
            sprintf(count_str, "%zu", node->data.program.statement_count);
            strcat(json, count_str);
            
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"statements\": [");
            
            for (size_t i = 0; i < node->data.program.statement_count; i++) {
                if (i > 0) {
                    strcat(json, ",");
                }
                strcat(json, "\n");
                
                char* child_json = ast_to_json_recursive(node->data.program.statements[i], depth + 2);
                ENSURE_BUFFER_SIZE(strlen(child_json) + 64);
                strcat(json, child_json);
                free(child_json);
            }
            
            if (node->data.program.statement_count > 0) {
                strcat(json, "\n");
                strcat(json, indent);
                strcat(json, "  ");
            }
            strcat(json, "]");
            break;
        }
        
        case AST_VARIABLE: {
            char* escaped_name = escape_json_string(node->data.variable.name);
            ENSURE_BUFFER_SIZE(strlen(escaped_name) + 64);
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"name\": ");
            strcat(json, escaped_name);
            free(escaped_name);
            break;
        }
        
        case AST_NUMBER: {
            ENSURE_BUFFER_SIZE(128);
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"value\": ");
            
            char value_str[32];
            sprintf(value_str, "%d", node->data.number.value);
            strcat(json, value_str);
            break;
        }
        
        case AST_BINARY_OP: {
            char op_str[8];
            char op = node->data.binary_op.op;
            
            // Convert special operators back to readable form
            if (op == 'G') {
                strcpy(op_str, "\">=\"");
            } else if (op == 'L') {
                strcpy(op_str, "\"<=\"");
            } else if (op == '=') {
                strcpy(op_str, "\"==\"");
            } else if (op == '!') {
                strcpy(op_str, "\"!=\"");
            } else {
                sprintf(op_str, "\"%c\"", op);
            }
            
            ENSURE_BUFFER_SIZE(strlen(op_str) + 128);
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"operator\": ");
            strcat(json, op_str);
            
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"left\": ");
            
            char* left_json = ast_to_json_recursive(node->data.binary_op.left, depth + 1);
            ENSURE_BUFFER_SIZE(strlen(left_json) + 64);
            strcat(json, left_json);
            free(left_json);
            
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"right\": ");
            
            char* right_json = ast_to_json_recursive(node->data.binary_op.right, depth + 1);
            ENSURE_BUFFER_SIZE(strlen(right_json) + 64);
            strcat(json, right_json);
            free(right_json);
            break;
        }
        
        case AST_ASSIGN: {
            char* escaped_name = escape_json_string(node->data.assign.name);
            ENSURE_BUFFER_SIZE(strlen(escaped_name) + 128);
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"variable\": ");
            strcat(json, escaped_name);
            free(escaped_name);
            
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"value\": ");
            
            char* value_json = ast_to_json_recursive(node->data.assign.value, depth + 1);
            ENSURE_BUFFER_SIZE(strlen(value_json) + 64);
            strcat(json, value_json);
            free(value_json);
            break;
        }
        
        case AST_IF: {
            ENSURE_BUFFER_SIZE(256);
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"condition\": ");
            
            char* condition_json = ast_to_json_recursive(node->data.if_statement.condition, depth + 1);
            ENSURE_BUFFER_SIZE(strlen(condition_json) + 64);
            strcat(json, condition_json);
            free(condition_json);
            
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"if_body\": ");
            
            char* if_body_json = ast_to_json_recursive(node->data.if_statement.if_body, depth + 1);
            ENSURE_BUFFER_SIZE(strlen(if_body_json) + 64);
            strcat(json, if_body_json);
            free(if_body_json);
            
            if (node->data.if_statement.else_body) {
                strcat(json, ",\n");
                strcat(json, indent);
                strcat(json, "  \"else_body\": ");
                
                char* else_body_json = ast_to_json_recursive(node->data.if_statement.else_body, depth + 1);
                ENSURE_BUFFER_SIZE(strlen(else_body_json) + 64);
                strcat(json, else_body_json);
                free(else_body_json);
            }
            break;
        }
        
        case AST_PRINT: {
            ENSURE_BUFFER_SIZE(128);
            strcat(json, ",\n");
            strcat(json, indent);
            strcat(json, "  \"expression\": ");
            
            char* expr_json = ast_to_json_recursive(node->data.print.expression, depth + 1);
            ENSURE_BUFFER_SIZE(strlen(expr_json) + 64);
            strcat(json, expr_json);
            free(expr_json);
            break;
        }
    }
    
    ENSURE_BUFFER_SIZE(64);
    strcat(json, "\n");
    strcat(json, indent);
    strcat(json, "}");
    
    #undef ENSURE_BUFFER_SIZE
    return json;
}

char* ast_to_json(ASTNode* node) {
    return ast_to_json_recursive(node, 0);
}
