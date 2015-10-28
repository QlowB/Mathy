#include "Environment.h"

const std::string& Symbol::getName(void) const
{
    return name;
}


Symbol::~Symbol(void)
{
}


VariableSymbol::VariableSymbol(const std::string& name, ExpressionNode* value) :
    value(value)
{
    this->name = name;
}


ExpressionNode* VariableSymbol::getValue(void)
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


ExpressionNode* Environment::evaluateExpression
(ExpressionNode* expr, GarbageBag& gb)
{
    ExpressionNode* evaluated = expr->evaluate(this, gb);
    /*if (dynamic_cast<StatementNode*> (evaluated) != 0) {
        AssignmentNode* an = dynamic_cast<AssignmentNode*> (evaluated);
        if (an != 0) {
            VariableNode* variable = dynamic_cast<VariableNode*> (an->a);
            if (variable != 0) {
                VariableSymbol* vs = new VariableSymbol(variable->getString(), an->b);
                addSymbol(vs);
            } else
                throw AssignmentException("left side of assignment must be a variable");
        }
    }*/
    //evaluated = rewriter.replace(evaluated, gb);
    return evaluated;
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

