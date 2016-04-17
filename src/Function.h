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

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <string>
#include <memory>
#include <vector>

#include "Node.h"

typedef double FloatVal;

class Environment;

class Function :
    ExpressionNode
{
protected:
    std::string name;
    size_t argumentCount;
public:
    Function(const std::string& name, size_t argumentCount);

    virtual std::shared_ptr<ExpressionNode> eval(
            Environment* e,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;

    virtual const std::string& getName(void) const;

    virtual size_t getArgumentCount(void) const;

    /*!
     * returns the derivative function (which may be an expression e.g.
     * the derivative of ln(x) will yield 1 / x.
     *
     * However this function will not make use of the chain rule when
     * evaluating. (e.g. the derivative of ln(2 * x) will just compute
     * 1 / (2 * x), the factor 2 has to be added separately.
     *
     * \param i the index after which argument the function should be derivated
     * \param args the arguments to the function
     */
    virtual std::shared_ptr<ExpressionNode> getDerivative(
            size_t i,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;
};

#endif // FUNCTION_H_

