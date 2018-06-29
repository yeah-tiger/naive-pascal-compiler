program prog;
const
  pi=3.1415926;
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
  writeln('x = ', x);
  if r <> x then writeln('x < 0')
  else writeln('x > 0');
  write('abs(x) = ', r);
end;
begin
  calcAbs(-pi);
end.