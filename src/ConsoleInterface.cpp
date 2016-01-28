#include "ConsoleInterface.h"
#include <iostream>
#include <memory>
#include "Node.h"
#include "Environment.h"

extern std::shared_ptr<ExpressionNode> expr;
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
        try {
            ::yyparse();
        } catch(const char* ex) {
            cerr <<
               "\x1B[31;1m" "error:" "\x1b[0m" " " << ex << "\n";
            cerr.flush(); 
        }

        if (end_of_file)
            break;
        if (::expr != 0) {
            try {
                std::shared_ptr<ExpressionNode> evaluated =
                        environment.evaluateExpression(::expr);
                //evaluated = rewriter.replace(evaluated, gb);
                cout << evaluated->getString() << "\n";
            } catch(exception& ex) {
                cerr <<
                   "\x1B[31;1m" "error:" "\x1b[0m" " " << ex.what() << "\n";
                cerr.flush();
            }
        }
    }
    cout << "\n";
    return 0;
}

