program prog;
const
  pi=3.1415926;
var
  radius_int: integer;
  radius_real, s, p: real;
begin
  radius_int := 5;
  radius_int := radius_int + 1;
  radius_real := radius_int / 2.0;
  radius_real := radius_real * 2;
  radius_real := radius_real - 3;
  radius_real := radius_real - 0.5;
  s := pi * radius_real * radius_real;
  p := 2 * pi * radius_real;
  writeln('s=', s);
  writeln('p=', p);
end.