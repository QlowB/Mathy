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

#include "Environment.h"
#include "Natives.h"

const std::string& Symbol::getName(void) const
{
    return name;
}


Symbol::~Symbol(void)
{
}


VariableSymbol::VariableSymbol(const std::string& name,
                               const std::shared_ptr<ExpressionNode>& value) :
    value(value)
{
    this->name = name;
}


const std::shared_ptr<ExpressionNode>& VariableSymbol::getValue(void) const
{
    return value;
}


void VariableSymbol::setValue(const std::shared_ptr<ExpressionNode>& v)
{
    value = v;
}



FunctionParameter::FunctionParameter(int index) :
    index(index)
{
}


int FunctionParameter::getIndex(void) const
{
    return index;
}


Environment::Environment(void)
{
    VariableSymbol* vs = new VariableSymbol("cos",
            std::make_shared<Cos>());
    addSymbol(vs);
    
    vs = new VariableSymbol("if",
            std::make_shared<If>());
    addSymbol(vs);
}


Environment::~Environment(void)
{
}


std::shared_ptr<ExpressionNode> Environment::evaluateExpression(
        const std::shared_ptr<ExpressionNode>& expr)
{
    return expr->evaluate(this);
}


void Environment::addSymbol(Symbol* s)
{
    symbols.push_back(s);
}


VariableSymbol* Environment::getVariable(const std::string& name)
{
    for (size_t i = 0; i < symbols.size(); i++) {
        if (dynamic_cast<VariableSymbol*> (symbols[i]) &&
                dynamic_cast<VariableSymbol*> (symbols[i])->getName() == name) {
            return dynamic_cast<VariableSymbol*> (symbols[i]);
        }
    }
    return 0;
}


AssignmentException::AssignmentException(const std::string& message) :
    message(message)
{
}


AssignmentException::~AssignmentException(void) throw()
{
}


const char* AssignmentException::what(void) const throw()
{
    return message.c_str();
}

