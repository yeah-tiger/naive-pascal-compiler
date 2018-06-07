# This makefile for flex and bison.

YACC=bison -dy
LEX=flex

all: scan.g.cpp parse.g.cpp

scan.g.cpp: src/scan.l
	$(LEX) src/scan.l; mv lex.yy.c src/scan.g.cpp

parse.g.cpp: src/parse.y
	$(YACC) src/parse.y; mv y.tab.c src/parse.g.cpp

clean:
	rm -f src/scan.g.cpp
	rm -f src/parse.g.cpp

.PHONY: all
.PHONY: clean

