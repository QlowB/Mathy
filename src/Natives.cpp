#include "Natives.h"
#include <utility>
#include <iostream>

#include <cmath>

#include "Node.h"

std::map<std::string, ExpressionNode*> Constants::constants;
bool Constants::initialized = false;


void Constants::initialize(void)
{
	add("pi", new RealNode(3.141592653589793238462643383279));
	add("e", new RealNode(2.718281828459045235360287471352));
}


void Constants::add(const std::string& name, ExpressionNode* value)
{
	constants.insert(
		std::pair<std::string, ExpressionNode*>(
			name, value 
		)
	);
}


NativeFunction::NativeFunction(const std::string& name, size_t argumentCount) :
	Function(name, argumentCount)
{
}


NativeNumFunction::NativeNumFunction(const std::string& name, size_t argumentCount) :
	NativeFunction(name, argumentCount)
{
}


ExpressionNode* NativeNumFunction::eval(const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	ExpressionNode* eval = args[0]->evaluate(gb);
	RealNode* real = dynamic_cast<RealNode*>(eval);
	IntegerNode* intN = dynamic_cast<IntegerNode*>(eval);
	
	if (real != 0) {
		FloatVal arg = 0;
		arg = real->getValue();
		return gb.addReference(new RealNode(evaluate(arg)));
	}
	return evaluate(args, gb);
}

ExpressionNode* NativeNumFunction::evaluate(const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	return gb.addReference(new FunctionNode(const_cast<Function*> (dynamic_cast<const Function*> (this)), args));
}


Sin::Sin(void) :
	NativeNumFunction("sin", 1)
{
}


FloatVal Sin::evaluate(FloatVal arg) const
{
	return ::sin(arg);
}


ExpressionNode* Sin::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	return new FunctionNode(&Functions::cos, args);
}


Cos::Cos(void) :
	NativeNumFunction("cos", 1)
{
}


FloatVal Cos::evaluate(FloatVal arg) const
{
	return ::cos(arg);
}


ExpressionNode* Cos::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	ExpressionNode* sine = new FunctionNode(&Functions::sin, args);
	return new SubtractionNode(new IntegerNode(0), sine);
}


Tan::Tan(void) :
	NativeNumFunction("tan", 1)
{
}


FloatVal Tan::evaluate(FloatVal arg) const
{
	return ::tan(arg);
}


ExpressionNode* Tan::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	ExpressionNode* cosine = new FunctionNode(&Functions::sin, args);
	return new DivisionNode(new IntegerNode(1), new MultiplicationNode(cosine, cosine));
}


Exp::Exp(void) :
	NativeNumFunction("exp", 1)
{
}


FloatVal Exp::evaluate(FloatVal arg) const
{
	return ::exp(arg);
}


ExpressionNode* Exp::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	return new FunctionNode(this, args);
}


Ln::Ln(void) :
	NativeNumFunction("ln", 1)
{
}


ExpressionNode* Ln::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	return new DivisionNode(new IntegerNode(1), args[0]);
}


FloatVal Ln::evaluate(FloatVal arg) const
{
	return ::log(arg);
}


Sinh::Sinh(void) :
    NativeNumFunction("sinh", 1)
{
}


ExpressionNode* Sinh::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	return new FunctionNode(&Functions::cosh, args);
}


FloatVal Sinh::evaluate(FloatVal arg) const
{
	return ::sinh(arg);
}


Cosh::Cosh(void) :
    NativeNumFunction("cosh", 1)
{
}


ExpressionNode* Cosh::getDerivative(size_t i, const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	return new FunctionNode(&Functions::sinh, args);
}


FloatVal Cosh::evaluate(FloatVal arg) const
{
	return ::cosh(arg);
}


DerivativeFunction::DerivativeFunction(const std::string& name) :
	NativeFunction(name, 2)
{
}


ExpressionNode* DerivativeFunction::eval(const std::vector<ExpressionNode*>& args, GarbageBag& gb) const
{
	ExpressionNode* eval = args[0]->evaluate(gb);
	ExpressionNode* variable = args[1]->evaluate(gb);
	
	return getDerivative(eval, variable, gb);
}



