#include "Rewriter.h"

AnyValue::AnyValue(bool defined) :
	defined(defined)
{
}


std::string AnyValue::getString(void) const
{
    return "";
}


bool AnyValue::isDefined(void) const
{
	return defined;
}


bool AnyValue::matches(const std::shared_ptr<ExpressionNode>&)
{
	return true;
}


IntegerTemplate::IntegerTemplate(bool defined, long long value) :
AnyValue(defined), value(value)
{
}


bool IntegerTemplate::matches(const std::shared_ptr<ExpressionNode>& expression)
{
    IntegerNode* it = dynamic_cast<IntegerNode*> (expression.get());
    if (it != 0) {
        if (!defined || it->getValue() == value)
            return true;
    }
    return false;
}



VariableTemplate::VariableTemplate(bool defined, const std::string& name) :
AnyValue(defined), name(name)
{
}


bool VariableTemplate::matches(const std::shared_ptr<ExpressionNode>& expression)
{
    VariableNode* vn = dynamic_cast<VariableNode*> (expression.get());
    if (vn != 0) {
        if (!defined || vn->getString() == name)
            return true;
    }
    return false;
}


OperationTemplate::OperationTemplate(bool defined, AnyValue* left, AnyValue* right) :
AnyValue(defined), left(left), right(right)
{
}


AdditionTemplate::AdditionTemplate(bool defined, AnyValue* left, AnyValue* right) :
OperationTemplate(defined, left, right)
{
}


bool AdditionTemplate::matches(const std::shared_ptr<ExpressionNode>& expression)
{
    AdditionNode* an = dynamic_cast<AdditionNode*> (expression.get());
	if (an != 0) {
        if (!defined || (left->matches(an->getLeft()) &&
                         right->matches(an->getRight())))
			return true;
	}
	return false;
}


RewriteRule::RewriteRule(AnyValue* find,
                         const std::shared_ptr<ExpressionNode>& replace) :
    find(find), replace(replace)
{
}


std::shared_ptr<ExpressionNode> RewriteRule::getReplace(
        const std::shared_ptr<ExpressionNode>& node)
{
	if (find->matches(node)) {
		return replace;
	}
	else {
		return node;
	}
}


void RewriteRule::initializeRules(void)
{
}


/*const std::vector<RewriteRule*> RewriteRule::getRules(void)
{
	return rules;
}*/


Rewriter::Rewriter(void)
{
    rules.push_back(new RewriteRule(new IntegerTemplate(true, 5),
                                    std::make_shared<IntegerNode>(8)));
    //AdditionNode* twice = new AdditionNode(new AnyValue(), new AdditionNode(new AnyValue(), new AnyValue()));
}


std::shared_ptr<ExpressionNode> Rewriter::replace(
        const std::shared_ptr<ExpressionNode>& node)
{
    std::shared_ptr<ExpressionNode> replaced = node;
	for (size_t i = 0; i < rules.size(); i++) {
        //std::shared_ptr<ExpressionNode> old = node;
        replaced = rules[i]->getReplace(replaced);
	}
    return replaced;
}

