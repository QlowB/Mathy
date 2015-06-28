#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <vector>
#include <exception>

#include "Natives.h"

class ExpressionNode;

/*!
 * class collecting references and deleting them
 * when needed
 */
class GarbageBag
{
	std::vector<ExpressionNode*> references;
public:
	ExpressionNode* addReference(ExpressionNode* n);
	void free(void);
};

class ExpressionNode
{
public:
	virtual ~ExpressionNode(void);
	
	virtual std::string getString(void) const = 0;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
	virtual bool equals(const ExpressionNode*) const;
};


class ConstantNode :
	public ExpressionNode
{
public:
};


class IntegerNode :
	public ConstantNode
{
	long long int value;
public:
	
	static const IntegerNode ZERO;

	IntegerNode(long long int value);
	IntegerNode(const std::string& value);
	
	long long int getValue(void) const;
	
	virtual std::string getString(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
	
	virtual bool equals(const ExpressionNode*) const;
};


class RealNode :
	public ConstantNode
{
	FloatVal value;
public:
	RealNode(FloatVal value);
	RealNode(const std::string& value);
	
	FloatVal getValue(void) const;
	
	virtual std::string getString(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
	
	virtual bool equals(const ExpressionNode*) const;
};


class VariableNode :
	public ExpressionNode
{
	std::string name;
	/*! if it's a constant */
	ExpressionNode* constant;
public:
	VariableNode(const std::string& value);
	
	virtual std::string getString(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
	
	virtual bool equals(const ExpressionNode*) const;
};


class ParentNode :
	public ExpressionNode
{
protected:
	bool deleteArguments;
	ParentNode(bool deleteArguments);
};


class FunctionNode :
	public ParentNode
{
	std::string functionName;
	std::vector<ExpressionNode*> arguments;
	Function* function;
	FunctionNode(const std::string& name);
	
	/*!
	 * true if <code>function</code> needs to be deleted
	 * on destruction of this node
	 */
	bool deleteFunction;
public:
	FunctionNode(const std::string& name,
		const std::vector<ExpressionNode*>& arguments);
	
	FunctionNode(Function* function);
	FunctionNode(Function* function, const std::vector<ExpressionNode*>& arguments);
	virtual ~FunctionNode(void);
	
	void addArgument(ExpressionNode* argument);
	
	virtual std::string getString(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
	
	virtual size_t getArgumentCount(void) const;
	virtual ExpressionNode* getArgument(size_t i) const;
	
	/*!
	 * calculates the derivative in the i-th parameter
	 */
	virtual ExpressionNode* getDerivative(size_t i) const;
	
	virtual bool equals(const ExpressionNode*) const;
};


class OperationNode :
	public ParentNode
{
public:
	ExpressionNode* a;
	ExpressionNode* b;
protected:
	using ParentNode::deleteArguments;
	
	OperationNode(ExpressionNode* a, ExpressionNode* b);
	virtual ~OperationNode(void);
	virtual std::string getString(void) const;
	virtual std::string getOperator(void) const = 0;
	
public:
	inline ExpressionNode* getLeft(void) { return a; }
	inline ExpressionNode* getRight(void) { return b; }
};


class PlusMinus :
	public OperationNode
{
public:
	inline PlusMinus(ExpressionNode* a, ExpressionNode* b) : OperationNode(a, b) {}
};


class AdditionNode :
	public PlusMinus
{
public:
	AdditionNode(ExpressionNode* a, ExpressionNode* b);
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
};


class SubtractionNode :
	public PlusMinus
{
public:
	SubtractionNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getString(void) const;
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
};


class MultDivMod :
	public OperationNode
{
public:
	inline MultDivMod(ExpressionNode* a, ExpressionNode* b) : OperationNode(a, b) {}
	virtual std::string getString(void) const;
};


class MultiplicationNode :
	public MultDivMod
{
public:
	MultiplicationNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
};


class ModuloNode :
	public MultDivMod
{
public:
	ModuloNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
};


class DivisionNode :
	public MultDivMod
{
public:
	DivisionNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
};


class PowerNode :
	public OperationNode
{
public:
	PowerNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(GarbageBag& gb);
	virtual std::string getString(void) const;
};


class ArithException :
	public std::exception
{
	std::string whatStr;
public:
	ArithException(const std::string& whatStr);
	~ArithException(void) throw();
	const char* what(void) const throw();
};


#endif // NODE_H_

