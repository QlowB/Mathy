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

#include "Natives.h"
#include <utility>
#include <iostream>

#include <cmath>

#include "Node.h"
#include "Environment.h"

std::map<std::string, std::shared_ptr<ExpressionNode> > Constants::constants;
bool Constants::initialized = false;


void Constants::initialize(void)
{
    add("pi", std::make_shared<RealNode>(3.141592653589793238462643383279));
    add("e", std::make_shared<RealNode>(2.718281828459045235360287471352));
}


void Constants::add(const std::string& name,
                    const std::shared_ptr<ExpressionNode>& value)
{
    constants.insert(
        std::pair<std::string, std::shared_ptr<ExpressionNode> >(
            name, value 
        )
    );
}


NativeFunction::NativeFunction(const std::string& name, size_t argumentCount) :
    name(name)
{
    // we do nothing with argumentCount, sorry...
}


NativeNumFunction::NativeNumFunction(const std::string& name,
                                     MathFunc function,
                                     NativeNumFunction* derivative) :
    NativeFunction(name, 1), function(function), derivative(derivative)
{
}


/*
std::shared_ptr<ExpressionNode> NativeNumFunction::eval(Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
    std::shared_ptr<ExpressionNode> eval = args[0]->evaluate(e);
    RealNode* real = dynamic_cast<RealNode*>(eval.get());
    IntegerNode* intN = dynamic_cast<IntegerNode*>(eval.get());
    
    if (real != nullptr) {
        FloatVal arg = 0;
        arg = real->getValue();
        return std::make_shared<RealNode> (evaluate(arg));
    }
    return evaluate(e, args);
}
*/


FloatVal NativeNumFunction::evaluate(FloatVal arg) const
{
    return function(arg);
}



std::shared_ptr<ExpressionNode> NativeNumFunction::evaluate(
        Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& args)
{
    std::shared_ptr<ExpressionNode> eval = args[0]->evaluate(e);
    RealNode* real = dynamic_cast<RealNode*>(eval.get());
    IntegerNode* intN = dynamic_cast<IntegerNode*>(eval.get());
    
    if (real != nullptr) {
        FloatVal arg = real->getValue();
        return std::make_shared<RealNode> (evaluate(arg));
    }
    return shared_from_this();
}


std::shared_ptr<ExpressionNode> NativeNumFunction::getDerivative(
        size_t i,
        const std::vector<std::shared_ptr<ExpressionNode> > &args) const
{
    return std::make_shared<FunctionCallNode> (derivative->shared_from_this(), args);
}


Log::Log(void) :
    NativeNumFunction("ln", &::log, nullptr)
{
}


std::shared_ptr<ExpressionNode> Log::getDerivative(
        size_t i,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
    return std::make_shared<DivisionNode>(std::make_shared<IntegerNode>(1),
                                          args[0]);
}



Cos::Cos(void) :
    NativeNumFunction("cos", &::cos, nullptr)
{
}


std::shared_ptr<ExpressionNode> Cos::getDerivative(
        size_t i,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
    return std::make_shared<SubtractionNode>(std::make_shared<IntegerNode>(0),
            std::make_shared<FunctionCallNode>(Functions::sin.shared_from_this(), args));
}



DerivativeFunction::DerivativeFunction(const std::string& name) :
    NativeFunction(name, 2)
{
}


std::shared_ptr<ExpressionNode> DerivativeFunction::eval(
        Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
    std::shared_ptr<ExpressionNode> eval = args[0]->evaluate(e);
    std::shared_ptr<ExpressionNode> variable = args[1]->evaluate(e);
    
    return getDerivative(e, eval, variable);
}


