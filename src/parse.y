%require "3.0.4"

%{
    #include <cstdio>
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

%token PROGRAM ID CONST NAME ARRAY VAR FUNCTION PROCEDURE _BEGIN END TYPE READ RECORD
%token INTEGER REAL CHAR STRING
%token SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE
%token IF THEN ELSE REPEAT UNTIL WHILE DO FOR TO DOWNTO CASE OF GOTO
%token ASSIGN EQUAL UNEQUAL LE LT GE GT
%token PLUS MINUS MUL DIV OR AND MOD NOT
%token DOT DOTDOT SEMI LP RP LB RB COMMA COLON

%start program

%%
program:
    program_head { printf("Finishing head parsing...\n"); }
    routine { printf("Finishing routine...\n"); }
    DOT { printf("End parsing\n"); exit(0); }
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
    |
    ;

type_decl_list: type_decl_list type_definition { $$ = $1; $$->add($2); }
|               type_definition { $$ = make_node<TypePartNode>(); $$->add($1); }
;

type_definition: NAME EQUAL type_decl SEMI
    {
        $$ = make_node<TypeDefNode>($1, $3);
    }
    ;

// TODO
type_decl: simple_type_decl | array_type_decl | record_type_decl
;

simple_type_decl: SYS_TYPE | NAME | LP name_list RP
    | const_value DOTDOT const_value
    | MINUS const_value DOTDOT const_value
    | MINUS const_value DOTDOT MINUS const_value
    | NAME DOTDOT NAME
;
array_type_decl: ARRAY LB simple_type_decl RB OF type_decl
;
record_type_decl: RECORD field_decl_list END
;
field_decl_list: field_decl_list field_decl
    | field_decl
;
field_decl: name_list COLON type_decl SEMI
;
name_list: name_list COMMA ID { $$ = $1; $$->add($3); }
    | ID { $$ = make_node<NameList>(); $$->add($1); }
;
var_part: VAR var_decl_list { $$ = $2; }
| { $$ = make_node<VarDeclList>(); }
;
var_decl_list: var_decl_list var_decl {
    $$ = $1;
    $$->merge_children($2->children());
}
    | var_decl { $$ = $1; }
;
var_decl: name_list COLON type_decl SEMI {
    $$ = make_node<VarDeclList>();
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
    | READ LP factor RP { $$ = make_node<ProcStmtNode>(cast_node<SysProcCallNode>(make_node<SysProcCallNode>($1, $3))); }
;
if_stmt: IF expression THEN stmt else_clause { $$ = make_node<IfStmtNode>($2, $4, $5); }
;
else_clause: ELSE stmt { $$ = $2; }
| { $$ = make_node<DummyStmtNode>(); }
;
repeat_stmt: REPEAT stmt_list UNTIL expression { $$ = make_node<RepeatStmtNode>($2, $4); }
;
while_stmt: WHILE expression DO stmt { $$ = make_node<WhileStmtNode>($2, $4); }
;
for_stmt: FOR ID ASSIGN expression direction expression DO stmt { $$ = make_node<ForStmtNode>(make_node<AssignStmtNode>($2, $4), $5, $6, $8); }
;
direction: TO { $$ = make_node<DirectionNode>(false); } | DOWNTO { $$ = make_node<DirectionNode>(true); }
;
case_stmt: CASE expression OF case_expr_list END { $$ = make_node<CaseStmtNode>($4, $2); }
;
case_expr_list: case_expr_list case_expr { $$ = $1; $$->add($2); }
    | case_expr { $$ = make_node<ExprList>(); $$->add($1); }
;
case_expr: const_value COLON stmt SEMI
| ID COLON stmt SEMI
;
goto_stmt: GOTO INTEGER { $$ = make_node<GotoStmtNode>($2); }
;
expression_list: expression_list COMMA expression { $$ = $1; $$->add($3); }
    | expression    { $$ = make_node<ExprList>(); $$->add($1); }
;
expression: expression GE expr
    | expression GT expr
    | expression LE expr
    | expression LT expr
    | expression EQUAL expr
    | expression UNEQUAL expr
    | expr
;
expr: expr PLUS term
    | expr MINUS term
    | expr OR term
    | term
;
term: term MUL factor
    | term DIV factor
    | term MOD factor
    | term AND factor
    | factor
;
factor: NAME
    | NAME LP args_list RP
    | ID LP args_list RP            /* FUNCTION is parsed as ID */
    | SYS_FUNCT
    | SYS_FUNCT LP args_list RP
    | const_value
    | LP expression RP
    | NOT factor
    | MINUS factor
    | ID LB expression RB
    | ID DOT ID
    | ID                            /* Add ID as one of the factors -- NOTICE */
;
args_list: args_list COMMA expression { $$ = $1; $$->add($3); }
    | expression { $$ = make_node<ExprList>(); $$->add($1); }
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
