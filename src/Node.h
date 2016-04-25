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

#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <vector>
#include <memory>
#include <exception>

//#include "Natives.h"


class ExpressionNode;
class Environment;
class SubstituteRule;

typedef double FloatVal;

/*!
 * \brief base class for any object parsed
 *
 * Since this class inherits std::enable_shared_from_this<ExpressionNode>,
 * do not create instances of it which are not owned by a smart pointer.
 *
 */
class ExpressionNode :
        public std::enable_shared_from_this<ExpressionNode>
{
public:
    virtual ~ExpressionNode(void);
    
    /*!
     * \brief transforms the expression to a string that can be written to
     *        the output
     *
     * \return a string representing this expression
     */
    virtual std::string getString(void) const = 0;

    /*!
     * \brief evaluates the expression as far as possible
     *
     * \param e the \link Environment in which to evaluate
     *
     * \return a reference to the evaluated expression. Note that an
     *         expression might evaluate to itself.
     */
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);

    /*!
     * \brief returns a simplified version of this Expression
     */
    virtual std::shared_ptr<ExpressionNode> basicSimplify(Environment* e);

    /*!
     * \brief substitutes variable names with other expressions
     *
     * creates a new expression with all variables replaced with the
     * corresponding expressions.
     */
    virtual std::shared_ptr<ExpressionNode> substitute(
            const std::vector<std::unique_ptr<SubstituteRule> >& rules);

    /*!
     * \brief compare this expression to another one
     *
     * \param other the expression to compare to
     *
     * The equals function should satisfy for each a, b:
     *
     * \li a == b ==> a->equals(b)
     * \li a->equals(b) <==> b->equals(a)
     * \li a->equals(a)
     * \li a->equals(b) && b->equals(c) ==> a->equals(c)
     *
     * In short: equals should be an equivalence relation on all expressions.
     *
     * \return <code>true</code>, if this expression is the same as the other
     *         one. Note that this is no mathematical comparison since it does
     *         not compare if the values of two expressions are equal but if
     *         the two expressions display the same expression.
     */
    virtual bool equals(const ExpressionNode* other) const;
};


class StatementNode :
        public virtual ExpressionNode
{
};


class ConstantNode :
    public ExpressionNode
{
public:
};


class IntegerNode :
    public ConstantNode
{
    long long int value;
public:
    
    static const IntegerNode ZERO;

    IntegerNode(long long int value);
    IntegerNode(const std::string& value);
    
    long long int getValue(void) const;
    
    virtual std::string getString(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment*);
    
    virtual bool equals(const ExpressionNode* other) const;
};


class RealNode :
    public ConstantNode
{
    FloatVal value;
public:
    RealNode(FloatVal value);
    RealNode(const std::string& value);
    
    FloatVal getValue(void) const;
    
    virtual std::string getString(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment*);
    
    virtual bool equals(const ExpressionNode* other) const;
};


class VariableNode :
    public ExpressionNode
{
    std::string name;
public:
    VariableNode(const std::string& name);
    
    /*!
     * \brief getString
     * \return the name of this variable
     */
    virtual std::string getString(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    
    virtual std::shared_ptr<ExpressionNode> substitute(
            const std::vector<std::unique_ptr<SubstituteRule> >& rules);
    
    virtual bool equals(const ExpressionNode*) const;
};


class ParentNode :
    public virtual ExpressionNode
{
protected:
    ParentNode(void) = default;
};


class FunctionNode :
    public ExpressionNode
{
    std::vector<std::shared_ptr<VariableNode> > argumentNames;
    std::shared_ptr<ExpressionNode> equation;

public:
    FunctionNode(void) = default;

    inline FunctionNode(const std::vector<std::shared_ptr<VariableNode> >&
            argumentNames, const std::shared_ptr<ExpressionNode>& equation) :
        argumentNames(argumentNames), equation(equation) {}

    virtual std::string getString(void) const;

    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e,
        const std::vector<std::shared_ptr<ExpressionNode> >& arguments);

    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);

    virtual std::shared_ptr<ExpressionNode> substitute(
            const std::vector<std::unique_ptr<SubstituteRule> >& rules);

    virtual std::shared_ptr<ExpressionNode> getDerivative(size_t i) const;
    virtual std::shared_ptr<ExpressionNode>
        eval(Environment* e, const std::vector<std::shared_ptr
                <ExpressionNode> >& arguments) const;
};


