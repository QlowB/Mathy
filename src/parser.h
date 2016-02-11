/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_IDENTIFIER = 258,
    TOKEN_INTEGER = 259,
    TOKEN_REAL = 260,
    TOKEN_ERROR = 261,
    TOKEN_NEWLINE = 262,
    TOKEN_LPAREN = 263,
    TOKEN_RPAREN = 264,
    TOKEN_LBRACE = 265,
    TOKEN_RBRACE = 266,
    TOKEN_COMMA = 267,
    TOKEN_DOT = 268,
    TOKEN_COLON = 269,
    TOKEN_OPERATOR = 270,
    TOKEN_ASSIGNMENT = 271,
    TOKEN_PLUS = 272,
    TOKEN_MINUS = 273,
    TOKEN_MUL = 274,
    TOKEN_DIV = 275,
    TOKEN_MOD = 276,
    TOKEN_POW = 277
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 52 "parser.y" /* yacc.c:1909  */

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

#line 99 "parser.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
