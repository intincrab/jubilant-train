#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/lexer.h"

// Helper function to verify tokens
void verify_token(Token* token, TokenType expected_type, const char* expected_value) {
    assert(token != NULL);
    assert(token->type == expected_type);
    
    if (expected_value != NULL) {
        assert(token->value != NULL);
        assert(strcmp(token->value, expected_value) == 0);
    } else {
        assert(token->value == NULL);
    }
}

// Test function for the lexer
void test_lexer() {
    // Test input containing various token types
    char* input = "x = 10 + 5;\n"
                  "if (x > 12) {\n"
                  "    print(x);\n"
                  "} else {\n"
                  "    y = x * (3 - 1);\n"
                  "    print(y);\n"
                  "}\n";
    
    Lexer* lexer = init_lexer(input);
    
    // Verify all tokens in sequence
    Token* token;
    
    // x = 10 + 5;
    token = get_next_token(lexer); verify_token(token, TOKEN_ID, "x");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_ASSIGN, "=");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_NUMBER, "10");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_PLUS, "+");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_NUMBER, "5");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_SEMICOLON, ";");
    free(token->value); free(token);
    
    // if (x > 12) {
    token = get_next_token(lexer); verify_token(token, TOKEN_IF, "if");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_LPAREN, "(");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_ID, "x");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_GREATER, ">");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_NUMBER, "12");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_RPAREN, ")");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_LBRACE, "{");
    free(token->value); free(token);
    
    // print(x);
    token = get_next_token(lexer); verify_token(token, TOKEN_PRINT, "print");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_LPAREN, "(");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_ID, "x");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_RPAREN, ")");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_SEMICOLON, ";");
    free(token->value); free(token);
    
    // } else {
    token = get_next_token(lexer); verify_token(token, TOKEN_RBRACE, "}");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_ELSE, "else");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_LBRACE, "{");
    free(token->value); free(token);
    
    // y = x * (3 - 1);
    token = get_next_token(lexer); verify_token(token, TOKEN_ID, "y");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_ASSIGN, "=");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_ID, "x");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_MULTIPLY, "*");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_LPAREN, "(");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_NUMBER, "3");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_MINUS, "-");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_NUMBER, "1");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_RPAREN, ")");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_SEMICOLON, ";");
    free(token->value); free(token);
    
    // print(y);
    token = get_next_token(lexer); verify_token(token, TOKEN_PRINT, "print");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_LPAREN, "(");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_ID, "y");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_RPAREN, ")");
    free(token->value); free(token);
    
    token = get_next_token(lexer); verify_token(token, TOKEN_SEMICOLON, ";");
    free(token->value); free(token);
    
    // }
    token = get_next_token(lexer); verify_token(token, TOKEN_RBRACE, "}");
    free(token->value); free(token);
    
    // End of file
    token = get_next_token(lexer); verify_token(token, TOKEN_EOF, NULL);
    free(token);
    
    free_lexer(lexer);
    
    printf("All lexer tests passed!\n");
}

int main() {
    test_lexer();
    return 0;
} 