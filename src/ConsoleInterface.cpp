// =============================================================================
//
// This file is part of the Mathy computer algebry system.
//
// Copyright (C) 2015-2016 Nicolas Winkler
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// =============================================================================

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
            printErrorMessage(ex);
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
                printErrorMessage(ex.what());
                cerr.flush();
            }
        }
    }
    cout << "\n";
    return 0;
}


void ConsoleInterface::printErrorMessage(const std::string& message)
{
    using std::cerr;
    cerr << "\x1B[31;1m" "error:" "\x1b[0m" " " << message << "\n";
}
