#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <string>
#include <vector>

typedef double FloatVal;

class ExpressionNode;
class GarbageBag;

class Function
{
protected:
	std::string name;
	size_t argumentCount;
public:
	Function(const std::string& name, size_t argumentCount);

	virtual ExpressionNode* eval(const std::vector<ExpressionNode*>& args, GarbageBag& gb) const = 0;
	virtual const std::string& getName(void) const;
	virtual size_t getArgumentCount(void) const;
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args) const;
};

#endif // FUNCTION_H_

