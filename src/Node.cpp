#include "Node.h"

#include <cstdlib>
#include <cmath>
#include <sstream>

#include "Natives.h"

ExpressionNode* GarbageBag::addReference(ExpressionNode* en)
{
	references.push_back(en);
	return en;
}


void GarbageBag::free(void)
{
	for (size_t i = 0; i < references.size(); i++) {
		delete references[i];
	}
	references.clear();
}


ExpressionNode::~ExpressionNode(void)
{
}


ExpressionNode* ExpressionNode::evaluate(GarbageBag& gb)
{
	return this;
}


bool ExpressionNode::equals(const ExpressionNode* en) const
{
	if (this == en)
		return true;
	else
		return false;
}


const IntegerNode IntegerNode::ZERO(0);


IntegerNode::IntegerNode(long long int value) :
	value(value)
{
}


IntegerNode::IntegerNode(const std::string& value) :
	value(::atoll(value.c_str()))
{
}


long long int IntegerNode::getValue(void) const
{
	return value;
}


std::string IntegerNode::getString(void) const
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}


ExpressionNode* IntegerNode::evaluate(GarbageBag& gb)
{
	return this;
}


bool IntegerNode::equals(const ExpressionNode* en) const
{
	if (ExpressionNode::equals(en))
		return true;
	
	const IntegerNode* type = dynamic_cast<const IntegerNode*> (en);
	
	if (type != 0) {
		return type->value == this->value;
	}
	else
		return false;
}


RealNode::RealNode(FloatVal value) :
	value(value)
{
}


RealNode::RealNode(const std::string& value) :
	value(::atof(value.c_str()))
{
}


FloatVal RealNode::getValue(void) const
{
	return value;
}


std::string RealNode::getString(void) const
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}


ExpressionNode* RealNode::evaluate(GarbageBag& gb)
{
	return this;
}


bool RealNode::equals(const ExpressionNode* en) const
{
	if (ExpressionNode::equals(en))
		return true;
	
	const RealNode* type = dynamic_cast<const RealNode*> (en);
	
	if (type != 0) {
		return type->value == this->value;
	}
	else
		return false;
}


VariableNode::VariableNode(const std::string& name) :
	name(name)
{
	constant = Constants::getConstant(name);
}


std::string VariableNode::getString(void) const
{
	return name;
}


ExpressionNode* VariableNode::evaluate(GarbageBag& gb)
{
	if (constant != 0)
		return constant;
	else
		return this;
}


bool VariableNode::equals(const ExpressionNode* en) const
{
	if (ExpressionNode::equals(en))
		return true;
	
	const VariableNode* type = dynamic_cast<const VariableNode*> (en);
	
	if (type != 0) {
		return type->name == this->name;
	}
	else
		return false;
}


ParentNode::ParentNode(bool deleteArguments) :
	deleteArguments(deleteArguments)
{
}


FunctionNode::FunctionNode(const std::string& name) :
	functionName(name), ParentNode(false)
{
	function = Functions::getNativeFunction(functionName, arguments.size());
}


FunctionNode::FunctionNode(Function* function) :
	functionName(function->getName()), ParentNode(false), function(function)
{
}


FunctionNode::FunctionNode(Function* function, const std::vector<ExpressionNode*>& arguments) :
	functionName(function->getName()), ParentNode(false), function(function), arguments(arguments)
{
}


FunctionNode::FunctionNode(const std::string& name, 
	const std::vector<ExpressionNode*>& arguments) :
	functionName(name), arguments(arguments),
	ParentNode(true)
{
	function = Functions::getNativeFunction(name, arguments.size());
}


FunctionNode::~FunctionNode(void)
{
	if (deleteArguments)
		for (size_t i = 0; i < arguments.size(); i++)
			delete arguments[i];
}


void FunctionNode::addArgument(ExpressionNode* argument)
{
	arguments.push_back(argument);
	function = Functions::getNativeFunction(functionName, arguments.size());
}


std::string FunctionNode::getString(void) const
{
	std::string ret = functionName + "(";
	for (size_t i = 0; i < arguments.size(); i++) {
		ret += arguments[i]->getString();
		if (i < arguments.size() - 1) {
			ret += ", ";
		}
	}
	ret += ")";
	return ret;
}


