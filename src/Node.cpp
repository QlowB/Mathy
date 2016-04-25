// =============================================================================
//
// This file is part of the Mathy computer algebry system.
//
// Copyright (C) 2015-2016 Nicolas Winkler
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// =============================================================================


#include "Node.h"

#include <cstdlib>
#include <cmath>
#include <sstream>

#include "Environment.h"
#include "Natives.h"
#include "Rewriter.h"


ExpressionNode::~ExpressionNode(void)
{
}


std::shared_ptr<ExpressionNode> ExpressionNode::evaluate(Environment*)
{
    return shared_from_this();
}


std::shared_ptr<ExpressionNode> ExpressionNode::basicSimplify(Environment*)
{
    return shared_from_this();
}


std::shared_ptr<ExpressionNode> ExpressionNode::substitute(
        const std::vector<std::unique_ptr<SubstituteRule> >& rules)
{
    return shared_from_this();
}


bool ExpressionNode::equals(const ExpressionNode* other) const
{
    if (this == other)
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


std::shared_ptr<ExpressionNode>
IntegerNode::evaluate(Environment*)
{
    return shared_from_this();
}


bool IntegerNode::equals(const ExpressionNode* other) const
{
    if (ExpressionNode::equals(other))
        return true;
    
    const IntegerNode* type = dynamic_cast<const IntegerNode*> (other);
    
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


std::shared_ptr<ExpressionNode> RealNode::evaluate(Environment*)
{
    return shared_from_this();
}


bool RealNode::equals(const ExpressionNode* other) const
{
    if (ExpressionNode::equals(other))
        return true;
    
    const RealNode* type = dynamic_cast<const RealNode*> (other);
    
    if (type != 0) {
        return type->value == this->value;
    }
    else
        return false;
}


VariableNode::VariableNode(const std::string& name) :
    name(name)
{
    //constant = Constants::getConstant(name);
}


std::string VariableNode::getString(void) const
{
    return name;
}


std::shared_ptr<ExpressionNode> VariableNode::evaluate(Environment* e)
{
    VariableSymbol* vs = 0;
    if ((vs = e->getVariable(name))) {      // double parantheses because
        return vs->getValue()->evaluate(e); // compiler is a smartass otherwise
    }
    else
        return shared_from_this();
}


std::shared_ptr<ExpressionNode> VariableNode::substitute(
        const std::vector<std::unique_ptr<SubstituteRule> >& rules)
{
    for (auto i = rules.begin(); i != rules.end(); i++) {
        SubstituteRule& sr = *i->get();
        if (sr.find.lock().get()->getString() == name) {
            return sr.replace;
        }
    }
    return shared_from_this();
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


std::string FunctionNode::getString(void) const
{
    //printf("FunctionNode::getString()\n");
    // TODO implement lambda notation output
    std::string out = "(\\";
    for (auto i = argumentNames.begin(); i != argumentNames.end(); i++) {
        out += i->get()->getString();
        if ((i + 1) != argumentNames.end())
            out += " ";
    }
    out += " -> ";
    if (equation.get() != nullptr)
        out += equation.get()->getString();
    else {
        out += "[built in]";
    }
    return out + ")";
}


std::shared_ptr<ExpressionNode> FunctionNode::evaluate(Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& arguments)
{
    if (argumentNames.size() != arguments.size()) {
        throw RuntimeException("cannot apply function");
    }

    std::vector<std::unique_ptr<SubstituteRule> > rules;
    for (size_t i = 0; i < argumentNames.size(); i++) {
        rules.push_back(std::unique_ptr<SubstituteRule>
                (new SubstituteRule(argumentNames[i], arguments[i])));
    }
    return equation.get()->substitute(rules)->evaluate(e);
}


std::shared_ptr<ExpressionNode> FunctionNode::evaluate(Environment* e)
{
    return shared_from_this();
}


std::shared_ptr<ExpressionNode> FunctionNode::substitute(
        const std::vector<std::unique_ptr<SubstituteRule> >& rules)
{
    // TODO: implement function substitution
    return shared_from_this();
}


std::shared_ptr<ExpressionNode> FunctionNode::getDerivative(size_t i) const
{
    return std::make_shared<FunctionNode>();
}


std::shared_ptr<ExpressionNode> FunctionNode::eval(Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& arguments) const
{
    return std::make_shared<FunctionNode>();
}


FunctionCallNode::FunctionCallNode(
        const std::shared_ptr<ExpressionNode>& function) :
            function(function)
{
    printf("yess\n");
}


FunctionCallNode::FunctionCallNode(
        const std::shared_ptr<ExpressionNode>& function,
        const std::vector<std::shared_ptr<ExpressionNode> >& arguments) :
    function(function), arguments(arguments)
{
}


FunctionCallNode::~FunctionCallNode(void)
{
}


void FunctionCallNode::addArgument(const std::shared_ptr<ExpressionNode>& argument)
{
    arguments.push_back(argument);
    //function = Functions::getNativeFunction(functionName, arguments.size());
}


std::string FunctionCallNode::getString(void) const
{
    std::string ret;
    if (dynamic_cast<VariableNode*> (function.get()) != nullptr)
        ret = function.get()->getString() + "(";
    else
        ret = "(" + function.get()->getString() + ")(";

    //printf("chelloo: %d\n", arguments.size());
    for (size_t i = 0; i < arguments.size(); i++) {
        ret += arguments[i]->getString();
        if (i < arguments.size() - 1) {
            ret += ", ";
        }
    }
    ret += ")";
    return ret;
}


#include <iostream>
std::shared_ptr<ExpressionNode> FunctionCallNode::evaluate(Environment* e)
{
    //std::cout << "needs: " << function->getName() << "\n";
    /*if (function.get() != nullptr) {
        std::shared_ptr<ExpressionNode> ev = nullptr; // function->eval(e, arguments);
        if (ev.get() != nullptr)
            return ev;
    }*/
    
    std::shared_ptr<ExpressionNode> func = function.get()->evaluate(e);
    std::shared_ptr<FunctionNode> realFunc =
        std::dynamic_pointer_cast<FunctionNode> (func);

    if (realFunc.get() != nullptr) {
        return realFunc.get()->evaluate(e, arguments);
    }

    //std::shared_ptr<ExpressionNode> eval = arguments[0]->evaluate(e);
    //RealNode* real = dynamic_cast<RealNode*>(eval.get());
    //IntegerNode* intN = dynamic_cast<IntegerNode*>(eval.get());

    /*
    std::vector<std::shared_ptr<ExpressionNode> > newArguments;

    for (size_t i = 0; i < arguments.size(); i++) {
        std::shared_ptr<ExpressionNode> en = arguments[i]->evaluate(e);
        newArguments.push_back(en);
    }*/
  
    std::shared_ptr<FunctionCallNode> fn =
            std::make_shared<FunctionCallNode>(func, arguments);


    return fn;
}


size_t FunctionCallNode::getArgumentCount(void) const
{
    return arguments.size();
}


const std::shared_ptr<ExpressionNode>&
FunctionCallNode::getArgument(size_t i) const
{
    return arguments[i];
}


std::shared_ptr<ExpressionNode> FunctionCallNode::substitute(
        const std::vector<std::unique_ptr<SubstituteRule> >& rules)
{
    std::vector<std::shared_ptr<ExpressionNode> > newArguments;
    for (size_t i = 0; i < arguments.size(); i++) {
        newArguments.push_back(arguments[i].get()->substitute(rules));
    }

    std::shared_ptr<ExpressionNode> newFunc = std::make_shared<FunctionCallNode>
        (function.get()->substitute(rules), newArguments);
    return newFunc;
}


#include <iostream>
std::shared_ptr<ExpressionNode> FunctionCallNode::getDerivative(size_t i) const
{
    //std::stringstream ind;
    //ind << i;
    std::shared_ptr<ExpressionNode> func =
            nullptr; //function->getDerivative(i);

    std::cerr << "not yet implemented in " << __FILE__ << " at " << __LINE__ <<
        std::endl;

    if (func != 0) {
        return func;
    }
    return nullptr;
    if (arguments.size() != 1) {
        // return new FunctionCallNode(functionName + "'(" +  ind.str() + ")", arguments);
    } else {
        // return new FunctionCallNode(functionName + "'", arguments);
    }
}


bool FunctionCallNode::equals(const ExpressionNode* en) const
{
    if (ExpressionNode::equals(en))
        return true;
    
    const FunctionCallNode* type = dynamic_cast<const FunctionCallNode*> (en);
    
    if (type != 0) {
        bool eq = type->function == this->function;
        if (!eq)
            return false;
        if (arguments.size() != type->arguments.size())
            return false;
        // assert: arguments.size() == type->arguments.size()
        for (size_t i = 0; i < arguments.size(); i++) {
            if (!arguments[i]->equals(type->arguments[i].get()))
                return false;
        }
        return true;
    }
    else
        return false;
}


OperationNode::OperationNode(const std::shared_ptr<ExpressionNode>& a,
                             const std::shared_ptr<ExpressionNode>& b) :
    a(a), b(b)
{
}


OperationNode::~OperationNode(void)
{
}


std::string OperationNode::getString(void) const
{
    return a->getString() + " " + getOperator() + " " + b->getString();
}


std::shared_ptr<ExpressionNode> OperationNode::substitute(
        const std::vector<std::unique_ptr<SubstituteRule> >& rules)
{
    std::shared_ptr<OperationNode> ret = clone();
    ret.get()->a = ret.get()->a.get()->substitute(rules);
    ret.get()->b = ret.get()->b.get()->substitute(rules);
    return ret;
}


AssignmentNode::AssignmentNode(const std::shared_ptr<ExpressionNode>& a,
                               const std::shared_ptr<ExpressionNode>& b) :
    OperationNode(a, b)
{
}


std::string AssignmentNode::getOperator(void) const
{
    return ":=";
}


std::string AssignmentNode::getString(void) const
{
    return a->getString() + " " + getOperator() + " " + b->getString();
}


std::shared_ptr<ExpressionNode> AssignmentNode::evaluate(Environment* e)
{
    std::shared_ptr<ExpressionNode> newValue = b->evaluate(e);

    std::shared_ptr<VariableNode> var =
            std::dynamic_pointer_cast<VariableNode> (a);
    
    if (var.get()->equals(newValue.get()))
        return shared_from_this();


    if (var) {
        const std::string varName = var->getString();
        VariableSymbol* vs = e->getVariable(varName);
        if (vs)
            vs->setValue(newValue);
        else
            e->addSymbol(new VariableSymbol(var->getString(), newValue));
    }
    else {
        throw ArithmeticException("left side of assignment must be a variable");
    }
    return std::make_shared<AssignmentNode> (a, newValue);
}


std::shared_ptr<OperationNode> AssignmentNode::clone(void) const
{
return std::make_shared<AssignmentNode>(a, b);
}


AdditionNode::AdditionNode(const std::shared_ptr<ExpressionNode>& a,
                           const std::shared_ptr<ExpressionNode>& b) :
    PlusMinus(a, b)
{
}


std::string AdditionNode::getOperator(void) const
{
    return "+";
}


std::shared_ptr<ExpressionNode> AdditionNode::evaluate(Environment* e)
{
    std::shared_ptr<ExpressionNode> left = a->evaluate(e);
    std::shared_ptr<ExpressionNode> right = b->evaluate(e);
    ConstantNode* cLeft = dynamic_cast<ConstantNode*>(&*left);
    ConstantNode* cRight = dynamic_cast<ConstantNode*>(&*right);
    
    if (cLeft != 0 && cRight != 0) {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
        RealNode* rRight = dynamic_cast<RealNode*>(cRight);
        
        if (iLeft != 0 && iRight != 0) {
            return std::make_shared<IntegerNode>
                    (iLeft->getValue() + iRight->getValue());
        }
        if (iLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (iLeft->getValue() + rRight->getValue());
        }
        if (rLeft != 0 && iRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() + iRight->getValue());
        }
        if (rLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() + rRight->getValue());
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
        
        return std::make_shared<AdditionNode>(left, right);
    }
}


std::shared_ptr<OperationNode> AdditionNode::clone(void) const
{
    return std::make_shared<AdditionNode>(a, b);
}


SubtractionNode::SubtractionNode(const std::shared_ptr<ExpressionNode>& a,
                                 const std::shared_ptr<ExpressionNode>& b) :
    PlusMinus(a, b)
{
}


std::string SubtractionNode::getString(void) const
{
    bool zero = false;
    IntegerNode* in;
    if ((in = dynamic_cast<IntegerNode*>(a.get())) != 0) {
        if (in->getValue() == 0)
            zero = true;
    }
    if (dynamic_cast<PlusMinus*> (b.get()) != 0) {
        return (!zero ? (a->getString() + " ") : "") + getOperator() +
            (zero ? "" : " ") + "(" + b->getString() + ")";
    }
    else
        return (!zero ? (a->getString() + " ") : "") + getOperator() +
            (zero ? "" : " ") + b->getString();
}


std::string SubtractionNode::getOperator(void) const
{
    return "-";
}


std::shared_ptr<ExpressionNode> SubtractionNode::evaluate(Environment* e)
{
    std::shared_ptr<ExpressionNode> left = a->evaluate(e);
    std::shared_ptr<ExpressionNode> right = b->evaluate(e);
    ConstantNode* cLeft = dynamic_cast<ConstantNode*>(&*left);
    ConstantNode* cRight = dynamic_cast<ConstantNode*>(&*right);
    
    if (cLeft != 0 && cRight != 0) {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
        RealNode* rRight = dynamic_cast<RealNode*>(cRight);
        
        if (iLeft != 0 && iRight != 0) {
            return std::make_shared<IntegerNode>
                    (iLeft->getValue() - iRight->getValue());
        }
        if (iLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (iLeft->getValue() - rRight->getValue());
        }
        if (rLeft != 0 && iRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() - iRight->getValue());
        }
        if (rLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() - rRight->getValue());
        }
        
        return 0;
    } else {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        if (iRight != 0 && iRight->getValue() == 0) {
            return left;
        }
        return std::make_shared<SubtractionNode> (left, right);
    }
}