std::shared_ptr<ExpressionNode> DerivativeFunction::getDerivative(
        Environment* e, std::shared_ptr<ExpressionNode> value,
        std::shared_ptr<ExpressionNode> variable) const
{
    RealNode* real = dynamic_cast<RealNode*>(value.get());
    if (real != 0) {
        return std::make_shared<RealNode>(0);
    }
    
    IntegerNode* intN = dynamic_cast<IntegerNode*>(value.get());
    if (intN != 0) {
        return std::make_shared<IntegerNode>(0);
    }
    
    VariableNode* var = dynamic_cast<VariableNode*>(value.get());
    if (var != 0) {
        if (value->equals(variable.get())) {
            return std::make_shared<IntegerNode>(1);
        }
        else {
            return std::make_shared<IntegerNode>(0);
        }
    }
    
    OperationNode* addsub = dynamic_cast<AdditionNode*>(value.get());
    if (addsub == 0)
        addsub = dynamic_cast<SubtractionNode*>(&*value);
    if (addsub != 0) {
        std::shared_ptr<ExpressionNode> a =
                getDerivative(e, addsub->a, variable);
        std::shared_ptr<ExpressionNode> b =
                getDerivative(e, addsub->b, variable);
        
        std::shared_ptr<OperationNode> result;
        if (dynamic_cast<AdditionNode*>(&*value) != 0)
            result = std::make_shared<AdditionNode>(a, b);
        else
            result = std::make_shared<SubtractionNode>(a, b);
        return result->evaluate(e);
    }
    
    MultiplicationNode* mul = dynamic_cast<MultiplicationNode*>(value.get());
    if (mul != 0) {
        std::shared_ptr<ExpressionNode> a = getDerivative(e, mul->a, variable);
        std::shared_ptr<ExpressionNode> b = getDerivative(e, mul->b, variable);
        
        std::shared_ptr<MultiplicationNode> first =
                std::make_shared<MultiplicationNode>(a, mul->b);
        std::shared_ptr<MultiplicationNode> second =
                std::make_shared<MultiplicationNode>(mul->a, b);
        std::shared_ptr<AdditionNode> newNode =
                std::make_shared<AdditionNode>(first, second);

        return newNode->evaluate(e);
    }
    
    DivisionNode* div = dynamic_cast<DivisionNode*>(value.get());
    if (div != 0) {
        std::shared_ptr<ExpressionNode> a = getDerivative(e, div->a, variable);
        std::shared_ptr<ExpressionNode> b = getDerivative(e, div->b, variable);


        std::shared_ptr<MultiplicationNode> first =
                std::make_shared<MultiplicationNode>(a, div->b);
        std::shared_ptr<MultiplicationNode> second =
                std::make_shared<MultiplicationNode>(b, div->a);
        
        std::shared_ptr<MultiplicationNode> bottomsq =
                std::make_shared<MultiplicationNode>(div->b, div->b);
        std::shared_ptr<SubtractionNode> newNode =
                std::make_shared<SubtractionNode>(first, second);
        std::shared_ptr<DivisionNode> final =
                std::make_shared<DivisionNode>(newNode, bottomsq);
        return final->evaluate(e);
    }
    
    std::shared_ptr<PowerNode> pow =
            std::dynamic_pointer_cast<PowerNode>(value);
    if (pow != 0) {
        
        //std::cout << "POW begin\n";
        static const FunctionNode* logfnc = &Functions::ln;
        
        // d/dx (f(x) ^ g(x)) = f(x)^g(x) * (g(x)*f'(x)/f(x) + log(f(x))g'(x))
        std::shared_ptr<ExpressionNode> a = getDerivative(e, pow->a, variable);
        std::shared_ptr<ExpressionNode> b = getDerivative(e, pow->b, variable);

        std::shared_ptr<MultiplicationNode> bastr =
                std::make_shared<MultiplicationNode>(pow->b, a);
        std::shared_ptr<DivisionNode> bruch =
                std::make_shared<DivisionNode>(bastr, pow->a);
        std::shared_ptr<ExpressionNode> logFunc = Functions::ln.shared_from_this();
        std::shared_ptr<FunctionCallNode> logFuncCall =
                std::make_shared<FunctionCallNode>(logFunc,
                std::vector<std::shared_ptr<ExpressionNode> >({pow->a}));
        //logFunc->addArgument(pow->a);
        std::shared_ptr<MultiplicationNode> secondMul =
                std::make_shared<MultiplicationNode>(logFunc, b);
        std::shared_ptr<AdditionNode> paranSum =
                std::make_shared<AdditionNode>(bruch, secondMul);
        std::shared_ptr<MultiplicationNode> result =
                std::make_shared<MultiplicationNode>(pow, paranSum);
        return result->evaluate(e);
        /*std::cout << result->getString();
        std::shared_ptr<ExpressionNode> expr = result->evaluate(e);
        std::cout << "POW end\n";
        return expr;*/
    }

#if 0
    FunctionNode* func = dynamic_cast<FunctionNode*>(&*value);
    if (func != 0) {
        // std::cout << "Function deriv\n";
        std::shared_ptr<AdditionNode> final(nullptr);
        for (size_t i = 0; i < func->getArgumentCount(); i++) {

            // std::cout << "chain rule!\n";
            std::shared_ptr<ExpressionNode> argDeriv =
                    getDerivative(e, func->getArgument(i), variable);
            // std::cout << "func der!\n";
            std::shared_ptr<MultiplicationNode> summand =
                std::make_shared<MultiplicationNode>
                    (argDeriv, func->getDerivative(i));
            
            // std::cout << "did summand!\n";
            if (func->getArgumentCount() == 1) {
                // std::cout << "one arg!\n";
                return summand->evaluate(e);
            }
            else if (final == 0) {
                final = std::make_shared<AdditionNode>(summand, nullptr);
            }
            else if (final->b == nullptr) {
                final->b = summand;
            }
            else {
                final->b = std::make_shared<AdditionNode>(final->b, summand);
            }
        }
        return final;
    }
#endif
    std::cout << "found no derivative\n";
    return nullptr;
}


