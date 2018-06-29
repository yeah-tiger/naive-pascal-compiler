program prog;
const
  pi=3.1415926;
var
  i,j,rep: integer;
  f: boolean;
begin
  for i:=1 to 10 do
  begin
    j := 1;
    repeat
      f := i < j;
      rep := 0;
      while rep < 2 do
      begin
        case f of
          true: write(i, ' ');
          false: write('  ');
        end;
        rep := rep + 1;
      end;
      j := j + 1;
    until j > 10;
    writeln();
  end;
end.