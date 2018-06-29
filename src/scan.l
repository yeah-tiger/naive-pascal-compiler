%{
    #include <cstdio>
    #include <cassert>
    #include <limits>
    #include "utils/ast.hpp"
    #include "utils/ast_utils.hpp"
    #include "y.tab.h"

    using namespace npc;

    int line_no = 1;
    void commenteof();
%}

A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
NQUOTE [^']

%option noyywrap
%%

{A}{N}{D}                   return AND;
{A}{R}{R}{A}{Y}             return ARRAY;
{B}{E}{G}{I}{N}             return _BEGIN;
{C}{A}{S}{E}                return CASE;
{C}{O}{N}{S}{T}             return CONST;
{D}{I}{V}                   return DIV;
{D}{O}                      return DO;
{D}{O}{W}{N}{T}{O}          return DOWNTO;
{E}{L}{S}{E}                return ELSE;
{E}{N}{D}                   return END;
{F}{O}{R}                   return FOR;
{F}{U}{N}{C}{T}{I}{O}{N}    return FUNCTION;
{G}{O}{T}{O}                return GOTO;
{I}{F}                      return IF;
{M}{O}{D}                   return MOD;
{N}{O}{T}                   return NOT;
{O}{F}                      return OF;
{O}{R}                      return OR;
{P}{R}{O}{C}{E}{D}{U}{R}{E} return PROCEDURE;
{P}{R}{O}{G}{R}{A}{M}       return PROGRAM;
{R}{E}{C}{O}{R}{D}          return RECORD;
{R}{E}{P}{E}{A}{T}          return REPEAT;
{T}{H}{E}{N}                return THEN;
{T}{O}                      return TO;
{T}{Y}{P}{E}                return TYPE;
{U}{N}{T}{I}{L}             return UNTIL;
{V}{A}{R}                   return VAR;
{W}{H}{I}{L}{E}             return WHILE;

{B}{O}{O}{L}{E}{A}{N} {
    yylval = make_node<SimpleTypeNode>(Type::BOOLEAN);
    return SYS_TYPE;
}
{I}{N}{T}{E}{G}{E}{R} {
    yylval = make_node<SimpleTypeNode>(Type::INTEGER);
    return SYS_TYPE;
}
{R}{E}{A}{L} {
    yylval = make_node<SimpleTypeNode>(Type::REAL);
    return SYS_TYPE;
}
{C}{H}{A}{R} {
    yylval = make_node<SimpleTypeNode>(Type::CHAR);
    return SYS_TYPE;
}
{S}{T}{R}{I}{N}{G} {
    yylval = make_node<StringTypeNode>();
    return SYS_TYPE;
}

{F}{A}{L}{S}{E}|{T}{R}{U}{E}|{M}{A}{X}{I}{N}{T} {
    switch (yytext[0])
    {
    case 'f':
    case 'F':
        yylval = make_node<BooleanNode>(false);
        break;
    case 't':
    case 'T':
        yylval = make_node<BooleanNode>(true);
        break;
    case 'm':
    case 'M':
        yylval = make_node<IntegerNode>(std::numeric_limits<int>::max());
        break;
    default:
        assert(false);
    }
    return SYS_CON;
}

{A}{B}{S} { yylval = make_node<SysRoutineNode>(SysRoutine::ABS); return SYS_FUNC; }
{C}{H}{R} { yylval = make_node<SysRoutineNode>(SysRoutine::CHR); return SYS_FUNC; }
{O}{D}{D} { yylval = make_node<SysRoutineNode>(SysRoutine::ODD); return SYS_FUNC; }
{O}{R}{D} { yylval = make_node<SysRoutineNode>(SysRoutine::ORD); return SYS_FUNC; }
{P}{R}{E}{D} { yylval = make_node<SysRoutineNode>(SysRoutine::PRED); return SYS_FUNC; }
{S}{Q}{R} { yylval = make_node<SysRoutineNode>(SysRoutine::SQR); return SYS_FUNC; }
{S}{Q}{R}{T} { yylval = make_node<SysRoutineNode>(SysRoutine::SQRT); return SYS_FUNC; }
{S}{U}{C}{C} { yylval = make_node<SysRoutineNode>(SysRoutine::SUCC); return SYS_FUNC; }
{R}{E}{A}{D} { yylval = make_node<SysRoutineNode>(SysRoutine::READ); return READ_FUNC; }
{R}{E}{A}{D}{L}{N} { yylval = make_node<SysRoutineNode>(SysRoutine::READLN); return READ_FUNC; }
{W}{R}{I}{T}{E} { yylval = make_node<SysRoutineNode>(SysRoutine::WRITE); return SYS_PROC; }
{W}{R}{I}{T}{E}{L}{N} { yylval = make_node<SysRoutineNode>(SysRoutine::WRITELN); return SYS_PROC; }

[a-zA-Z_]([a-zA-Z0-9_])* {
    yylval = make_node<IdentifierNode>(yytext);
    return ID;
}

[+-]?[0-9]+             { yylval = make_node<IntegerNode>(yytext); return INTEGER; }
[+-]?[0-9]+"."[0-9]+    { yylval = make_node<RealNode>(yytext); return REAL; }
'{NQUOTE}'              { yylval = make_node<CharNode>(yytext); return CHAR; }
'({NQUOTE}|'')+'        { yylval = make_node<StringNode>(yytext); return STRING; }

":="                return ASSIGN;
":"                 return COLON;
","                 return COMMA;
"."                 return DOT;
".."                return DOTDOT;
"="                 return EQUAL;
">="                return GE;
">"                 return GT;
"["                 return LB;
"<="                return LE;
"("                 return LP;
"<"                 return LT;
"-"                 return MINUS;
"<>"                return UNEQUAL;
"+"                 return PLUS;
"]"                 return RB;
")"                 return RP;
";"                 return SEMI;
"/"                 return TRUEDIV;
"*"                 return MUL;
"{"    {
    int c;
    while ((c = yyinput())) {
        if (c == '}') break;
        else if (c == '*') {
            if ((c = yyinput()) == ')') {
                break;
            } else {
                unput(c);
            }
        } else if (c == '\n') line_no++;
        else if (c == 0) commenteof();
    }
}

[ \t\f]    ;

\n   { line_no++; }

.    { fprintf (stderr, "'%c' (0%o): illegal character at line %d\n", yytext[0], yytext[0], line_no); }

%%

inline void commenteof() {
    fprintf(stderr, "unexpected EOF inside comment at line %d\n", line_no);
    exit(1);
}
