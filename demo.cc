#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "execute.h"
#include <vector>
#include <iostream>
#include "lexer.h"

using namespace std;

struct InstructionNode* parse_stmt_list();
struct InstructionNode* parse_body();
void parse_input();
struct InstructionNode* parse_stmt();
struct InstructionNode* parse_assign_stmt();
struct InstructionNode* parse_switch_stmt();
struct InstructionNode* parse_while_stmt();
struct InstructionNode* parse_if_stmt();
struct InstructionNode* parse_in_stmt();
struct InstructionNode* parse_out_stmt();
struct InstructionNode* parse_for_stmt();
void parse_condition(InstructionNode* node);
void parse_id_list();
void syntax_error();
Token expect(TokenType expected_type);
int location(Token t);

LexicalAnalyzer lexer;   // NOTE: even though this file does not peek() at or get() any
                         // tokens, the very fact that a lexer is declared
                         // requires that you provide input to the program
                         // when you execute it even if you don't add any code
                         // to execute just run ./a.out < test, where test is any
                         // test case

//new struct to link an index to a name
struct locBind {
    int index;
    string name;
};

vector<locBind*> locTable;

struct InstructionNode * parse_generate_intermediate_representation()
{
    /*
    // Sample program for demonstration purpose only
    // Replace the following with a call to a parser that reads the 
    // program from stdin & creates appropriate data structures to be 
    // executed by execute_program()
    // This is the imaginary input for the following construction:
    
    // a, b, c, d;
    // {
    //     input a;
    //     input b;
    //     c = 10;
    // 
    //     IF c <> a
    //     {
    //         output b;
    //     }
    // 
    //     IF c > 1
    //     {
    //         a = b + 900;
    //         input d;
    //         IF a > 10
    //         {
    //             output d;
    //         }
    //     }
    // 
    //     d = 0;
    //     WHILE d < 4
    //     {
    //         c = a + d;
    //         IF d > 1
    //         {
    //             output d;
    //         }
    //         d = d + 1;
    //     }
    // }
    // 1 2 3 4 5 6


    // Assigning location for variable "a"
    int address_a = next_available;
    mem[next_available] = 0;    // next_available is a global variable that is decalred in 
    next_available++;           // execute.cc

    // Assigning location for variable "b"
    int address_b = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "c"
    int address_c = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "d"
    int address_d = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 10
    int address_ten = next_available;
    mem[next_available] = 10;
    next_available++;

    // Assigning location for constant 1
    int address_one = next_available;
    mem[next_available] = 1;
    next_available++;

    // Assigning location for constant 900
    int address_ninehundred = next_available;
    mem[next_available] = 900;
    next_available++;

    // Assigning location for constant 3
    int address_three = next_available;
    mem[next_available] = 3;
    next_available++;

    // Assigning location for constant 0
    int address_zero = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 4
    int address_four = next_available;
    mem[next_available] = 4;
    next_available++;

    struct InstructionNode * i1 = new InstructionNode;
    struct InstructionNode * i2 = new InstructionNode;
    struct InstructionNode * i3 = new InstructionNode;
    struct InstructionNode * i4 = new InstructionNode;
    struct InstructionNode * i5 = new InstructionNode;
    struct InstructionNode * i6 = new InstructionNode;
    struct InstructionNode * i7 = new InstructionNode;
    struct InstructionNode * i8 = new InstructionNode;
    struct InstructionNode * i9 = new InstructionNode;
    struct InstructionNode * i10 = new InstructionNode;
    struct InstructionNode * i11 = new InstructionNode;
    struct InstructionNode * i12 = new InstructionNode;
    struct InstructionNode * i13 = new InstructionNode;
    struct InstructionNode * i14 = new InstructionNode;
    struct InstructionNode * i15 = new InstructionNode;
    struct InstructionNode * i16 = new InstructionNode;
    struct InstructionNode * i17 = new InstructionNode;
    struct InstructionNode * i18 = new InstructionNode;
    struct InstructionNode * i19 = new InstructionNode;
    struct InstructionNode * i20 = new InstructionNode;
    struct InstructionNode * i21 = new InstructionNode;
    struct InstructionNode * i22 = new InstructionNode;

    i1->type = IN;                                      // input a
    i1->input_inst.var_index = address_a;
    i1->next = i2;

    i2->type = IN;                                      // input b
    i2->input_inst.var_index = address_b;
    i2->next = i3;

    i3->type = ASSIGN;                                  // c = 10
    i3->assign_inst.left_hand_side_index = address_c;
    i3->assign_inst.op = OPERATOR_NONE;
    i3->assign_inst.opernd1_index = address_ten;
    i3->next = i4;

    i4->type = CJMP;                                    // if c <> a
    i4->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    i4->cjmp_inst.opernd1_index = address_c;
    i4->cjmp_inst.opernd2_index = address_a;
    i4->cjmp_inst.target = i6;                          // if not (c <> a) skip forward to NOOP
    i4->next = i5;

    i5->type = OUT;                                     // output b
    i5->output_inst.var_index = address_b;
    i5->next = i6;

    i6->type = NOOP;                                    // NOOP after IF
    i6->next = i7;

    i7->type = CJMP;                                    // if c > 1
    i7->cjmp_inst.condition_op = CONDITION_GREATER;
    i7->cjmp_inst.opernd1_index = address_c;
    i7->cjmp_inst.opernd2_index = address_one;
    i7->cjmp_inst.target = i13;                         // if not (c > 1) skip forward to NOOP (way down)
    i7->next = i8;

    i8->type = ASSIGN;                                  // a = b + 900
    i8->assign_inst.left_hand_side_index = address_a;
    i8->assign_inst.op = OPERATOR_PLUS;
    i8->assign_inst.opernd1_index = address_b;
    i8->assign_inst.opernd2_index = address_ninehundred;
    i8->next = i9;

    i9->type = IN;                                      // input d
    i9->input_inst.var_index = address_d;
    i9->next = i10;

    i10->type = CJMP;                                   // if a > 10
    i10->cjmp_inst.condition_op = CONDITION_GREATER;
    i10->cjmp_inst.opernd1_index = address_a;
    i10->cjmp_inst.opernd2_index = address_ten;
    i10->cjmp_inst.target = i12;                        // if not (a > 10) skipp forward to NOOP
    i10->next = i11;

    i11->type = OUT;                                    // output d
    i11->output_inst.var_index = address_d;
    i11->next = i12;

    i12->type = NOOP;                                   // NOOP after inner IF
    i12->next = i13;

    i13->type = NOOP;                                   // NOOP after outer IF
    i13->next = i14;

    i14->type = ASSIGN;                                 // d = 0
    i14->assign_inst.left_hand_side_index = address_d;
    i14->assign_inst.op = OPERATOR_NONE;
    i14->assign_inst.opernd1_index = address_zero;
    i14->next = i15;

    i15->type = CJMP;                                   // if d < 4
    i15->cjmp_inst.condition_op = CONDITION_LESS;
    i15->cjmp_inst.opernd1_index = address_d;
    i15->cjmp_inst.opernd2_index = address_four;
    i15->cjmp_inst.target = i22;                        // if not (d < 4) skip whole WHILE body
    i15->next = i16;

    i16->type = ASSIGN;                                 // c = a + d
    i16->assign_inst.left_hand_side_index = address_c;
    i16->assign_inst.op = OPERATOR_PLUS;
    i16->assign_inst.opernd1_index = address_a;
    i16->assign_inst.opernd2_index = address_d;
    i16->next = i17;

    i17->type = CJMP;                                   // if d > 1
    i17->cjmp_inst.condition_op = CONDITION_GREATER;
    i17->cjmp_inst.opernd1_index = address_d;
    i17->cjmp_inst.opernd2_index = address_one;
    i17->cjmp_inst.target = i19;                        // if not (d > 1) skip body of IF
    i17->next = i18;

    i18->type = OUT;                                    // output d
    i18->output_inst.var_index = address_d;
    i18->next = i19;

    i19->type = NOOP;                                   // NOOP after body of IF
    i19->next = i20;

    i20->type = ASSIGN;                                 // d = d + 1
    i20->assign_inst.left_hand_side_index = address_d;
    i20->assign_inst.op = OPERATOR_PLUS;
    i20->assign_inst.opernd1_index = address_d;
    i20->assign_inst.opernd2_index = address_one;
    i20->next = i21;

    i21->type = JMP;
    i21->jmp_inst.target = i15;
    i21->next = i22;

    i22->type = NOOP;                                   // NOOP after body of WHILE
    i22->next = NULL;

    // Inputs
    inputs.push_back(1);
    inputs.push_back(2);
    inputs.push_back(3);
    inputs.push_back(4);
    inputs.push_back(5);
    inputs.push_back(6);

    return i1; */
    
//loop through variables and for each one, free the location in memory and add a bind to location vector