std::shared_ptr<OperationNode> SubtractionNode::clone(void) const
{
    return std::make_shared<SubtractionNode>(a, b);
}


std::string MultDivMod::getString(void) const
{
    bool addA = dynamic_cast<PlusMinus*> (&*a);
    bool addB = dynamic_cast<PlusMinus*> (&*b);
    
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


MultiplicationNode::MultiplicationNode(
        const std::shared_ptr<ExpressionNode>& a,
        const std::shared_ptr<ExpressionNode>& b) :
    MultDivMod(a, b)
{
}


std::string MultiplicationNode::getOperator(void) const
{
    return "*";
}


std::shared_ptr<ExpressionNode> MultiplicationNode::evaluate(Environment* e)
{
    std::shared_ptr<ExpressionNode> left = a->evaluate(e);
    std::shared_ptr<ExpressionNode> right = b->evaluate(e);
    ConstantNode* cLeft = dynamic_cast<ConstantNode*>(&*left);
    ConstantNode* cRight = dynamic_cast<ConstantNode*>(&*right);
    
    if (cLeft != 0 && cRight != 0) {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
        RealNode* rRight = dynamic_cast<RealNode*>(cRight);
        
        if (iLeft != 0 && iRight != 0) {
            return std::make_shared<IntegerNode>
                    (iLeft->getValue() * iRight->getValue());
        }
        if (iLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (iLeft->getValue() * rRight->getValue());
        }
        if (rLeft != 0 && iRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() * iRight->getValue());
        }
        if (rLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() * rRight->getValue());
        }
        
        return 0;
    } else {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        if (iLeft != 0 && iLeft->getValue() == 1) {
            return right;
        }
        if (iLeft != 0 && iLeft->getValue() == 0) {
            return std::make_shared<IntegerNode>(0);
        }
        if (iRight != 0 && iRight->getValue() == 1) {
            return left;
        }
        if (iRight != 0 && iRight->getValue() == 0) {
            return std::make_shared<IntegerNode>(0);
        }
        return std::make_shared<MultiplicationNode>(left, right);
    }
}


