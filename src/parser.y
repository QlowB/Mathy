/* =============================================================================
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
// ===========================================================================*/

%{

#include "Node.h"
#include "Natives.h"
#include <cstdlib>
#include <exception>
#include <memory>
#include <cstdio>
#include <iostream>

/*! \brief root node of the AST */
std::shared_ptr<ExpressionNode> expr;

extern int yylex();
void yyerror(const char *s)
{
    while(yylex());
    throw "parse error";
}

template <typename T>
using sp = std::shared_ptr<T>;


%}

/* %name-prefix "p" */
/* %define api.prefix p */

/* data structure for parser */
%union {
    ExpressionNode* expressionNode;
    StatementNode* statementNode;

    ConstantNode* constantNode;
    IntegerNode* integerNode;
    RealNode* realNode;
    VariableNode* variableNode;
    FunctionNode* functionNode;
    
    OperationNode* operationNode;
    AdditionNode* additionNode;
    SubtractionNode* subtractionNode;

    AssignmentNode* assignmentNode;
    
    std::vector<std::shared_ptr<ExpressionNode> >* expressionList;
    
    int token;
    std::string* string;
}


/*
 * terminals
 */
%token <string> TOKEN_IDENTIFIER TOKEN_INTEGER TOKEN_REAL

%token <token> TOKEN_ERROR
%token <token> TOKEN_NEWLINE
%token <token> TOKEN_LPAREN TOKEN_RPAREN TOKEN_LBRACE TOKEN_RBRACE TOKEN_COMMA
%token <token> TOKEN_DOT TOKEN_COLON
%token <string> TOKEN_OPERATOR

/*
 * non-terminals
 */
%type <expressionNode> expression parenthExpr
%type <constantNode> constant
%type <integerNode> integerConst
%type <realNode> realConst
%type <variableNode> variable
%type <functionNode> function
%type <expressionList> expressionList

%type <operationNode> operation
%type <operationNode> addition subtraction multiplication modulo division power
%type <assignmentNode> assignment
%type <statementNode> statement

/* operator precedence */
%nonassoc TOKEN_ASSIGNMENT
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MUL TOKEN_DIV TOKEN_MOD
%left TOKEN_POW

%start oneExpression

%%

oneExpression:
    expression {
        $<expressionNode>$ = $1;
        expr = std::shared_ptr<ExpressionNode>($<expressionNode>$);
    }
    |
    /* empty */
    ;

expression:
    constant {
        $$ = $1;
    }
    |
    function {
        $$ = $1;
    }
    |
    operation {
        $$ = $1;
    }
    |
    parenthExpr {
        $$ = $1;
    }
    |
    variable {
        $$ = $1;
    }
    |
    TOKEN_MINUS expression {
        $$ = new SubtractionNode(
            std::make_shared<IntegerNode>(0),
            std::shared_ptr<ExpressionNode>($2)
        );
    }
    |
    statement {
        $$ = $1;
    };

statement:
    assignment {
        $$ = $1;
    };

assignment:
    expression TOKEN_ASSIGNMENT expression {
        $$ = new AssignmentNode(
            std::shared_ptr<ExpressionNode>($1),
            std::shared_ptr<ExpressionNode>($3)
        );
    };

constant:
    integerConst {
        $$ = $1;
    }
    |
    realConst {
        $$ = $1;
    };

variable:
    TOKEN_IDENTIFIER {
        $$ = new VariableNode(*$1);
        delete $1;
        $1 = 0;
    };

integerConst:
    TOKEN_INTEGER {
        $$ = new IntegerNode(*$1);
        delete $1;
        $1 = 0;
    };

realConst:
    TOKEN_REAL {
        $$ = new RealNode(*$1);
        delete $1;
        $1 = 0;
    };

expressionList:
    expression {
        $$ = new std::vector<std::shared_ptr<ExpressionNode> >();
        $$->push_back(std::shared_ptr<ExpressionNode>($1));
    }
    |
    expressionList TOKEN_COMMA expression {
        $1->push_back(std::shared_ptr<ExpressionNode>($3));
    };

function:
    TOKEN_IDENTIFIER TOKEN_LPAREN expressionList TOKEN_RPAREN {
        Function* func = Functions::getNativeFunction(*$1, $3->size());
        if (func == 0) {
            std::cout << "no function " << *$1 << " with " << $3->size() << " arguments.\n";
            func = new Function(*$1, $3->size());
        }
        $$ = new FunctionNode(func, *$3);
        delete $1;
        delete $3;
        $1 = 0;
        $3 = 0;
    };

operation:
    addition {
        $$ = $1;
    }
    |
    subtraction {
        $$ = $1;
    }
    |
    multiplication {
        $$ = $1;
    }
    |
    modulo {
        $$ = $1;
    }
    |
    division {
        $$ = $1;
    }
    |
    power {
        $$ = $1;
    };

addition:
    expression TOKEN_PLUS expression {
        $$ = new AdditionNode(
            std::shared_ptr<ExpressionNode>($1),
            std::shared_ptr<ExpressionNode>($3)
        );
    };

subtraction:
    expression TOKEN_MINUS expression {
        $$ = new SubtractionNode(
            std::shared_ptr<ExpressionNode>($1),
            std::shared_ptr<ExpressionNode>($3)
        );
    };

multiplication:
    expression TOKEN_MUL expression {
        $$ = new MultiplicationNode(
            std::shared_ptr<ExpressionNode>($1),
            std::shared_ptr<ExpressionNode>($3)
        );
    };

modulo:
    expression TOKEN_MOD expression {
        $$ = new ModuloNode(
            std::shared_ptr<ExpressionNode>($1),
            std::shared_ptr<ExpressionNode>($3)
        );
    };

division:
    expression TOKEN_DIV expression {
        $$ = new DivisionNode(
            std::shared_ptr<ExpressionNode>($1),
            std::shared_ptr<ExpressionNode>($3)
        );
    };

power:
    expression TOKEN_POW expression {
        $$ = new PowerNode(
            std::shared_ptr<ExpressionNode>($1),
            std::shared_ptr<ExpressionNode>($3)
        );
    };

parenthExpr:
    TOKEN_LPAREN expression TOKEN_RPAREN {
        $$ = $2;
    };

%%


