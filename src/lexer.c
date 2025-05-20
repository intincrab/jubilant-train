#include "lexer.h"

Lexer* init_lexer(char* src) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->src = src;
    lexer->position = 0;
    lexer->length = strlen(src);
    lexer->current_char = lexer->length > 0 ? src[0] : '\0';
    return lexer;
}

void advance(Lexer* lexer) {
    if (lexer->position < lexer->length) {
        lexer->position++;
        lexer->current_char = lexer->position < lexer->length ? 
                              lexer->src[lexer->position] : '\0';
    }
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->current_char != '\0' && isspace(lexer->current_char)) {
        advance(lexer);
    }
}

Token* create_token(TokenType type, char* value) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value = value;
    return token;
}

char* read_identifier(Lexer* lexer) {
    size_t start = lexer->position;
    
    while (lexer->current_char != '\0' && (isalnum(lexer->current_char) || lexer->current_char == '_')) {
        advance(lexer);
    }
    
    size_t length = lexer->position - start;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->src[start], length);
    value[length] = '\0';
    
    return value;
}

char* read_number(Lexer* lexer) {
    size_t start = lexer->position;
    
    while (lexer->current_char != '\0' && isdigit(lexer->current_char)) {
        advance(lexer);
    }
    
    size_t length = lexer->position - start;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->src[start], length);
    value[length] = '\0';
    
    return value;
}

void skip_comment(Lexer* lexer) {
    // Skip until the end of the line
    while (lexer->current_char != '\0' && lexer->current_char != '\n') {
        advance(lexer);
    }
    
    // Skip the newline if present
    if (lexer->current_char == '\n') {
        advance(lexer);
    }
}

Token* get_next_token(Lexer* lexer) {
    while (lexer->current_char != '\0') {
        // Skip whitespace
        if (isspace(lexer->current_char)) {
            skip_whitespace(lexer);
            continue;
        }
        
        // Skip comments
        if (lexer->current_char == '/' && lexer->position + 1 < lexer->length && 
            lexer->src[lexer->position + 1] == '/') {
            skip_comment(lexer);
            continue;
        }
        
        // Identifiers and keywords
        if (isalpha(lexer->current_char) || lexer->current_char == '_') {
            char* value = read_identifier(lexer);
            
            if (strcmp(value, "if") == 0) {
                return create_token(TOKEN_IF, value);
            } else if (strcmp(value, "else") == 0) {
                return create_token(TOKEN_ELSE, value);
            } else if (strcmp(value, "print") == 0) {
                return create_token(TOKEN_PRINT, value);
            } else {
                return create_token(TOKEN_ID, value);
            }
        }
        
        // Numbers
        if (isdigit(lexer->current_char)) {
            return create_token(TOKEN_NUMBER, read_number(lexer));
        }
        
        // Operators and special characters
        switch (lexer->current_char) {
            case '+': {
                char* value = malloc(2);
                value[0] = '+';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_PLUS, value);
            }
            case '-': {
                char* value = malloc(2);
                value[0] = '-';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_MINUS, value);
            }
            case '*': {
                char* value = malloc(2);
                value[0] = '*';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_MULTIPLY, value);
            }
            case '/': {
                char* value = malloc(2);
                value[0] = '/';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_DIVIDE, value);
            }
            case '=': {
                advance(lexer);
                // Check if it's == (equality)
                if (lexer->current_char == '=') {
                    advance(lexer);
                    char* value = malloc(3);
                    value[0] = '=';
                    value[1] = '=';
                    value[2] = '\0';
                    return create_token(TOKEN_EQUAL, value);
                } else {
                    // It's just = (assignment)
                    char* value = malloc(2);
                    value[0] = '=';
                    value[1] = '\0';
                    return create_token(TOKEN_ASSIGN, value);
                }
            }
            case '>': {
                advance(lexer);
                // Check if it's >= (greater or equal)
                if (lexer->current_char == '=') {
                    advance(lexer);
                    char* value = malloc(3);
                    value[0] = '>';
                    value[1] = '=';
                    value[2] = '\0';
                    return create_token(TOKEN_GREATER_EQUAL, value);
                } else {
                    // It's just > (greater than)
                    char* value = malloc(2);
                    value[0] = '>';
                    value[1] = '\0';
                    return create_token(TOKEN_GREATER, value);
                }
            }
            case '<': {
                advance(lexer);
                // Check if it's <= (less or equal)
                if (lexer->current_char == '=') {
                    advance(lexer);
                    char* value = malloc(3);
                    value[0] = '<';
                    value[1] = '=';
                    value[2] = '\0';
                    return create_token(TOKEN_LESS_EQUAL, value);
                } else {
                    // It's just < (less than)
                    char* value = malloc(2);
                    value[0] = '<';
                    value[1] = '\0';
                    return create_token(TOKEN_LESS, value);
                }
            }
            case '!': {
                advance(lexer);
                // Check if it's != (not equal)
                if (lexer->current_char == '=') {
                    advance(lexer);
                    char* value = malloc(3);
                    value[0] = '!';
                    value[1] = '=';
                    value[2] = '\0';
                    return create_token(TOKEN_NOT_EQUAL, value);
                } else {
                    // Unsupported operator
                    printf("Unexpected operator: !\n");
                    advance(lexer);
                    continue;
                }
            }
            case ';': {
                char* value = malloc(2);
                value[0] = ';';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_SEMICOLON, value);
            }
            case '(': {
                char* value = malloc(2);
                value[0] = '(';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_LPAREN, value);
            }
            case ')': {
                char* value = malloc(2);
                value[0] = ')';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_RPAREN, value);
            }
            case '{': {
                char* value = malloc(2);
                value[0] = '{';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_LBRACE, value);
            }
            case '}': {
                char* value = malloc(2);
                value[0] = '}';
                value[1] = '\0';
                advance(lexer);
                return create_token(TOKEN_RBRACE, value);
            }
            default: {
                printf("Unknown character: %c\n", lexer->current_char);
                advance(lexer);
                continue;
            }
        }
    }
    
    return create_token(TOKEN_EOF, NULL);
}

void free_lexer(Lexer* lexer) {
    free(lexer);
} 