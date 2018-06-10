# This makefile for flex and bison.

UNAME := $(shell uname)

YACC=bison -dy
LEX=flex

ifneq (,$(wildcard /usr/local/opt/bison/bin/bison))
    YACC=/usr/local/opt/bison/bin/bison -dy
    $(info YACC detected in /usr/local/opt/bison/bin/bison)
endif

ifneq (,$(wildcard /usr/local/opt/flex/bin/flex))
    LEX=/usr/local/opt/flex/bin/flex
    $(info LEX detected in /usr/local/opt/flex/bin/flex)
endif

all: src/scan.g.cpp src/parse.g.cpp

src/scan.g.cpp: src/scan.l
	$(LEX) src/scan.l; mv lex.yy.c src/scan.g.cpp

src/parse.g.cpp: src/parse.y
	$(YACC) src/parse.y; mv y.tab.c src/parse.g.cpp; mv y.tab.h src/y.tab.h

clean:
	rm -f src/scan.g.cpp
	rm -f src/parse.g.cpp
	rm -f src/y.tab.h

.PHONY: all
.PHONY: clean

