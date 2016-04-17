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

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Node.h"
#include <vector>
#include <memory>
#include <exception>


class Symbol
{
protected:
    std::string name;
public:
    const std::string& getName(void) const;
    virtual ~Symbol(void);
};


class VariableSymbol : public Symbol
{
protected:
    std::shared_ptr<ExpressionNode> value;
public:
    VariableSymbol(const std::string& name,
                   const std::shared_ptr<ExpressionNode>& value);
    const std::shared_ptr<ExpressionNode>& getValue(void) const;
    void setValue(const std::shared_ptr<ExpressionNode>& val);
};


class FunctionParameter : public ExpressionNode
{
    int index;
public:
    FunctionParameter(int index);
    int getIndex(void) const;
};


class FunctionSymbol : public Symbol
{
protected:
    size_t argumentCount;

    //! function equation
    ExpressionNode* definition;
};


class Environment
{
    // TODO implement this as a hash table
    std::vector<Symbol*> symbols;
public:

    Environment(void) = default;

    std::shared_ptr<ExpressionNode> evaluateExpression(
            const std::shared_ptr<ExpressionNode>& expr);

    void addSymbol(Symbol* s);
    VariableSymbol* getVariable(const std::string& name);
};


class AssignmentException :
        public std::exception
{
    std::string message;
public:
    AssignmentException(const std::string& message);
    ~AssignmentException(void) throw();
    const char* what(void) const throw();
};

#endif // ENVIRONMENT_H_