std::shared_ptr<OperationNode> MultiplicationNode::clone(void) const
{
    return std::make_shared<MultiplicationNode>(a, b);
}


ModuloNode::ModuloNode(const std::shared_ptr<ExpressionNode>& a,
                       const std::shared_ptr<ExpressionNode>& b) :
    MultDivMod(a, b)
{
}


std::string ModuloNode::getOperator(void) const
{
    return "mod";
}


std::shared_ptr<ExpressionNode> ModuloNode::evaluate(Environment* e)
{
    std::shared_ptr<ExpressionNode> left = a->evaluate(e);
    std::shared_ptr<ExpressionNode> right = b->evaluate(e);
    ConstantNode* cLeft = dynamic_cast<ConstantNode*>(&*left);
    ConstantNode* cRight = dynamic_cast<ConstantNode*>(&*right);
    
    if (cLeft != 0 && cRight != 0) {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
        RealNode* rRight = dynamic_cast<RealNode*>(cRight);
        
        if (iLeft != 0 && iRight != 0) {
            return std::make_shared<IntegerNode>
                    (iLeft->getValue() % iRight->getValue());
        }
        throw ArithmeticException("modulo operator only defined for integer operands!");
    } else {
        return std::make_shared<ModuloNode>(left, right);
    }
}