    parse_id_list();
    expect(SEMICOLON);
    
    InstructionNode* instList = parse_body();

    parse_input();

    return instList;
}



struct InstructionNode* parse_stmt_list() {
    InstructionNode* inst;
    InstructionNode* instList;

    inst = parse_stmt();
    if (lexer.peek(1).token_type != RBRACE) {
        instList = parse_stmt_list();

        InstructionNode* tmp = inst;
        while (tmp->next != nullptr)
            tmp = tmp->next;
        tmp->next = instList;
    }
    return inst;
}

struct InstructionNode* parse_body() {
    struct InstructionNode* instList;

    expect(LBRACE);
    instList = parse_stmt_list();
    expect(RBRACE);

    return instList;
}

struct InstructionNode* parse_stmt() {
    Token t = lexer.peek(1);
    //assign statement
    if (t.token_type == ID) { // ID EQUAL
        return parse_assign_stmt();
    } // output statement
    else if (t.token_type == OUTPUT) {
        return parse_out_stmt();
    } // input statement
    else if (t.token_type == INPUT) {
        return parse_in_stmt();
    } // while statement
    else if (t.token_type == IF) {
        return parse_if_stmt();
    } // while stmt
    else if (t.token_type == WHILE) {
        return parse_while_stmt();
    } // for
    else if (t.token_type == FOR) {
        return parse_for_stmt();
    } // switch
    else if (t.token_type == SWITCH) {
        return parse_switch_stmt();
    }
}

