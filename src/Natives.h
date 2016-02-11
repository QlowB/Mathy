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

#ifndef NATIVES_H_
#define NATIVES_H_
#include <utility>
#include <memory>
#include <unordered_map>
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
    
    //virtual const std::string& getName(void) const { return asa; }
    /*virtual std::shared_ptr<ExpressionNode> eval(
        Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& args) const;*/
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

    //virtual const std::string& getName(void) const;

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
    //virtual const std::string& getName(void) const;
    virtual std::shared_ptr<ExpressionNode> getDerivative(
            size_t i,
            const std::vector<std::shared_ptr<ExpressionNode> >& args) const;
};


class Cos :
    public NativeNumFunction
{
public:
    Cos(void);

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
    static std::map<std::pair<std::string, size_t>, NativeFunction*>
        functions;
    static bool initialized;
    static void initialize(void);
public:
    static void add(NativeFunction* value);
    static NativeFunction* getNativeFunction(const std::string& name, int nArgs);

    static NativeNumFunction sin;
    static Cos cos;
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

