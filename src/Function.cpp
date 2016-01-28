#include "Natives.h"
#include "Node.h"
#include <sstream>
#include <iostream>
#include <typeinfo>

Function::Function(const std::string& name, size_t argumentCount) :
	name(name), argumentCount(argumentCount)
{
    std::cout << getName() << std::endl;
    //std::cout << typeid(this).name() << std::endl;
}

#include <iostream>
std::shared_ptr<ExpressionNode> Function::eval(
        Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
    // std::cout << "Function::eval\n";
    return std::make_shared<FunctionNode> (this, args);
    //return this;
}


const std::string& Function::getName(void) const
{
	return name;
}


size_t Function::getArgumentCount(void) const
{
	return argumentCount;
}


std::shared_ptr<ExpressionNode> Function::getDerivative(
        size_t i,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
	Function* derivFunc;

    if (argumentCount == 1)
        derivFunc = new Function(name + "'", argumentCount);
    else {
        std::stringstream ind;
        ind << i;
        derivFunc = new Function(name + "'(" + ind.str() + ")", argumentCount);
    }
    return std::make_shared<FunctionNode>(derivFunc, args);
}