ExpressionNode* FunctionNode::evaluate(GarbageBag& gb)
{
	if (function != 0) {
		ExpressionNode* evaluated = function->eval(arguments, gb);
		if (evaluated != 0)
			return evaluated;
	}
	
	
	ExpressionNode* eval = arguments[0]->evaluate(gb);
	RealNode* real = dynamic_cast<RealNode*>(eval);
	IntegerNode* intN = dynamic_cast<IntegerNode*>(eval);
	
	FunctionNode* fn = new FunctionNode(functionName);
	for (size_t i = 0; i < arguments.size(); i++) {
		ExpressionNode* en = arguments[i]->evaluate(gb);
		fn->addArgument(en);
	}
	return gb.addReference(fn);
	/*
	ExpressionNode* eval = arguments[0]->evaluate(gb);
	RealNode* real = dynamic_cast<RealNode*>(eval);
	IntegerNode* intN = dynamic_cast<IntegerNode*>(eval);
	
	if (function != 0 && (real != 0 || intN != 0)) {
		FloatVal arg = 0;
		if (real != 0) {
			arg = real->getValue();
		}
		if (intN != 0) {
			arg = intN->getValue();
		}
		std::vector<FloatVal> args;
		args.push_back(arg);
		return gb.addReference(new RealNode(function->eval(args)));
	}*/
}


size_t FunctionNode::getArgumentCount(void) const
{
	return arguments.size();
}


ExpressionNode* FunctionNode::getArgument(size_t i) const
{
	return arguments[i];
}


ExpressionNode* FunctionNode::getDerivative(size_t i) const
{
	std::stringstream ind;
	ind << i;
	
	ExpressionNode* func = function->getDerivative(i, arguments);
	
	if (func != 0) {
		return func;
	}
	
	if (arguments.size() != 1) {
		return new FunctionNode(functionName + "'(" +  ind.str() + ")", arguments);
	} else {
		return new FunctionNode(functionName + "'", arguments);
	}
}


bool FunctionNode::equals(const ExpressionNode* en) const
{
	if (ExpressionNode::equals(en))
		return true;
	
	const FunctionNode* type = dynamic_cast<const FunctionNode*> (en);
	
	if (type != 0) {
		bool eq = type->function == this->function;
		if (!eq)
			return false;
		if (arguments.size() != type->arguments.size())
			return false;
		// assert: arguments.size() == type->arguments.size()
		for (size_t i = 0; i < arguments.size(); i++) {
			if (!arguments[i]->equals(type->arguments[i]))
				return false;
		}
		return true;
	}
	else
		return false;
}


OperationNode::OperationNode(ExpressionNode* a, ExpressionNode* b) :
	a(a), b(b), ParentNode(false)
{
}


OperationNode::~OperationNode(void)
{
	if (deleteArguments) {
		delete a;
		delete b;
	}
}


std::string OperationNode::getString(void) const
{
	return a->getString() + " " + getOperator() + " " + b->getString();
}


AdditionNode::AdditionNode(ExpressionNode* a, ExpressionNode* b) :
	PlusMinus(a, b)
{
}


std::string AdditionNode::getOperator(void) const
{
	return "+";
}


ExpressionNode* AdditionNode::evaluate(GarbageBag& gb)
{
	ExpressionNode* left = a->evaluate(gb);
	ExpressionNode* right = b->evaluate(gb);
	ConstantNode* cLeft = dynamic_cast<ConstantNode*>(left);
	ConstantNode* cRight = dynamic_cast<ConstantNode*>(right);
	
	if (cLeft != 0 && cRight != 0) {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
		RealNode* rRight = dynamic_cast<RealNode*>(cRight);
		
		if (iLeft != 0 && iRight != 0) {
			return gb.addReference(new IntegerNode(iLeft->getValue() + iRight->getValue()));
		}
		if (iLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(iLeft->getValue() + rRight->getValue()));
		}
		if (rLeft != 0 && iRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() + iRight->getValue()));
		}
		if (rLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() + rRight->getValue()));
		}
		return 0;
	} else {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		/*if (iLeft != 0 && iLeft->getValue() == 0) {
			return right;
		}
		if (iRight != 0 && iRight->getValue() == 0) {
			return left;
		}*/
		
		AdditionNode* an = new AdditionNode(left, right);
		an->deleteArguments = false;
		return gb.addReference(an);
	}
}


