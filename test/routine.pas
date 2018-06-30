program prog;
var
  x: integer;

function f(x: integer): integer;
  begin
    if x = 0 then f := 0
    else f := f(x-1) + x;
  end;

procedure g(x: integer);
  var
    i: integer;
  begin
    for i := 1 to x do writeln(f(i));
  end;

begin
  readln(x);
  g(x);
end.
