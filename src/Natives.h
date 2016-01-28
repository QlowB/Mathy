#ifndef NATIVES_H_
#define NATIVES_H_
#include <utility>
#include <memory>
#include <map>

#include "Function.h"

class ExpressionNode;
class Environment;

class Constants
{
    static std::map<std::string, std::shared_ptr<ExpressionNode> > constants;
	static bool initialized;
	static void initialize(void);
public:
    static void add(const std::string& name,
                    const std::shared_ptr<ExpressionNode>& value);
    static const std::shared_ptr<ExpressionNode>& getConstant(
            const std::string& name);
};


class NativeFunction :
	public Function
{
public:
    NativeFunction(const std::string& name, size_t argumentCount);
    virtual std::shared_ptr<ExpressionNode> eval(
        Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const = 0;
};


class NativeNumFunction :
	public NativeFunction
{
public:
    typedef double (*MathFunc)(double);
private:

    MathFunc function;
    NativeNumFunction* derivative;
public:
    NativeNumFunction(const std::string& name,
                      MathFunc function,
                      NativeNumFunction* derivative);

    virtual const std::string& getName(void) const;

    virtual std::shared_ptr<ExpressionNode> eval(
            Environment* e,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;
	
    FloatVal evaluate(FloatVal args) const;

    virtual std::shared_ptr<ExpressionNode> evaluate(
            Environment* e,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;

    virtual std::shared_ptr<ExpressionNode> getDerivative(
            size_t i,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;
};


class Log :
    public NativeNumFunction
{
public:
    Log(void);
    virtual const std::string& getName(void) const;
    virtual std::shared_ptr<ExpressionNode> getDerivative(
            size_t i,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;
};


class DerivativeFunction :
	public NativeFunction
{
public:
	DerivativeFunction(const std::string& name);
    virtual std::shared_ptr<ExpressionNode> eval(
            Environment* e,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;
	
    virtual std::shared_ptr<ExpressionNode> getDerivative(
            Environment* e,std::shared_ptr<ExpressionNode> value,
            std::shared_ptr<ExpressionNode> variable) const;
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

    static NativeNumFunction sin;
    static NativeNumFunction cos;
    static NativeNumFunction tan;
    static NativeNumFunction asin;
    static NativeNumFunction acos;
    static NativeNumFunction atan;
    static NativeNumFunction exp;
    static Log ln;
    static NativeNumFunction sinh;
    static NativeNumFunction cosh;

private:
};



#endif // NATIVES_H_

