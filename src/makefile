all:
	bison -d -o parser.cpp parser.y
	lex -o tokens.cpp tokens.l
	g++ -o mathy *.cpp

release:
	bison -d -o parser.cpp parser.y
	lex -o tokens.cpp tokens.l
	g++ -o mathy *.cpp -static -O3

