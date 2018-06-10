//
// Created by yzy on 6/7/18.
//
#define YY_STDINIT
#include "y.tab.h"

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif
YY_BUFFER_STATE yy_scan_string ( const char *yy_str  );

int main(int argc, char *argv[])
{
    char s[] = "program x;"
               "const pi=3.1415926;"
               "var "
               "r: integer;";
//    yy_scan_string(s);
    yyparse();
    return 0;
}