SubtractionNode::SubtractionNode(ExpressionNode* a, ExpressionNode* b) :
	PlusMinus(a, b)
{
}


std::string SubtractionNode::getString(void) const
{
	if (dynamic_cast<PlusMinus*> (b) != 0) {
		return a->getString() + " " + getOperator() + " (" + b->getString() + ")";
	}
	else
		return a->getString() + " " + getOperator() + " " + b->getString();
}


std::string SubtractionNode::getOperator(void) const
{
	return "-";
}


ExpressionNode* SubtractionNode::evaluate(GarbageBag& gb)
{
	ExpressionNode* left = a->evaluate(gb);
	ExpressionNode* right = b->evaluate(gb);
	ConstantNode* cLeft = dynamic_cast<ConstantNode*>(left);
	ConstantNode* cRight = dynamic_cast<ConstantNode*>(right);
	
	if (cLeft != 0 && cRight != 0) {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
		RealNode* rRight = dynamic_cast<RealNode*>(cRight);
		
		if (iLeft != 0 && iRight != 0) {
			return gb.addReference(new IntegerNode(iLeft->getValue() - iRight->getValue()));
		}
		if (iLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(iLeft->getValue() - rRight->getValue()));
		}
		if (rLeft != 0 && iRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() - iRight->getValue()));
		}
		if (rLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() - rRight->getValue()));
		}
		
		return 0;
	} else {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		if (iRight != 0 && iRight->getValue() == 0) {
			return left;
		}
		SubtractionNode* an = new SubtractionNode(left, right);
		an->deleteArguments = false;
		return gb.addReference(an);
	}
}


std::string MultDivMod::getString(void) const
{
	bool addA = dynamic_cast<PlusMinus*> (a);
	bool addB = dynamic_cast<PlusMinus*> (b);
	
	std::string left;
	std::string right;
	
	if (addA)
		left = "(" + a->getString() + ")";
	else
		left = a->getString();
	
	if (addB)
		right = "(" + b->getString() + ")";
	else
		right = b->getString();
	
	return left + " " + getOperator() + " " + right;
}


MultiplicationNode::MultiplicationNode(ExpressionNode* a, ExpressionNode* b) :
	MultDivMod(a, b)
{
}


std::string MultiplicationNode::getOperator(void) const
{
	return "*";
}


ExpressionNode* MultiplicationNode::evaluate(GarbageBag& gb)
{
	ExpressionNode* left = a->evaluate(gb);
	ExpressionNode* right = b->evaluate(gb);
	ConstantNode* cLeft = dynamic_cast<ConstantNode*>(left);
	ConstantNode* cRight = dynamic_cast<ConstantNode*>(right);
	
	if (cLeft != 0 && cRight != 0) {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
		RealNode* rRight = dynamic_cast<RealNode*>(cRight);
		
		if (iLeft != 0 && iRight != 0) {
			return gb.addReference(new IntegerNode(iLeft->getValue() * iRight->getValue()));
		}
		if (iLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(iLeft->getValue() * rRight->getValue()));
		}
		if (rLeft != 0 && iRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() * iRight->getValue()));
		}
		if (rLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() * rRight->getValue()));
		}
		
		return 0;
	} else {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		if (iLeft != 0 && iLeft->getValue() == 1) {
			return right;
		}
		if (iLeft != 0 && iLeft->getValue() == 0) {
			return gb.addReference(new IntegerNode(0));
		}
		if (iRight != 0 && iRight->getValue() == 1) {
			return left;
		}
		if (iRight != 0 && iRight->getValue() == 0) {
			return gb.addReference(new IntegerNode(0));
		}
		MultiplicationNode* an = new MultiplicationNode(left, right);
		an->deleteArguments = false;
		return gb.addReference(an);
	}
}


ModuloNode::ModuloNode(ExpressionNode* a, ExpressionNode* b) :
	MultDivMod(a, b)
{
}


std::string ModuloNode::getOperator(void) const
{
	return "mod";
}