std::shared_ptr<OperationNode> ModuloNode::clone(void) const
{
    return std::make_shared<ModuloNode>(a, b);
}



DivisionNode::DivisionNode(const std::shared_ptr<ExpressionNode>& a,
                           const std::shared_ptr<ExpressionNode>& b) :
    MultDivMod(a, b)
{
}


std::string DivisionNode::getOperator(void) const
{
    return "/";
}


std::shared_ptr<ExpressionNode> DivisionNode::evaluate(Environment* e)
{
    std::shared_ptr<ExpressionNode> left = a->evaluate(e);
    std::shared_ptr<ExpressionNode> right = b->evaluate(e);
    ConstantNode* cLeft = dynamic_cast<ConstantNode*>(&*left);
    ConstantNode* cRight = dynamic_cast<ConstantNode*>(&*right);
    
    if (cLeft != 0 && cRight != 0) {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
        RealNode* rRight = dynamic_cast<RealNode*>(cRight);
        
        if (iLeft != 0 && iRight != 0) {
            return shared_from_this();
        }
        if (iLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (iLeft->getValue() / rRight->getValue());
        }
        if (rLeft != 0 && iRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() / iRight->getValue());
        }
        if (rLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (rLeft->getValue() / rRight->getValue());
        }
        
        return 0;
    } else {
        return std::make_shared<DivisionNode>(left, right);
    }
}