struct InstructionNode* parse_switch_stmt() {
    InstructionNode* node = new InstructionNode;
    InstructionNode* end = new InstructionNode;
    end->type = NOOP;
    InstructionNode* ptr = new InstructionNode;
    node->next = ptr;
    expect(SWITCH);
    Token t = expect(ID);
    expect(LBRACE);
    while (lexer.peek(1).token_type == CASE) {
        InstructionNode* ifNode = new InstructionNode;
        ptr->next = ifNode;

        expect(CASE);
        Token n = expect(NUM);
        expect(COLON);

        //body of switch statement and add goto label at end
        InstructionNode* body = parse_body();
        ptr = body;
        while (ptr->next != nullptr)
            ptr = ptr->next;
        ptr->next = end;

        //set up if node so if not equal it will go to mf 
        
        ifNode->type = CJMP;
        ifNode->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
        ifNode->cjmp_inst.opernd1_index = location(t);
        ifNode->cjmp_inst.opernd2_index = location(n);
        ifNode->cjmp_inst.target = body;
        ifNode->next = nullptr;

       
        //add a jump at end of body but i dont think i need it cuz i just made nxt the end node
        /*InstructionNode* jmp = new InstructionNode;
        jmp->type = JMP;
        jmp->jmp_inst.target = end;

        ptr->next = jmp;*/

        ptr = ifNode;
    }

    //check for default
    if (lexer.peek(1).token_type == DEFAULT) {
        expect(DEFAULT);
        expect(COLON);
        InstructionNode* defBody = parse_body();
        ptr->next = defBody;
        while (ptr->next != nullptr)
            ptr = ptr->next;
    }

    //add label to end of switch

    expect(RBRACE);

    ptr->next = end;
    end->next = nullptr;

    return node->next->next;
}


struct InstructionNode* parse_for_stmt() {
    //Get first assign statement
    expect(FOR);
    expect(LPAREN);
    InstructionNode* node = parse_assign_stmt();
    node->type = ASSIGN;
    
    //create the loop node and attach to first statement
    InstructionNode* loopNode = new InstructionNode;
    loopNode->type = CJMP;
    node->next = loopNode;

    //add condition to loop node
    parse_condition(loopNode);
    expect(SEMICOLON);

    //save second assign stmt
    InstructionNode* node2 = parse_assign_stmt();
    expect(RPAREN);

    //get body of while loop and go to end of it
    loopNode->next = parse_body();
    InstructionNode* tmp = loopNode;
    while (tmp->next != nullptr) 
        tmp = tmp->next;
    
    //set final assign stmt to end of body
    tmp->next = node2;
    //create jmp node and add to end
    InstructionNode* jmp = new InstructionNode;
    jmp->type = JMP;
    jmp->jmp_inst.target = loopNode;
    node2->next = jmp;
    //create noop node and add
    InstructionNode* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;
    jmp->next = noop;
    loopNode->cjmp_inst.target = noop;
    return node;
}

struct InstructionNode* parse_while_stmt() {
    //expect while and create new cjmp node
    expect(WHILE);
    InstructionNode* node = new InstructionNode;
    node->type = CJMP;
    //parse condition and body 
    parse_condition(node);
    node->next = parse_body();
    //create jmp node
    InstructionNode* jmp = new InstructionNode;
    jmp->type = JMP;
    jmp->jmp_inst.target = node;
    //go to end of body
    InstructionNode* tmp = node;
    while (tmp->next != nullptr) 
        tmp = tmp->next;
    //attach jmp to end
    tmp->next = jmp;
    //make noop and attach
    InstructionNode* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;
    jmp->next = noop;
    node->cjmp_inst.target = noop;

