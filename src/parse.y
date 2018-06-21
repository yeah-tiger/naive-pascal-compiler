%require "3.0.4"

%{
    #include <cstdio>
    #include <iostream>
    #include <cstdlib>
    #include <memory>
    #include <utility>
    int yylex();
    int yyerror(const char *s);

    #include "utils/ast.hpp"
    #include "utils/ast_utils.hpp"

    using namespace npc;

    #ifdef YYSTYPE
    #undef YYSTYPE
    #endif  // YYSTYPE

    #define YYSTYPE_IS_DECLARED

    using YYSTYPE = std::shared_ptr<AbstractNode>;

    // must be placed here, avoiding conflict with YYSTYPE
    #include "y.tab.h"
%}

%define parse.error verbose
%define parse.lac full

%token PROGRAM ID CONST NAME ARRAY VAR FUNCTION PROCEDURE _BEGIN END TYPE READ RECORD
%token INTEGER REAL CHAR STRING
%token SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE
%token IF THEN ELSE REPEAT UNTIL WHILE DO FOR TO DOWNTO CASE OF GOTO
%token ASSIGN EQUAL UNEQUAL LE LT GE GT
%token PLUS MINUS MUL DIV OR AND MOD NOT
%token DOT DOTDOT SEMI LP RP LB RB COMMA COLON

%start program

%%
// TODO:
program:
    program_head
    {
        std::cout << "Finishing head parsing..." << std::endl;
    }
    routine
    {
        std::cout << "Finishing routine...\n" << std::endl;
    }
    DOT
    {
        std::cout << "End parsing\n" << std::endl;
    }
    ;

program_head: PROGRAM ID SEMI
;
routine: routine_head routine_body
;
sub_routine: routine_head routine_body
;
routine_head: label_part const_part type_part var_part routine_part
;
label_part:
;

const_part: CONST const_expr_list
    {
        $$ = $2;
    }
    | { $$ = make_node<ConstPartNode>(); }
    ;

const_expr_list: const_expr_list NAME EQUAL const_value SEMI
    {
        $$ = $1;
        $$->add(make_node<ConstExprNode>($2, $4));
    }
    | NAME EQUAL const_value SEMI
    {
        $$ = make_node<ConstPartNode>();
        $$->add(make_node<ConstExprNode>($1, $3));
    }
    ;

const_value: INTEGER { $$ = $1; }
    | REAL { $$ = $1; }
    | CHAR { $$ = $1; }
    | STRING { $$ = $1; }
    | SYS_CON { $$ = $1; }
    ;

type_part: TYPE type_decl_list
    {
        $$ = $2;
    }
    |  // intended blank
    ;

type_decl_list: type_decl_list type_definition { $$ = $1; $$->add($2); }
    | type_definition { $$ = make_node<TypePartNode>(); $$->add($1); }
    ;

type_definition: NAME EQUAL type_decl SEMI
    {
        $$ = make_node<TypeDefNode>($1, $3);
    }
    ;

type_decl: simple_type_decl { $$ = $1; }
    | array_type_decl { $$ = $1; }
    | record_type_decl { $$ = $1; }
    ;

simple_type_decl: SYS_TYPE { $$ = $1; }
    | NAME { $$ = make_node<AliasTypeNode>($1); }
    | LP name_list RP { $$ = make_node<EnumTypeNode>(); $$->merge_children($2->children()); }
    // TODO: checking type of both const_value is same.
    | const_value DOTDOT const_value
    {
        auto lhs = std::dynamic_pointer_cast<ConstValueNode>($1);
        auto rhs = std::dynamic_pointer_cast<ConstValueNode>($3);
        $$ = make_node<RangeTypeNode>(lhs, rhs);
    }
    | MINUS const_value DOTDOT const_value
    {
        auto lhs = std::dynamic_pointer_cast<ConstValueNode>($2)->negative();
        auto rhs = std::dynamic_pointer_cast<ConstValueNode>($4);
        $$ = make_node<RangeTypeNode>(lhs, rhs);
    }
    | MINUS const_value DOTDOT MINUS const_value
    {
        auto lhs = std::dynamic_pointer_cast<ConstValueNode>($2)->negative();
        auto rhs = std::dynamic_pointer_cast<ConstValueNode>($5)->negative();
        $$ = make_node<RangeTypeNode>(lhs, rhs);
    }
    | NAME DOTDOT NAME
    {
        auto lhs = std::dynamic_pointer_cast<ConstValueNode>($1);
        auto rhs = std::dynamic_pointer_cast<ConstValueNode>($3);
        $$ = make_node<RangeTypeNode>(lhs, rhs);
    }
    ;

// TODO
array_type_decl: ARRAY LB simple_type_decl RB OF type_decl
    {
    }
    ;

