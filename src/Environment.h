#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Node.h"
#include <vector>
#include <exception>


class Symbol
{
protected:
    std::string name;
};


class VariableSymbol : public Symbol
{
protected:
    ExpressionNode* value;
public:
    VariableSymbol(const std::string& name, ExpressionNode* value);
};


class FunctionParameter : public ExpressionNode
{
    int index;
public:
    FunctionParameter(int index);
    int getIndex(void) const;
};


class FunctionSymbol : public Symbol
{
protected:
    size_t argumentCount;

    //! function equation
    ExpressionNode* definition;
};


class Environment
{
    std::vector<Symbol*> symbols;
public:

    Environment(void);

    ExpressionNode* evaluateExpression(ExpressionNode* expr, GarbageBag& gb);

    void addSymbol(Symbol* s);
};


class AssignmentException :
        public std::exception
{
    std::string message;
public:
    AssignmentException(const std::string& message);
    ~AssignmentException(void) throw();
    const char* what(void) const throw();
};

#endif // ENVIRONMENT_H_

