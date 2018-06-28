//
// Created by yzy on 6/7/18.
//

#include <cstdio>
#include <memory>
#include "utils/ast.hpp"
#include "y.tab.h"

extern YYSTYPE program;

int main(int argc, char *argv[])
{
    if (argc > 1) freopen(argv[1], "r", stdin);
    yyparse();
    return 0;
}