record_type_decl: RECORD field_decl_list END
    {
        $$ = $2;
    }
    ;

field_decl_list: field_decl_list field_decl { $1->add($2); $$ = $1; }
    | field_decl { $$ = make_node<RecordTypeNode>(); $$->add($1); }
    ;

field_decl: name_list COLON type_decl SEMI { $$ = make_node<FieldDeclNode>($1, $3); }
    ;

name_list: name_list COMMA ID { $$ = $1; $$->add($3); }
    | ID { $$ = make_node<NameListNode>(); $$->add($1); }
;
var_part: VAR var_decl_list { $$ = $2; }
| { $$ = make_node<VarDeclListNode>(); }
;
var_decl_list: var_decl_list var_decl {
    $$ = $1;
    $$->merge_children($2->children());
}
    | var_decl { $$ = $1; }
;
var_decl: name_list COLON type_decl SEMI {
    $$ = make_node<VarDeclListNode>();
    for (const auto node : $1->children()) {
        $$->add(make_node<VarDeclNode>(node, $3));
    }
}
;

routine_part: routine_part function_decl { $$ = $1; $$->add($2); }
    | routine_part procedure_decl { $$ = $1; $$->add($2); }
    | { $$ = make_node<RoutineNode>(); }
    ;

function_decl: function_head SEMI sub_routine SEMI
    {
        $$ = make_node<FunctionNode>($1, $3);
    }
    ;

function_head: FUNCTION ID parameters COLON simple_type_decl
    {
        $$ = make_node<FunctionHeadNode>($2, $3, $5);
    }
    ;

procedure_decl: procedure_head SEMI sub_routine SEMI
    {
        $$ = make_node<ProcedureNode>($1, $3);
    }
    ;

procedure_head: PROCEDURE ID parameters { $$ = make_node<ProcedureHeadNode>($2, $3); }
;

parameters: LP para_decl_list RP { $$ = $2; }
|   { $$ = make_node<ParametersNode>(); }
;
para_decl_list: para_decl_list SEMI para_type_list { $$ = $1; $$->add($3); }
|   para_type_list  { $$ = make_node<ParametersNode>(); $$->add($1); }
;
para_type_list: var_para_list COLON simple_type_decl { $$ = make_node<ParameterNode>($1, $3); }
;
var_para_list: VAR name_list    { $$ = $2; }
    | name_list { $$ = $1; }
;
routine_body: compound_stmt { $$ = $1; }
;
compound_stmt: _BEGIN stmt_list END { $$ = $2; }
;
stmt_list: stmt_list stmt SEMI { $$ = $1; $$->add($1); }
| { $$ = make_node<StmtList>(); }
;

stmt: INTEGER COLON non_label_stmt { $$ = make_node<StmtNode>(dynamic_cast<IntegerNode *>($1.get())->val, $3); }
    | non_label_stmt { $$ = make_node<StmtNode>($1); }
    ;

non_label_stmt: assign_stmt { $$ = $1; }
    | proc_stmt { $$ = $1; }
    | compound_stmt { $$ = $1; }
    | if_stmt { $$ = $1; }
    | repeat_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | case_stmt { $$ = $1; }
    | goto_stmt { $$ = $1; }
    ;

assign_stmt: ID ASSIGN expression { $$ = make_node<AssignStmtNode>($1, $3, false); }
    | ID LB expression RB ASSIGN expression { $$ = make_node<AssignStmtNode>(make_node<ArrayRefNode>($1, $3), $5, true); }
    | ID DOT ID ASSIGN expression { $$ = make_node<AssignStmtNode>(make_node<ArrayRefNode>($1, $3), $5, true); } // TODO
;
proc_stmt: ID   { $$ = make_node<ProcStmtNode>(cast_node<ProcCallNode>(make_node<ProcCallNode>($1))); }
    | ID LP args_list RP { $$ = make_node<ProcStmtNode>(cast_node<ProcCallNode>(make_node<ProcCallNode>($1, $3))); }
    | SYS_PROC { $$ = make_node<ProcStmtNode>(cast_node<SysProcCallNode>(make_node<SysProcCallNode>($1))); }
    | SYS_PROC LP expression_list RP { $$ = make_node<ProcStmtNode>(cast_node<SysProcCallNode>(make_node<SysProcCallNode>($1, $3))); }
    | READ LP factor RP { $$ = make_node<ProcStmtNode>(cast_node<SysProcCallNode>(make_node<SysProcCallNode>($1, $3))); }  // FIXME on factor or expression_list
    ;

