#ifndef REWRITER_H_
#define REWRITER_H_

#include <string>
#include <vector>

#include "Node.h"

class AnyValue
{
protected:
	/*!
	 * determines if the template represents one specific value,
	 * or the whole type of value
	 */
	bool defined;
public:
	AnyValue(bool defined);
    virtual std::string getString(void) const;
    
    bool isDefined(void) const;
    
    virtual bool matches(const std::shared_ptr<ExpressionNode>& expression);
};


class IntegerTemplate : public AnyValue
{
protected:
    long long value;
public:
    IntegerTemplate(bool defined, long long value);
    virtual bool matches(const std::shared_ptr<ExpressionNode>& expression);
};


class VariableTemplate : public AnyValue
{
protected:
    std::string name;
public:
    VariableTemplate(bool defined, const std::string& name);
    virtual bool matches(const std::shared_ptr<ExpressionNode>& expression);
};


class OperationTemplate : public AnyValue
{
protected:
	AnyValue* left;
	AnyValue* right;
public:
	OperationTemplate(bool defined, AnyValue* left, AnyValue* right);
	inline AnyValue* getLeft(void) { return left; }
	inline AnyValue* getRight(void) { return right; }
};


class AdditionTemplate : public OperationTemplate
{
protected:
public:
	AdditionTemplate(bool defined, AnyValue* left, AnyValue* right);
    virtual bool matches(const std::shared_ptr<ExpressionNode>& expression);
};


class IdentifierTemplate : public AnyValue
{
};


class ReplaceIdentifier : public AnyValue
{
};


class RewriteRule
{
	AnyValue* find;
    const std::shared_ptr<ExpressionNode> replace;
	
	static std::vector<RewriteRule*> rules;
public:
    RewriteRule(AnyValue* find,
                const std::shared_ptr<ExpressionNode>& replace);
	
    std::shared_ptr<ExpressionNode> getReplace(
            const std::shared_ptr<ExpressionNode>& node);
	
	static void initializeRules(void);
	//static const std::vector<RewriteRule*> getRules(void);
};


class Rewriter
{
    std::vector<RewriteRule*> rules;
public:
	Rewriter(void);
	
    std::shared_ptr<ExpressionNode> replace(
            const std::shared_ptr<ExpressionNode>& node);
};

#endif // REWRITER_H_

