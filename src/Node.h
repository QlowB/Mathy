#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <vector>
#include <exception>

#include "Natives.h"

class ExpressionNode;
class Environment;

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
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
	virtual bool equals(const ExpressionNode*) const;
};


class StatementNode :
        public virtual ExpressionNode
{
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
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
	
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
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
	
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
	
    /*!
     * \brief getString
     * \return the name of this variable
     */
	virtual std::string getString(void) const;
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
	
	virtual bool equals(const ExpressionNode*) const;
};


class ParentNode :
    public virtual ExpressionNode
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
	const Function* function;
	
	/*!
	 * true if <code>function</code> needs to be deleted
	 * on destruction of this node
	 */
	bool deleteFunction;
public:
	
	FunctionNode(const Function* function);
	FunctionNode(const Function* function, const std::vector<ExpressionNode*>& arguments);
	virtual ~FunctionNode(void);
	
	void addArgument(ExpressionNode* argument);
	
	virtual std::string getString(void) const;
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
	
	virtual size_t getArgumentCount(void) const;
	virtual ExpressionNode* getArgument(size_t i) const;
	
	/*!
	 * calculates the derivative in the i-th parameter
	 */
	virtual ExpressionNode* getDerivative(size_t i, GarbageBag& gb) const;
	
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


class AssignmentNode :
        public OperationNode,
        public StatementNode
{
public:
    AssignmentNode(ExpressionNode* a, ExpressionNode* b);
    virtual std::string getOperator(void) const;
    virtual std::string getString(void) const;
    virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
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
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
};


class SubtractionNode :
	public PlusMinus
{
public:
	SubtractionNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getString(void) const;
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
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
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
};


class ModuloNode :
	public MultDivMod
{
public:
	ModuloNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
};


class DivisionNode :
	public MultDivMod
{
public:
	DivisionNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
};


class PowerNode :
	public OperationNode
{
public:
	PowerNode(ExpressionNode* a, ExpressionNode* b);
	
	virtual std::string getOperator(void) const;
	virtual ExpressionNode* evaluate(Environment* e, GarbageBag& gb);
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

