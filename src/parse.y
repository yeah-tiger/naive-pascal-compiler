%require "3.0"

%{
    #include <cstdio>
    #include <memory>
    #include "utils/ast.hpp"
    #include "utils/ast_utils.hpp"
    #include "y.tab.h"

    using namespace npc;

    int yylex();
    int yyerror(const char *s);
%}

%define api.value.type {std::shared_ptr<npc::AbstractNode>}
%define parse.error verbose
%define parse.lac full

%token PROGRAM ID CONST ARRAY VAR FUNCTION PROCEDURE _BEGIN END TYPE READ RECORD
%token INTEGER REAL CHAR STRING
%token SYS_CON SYS_FUNC SYS_PROC SYS_TYPE
%token IF THEN ELSE REPEAT UNTIL WHILE DO FOR TO DOWNTO CASE OF GOTO
%token ASSIGN EQUAL UNEQUAL LE LT GE GT
%token PLUS MINUS MUL DIV OR AND MOD NOT
%token DOT DOTDOT SEMI LP RP LB RB COMMA COLON

%start program

%%

program
    : PROGRAM ID SEMI routine_head routine_body DOT
        { $$ = make_node<ProgramNode>($2, $4); $$->move_children($5); }
    ;

routine_head
    : const_part type_part var_part routine_part
        { $$ = make_node<HeadListNode>($1, $2, $3, $4); }
;

const_part
    : CONST const_expr_list { $$ = $2; }
    | { $$ = make_node<ConstListNode>(); }
    ;

const_expr_list
    : const_expr_list ID EQUAL const_value SEMI
        { $$ = $1; $$->add_child(make_node<ConstDeclNode>($2, $4)); }
    | ID EQUAL const_value SEMI
        { $$ = make_node<ConstListNode>(); $$->add_child(make_node<ConstDeclNode>($1, $3)); }
    ;

const_value
    : INTEGER { $$ = $1; }
    | REAL    { $$ = $1; }
    | CHAR    { $$ = $1; }
    | STRING  { $$ = $1; }
    | SYS_CON { $$ = $1; }
    ;

type_part
    : TYPE type_decl_list { $$ = $2; }
    | { $$ = make_node<TypeListNode>(); }
    ;

type_decl_list
    : type_decl_list type_definition { $$ = $1; $$->add_child($2); }
    | type_definition { $$ = make_node<TypeListNode>(); $$->add_child($1); }
    ;

type_definition
    : ID EQUAL type_decl SEMI
        { $$ = make_node<TypeDefNode>($1, $3); }
    ;

type_decl
    : simple_type_decl { $$ = $1; }
    | array_type_decl  { $$ = $1; }
    | record_type_decl { $$ = $1; }
    | LP name_list RP
        { $$ = make_node<SetTypeNode>(); $$->move_children($2); }
    ;

simple_type_decl
    : SYS_TYPE { $$ = $1; }
    | ID { $$ = make_node<AliasTypeNode>($1); }
    ;

array_type_decl
    : ARRAY LB array_range RB OF type_decl
        { $$ = make_node<ArrayTypeNode>($3, $6); }
    ;

array_range
    : const_value DOTDOT const_value
        { $$ = make_node<RangeNode>($1, $3); }
    | ID DOTDOT ID
        { $$ = make_node<RangeNode>($1, $3); }
    ;

record_type_decl
    : RECORD field_decl_list END { $$ = $2; }
    ;

field_decl_list
    : field_decl_list field_decl { $$ = $1; $$->lift_children($2); }
    | field_decl { $$ = $1; }
    ;

field_decl
    : name_list COLON type_decl SEMI
        { $$ = make_node<RecordTypeNode>();
          for (auto name : $1->children()) $$->add_child(make_node<VarDeclNode>(name, $3)); }
    ;

var_part
    : VAR var_decl_list { $$ = $2; }
    | { $$ = make_node<VarListNode>(); }
    ;

var_decl_list
    : var_decl_list var_decl { $$ = $1; $$->lift_children($2); }
    | var_decl { $$ = $1; }
    ;

