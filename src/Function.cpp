#include "Natives.h"
#include "Node.h"
#include <sstream>


Function::Function(const std::string& name, size_t argumentCount) :
	name(name), argumentCount(argumentCount)
{
}

#include <iostream>
ExpressionNode* Function::eval(const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
    // std::cout << "Function::eval\n";
    return gb.addReference(new FunctionNode(this, args));
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


ExpressionNode* Function::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	Function* derivFunc;

    if (argumentCount == 1)
        derivFunc = new Function(name + "'", argumentCount);
    else {
        std::stringstream ind;
        ind << i;
        derivFunc = new Function(name + "'(" + ind.str() + ")", argumentCount);
    }
    return gb.addReference(new FunctionNode(derivFunc, args));
}