    return node;
}

struct InstructionNode* parse_if_stmt() {
    expect(IF);
    InstructionNode* node = new InstructionNode;
    node->type = CJMP;
    parse_condition(node);
    node->next = parse_body();

    //create noop node
    InstructionNode* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;

    //go to end of body and add noop to end then set target
    InstructionNode* tmp = node;
    while (tmp->next != nullptr)
        tmp = tmp->next;

    tmp->next = noop;
    node->cjmp_inst.target = noop;

    return node;
}

void parse_condition(InstructionNode* node) {
    Token t = lexer.GetToken();
    if (t.token_type != ID && t.token_type != NUM)
        syntax_error();

    node->cjmp_inst.opernd1_index = location(t);
    
    //set relop
    t = lexer.GetToken();
    switch (t.token_type) {
    case GREATER:
        node->cjmp_inst.condition_op = CONDITION_GREATER;
        break;
    case LESS:
        node->cjmp_inst.condition_op = CONDITION_LESS;
        break;
    case NOTEQUAL:
        node->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
        break;
    default:
        syntax_error();
    }

    t = lexer.GetToken();
    if (t.token_type != ID && t.token_type != NUM)
        syntax_error();

    node->cjmp_inst.opernd2_index = location(t);
}

struct InstructionNode* parse_in_stmt() {
    expect(INPUT);
    InstructionNode* node = new InstructionNode;
    node->type = IN;
    node->next = nullptr;
    Token t = expect(ID);
    node->input_inst.var_index = location(t);
    expect(SEMICOLON);
    return node;
}

struct InstructionNode* parse_out_stmt() {
    expect(OUTPUT);
    InstructionNode* node = new InstructionNode;
    node->type = OUT;
    node->next = nullptr;
    Token t = expect(ID);
    node->output_inst.var_index = location(t);
    expect(SEMICOLON);
    return node;
}

struct InstructionNode* parse_assign_stmt() {
    InstructionNode* node = new InstructionNode;
    node->type = ASSIGN;
    node->next = nullptr;

    //get ID and put into lhs of instnode
    Token t = lexer.GetToken();
    node->assign_inst.left_hand_side_index = location(t);

    expect(EQUAL);

    t = lexer.GetToken();
    if (t.token_type != ID && t.token_type != NUM)
        syntax_error();

    if (lexer.peek(1).token_type == SEMICOLON) {
        node->assign_inst.op = OPERATOR_NONE;
        node->assign_inst.opernd1_index = location(t);
        expect(SEMICOLON);
    }
    else {
        //get operator type
        Token op = lexer.GetToken();
        switch (op.token_type) {
        case PLUS:
            node->assign_inst.op = OPERATOR_PLUS;
            break;
        case MINUS:
            node->assign_inst.op = OPERATOR_MINUS;
            break;
        case MULT:
            node->assign_inst.op = OPERATOR_MULT;
            break;
        case DIV:
            node->assign_inst.op = OPERATOR_DIV;
            break;
        default:
            syntax_error();
        }

        //
        node->assign_inst.opernd1_index = location(t);
        node->assign_inst.opernd2_index = location(lexer.GetToken());
        expect(SEMICOLON);
    }

    return node;
}

void parse_id_list() {
    while (lexer.peek(1).token_type != SEMICOLON) {
        Token t = expect(ID);
        if (lexer.peek(1).token_type == COMMA) {
            lexer.GetToken();
        }

        locBind* l = new locBind;
        l->index = next_available;
        mem[next_available] = 0;
        next_available++;
        l->name = t.lexeme;
        locTable.push_back(l);
    }
}

void parse_input() {
    Token t = lexer.peek(1);
    if (t.token_type != END_OF_FILE) {
        t = expect(NUM);
        inputs.push_back(stoi(t.lexeme));
        parse_input();  
    }
}

int location(Token t) {
    //if it exists return it
    for (int i = 0; i < locTable.size(); i++) {
        if (locTable.at(i)->name == t.lexeme)
            return locTable.at(i)->index;
    }

    //otherwise if it is a constant, free space for it 
    if (t.token_type == NUM) {
        locBind* lb = new locBind;
        lb->index = next_available;
        lb->name = t.lexeme;
        locTable.push_back(lb);
        mem[next_available] = stoi(t.lexeme);
        next_available++;
        return next_available - 1;
    }
    //otherwise it was not initialized
    syntax_error();
}

void syntax_error() {
    cout << "SYNTAX ERORR" << endl;
    exit(1);
}

Token expect(TokenType expected_type) {
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}


