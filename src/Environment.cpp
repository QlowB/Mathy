#include "Environment.h"

const std::string& Symbol::getName(void) const
{
    return name;
}


Symbol::~Symbol(void)
{
}


VariableSymbol::VariableSymbol(const std::string& name,
                               const std::shared_ptr<ExpressionNode>& value) :
    value(value)
{
    this->name = name;
}


const std::shared_ptr<ExpressionNode>& VariableSymbol::getValue(void)
{
    return value;
}


FunctionParameter::FunctionParameter(int index) :
    index(index)
{
}


int FunctionParameter::getIndex(void) const
{
    return index;
}


Environment::Environment(void)
{
}


std::shared_ptr<ExpressionNode> Environment::evaluateExpression(
        const std::shared_ptr<ExpressionNode>& expr)
{
    return expr->evaluate(this);
}


void Environment::addSymbol(Symbol* s)
{
    symbols.push_back(s);
}


VariableSymbol* Environment::getVariable(const std::string& name)
{
    for (size_t i = 0; i < symbols.size(); i++) {
        if (dynamic_cast<VariableSymbol*> (symbols[i]) &&
                dynamic_cast<VariableSymbol*> (symbols[i])->getName() == name) {
            return dynamic_cast<VariableSymbol*> (symbols[i]);
        }
    }
    return 0;
}


AssignmentException::AssignmentException(const std::string& message) :
    message(message)
{
}


AssignmentException::~AssignmentException(void) throw()
{
}


const char* AssignmentException::what(void) const throw()
{
    return message.c_str();
}

