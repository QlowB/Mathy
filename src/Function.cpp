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
#include "Node.h"
#include <sstream>
#include <iostream>

Function::Function(const std::string& name, size_t argumentCount) :
	name(name), argumentCount(argumentCount)
{
    //std::cout << this << " --> " << getName() << std::endl;
    //std::cout << typeid(this).name() << std::endl;
}

#include <iostream>
std::shared_ptr<ExpressionNode> Function::eval(
        Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
    // std::cout << "Function::eval\n";
    return std::make_shared<FunctionNode> (this, args);
    //return this;
}


const std::string& Function::getName(void) const
{
	return name;
}


size_t Function::getArgumentCount(void) const
{
	return argumentCount;
}


std::shared_ptr<ExpressionNode> Function::getDerivative(
        size_t i,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const
{
	Function* derivFunc;

    if (argumentCount == 1)
        derivFunc = new Function(name + "'", argumentCount);
    else {
        std::stringstream ind;
        ind << i;
        derivFunc = new Function(name + "'(" + ind.str() + ")", argumentCount);
    }
    return std::make_shared<FunctionNode>(derivFunc, args);
}
