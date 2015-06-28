%{
    #include "Node.h"
    #include "Natives.h"
    #include <stdlib.h>
    #include <stdio.h>
    ExpressionNode* expr; /* the top level root node of our syntax tree */

    extern int yylex();
    void yyerror(const char *s) { printf("parse error: %s\n", s); }
%}

/* define data structure for parser use */
%union {
    ExpressionNode* expressionNode;
    ConstantNode* constantNode;
    IntegerNode* integerNode;
    RealNode* realNode;
    VariableNode* variableNode;
    FunctionNode* functionNode;
    
    OperationNode* operationNode;
    AdditionNode* additionNode;
    SubtractionNode* subtractionNode;
    
    std::vector<ExpressionNode*>* expressionList;
    
    int token;
    std::string* string;
}

/*
 * terminals
 *
 * These constants should match the ones defined in tokens.l
 */
%token <string> TIDENTIFIER TINTEGER TREAL

%token <token> TNEWLINE
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT TCOLON
%token <string> TOPERATOR

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


/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV TMOD
%left TPOW

%start oneExpression

%%

oneExpression:
    expression {
        $<expressionNode>$ = $1;
        expr = $<expressionNode>$;
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
    TMINUS expression {
        $$ = new SubtractionNode(new IntegerNode(0), $2);
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
    TIDENTIFIER {
        $$ = new VariableNode(*$1);
        delete $1;
        $1 = 0;
    };

integerConst:
    TINTEGER {
        $$ = new IntegerNode(*$1);
        delete $1;
        $1 = 0;
    };

realConst:
    TREAL {
        $$ = new RealNode(*$1);
        delete $1;
        $1 = 0;
    };

expressionList:
    expression {
        $$ = new std::vector<ExpressionNode*>();
        $$->push_back($1);
    }
    |
    expressionList TCOMMA expression {
        $1->push_back($3);
    };

function:
    TIDENTIFIER TLPAREN expressionList TRPAREN {
        Function* func = Functions::getNativeFunction(*$1, $3->size());
        if (func == 0)
            func = new Function(*$1, $3->size());
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
    expression TPLUS expression {
        $$ = new AdditionNode($1, $3);
    };

subtraction:
    expression TMINUS expression {
        $$ = new SubtractionNode($1, $3);
    };

multiplication:
    expression TMUL expression {
        $$ = new MultiplicationNode($1, $3);
    };

modulo:
    expression TMOD expression {
        $$ = new ModuloNode($1, $3);
    };

division:
    expression TDIV expression {
        $$ = new DivisionNode($1, $3);
    };

power:
    expression TPOW expression {
        $$ = new PowerNode($1, $3);
    };

parenthExpr:
    TLPAREN expression TRPAREN {
        $$ = $2;
    };

%%


