//
// Created by yzy on 6/7/18.
//

#include <cstdio>

int yyparse();

int main(int argc, char *argv[])
{
    if (argc >= 2) freopen(argv[1], "r", stdin);
    yyparse();
    return 0;
}
