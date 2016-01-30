CXX         := g++
WARNFLAGS   := -Wno-deprecated-register
CXXFLAGS    := $(WARNFLAGS) -std=c++11
LNFLAGS     := 
YACC        := bison
LEX         := flex

OBJECTS     := Function.o main.o Natives.o Node.o parser.o Rewriter.o ConsoleInterface.o Environment.o tokens.o
EXECUTABLE  := mathy

debug: CXXFLAGS += -g
debug: all

release: CXXFLAGS += -O3 -fomit-frame-pointer -flto
release: LNFLAGS += -flto -O3
release: all

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LNFLAGS)

%.o: %.c
		$(CXX) -c -o $@ $< $(CXXFLAGS)

parser.cpp: parser.y
	$(YACC) --defines=parser.h -o $@ $<

tokens.cpp: tokens.l
	$(LEX) -o $@ $<

.PHONY: clean
clean:
	rm *.o parser.h parser.cpp tokens.cpp