class FunctionCallNode :
    public ParentNode
{
    //std::string functionName;
    std::shared_ptr<ExpressionNode> function;
    std::vector<std::shared_ptr<ExpressionNode> > arguments;

public:

    FunctionCallNode(const std::shared_ptr<ExpressionNode>& function);
    FunctionCallNode(const std::shared_ptr<ExpressionNode>& function,
               const std::vector<std::shared_ptr<ExpressionNode> >& arguments);
    virtual ~FunctionCallNode(void);

    void addArgument(const std::shared_ptr<ExpressionNode>& argument);

    virtual std::string getString(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);

    virtual size_t getArgumentCount(void) const;
    virtual const std::shared_ptr<ExpressionNode>& getArgument(size_t i) const;

    virtual std::shared_ptr<ExpressionNode> substitute(
            const std::vector<std::unique_ptr<SubstituteRule> >& rules);

    /*!
     * calculates the derivative in the i-th parameter
     */
    virtual std::shared_ptr<ExpressionNode> getDerivative(size_t i) const;

    virtual bool equals(const ExpressionNode*) const;
};


class OperationNode :
    public ParentNode
{
public:
    std::shared_ptr<ExpressionNode> a;
    std::shared_ptr<ExpressionNode> b;
protected:

    OperationNode(const std::shared_ptr<ExpressionNode>& a,
                  const std::shared_ptr<ExpressionNode>& b);
    virtual ~OperationNode(void);
    virtual std::string getString(void) const;
    virtual std::string getOperator(void) const = 0;
    virtual std::shared_ptr<OperationNode> clone(void) const = 0;
    
public:
    inline const std::shared_ptr<ExpressionNode>& getLeft(void) { return a; }
    inline const std::shared_ptr<ExpressionNode>& getRight(void) { return b; }
    virtual std::shared_ptr<ExpressionNode> substitute(
            const std::vector<std::unique_ptr<SubstituteRule> >& rules);
};


class AssignmentNode :
        public OperationNode,
        public StatementNode
{
public:
    AssignmentNode(const std::shared_ptr<ExpressionNode>& a,
                   const std::shared_ptr<ExpressionNode>& b);
    virtual std::string getOperator(void) const;
    virtual std::string getString(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    virtual std::shared_ptr<OperationNode> clone(void) const;
};


class PlusMinus :
    public OperationNode
{
public:
    inline PlusMinus(const std::shared_ptr<ExpressionNode>& a,
                     const std::shared_ptr<ExpressionNode>& b) :
    OperationNode(a, b) {}
};


class AdditionNode :
    public PlusMinus
{
public:
    AdditionNode(const std::shared_ptr<ExpressionNode>& a,
                 const std::shared_ptr<ExpressionNode>& b);
    virtual std::string getOperator(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    virtual std::shared_ptr<OperationNode> clone(void) const;
};


class SubtractionNode :
    public PlusMinus
{
public:
    SubtractionNode(const std::shared_ptr<ExpressionNode>& a,
                    const std::shared_ptr<ExpressionNode>& b);
    
    virtual std::string getString(void) const;
    virtual std::string getOperator(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    virtual std::shared_ptr<OperationNode> clone(void) const;
};


class MultDivMod :
    public OperationNode
{
public:
    inline MultDivMod(const std::shared_ptr<ExpressionNode>& a,
                      const std::shared_ptr<ExpressionNode>& b) :
        OperationNode(a, b) {}
    virtual std::string getString(void) const;
};


class MultiplicationNode :
    public MultDivMod
{
public:
    MultiplicationNode(const std::shared_ptr<ExpressionNode>& a,
                       const std::shared_ptr<ExpressionNode>& b);
    
    virtual std::string getOperator(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    virtual std::shared_ptr<OperationNode> clone(void) const;
};


class ModuloNode :
    public MultDivMod
{
public:
    ModuloNode(const std::shared_ptr<ExpressionNode>& a,
               const std::shared_ptr<ExpressionNode>& b);
    
    virtual std::string getOperator(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    virtual std::shared_ptr<OperationNode> clone(void) const;
};


class DivisionNode :
    public MultDivMod
{
public:
    DivisionNode(const std::shared_ptr<ExpressionNode>& a,
                 const std::shared_ptr<ExpressionNode>& b);
    
    virtual std::string getOperator(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    virtual std::shared_ptr<OperationNode> clone(void) const;
};


class PowerNode :
    public OperationNode
{
public:
    PowerNode(const std::shared_ptr<ExpressionNode>& a,
              const std::shared_ptr<ExpressionNode>& b);
    
    virtual std::string getOperator(void) const;
    virtual std::shared_ptr<ExpressionNode> evaluate(Environment* e);
    virtual std::string getString(void) const;
    virtual std::shared_ptr<OperationNode> clone(void) const;
};

class RuntimeException :
    public std::exception
{
    std::string whatStr;
public:
    inline RuntimeException(const std::string& whatStr) : whatStr(whatStr) {}
    ~RuntimeException(void) throw() = default;
    inline const char* what(void) const throw() { return whatStr.c_str(); }
};



class ArithmeticException :
    public std::exception
{
    std::string whatStr;
public:
    ArithmeticException(const std::string& whatStr);
    ~ArithmeticException(void) throw();
    const char* what(void) const throw();
};


#endif // NODE_H_

