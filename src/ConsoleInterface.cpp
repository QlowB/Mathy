#include "ConsoleInterface.h"
#include <iostream>
#include "Node.h"
#include "Environment.h"

extern ExpressionNode* expr;
extern int yyparse(void);
extern bool end_of_file;


ConsoleInterface::ConsoleInterface(int argc, char **argv)
{

}


int ConsoleInterface::run(void)
{
    using namespace std;

    Environment environment;

    while (true) {
        ::yyparse();

        if (end_of_file)
            break;
        if (::expr != 0) {
            GarbageBag gb;
            try {
                ExpressionNode* evaluated =
                        environment.evaluateExpression(::expr, gb);
                //evaluated = rewriter.replace(evaluated, gb);
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
    cout << "\n";
    return 0;
}