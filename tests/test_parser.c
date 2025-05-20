#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/parser.h"
#include "../src/lexer.h"

// Helper function to check if the AST structure is correct
void test_ast_structure(ASTNode* node) {
    assert(node != NULL);
    assert(node->type == AST_PROGRAM);
    assert(node->data.program.statement_count == 3);
    
    // First statement: x = 5;
    ASTNode* assign1 = node->data.program.statements[0];
    assert(assign1->type == AST_ASSIGN);
    assert(strcmp(assign1->data.assign.name, "x") == 0);
    assert(assign1->data.assign.value->type == AST_NUMBER);
    assert(assign1->data.assign.value->data.number.value == 5);
    
    // Second statement: y = x + 3;
    ASTNode* assign2 = node->data.program.statements[1];
    assert(assign2->type == AST_ASSIGN);
    assert(strcmp(assign2->data.assign.name, "y") == 0);
    assert(assign2->data.assign.value->type == AST_BINARY_OP);
    assert(assign2->data.assign.value->data.binary_op.op == '+');
    assert(assign2->data.assign.value->data.binary_op.left->type == AST_VARIABLE);
    assert(strcmp(assign2->data.assign.value->data.binary_op.left->data.variable.name, "x") == 0);
    assert(assign2->data.assign.value->data.binary_op.right->type == AST_NUMBER);
    assert(assign2->data.assign.value->data.binary_op.right->data.number.value == 3);
    
    // Third statement: if (y > 7) { ... }
    ASTNode* if_stmt = node->data.program.statements[2];
    assert(if_stmt->type == AST_IF);
    assert(if_stmt->data.if_statement.condition->type == AST_BINARY_OP);
    assert(if_stmt->data.if_statement.condition->data.binary_op.op == '>');
    assert(if_stmt->data.if_statement.condition->data.binary_op.left->type == AST_VARIABLE);
    assert(strcmp(if_stmt->data.if_statement.condition->data.binary_op.left->data.variable.name, "y") == 0);
    assert(if_stmt->data.if_statement.condition->data.binary_op.right->type == AST_NUMBER);
    assert(if_stmt->data.if_statement.condition->data.binary_op.right->data.number.value == 7);
    
    // Check if body (print statement)
    ASTNode* if_body = if_stmt->data.if_statement.if_body;
    assert(if_body->type == AST_PROGRAM);
    assert(if_body->data.program.statement_count == 1);
    assert(if_body->data.program.statements[0]->type == AST_PRINT);
    
    printf("All parser tests passed!\n");
}

int main() {
    // Test input
    char* input = "x = 5;\n"
                  "y = x + 3;\n"
                  "if (y > 7) {\n"
                  "    print(y);\n"
                  "}\n";
    
    // Initialize lexer and parser
    Lexer* lexer = init_lexer(input);
    Parser* parser = init_parser(lexer);
    
    // Parse the input
    ASTNode* ast = parse(parser);
    
    // Test the AST structure
    test_ast_structure(ast);
    
    // Clean up
    free_ast(ast);
    free_parser(parser);
    free_lexer(lexer);
    
    return 0;
} 