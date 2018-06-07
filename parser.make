# This makefile for flex and bison.

YACC=bison -dy
LEX=lex --c++

all: src/scan.g.cpp src/parse.g.cpp

src/scan.g.cpp: src/scan.l
	$(LEX) src/scan.l; mv lex.yy.cc src/scan.g.cpp

src/parse.g.cpp: src/parse.y
	$(YACC) src/parse.y; mv y.tab.c src/parse.g.cpp; mv y.tab.h src/y.tab.h

clean:
	rm -f src/scan.g.cpp
	rm -f src/parse.g.cpp
	rm -f src/y.tab.h

.PHONY: all
.PHONY: clean

