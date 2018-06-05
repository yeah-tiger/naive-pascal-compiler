program x;
const pi=3.1415926;
var
    r: integer;
    s: real;
begin
    readln(r);
    s := pi * sqr(r);
    writeln('s=', s);
end.