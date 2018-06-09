%{
    #include <cstdio>
    #include <string>
    #include <cassert>
    #include "utils/ast.hpp"
    #include "utils/ast_utils.hpp"

    using namespace npc;

    #ifdef YYSTYPE
    #undef YYSTYPE
    #endif  // YYSTYPE

    #define YYSTYPE_IS_DECLARED

    using YYSTYPE = std::shared_ptr<AbstractNode>;
    #include "y.tab.h"

    int line_no;
    void commenteof();
    // int yywrap();
    int yyerror(const char *s);

    #define HASH_TABLE_SIZE 2000
    char* dummy_hash_table[HASH_TABLE_SIZE];
    int hash_size = 0;
    bool naming = false;
    void add_to_hash_table(const char *text) {
        char *str = (char *)malloc((strlen(text) + 1) * sizeof(char));
        strcpy(str, text);
        dummy_hash_table[hash_size++] = str;
    }
    bool in_hash_table(const char *text) {
        for (int i=0; i<hash_size; i++) {
            if (strcmp(dummy_hash_table[i], text) == 0) {
                return true;
            }
        }
        return false;
    }

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

%%

{A}{N}{D}                   return AND;
{A}{R}{R}{A}{Y}             return ARRAY;
{B}{E}{G}{I}{N}             { naming = false; return(_BEGIN); }
{C}{A}{S}{E}                return CASE;
{C}{O}{N}{S}{T}             { naming = true; return CONST; }
{D}{I}{V}                   return DIV;
{D}{O}                      return DO;
{D}{O}{W}{N}{T}{O}          return DOWNTO;
{E}{L}{S}{E}                return ELSE;
{E}{N}{D}                   return END;
{F}{O}{R}                   return FOR;
{F}{U}{N}{C}{T}{I}{O}{N}    { naming = false; return FUNCTION; }
{G}{O}{T}{O}                return GOTO;
{I}{F}                      return IF;
{M}{O}{D}                   return MOD;
{N}{O}{T}                   return NOT;
{O}{F}                      return OF;
{O}{R}                      return OR;
{P}{R}{O}{C}{E}{D}{U}{R}{E} { naming = false; return PROCEDURE; }
{P}{R}{O}{G}{R}{A}{M}       return PROGRAM;
{R}{E}{C}{O}{R}{D}          return RECORD;
{R}{E}{P}{E}{A}{T}          return REPEAT;
{T}{H}{E}{N}                return THEN;
{T}{O}                      return TO;
{T}{Y}{P}{E}                { naming = true; return TYPE; }
{U}{N}{T}{I}{L}             return UNTIL;
{V}{A}{R}                   { naming = false; return VAR; }
{W}{H}{I}{L}{E}             return WHILE;

{I}{N}{T}{E}{G}{E}{R}|{R}{E}{A}{L}|{B}{O}{O}{L}{E}{A}{N}|{C}{H}{A}{R}   return SYS_TYPE;
{F}{A}{L}{S}{E}|{T}{R}{U}{E}|{M}{A}{X}{I}{N}{T} {
    switch (yytext[0])
    {
    case 'f':
    case 'F':
        yylval = make_node<SysConNode>(SysConEnum::FALSE);
        break;
    case 't':
    case 'T':
        yylval = make_node<SysConNode>(SysConEnum::TRUE);
        break;
    case 'm':
    case 'M':
        yylval = make_node<SysConNode>(SysConEnum::MAXINT);
        break;
    default:
        assert(false);
    }
    return SYS_CON;
}
{A}{B}{S}|{C}{H}{R}|{O}{R}{D}|{P}{R}{E}{D}|{S}{Q}{R}|{S}{U}{C}{C}       return SYS_FUNCT;
{R}{E}{A}{D}|{R}{E}{A}{D}{L}{N}                                         return READ;
{W}{R}{I}{T}{E}|{W}{R}{I}{T}{E}{L}{N}                                   return SYS_PROC;

[a-zA-Z]([a-zA-Z0-9])* {
    if (naming) {
        add_to_hash_table(yytext);
        yylval = make_node<NameNode>(std::string(yytext));
        return NAME;
    }
    if (in_hash_table(yytext)) {
        yylval = make_node<NameNode>(std::string(yytext));
        return NAME;
    }
    // printf("\nReturning Id: %s\n", yytext);
    return ID;
}

":="                return ASSIGN;
'({NQUOTE}|'')+'    return STRING;
":"                 return COLON;
","                 return COMMA;
[0-9]+              return INTEGER;
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
[0-9]+"."[0-9]+     return REAL;
")"                 return RP;
";"                 return SEMI;
"/"                 return DIV;
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

.    { fprintf (stderr, "'%c' (0%o): illegal charcter at line %d\n", yytext[0], yytext[0], line_no); }

%%

inline void commenteof() {
    fprintf(stderr, "unexpected EOF inside comment at line %d\n", line_no);
    exit(1);
}

inline int yywrap() {
    return 1;
}

inline int yyerror(const char *s) {
    fprintf(stderr, "Yacc Error: %s\n", s);
    exit(-1);
}