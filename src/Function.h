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

	virtual ExpressionNode* eval(const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
	virtual const std::string& getName(void) const;
	virtual size_t getArgumentCount(void) const;

    /*!
     * returns the derivative function (which may be an expression e.g.
     * the derivative of ln(x) will yield 1 / x.
     *
     * However this function will not make use of the chain rule when evaluating.
     * (e.g. the derivative of ln(2 * x) will just compute 1 / (2 * x), the factor 2 has
     * to be added separately.
     *
     * \param i the index after which argument the function should be derivated
     * \param args the arguments to the function
     * \param gb a garbage bag collecting references which can be eliminated sometime
     */
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};

#endif // FUNCTION_H_