const std::shared_ptr<ExpressionNode>&
Constants::getConstant(const std::string& name)
{
    if (!initialized) {
        initialize();
        initialized = true;
    }
    if (constants.find(name) != constants.end())
        return constants[name];
    else {
        static const std::shared_ptr<ExpressionNode> null = nullptr;
        return null;
    }
}


std::map<std::pair<std::string, size_t>, NativeFunction*>
    Functions::functions;
bool Functions::initialized = false;


void Functions::initialize(void)
{
    add(&sin);
    add(&cos);
    add(&tan);
    add(&asin);
    add(&acos);
    add(&atan);
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
    using std::pair;
    using std::string;
    const std::string& name = value->getName();
    size_t nArgs = 0; //value->getArgumentCount();
    functions.insert(
        pair<pair<string, size_t>, NativeFunction*>(
            pair<string, size_t> (name, nArgs), value 
        )
    );
}


NativeFunction* Functions::getNativeFunction(const std::string& name, int nArgs)
{
    if (!initialized) {
        initialize();
        initialized = true;
    }

    //std::cout << "looking for: " << name << " with " << nArgs << " args" << std::endl;
    //std::cout << "list has " << functions.size() << " entries." << std::endl;
    //for (auto i = functions.begin(); i != functions.end(); i++)
    //{
    //    std::cout << "element: " << (*i).second->getName();
    //}

    auto elem = functions.find(std::pair<std::string, int> (name, nArgs)); 
    if (elem != functions.end())
        return elem->second;
    else
        return 0;
}

NativeNumFunction Functions::sin("sin", &::sin, &Functions::cos);
Cos Functions::cos;
NativeNumFunction Functions::tan("tan", &::tan, &Functions::cos);
NativeNumFunction Functions::asin("asin", &::asin, &Functions::cos);
NativeNumFunction Functions::acos("acos", &::acos, &Functions::cos);
NativeNumFunction Functions::atan("atan", &::atan, &Functions::cos);
NativeNumFunction Functions::exp("exp", &::exp, &Functions::cos);
Log Functions::ln;
NativeNumFunction Functions::sinh("sinh", &::sinh, &Functions::cosh);
NativeNumFunction Functions::cosh("cosh", &::cosh, &Functions::sinh);

