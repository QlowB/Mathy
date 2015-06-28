#include <iostream>
#include "Node.h"
#include "Rewriter.h"

extern ExpressionNode* expr;
extern int yyparse(void);
extern bool end_of_file;

int main(int argc, char** argv) {
	using namespace std;
	
	Rewriter rewriter;
	
	while (true) {
		cout << ">> ";
		cout.flush();
                ::yyparse();

                if (end_of_file)
                    break;
		if (::expr != 0) {
			GarbageBag gb;
			try {
				//cout << "evaluating: " << ::expr->getString() << "\n";
				ExpressionNode* evaluated = ::expr->evaluate(gb);
				evaluated = rewriter.replace(evaluated, gb);
				cout << evaluated->getString() << "\n";
				delete ::expr;
				::expr = 0;
			} catch(exception& ex) {
				cerr << "error: " << ex.what() << "\n";
				cerr.flush();
			}
			gb.free();
                } 
	}
}

