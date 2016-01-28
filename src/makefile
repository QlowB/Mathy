CXX         := clang++
CXXFLAGS    := -g #-O3 -fomit-frame-pointer -flto
LNFLAGS     := #-flto
YACC        := bison
LEX         := flex

OBJECTS     := Function.o main.o Natives.o Node.o parser.o Rewriter.o ConsoleInterface.o Environment.o tokens.o
EXECUTABLE  := mathy

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LNFLAGS)

%.o: %.c
		$(CXX) -c -o $@ $< $(CXXFLAGS)

parser.cpp: parser.y
	$(YACC) --defines=parser.h -o $@ $<

tokens.cpp: tokens.l
	$(LEX) -o $@ $<