if_stmt: IF expression THEN stmt else_clause { $$ = make_node<IfStmtNode>($2, $4, $5); }
;
else_clause: ELSE stmt { $$ = $2; }
| { $$ = make_node<DummyStmtNode>(); }
;
repeat_stmt: REPEAT stmt_list UNTIL expression { $$ = make_node<RepeatStmtNode>($4); $$->lift_children($2); }
;
while_stmt: WHILE expression DO stmt { $$ = make_node<WhileStmtNode>($2, $4); }
;
for_stmt: FOR ID ASSIGN expression direction expression DO stmt { $$ = make_node<ForStmtNode>(make_node<AssignStmtNode>($2, $4), $5, $6, $8); }
;
direction: TO { $$ = make_node<DirectionNode>(false); } | DOWNTO { $$ = make_node<DirectionNode>(true); }
;
case_stmt: CASE expression OF case_expr_list END { $$ = make_node<CaseStmtNode>($2); $$->lift_children($4); }
;
case_expr_list: case_expr_list case_expr { $$ = $1; $$->add($2); }
    | case_expr { $$ = make_node<ExprListNode>(); $$->add($1); }
;

case_expr: const_value COLON stmt SEMI
    {
        $$ = make_node<CaseExprNode>($1, $3);
    }
    | ID COLON stmt SEMI
    {
        $$ = make_node<CaseExprNode>($1, $3);
    }
    ;

goto_stmt: GOTO INTEGER { $$ = make_node<GotoStmtNode>($2); }
;
expression_list: expression_list COMMA expression { $$ = $1; $$->add($3); }
    | expression    { $$ = make_node<ExprListNode>(); $$->add($1); }
;
expression: expression GE expr { $$ = make_node<BinopExprNode>(BinopExprNode::OP::ge, $1, $3); }
    | expression GT expr { $$ = make_node<BinopExprNode>(BinopExprNode::OP::gt, $1, $3); }
    | expression LE expr { $$ = make_node<BinopExprNode>(BinopExprNode::OP::le, $1, $3); }
    | expression LT expr { $$ = make_node<BinopExprNode>(BinopExprNode::OP::lt, $1, $3); }
    | expression EQUAL expr { $$ = make_node<BinopExprNode>(BinopExprNode::OP::eq, $1, $3); }
    | expression UNEQUAL expr { $$ = make_node<BinopExprNode>(BinopExprNode::OP::neq, $1, $3); }
    | expr { $$ = $1; }
;
expr: expr PLUS term { $$ = make_node<BinopExprNode>(BinopExprNode::OP::plus, $1, $3); }
    | expr MINUS term { $$ = make_node<BinopExprNode>(BinopExprNode::OP::minus, $1, $3); }
    | expr OR term { $$ = make_node<BinopExprNode>(BinopExprNode::OP::_or, $1, $3); }
    | term { $$ = $1; }
;
term: term MUL factor { $$ = make_node<BinopExprNode>(BinopExprNode::OP::mult, $1, $3); }
    | term DIV factor { $$ = make_node<BinopExprNode>(BinopExprNode::OP::div, $1, $3); }
    | term MOD factor { $$ = make_node<BinopExprNode>(BinopExprNode::OP::mod, $1, $3); }
    | term AND factor { $$ = make_node<BinopExprNode>(BinopExprNode::OP::_and, $1, $3); }
    | factor { $$ = $1; }
;

factor: NAME { $$ = $1; }
    /*| NAME LP args_list RP*/
    | ID LP args_list RP            /* FUNCTION is parsed as ID */
    { $$ = make_node<ProcCallNode>($1, $3); }
    | SYS_FUNCT { $$ = make_node<SysProcCallNode>($1); }
    | SYS_FUNCT LP args_list RP { $$ = make_node<SysProcCallNode>($1, $3); }
    | const_value { $$ = $1; }
    | LP expression RP { $$ = $2; }
    | NOT factor { $$ = make_node<BinopExprNode>(BinopExprNode::OP::_xor, make_node<SysConNode>(SysConEnum::TRUE), $2); }
    | MINUS factor { $$ = make_node<BinopExprNode>(BinopExprNode::OP::minus, make_node<IntegerNode>(0), $2); }
    | ID LB expression RB { $$ = make_node<ArrayRefNode>($1, $3); }
    | ID DOT ID { make_node<ArrayRefNode>($1, $3); }
    | ID                            /* Add ID as one of the factors -- NOTICE */
    { $$ = $1; }
;
args_list: args_list COMMA expression { $$ = $1; $$->add($3); }
    | expression { $$ = make_node<ExprListNode>(); $$->add($1); }
;

%%

/*
int main() {
    yyparse();
}
*/

inline int yyerror(const char *s) {
    fprintf(stderr, "Yacc Error: %s\n", s);
    exit(-1);
}
