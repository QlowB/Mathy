#include "Natives.h"
#include "Node.h"

Function::Function(const std::string& name, size_t argumentCount) :
	name(name), argumentCount(argumentCount)
{
}


const std::string& Function::getName(void) const
{
	return name;
}


size_t Function::getArgumentCount(void) const
{
	return argumentCount;
}


ExpressionNode* Function::getDerivative(size_t i, const std::vector<ExpressionNode*>& args) const
{
	return 0;
}