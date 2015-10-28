#ifndef NATIVES_H_
#define NATIVES_H_
#include <utility>
#include <map>

#include "Function.h"

class ExpressionNode;
class Environment;

class Constants
{
	static std::map<std::string, ExpressionNode*> constants;
	static bool initialized;
	static void initialize(void);
public:
	static void add(const std::string& name, ExpressionNode* value);
	static ExpressionNode* getConstant(const std::string& name);
};


class NativeFunction :
	public Function
{
public:
	NativeFunction(const std::string& name, size_t argumentCount);
	virtual ExpressionNode* eval(Environment* e, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const = 0;
};


/*!
 * base class for sin, cos etc.
 */
class NativeNumFunction :
	public NativeFunction
{
public:
	NativeNumFunction(const std::string& name, size_t argumentCount);
	virtual ExpressionNode* eval(Environment* e, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
	
	virtual FloatVal evaluate(FloatVal args) const = 0;
	virtual ExpressionNode* evaluate(Environment* e, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class Sin :
	public NativeNumFunction
{
public:
	Sin(void);
	virtual FloatVal evaluate(FloatVal args) const;
	
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class Cos :
	public NativeNumFunction
{
public:
	Cos(void);
	virtual FloatVal evaluate(FloatVal args) const;
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class Tan :
	public NativeNumFunction
{
public:
	Tan(void);
	virtual FloatVal evaluate(FloatVal args) const;
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class Exp :
	public NativeNumFunction
{
public:
	Exp(void);
	virtual FloatVal evaluate(FloatVal args) const;
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class Ln :
	public NativeNumFunction
{
public:
	Ln(void);
	virtual FloatVal evaluate(FloatVal args) const;
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class Sinh :
	public NativeNumFunction
{
public:
	Sinh(void);
	virtual FloatVal evaluate(FloatVal args) const;
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class Cosh :
	public NativeNumFunction
{
public:
	Cosh(void);
	virtual FloatVal evaluate(FloatVal args) const;
	virtual ExpressionNode* getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
};


class DerivativeFunction :
	public NativeFunction
{
public:
	DerivativeFunction(const std::string& name);
	virtual ExpressionNode* eval(Environment* e, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const;
	
	virtual ExpressionNode* getDerivative(Environment* e, ExpressionNode* value, ExpressionNode* variable, GarbageBag& gb) const;
};


class Functions
{
private:
    static std::map<std::pair<std::string, size_t>, NativeFunction*> functions;
    static bool initialized;
    static void initialize(void);
public:
    static void add(NativeFunction* value);
    static NativeFunction* getNativeFunction(const std::string& name, int nArgs);
    
    static Sin sin;
    static Cos cos;
    static Tan tan;
    static Exp exp;
    static Ln ln;
    static Sinh sinh;
    static Cosh cosh;

private:
};



#endif // NATIVES_H_