std::shared_ptr<OperationNode> DivisionNode::clone(void) const
{
    return std::make_shared<DivisionNode>(a, b);
}


PowerNode::PowerNode(const std::shared_ptr<ExpressionNode>& a,
                     const std::shared_ptr<ExpressionNode>& b) :
    OperationNode(a, b)
{
}


std::string PowerNode::getOperator(void) const
{
    return "^";
}


std::string PowerNode::getString(void) const
{
    bool addA = dynamic_cast<PlusMinus*> (&*a);
    bool addB = dynamic_cast<PlusMinus*> (&*b);
    
    addA |= dynamic_cast<MultDivMod*> (&*a) != 0;
    addB |= dynamic_cast<MultDivMod*> (&*b) != 0;
    
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


std::shared_ptr<ExpressionNode> PowerNode::evaluate(Environment* e)
{
    std::shared_ptr<ExpressionNode> left = a->evaluate(e);
    std::shared_ptr<ExpressionNode> right = b->evaluate(e);
    ConstantNode* cLeft = dynamic_cast<ConstantNode*>(&*left);
    ConstantNode* cRight = dynamic_cast<ConstantNode*>(&*right);
    
    if (cLeft != 0 && cRight != 0) {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        RealNode* rLeft = dynamic_cast<RealNode*>(cLeft);
        RealNode* rRight = dynamic_cast<RealNode*>(cRight);
        
        if (iLeft != 0 && iRight != 0) {
            return std::make_shared<IntegerNode>
                    (::pow(iLeft->getValue(), iRight->getValue()));
        }
        if (iLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (::pow(iLeft->getValue(), rRight->getValue()));
        }
        if (rLeft != 0 && iRight != 0) {
            return std::make_shared<RealNode>
                    (::pow(rLeft->getValue(), iRight->getValue()));
        }
        if (rLeft != 0 && rRight != 0) {
            return std::make_shared<RealNode>
                    (::pow(rLeft->getValue(), rRight->getValue()));
        }
        
        return 0;
    } else {
        IntegerNode* iLeft = dynamic_cast<IntegerNode*>(cLeft);
        IntegerNode* iRight = dynamic_cast<IntegerNode*>(cRight);
        
        if (iLeft != 0 && iLeft->getValue() == 1) {
            return std::make_shared<IntegerNode>(1);
        }
        if (iRight != 0 && iRight->getValue() == 1) {
            return left;
        }
        return std::make_shared<PowerNode>(left, right);
    }
}


std::shared_ptr<OperationNode> PowerNode::clone(void) const
{
    return std::make_shared<PowerNode>(a, b);
}


ArithmeticException::ArithmeticException(const std::string& whatStr) :
    whatStr(whatStr)
{
}


ArithmeticException::~ArithmeticException(void) throw()
{
}


const char* ArithmeticException::what(void) const throw()
{
    return whatStr.c_str();
}


