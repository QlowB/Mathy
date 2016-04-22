CXX         := g++
WARNFLAGS   := -Wno-deprecated-register
CXXFLAGS    := $(WARNFLAGS) -std=c++11
LNFLAGS     :=
YACC        := bison
LEX         := flex

OBJECTS     := main.o Natives.o Node.o parser.o Rewriter.o ConsoleInterface.o Environment.o tokens.o sys.o
EXECUTABLE  := mathy

#bit32: CXXFLAGS += -m32
#bit32: LNFLAGS += -m32
#bit32: all

debug: CXXFLAGS += -g
debug: all

fast: CXXFLAGS += -flto -O3 -fomit-frame-pointer
fast: LNFLAGS += -flto -O3
fast: all

release: CXXFLAGS += -O3 -fomit-frame-pointer
release: LNFLAGS += -O3
release: all

release32: CXXFLAGS += -m32
release32: LNFLAGS += -m32
release32: release

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