ExpressionNode* ModuloNode::evaluate(GarbageBag& gb)
{
	ExpressionNode* left = a->evaluate(gb);
	ExpressionNode* right = b->evaluate(gb);
	ConstantNode* cLeft = dynamic_cast<ConstantNode*>(left);
	ConstantNode* cRight = dynamic_cast<ConstantNode*>(right);
	
	if (cLeft != 0 && cRight != 0) {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
		RealNode* rRight = dynamic_cast<RealNode*>(cRight);
		
		if (iLeft != 0 && iRight != 0) {
			return gb.addReference(new IntegerNode(iLeft->getValue() % iRight->getValue()));
		}
		throw ArithException("modulo operator only defined for integer operands!");
	} else {
		ModuloNode* an = new ModuloNode(left, right);
		an->deleteArguments = false;
		return gb.addReference(an);
	}
}


DivisionNode::DivisionNode(ExpressionNode* a, ExpressionNode* b) :
	MultDivMod(a, b)
{
}


std::string DivisionNode::getOperator(void) const
{
	return "/";
}


ExpressionNode* DivisionNode::evaluate(GarbageBag& gb)
{
	ExpressionNode* left = a->evaluate(gb);
	ExpressionNode* right = b->evaluate(gb);
	ConstantNode* cLeft = dynamic_cast<ConstantNode*>(left);
	ConstantNode* cRight = dynamic_cast<ConstantNode*>(right);
	
	if (cLeft != 0 && cRight != 0) {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
		RealNode* rRight = dynamic_cast<RealNode*>(cRight);
		
		if (iLeft != 0 && iRight != 0) {
			return this;
		}
		if (iLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(iLeft->getValue() / rRight->getValue()));
		}
		if (rLeft != 0 && iRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() / iRight->getValue()));
		}
		if (rLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(rLeft->getValue() / rRight->getValue()));
		}
		
		return 0;
	} else {
		DivisionNode* an = new DivisionNode(left, right);
		an->deleteArguments = false;
		return gb.addReference(an);
	}
}


PowerNode::PowerNode(ExpressionNode* a, ExpressionNode* b) :
	OperationNode(a, b)
{
}


std::string PowerNode::getOperator(void) const
{
	return "^";
}


std::string PowerNode::getString(void) const
{
	bool addA = dynamic_cast<PlusMinus*> (a);
	bool addB = dynamic_cast<PlusMinus*> (b);
	
	addA |= dynamic_cast<MultDivMod*> (a) != 0;
	addB |= dynamic_cast<MultDivMod*> (b) != 0;
	
	std::string left;
	std::string right;
	
	if (addA)
		left = "(" + a->getString() + ")";
	else
		left = a->getString();
	
	if (addB)
		right = "(" + b->getString() + ")";
	else
		right = b->getString();
	
	return left + " " + getOperator() + " " + right;
}


ExpressionNode* PowerNode::evaluate(GarbageBag& gb)
{
	ExpressionNode* left = a->evaluate(gb);
	ExpressionNode* right = b->evaluate(gb);
	ConstantNode* cLeft = dynamic_cast<ConstantNode*>(left);
	ConstantNode* cRight = dynamic_cast<ConstantNode*>(right);
	
	if (cLeft != 0 && cRight != 0) {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
		RealNode* rRight = dynamic_cast<RealNode*>(cRight);
		
		if (iLeft != 0 && iRight != 0) {
			return gb.addReference(new IntegerNode(::pow(iLeft->getValue(), iRight->getValue())));
		}
		if (iLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(::pow(iLeft->getValue(), rRight->getValue())));
		}
		if (rLeft != 0 && iRight != 0) {
			return gb.addReference(new RealNode(::pow(rLeft->getValue(), iRight->getValue())));
		}
		if (rLeft != 0 && rRight != 0) {
			return gb.addReference(new RealNode(::pow(rLeft->getValue(), rRight->getValue())));
		}
		
		return 0;
	} else {
		IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
		IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
		
		if (iLeft != 0 && iLeft->getValue() == 1) {
			return gb.addReference(new IntegerNode(1));
		}
		if (iRight != 0 && iRight->getValue() == 1) {
			return left;
		}
		PowerNode* an = new PowerNode(left, right);
		an->deleteArguments = false;
		return gb.addReference(an);
	}
}


ArithException::ArithException(const std::string& whatStr) :
	whatStr(whatStr)
{
}


ArithException::~ArithException(void) throw()
{
}


const char* ArithException::what(void) const throw()
{
	return whatStr.c_str();
}


