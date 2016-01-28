#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Node.h"
#include <vector>
#include <memory>
#include <exception>


class Symbol
{
protected:
    std::string name;
public:
    const std::string& getName(void) const;
    virtual ~Symbol(void);
};


class VariableSymbol : public Symbol
{
protected:
    std::shared_ptr<ExpressionNode> value;
public:
    VariableSymbol(const std::string& name,
                   const std::shared_ptr<ExpressionNode>& value);
    const std::shared_ptr<ExpressionNode>& getValue(void);
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
    // TODO implement this as a hash table
    std::vector<Symbol*> symbols;
public:

    Environment(void);

    std::shared_ptr<ExpressionNode> evaluateExpression(
            const std::shared_ptr<ExpressionNode>& expr);

    void addSymbol(Symbol* s);
    VariableSymbol* getVariable(const std::string& name);
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