ExpressionNode* DerivativeFunction::getDerivative(ExpressionNode* value, ExpressionNode* variable, GarbageBag& gb) const
{
	RealNode* real = dynamic_cast<RealNode*>(value);
	if (real != 0) {
		return gb.addReference(new RealNode(0));
	}
	
	IntegerNode* intN = dynamic_cast<IntegerNode*>(value);
	if (intN != 0) {
		return gb.addReference(new IntegerNode(0));
	}
	
	VariableNode* var = dynamic_cast<VariableNode*>(value);
	if (var != 0) {
		if (value->equals(variable)) {
			return gb.addReference(new IntegerNode(1));
		}
		else {
			return gb.addReference(new IntegerNode(0));
		}
	}
	
	OperationNode* addsub = dynamic_cast<AdditionNode*>(value);
	if (addsub == 0)
		addsub = dynamic_cast<SubtractionNode*>(value);
	if (addsub != 0) {
		ExpressionNode* a = getDerivative(addsub->a, variable, gb);
		ExpressionNode* b = getDerivative(addsub->b, variable, gb);
		
		OperationNode* result;
		if (dynamic_cast<AdditionNode*>(value) != 0)
			result = new AdditionNode(a, b);
		else
			result = new SubtractionNode(a, b);
		return gb.addReference(result)->evaluate(gb);
	}
	
	MultiplicationNode* mul = dynamic_cast<MultiplicationNode*>(value);
	if (mul != 0) {
		ExpressionNode* a = getDerivative(mul->a, variable, gb);
		ExpressionNode* b = getDerivative(mul->b, variable, gb);
		
		MultiplicationNode* first = new MultiplicationNode(a, mul->b);
		MultiplicationNode* second = new MultiplicationNode(b, mul->a);
		
		AdditionNode* newNode = new AdditionNode(gb.addReference(first), gb.addReference(second));
		return gb.addReference(newNode)->evaluate(gb);
	}
	
	DivisionNode* div = dynamic_cast<DivisionNode*>(value);
	if (div != 0) {
		ExpressionNode* a = getDerivative(div->a, variable, gb);
		ExpressionNode* b = getDerivative(div->b, variable, gb);
		
		MultiplicationNode* first = new MultiplicationNode(a, div->b);
		MultiplicationNode* second = new MultiplicationNode(b, div->a);
		
		MultiplicationNode* bottomsq = new MultiplicationNode(div->b, div->b);
		SubtractionNode* newNode = new SubtractionNode(gb.addReference(first), gb.addReference(second));
		DivisionNode* final = new DivisionNode(gb.addReference(newNode), gb.addReference(bottomsq));
		return gb.addReference(final)->evaluate(gb);
	}
	
	PowerNode* pow = dynamic_cast<PowerNode*>(value);
	if (pow != 0) {
		
		static NativeFunction* logfnc = Functions::getNativeFunction("ln", 1);
		
		// d/dx (f(x) ^ g(x)) = f(x)^g(x) * (g(x)*f'(x)/f(x) + log(f(x))g'(x))
		ExpressionNode* a = getDerivative(pow->a, variable, gb);
		ExpressionNode* b = getDerivative(pow->b, variable, gb);
		
		MultiplicationNode* bastr = new MultiplicationNode(pow->b, a);
		DivisionNode* bruch = new DivisionNode(bastr, pow->a);
		FunctionNode* logFunc = new FunctionNode(logfnc);
		logFunc->addArgument(pow->a);
		MultiplicationNode* secondMul = new MultiplicationNode(logFunc, b);
		AdditionNode* paranSum = new AdditionNode(bruch, secondMul);
		MultiplicationNode* result = new MultiplicationNode(pow, paranSum);
		
		gb.addReference(bastr);
		gb.addReference(bruch);
		gb.addReference(logFunc);
		gb.addReference(secondMul);
		gb.addReference(paranSum);
		return gb.addReference(result)->evaluate(gb);
	}

	FunctionNode* func = dynamic_cast<FunctionNode*>(value);
	if (func != 0) {
        // std::cout << "Function deriv\n";
		AdditionNode* final = 0;
		for (size_t i = 0; i < func->getArgumentCount(); i++) {
            // std::cout << "chain rule!\n";
			ExpressionNode* argDeriv = getDerivative(func->getArgument(i), variable, gb);
            // std::cout << "func der!\n";
			MultiplicationNode* summand = new MultiplicationNode(argDeriv, func->getDerivative(i, gb));
			
            // std::cout << "did summand!\n";
			if (func->getArgumentCount() == 1) {
                // std::cout << "one arg!\n";
				return gb.addReference(summand)->evaluate(gb);
			}
			else if (final == 0) {
				final = new AdditionNode(gb.addReference(summand), 0);
				gb.addReference(final);
			}
			else if (final->b == 0) {
				final->b = summand;
				gb.addReference(summand);
			}
			else {
				final->b = new AdditionNode(final->b, gb.addReference(summand));
			}
		}
		return final;
	}
    std::cout << "found no derivative\n";
	return 0;
}


ExpressionNode* Constants::getConstant(const std::string& name)
{
	if (!initialized) {
		initialize();
		initialized = true;
	}
	if (constants.find(name) != constants.end())
		return constants[name];
	else
		return 0;
}


std::map<std::pair<std::string, size_t>, NativeFunction*> Functions::functions;
bool Functions::initialized = false;


void Functions::initialize(void)
{
	add(&sin);
	add(&cos);
	add(&tan);
	add(&exp);
	add(&ln);
    add(&sinh);
    add(&cosh);
	add(new DerivativeFunction("d"));
	/*add("cos", 1, cos);
	add("tan", 1, tan);
	add("asin", 1, asin);
	add("acos", 1, acos);
	add("atan", 1, atan);
	add("sinh", 1, sinh);
	add("cosh", 1, cosh);
	add("tanh", 1, tanh);
	add("exp", 1, exp);
	add("log10", 1, log10);
	add("ln", 1, ln);
	add("sqrt", 1, sqrt);*/
}


void Functions::add(NativeFunction* value)
{
	const std::string& name = value->getName();
	size_t nArgs = value->getArgumentCount();
	functions.insert(
		std::pair<std::pair<std::string, size_t>, NativeFunction*>(
			std::pair<std::string, size_t> (name, nArgs), value 
		)
	);
}


NativeFunction* Functions::getNativeFunction(const std::string& name, int nArgs)
{
	if (!initialized) {
		initialize();
		initialized = true;
	}
	
	if (functions.find(std::pair<std::string, int> (name, nArgs)) != functions.end())
		return functions[std::pair<std::string, int> (name, nArgs)];
	else
		return 0;
}

Sin Functions::sin;
Cos Functions::cos;
Tan Functions::tan;
Exp Functions::exp;
Ln Functions::ln;
Sinh Functions::sinh;
Cosh Functions::cosh;

