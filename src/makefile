CXX := g++
CXXFLAGS := 
LINKFLAGS := -O3
YACC := bison
LEX := flex

OBJECTS := Function.o main.o Natives.o Node.o parser.o Rewriter.o tokens.o
EXECUTABLE := mathy

all: mathy

mathy: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
		$(CXX) -c -o $@ $< $(CXXFLAGS)

parser.cpp: parser.y
	$(YACC) -d -o $@ $<

tokens.cpp: tokens.l
	$(LEX) -o $@ $<