var_decl
    : name_list COLON type_decl SEMI
        { $$ = make_node<VarListNode>();
          for (auto name : $1->children()) $$->add_child(make_node<VarDeclNode>(name, $3)); }
    ;

routine_part: routine_part function_decl { $$ = $1; $$->add_child($2); }
    | routine_part procedure_decl { $$ = $1; $$->add_child($2); }
    | { $$ = make_node<SubroutineListNode>(); }
    ;

function_decl
    : FUNCTION ID parameters COLON simple_type_decl SEMI routine_head routine_body SEMI
        { $$ = make_node<FunctionNode>($2, $3, $5, $7); $$->move_children($8); }
    ;

procedure_decl
    : PROCEDURE ID parameters SEMI routine_head routine_body SEMI
        { $$ = make_node<ProcedureNode>($2, $3, $5); $$->move_children($6); }
    ;

parameters
    : LP para_decl_list RP { $$ = $2; }
    | LP RP { $$ = make_node<ParamListNode>(); }
    | { $$ = make_node<ParamListNode>(); }
    ;

para_decl_list
    : para_decl_list SEMI para_type_list { $$ = $1; $$->lift_children($3); }
    | para_type_list { $$ = $1; }
    ;

para_type_list
    : var_para_list COLON simple_type_decl
        { $$ = make_node<ParamListNode>();
          for (auto name : $1->children()) $$->add_child(make_node<ParamDeclNode>(name, $3)); }
    ;

// TODO: distinguish var and const
var_para_list
    : VAR name_list { $$ = $2; }
    | name_list { $$ = $1; }
;

name_list
    : name_list COMMA ID { $$ = $1; $$->add_child($3); }
    | ID { $$ = make_node<NameListNode>(); $$->add_child($1); }
    ;

routine_body
    : compound_stmt { $$ = $1; }
    ;

compound_stmt
    : _BEGIN stmt_list END
        { $$ = make_node<CompoundStmtNode>(); $$->move_children($2); }
    ;

stmt_list
    : stmt_list stmt SEMI { $$ = $1; $$->add_child($2); }
    | { $$ = make_node<StmtList>(); }
    ;

stmt
    : assign_stmt   { $$ = $1; }
    | proc_stmt     { $$ = $1; }
    | compound_stmt { $$ = $1; }
    | if_stmt       { $$ = $1; }
    | repeat_stmt   { $$ = $1; }
    | while_stmt    { $$ = $1; }
    | for_stmt      { $$ = $1; }
    | case_stmt     { $$ = $1; }
    ;

assign_stmt
    : ID ASSIGN expression
        { $$ = make_node<AssignStmtNode>($1, $3); }
    | ID LB expression RB ASSIGN expression
        { $$ = make_node<AssignStmtNode>(make_node<ArrayRefNode>($1, $3), $5); }
    | ID DOT ID ASSIGN expression
        { $$ = make_node<AssignStmtNode>(make_node<RecordRefNode>($1, $3), $5); }
    ;

proc_stmt
    : ID LP RP
        { $$ = make_node<ProcStmtNode>(make_node<ProcCallNode>($1)); }
    | ID LP args_list RP
        { $$ = make_node<ProcStmtNode>(make_node<ProcCallNode>($1, $3)); }
    | SYS_PROC LP args_list RP
        { $$ = make_node<ProcStmtNode>(make_node<SysCallNode>($1, $3)); }
    | READ LP variable_list RP
        { $$ = make_node<ProcStmtNode>(make_node<SysCallNode>($1, $3)); }
    ;

variable_list
    : variable_list COMMA ID
        { $$ = $1; $$->add_child($3); }
    | ID
        { $$ = make_node<ExprListNode>(); $$->add_child($1); }
    ;

if_stmt
    : IF expression THEN stmt else_clause
        { $$ = make_node<IfStmtNode>($2, $4, $5); }
    ;

else_clause
    : ELSE stmt { $$ = $2; }
    | { $$ = make_node<StmtNode>(); }
    ;

repeat_stmt
    : REPEAT stmt_list UNTIL expression
        { $$ = make_node<RepeatStmtNode>($4); $$->move_children($2); }
    ;

