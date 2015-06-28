#include "Environment.h"

VariableSymbol::VariableSymbol(const std::string& name, ExpressionNode* value) :
    value(value)
{
    this->name = name;
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
    ExpressionNode* evaluated = expr->evaluate(gb);
    if (dynamic_cast<StatementNode*> (evaluated) != 0) {
        AssignmentNode* an = dynamic_cast<AssignmentNode*> (evaluated);
        if (an != 0) {
            VariableNode* variable = dynamic_cast<VariableNode*> (an->a);
            if (variable != 0) {
                VariableSymbol* vs = new VariableSymbol(variable->getString(), an->b);
                addSymbol(vs);
            } else
                throw AssignmentException("left side of assignment must be a variable");
        }
    }
    //evaluated = rewriter.replace(evaluated, gb);
    return evaluated;
}


void Environment::addSymbol(Symbol* s)
{
    symbols.push_back(s);
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

