//
// Created by yzy on 6/7/18.
//
#define YY_STDINIT

#include "y.tab.h"

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

YY_BUFFER_STATE yy_scan_string(const char *yy_str);

#define NPC_BASIC_TEST

int main(int argc, char *argv[])
{
#ifdef NPC_BASIC_TEST
    const char s[] = R"EOF(
program x;
const pi=3.1415926;
var
    r: integer;
    s: real;
    x, y: integer;
    f: boolean;
    eq: boolean;
    i, sum: integer;
    j: char;
    arr: array[1..10] of integer;
function myabs(x: real):real;
var ret: real;
begin
    if x >= 0 then ret := x
    else ret := -x;
    myabs := ret;
end;
procedure calcAbs(x:real);
var r: real;
begin
    r := myabs(x);
    if r <> x then write('x < 0')
    else write('x > 0');
end;
begin
    readln(r);
    readln(x);
    readln(y);
    s := pi * sqr(r);

    { test if statement }
    if x < y then f := true
    else if x = y then eq := true
    else f := false;

    { test case statement }
    case x div 10 of
        10: writeln('10');
        9: writeln('9');
        8: if y <> 8 then writeln('x=8,y!=8') else writeln('x=8,y=8'); { case if compact }
        { else writeln('other'); }
        { seems this grammar is not required. }
    end;

    { for statement }
    sum := 0;
    for j := 'A' to 'Z' do begin
        write(j);
        { compact for }
        for i := 1 to 10 do sum := sum + i;
    end;
    writeln('sum=', sum);

    { repeat statment }
    sum := 0;
    i := 1;
    repeat
        sum := sum + i;
        i := i + 1;
        a[i] := i; { test array }
        if i > 5 then break; { test break }
    until (i >= 10);
    writeln('sum=', sum);
end.
    )EOF";

    yy_scan_string(s);

#else
    yyparse();
#endif
    return 0;
}