while_stmt
    : WHILE expression DO stmt
        { $$ = make_node<WhileStmtNode>($2, $4); }
    ;

for_stmt
    : FOR ID ASSIGN expression TO expression DO stmt
        { $$ = make_node<ForStmtNode>(DirectionEnum::TO, $2, $4, $6, $8); }
    | FOR ID ASSIGN expression DOWNTO expression DO stmt
        { $$ = make_node<ForStmtNode>(DirectionEnum::DOWNTO, $2, $4, $6, $8); }
    ;

case_stmt
    : CASE expression OF case_expr_list END
        { $$ = $4; cast_node<CaseStmtNode>($$)->add_expr($2); }
    ;

case_expr_list
    : case_expr_list case_expr { $$ = $1; $$->add_child($2); }
    | case_expr { $$ = make_node<CaseStmtNode>(); $$->add_child($1); }
    ;

case_expr
    : const_value COLON stmt SEMI { $$ = make_node<CaseExprNode>($1, $3); }
    | ID COLON stmt SEMI { $$ = make_node<CaseExprNode>($1, $3); }
    ;

expression
    : expression GE expr { $$ = make_node<BinopExprNode>(BinaryOperator::GE, $1, $3); }
    | expression GT expr { $$ = make_node<BinopExprNode>(BinaryOperator::GT, $1, $3); }
    | expression LE expr { $$ = make_node<BinopExprNode>(BinaryOperator::LE, $1, $3); }
    | expression LT expr { $$ = make_node<BinopExprNode>(BinaryOperator::LT, $1, $3); }
    | expression EQUAL expr { $$ = make_node<BinopExprNode>(BinaryOperator::EQ, $1, $3); }
    | expression UNEQUAL expr { $$ = make_node<BinopExprNode>(BinaryOperator::NE, $1, $3); }
    | expr { $$ = $1; }
    ;

expr
    : expr PLUS term { $$ = make_node<BinopExprNode>(BinaryOperator::ADD, $1, $3); }
    | expr MINUS term { $$ = make_node<BinopExprNode>(BinaryOperator::SUB, $1, $3); }
    | expr OR term { $$ = make_node<BinopExprNode>(BinaryOperator::OR, $1, $3); }
    | term { $$ = $1; }
    ;

term
    : term MUL factor { $$ = make_node<BinopExprNode>(BinaryOperator::MUL, $1, $3); }
    | term DIV factor { $$ = make_node<BinopExprNode>(BinaryOperator::DIV, $1, $3); }
    | term MOD factor { $$ = make_node<BinopExprNode>(BinaryOperator::MOD, $1, $3); }
    | term AND factor { $$ = make_node<BinopExprNode>(BinaryOperator::AND, $1, $3); }
    | factor { $$ = $1; }
    ;

factor
    : ID { $$ = $1; }
    | ID LP RP
        { $$ = make_node<FuncExprNode>(make_node<FuncCallNode>($1)); }
    | ID LP args_list RP
        { $$ = make_node<FuncExprNode>(make_node<FuncCallNode>($1, $3)); }
    | SYS_FUNC LP args_list RP
        { $$ = make_node<FuncExprNode>(make_node<SysCallNode>($1, $3)); }
    | const_value { $$ = $1; }
    | LP expression RP { $$ = $2; }
    | NOT factor
        { $$ = make_node<BinopExprNode>(BinaryOperator::XOR, make_node<BooleanNode>(true), $2); }
    | MINUS factor
        { $$ = make_node<BinopExprNode>(BinaryOperator::SUB, make_node<IntegerNode>(0), $2); }
    | ID LB expression RB
        { $$ = make_node<ArrayRefNode>($1, $3); }
    | ID DOT ID
        { make_node<RecordRefNode>($1, $3); }
    ;

args_list
    : args_list COMMA expression { $$ = $1; $$->add_child($3); }
    | expression { $$ = make_node<ExprListNode>(); $$->add_child($1); }
    ;

%%

extern int line_no;

inline int yyerror(const char *s) {
    fprintf(stderr, "Bison error at line %d: %s\n", line_no, s);
    exit(-1);
}
