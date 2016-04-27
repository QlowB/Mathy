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
/*
#include <termios.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
*/
extern std::shared_ptr<ExpressionNode> expr;
extern int yyparse(void);
extern FILE* yyin;
extern bool end_of_file;


using mathy::InputProcessor;
using mathy::ConsoleInterface;


InputProcessor::InputProcessor(::FILE* in, ::FILE* out) :
    in(in), out(out)
{
}


int InputProcessor::run(void)
{
    Environment environment;

    while (true) {
        try {
            ::yyin = this->in;
            ::yyparse();
        } catch(const char* ex) {
            printErrorMessage(ex);
        }

        if (end_of_file)
            break;
        if (::expr.get() != nullptr) {
            try {
                std::shared_ptr<ExpressionNode> evaluated =
                        environment.evaluateExpression(::expr);
                ::fprintf(this->out, "%s\n", evaluated->getString().c_str());
                ::expr = std::shared_ptr<ExpressionNode>(nullptr);
            } catch(std::exception& ex) {
                printErrorMessage(ex.what());
            }
        }
    }
    ::fprintf(this->out, "\n");
    return 0;
}


void InputProcessor::printErrorMessage(const std::string& message)
{
    using std::cerr;
    cerr << "\x1B[31;1m" "error:" "\x1b[0m" " " << message << "\n";
    cerr.flush();
}


ConsoleInterface::ConsoleInterface(::FILE* in, ::FILE* out) :
    InputProcessor(in, out)
{
}


int ConsoleInterface::run(void)
{
    Environment environment;
/*
    struct termios term, term2;
    tcgetattr(STDIN_FILENO, &term2);
    term = term2;
    term.c_lflag &= ~ICANON;
    term.c_cc[VMIN] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    sigset_t mask = 0;
    sigemptyset(&mask);

    struct pollfd pollstruct;
    pollstruct.events = POLLIN;
    pollstruct.fd = STDIN_FILENO;
    size_t count;
*/
    while (true) {
        /*
        pollstruct.revents = 0;
        int ret = ::poll(&pollstruct,
                sizeof(pollstruct) / sizeof(struct pollfd), 0);
        
        if (pollstruct.revents & POLLIN)
        {
            char buf[1024];
            ret = read(STDIN_FILENO, buf, sizeof(buf) - count);
            printf("hello: %s", buf);
        }
        else {
        }*/

        try {
            ::yyin = this->in;
            ::yyparse();
        } catch(const char* ex) {
            printErrorMessage(ex);
        }

        if (end_of_file) {
            break;
        }

        if (::expr != 0) {
            try {
                std::shared_ptr<ExpressionNode> evaluated =
                        environment.evaluateExpression(::expr);
                ::fprintf(this->out, "\x1b[36m" " --> " "\x1b[32m" "%s"
                        "\x1b[0m" "\n",
                        evaluated->getString().c_str());
                ::expr = std::shared_ptr<ExpressionNode>(nullptr);
            } catch(std::exception& ex) {
                printErrorMessage(ex.what());
            }
        }
    }
    
    ::fprintf(this->out, "\n");


    return 0;
}




