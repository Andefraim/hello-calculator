#include "expression.h"

ExpressionNumber::ExpressionNumber(QString stringNumber)
{
    int i;
    for (i = 0; i < stringNumber.length(); i++)
        if (!tr("0123456789").contains(stringNumber[i]))
            throw ExceptionExpressionNumberInvalid();
    _number = stringNumber.toDouble();
}

double ExpressionNumber::getValue() const
{
    return _number;
}

void ExpressionOperation::check()
{
    if (_operandLeft && _operandRight)
        if (!tr("+-*/^").contains(_operatorSymbol))
            throw ExceptionExpressionOperationInvalid();

    if (!_operandLeft && _operandRight)
        if (!tr("-").contains(_operatorSymbol))
            throw ExceptionExpressionOperationInvalid();

    if (_operandLeft && !_operandRight)
        throw ExceptionExpressionOperationInvalid();

    if (!_operandLeft && !_operandRight)
        throw ExceptionExpressionOperationInvalid();
}

ExpressionOperation::ExpressionOperation(Expression *operandLeft, QChar operatorSymbol, Expression *operandRight)
{
    _operandLeft = operandLeft;
    _operatorSymbol = operatorSymbol;
    _operandRight = operandRight;
    check();
}

ExpressionOperation::ExpressionOperation(QChar operatorSymbol, Expression *operandRight)
{
    _operandLeft = 0;
    _operatorSymbol = operatorSymbol;
    _operandRight = operandRight;
    check();
}

ExpressionOperation::~ExpressionOperation()
{
    if (_operandLeft)
        delete _operandLeft;
    if (_operandRight)
        delete _operandRight;
}

double ExpressionOperation::getValue() const
{
    if (_operandLeft && _operandRight)
    {
        double left = _operandLeft->getValue();
        double right = _operandRight->getValue();
        if (_operatorSymbol == '+')
            return left + right;
        if (_operatorSymbol == '-')
            return left - right;
        if (_operatorSymbol == '*')
            return left * right;
        if (_operatorSymbol == '/')
        {
            if (right == 0) throw ExceptionExpressionOperationDivisionByZero();
            return left / right;
        }
        if (_operatorSymbol == '^')
        {
            if (left < 0 && qRound(right) != right) throw ExceptionExpressionOperationNegativeBase();
            if (left == 0 && right <= 0) throw ExceptionExpressionOperationPowerOfZero();
            return qPow(left, right);
        }
    }

    if (!_operandLeft && _operandRight)
    {
        double right = _operandRight->getValue();
        if (_operatorSymbol == '-')
            return -right;
    }
    return 0;
}

void ExpressionRaw::extractParentheses()
{
    QMutableLinkedListIterator<Fragment> iterator(fragments);
    while (iterator.hasNext())
    {
        Fragment fragment;
        fragment = iterator.next();
        if (!fragment._expression)
        {
            QString raw = fragment._raw;
            int index;
            int begin;
            int level;
            QString before;
            QString after;
            Expression* expression = 0;
            level = 0;
            for (index = 0; index < raw.length(); index++)
            {
                if (raw[index] == '(')
                {
                    if (level == 0)
                    {
                        begin = index;
                        before = raw.left(index);
                    }
                    level++;
                }
                if (raw[index] == ')')
                {
                    if (level == 0)
                        throw ExceptionExpression();
                    level--;
                    if (level == 0)
                    {
                        expression = new ExpressionRaw(raw.mid(begin + 1, index - begin - 1));
                        after = raw.mid(index + 1);
                        index = raw.length();
                    }
                }
            }
            if (expression)
            {
                iterator.remove();
                if (before.length() > 0)
                    iterator.insert(before);
                iterator.insert(expression);
                if (after.length() > 0)
                {
                    iterator.insert(after);
                    iterator.previous();
                }
            }
        }
    }
}

void ExpressionRaw::extractNumbers()
{
    QMutableLinkedListIterator<Fragment> iterator(fragments);
    while (iterator.hasNext())
    {
        Fragment fragment;
        fragment = iterator.next();
        if (!fragment._expression)
        {
            QString raw = fragment._raw;
            int index;
            int begin;
            bool digits;
            QString before;
            QString after;
            Expression* expression = 0;
            digits = false;
            for (index = 0; index < raw.length(); index++)
            {
                if (tr("0123456789").contains(raw[index]))
                {
                    if (!digits)
                    {
                        digits = true;
                        begin = index;
                        before = raw.left(index);
                    }
                }
                else
                {
                    if (digits)
                    {
                        digits = false;
                        expression = new ExpressionNumber(raw.mid(begin, index - begin));
                        after = raw.mid(index);
                        index = raw.length();
                    }
                }
            }
            if (digits)
            {
                expression = new ExpressionNumber(raw.mid(begin));
                after = "";
            }
            if (expression)
            {
                iterator.remove();
                if (before.length() > 0)
                    iterator.insert(before);
                iterator.insert(expression);
                if (after.length() > 0)
                {
                    iterator.insert(after);
                    iterator.previous();
                }
            }
        }
    }
}

void ExpressionRaw::extractNegation()
{
    QMutableLinkedListIterator<Fragment> iterator(fragments);
    while (iterator.hasNext())
    {
        Fragment fragment;
        fragment = iterator.next();
        if (!fragment._expression)
        {
            QString raw = fragment._raw;
            if (raw == "-")
                if (iterator.hasNext())
                    if (iterator.peekNext()._expression)
                    {
                        iterator.previous();
                        if (!iterator.hasPrevious())
                        {
                            iterator.remove();
                            Expression* expression = iterator.next()._expression;
                            iterator.remove();
                            iterator.insert(new ExpressionOperation('-', expression));
                        }
                        else
                            iterator.next();
                    }
        }
    }
}

void ExpressionRaw::extractPower()
{
    extractBinaryOperations("^");
}

void ExpressionRaw::extractMultiplication()
{
    extractBinaryOperations("*/");
}

void ExpressionRaw::extractAddition()
{
    extractBinaryOperations("+-");
}

void ExpressionRaw::extractBinaryOperations(QString symbols)
{
    QMutableLinkedListIterator<Fragment> iterator(fragments);
    while (iterator.hasNext())
    {
        Fragment fragment;
        fragment = iterator.next();
        if (!fragment._expression)
        {
            QString raw = fragment._raw;
            if (raw.length() == 1 && symbols.contains(raw))
            {
                Expression* left;
                Expression* right;
                if (iterator.hasNext())
                    if (iterator.peekNext()._expression)
                    {
                        right = iterator.peekNext()._expression;
                        iterator.previous();
                        if (iterator.hasPrevious())
                            if (iterator.peekPrevious()._expression)
                            {
                                left = iterator.previous()._expression;
                                iterator.remove();
                                iterator.next();
                                iterator.remove();
                                iterator.next();
                                iterator.remove();
                                iterator.insert(new ExpressionOperation(left, raw[0], right));
                            }
                    }
            }
        }
    }
}

ExpressionRaw::ExpressionRaw(QString string)
{
    mainExpression = 0;
    fragments.append(string);
    extractParentheses();
    extractNumbers();
    extractNegation();
    extractPower();
    extractMultiplication();
    extractAddition();
    if (fragments.count() == 1)
        if (fragments.first()._expression)
            mainExpression = fragments.first()._expression;
}

ExpressionRaw::~ExpressionRaw()
{
    if (mainExpression)
        delete mainExpression;
}

double ExpressionRaw::getValue() const
{
    if (mainExpression)
        return mainExpression->getValue();
    throw ExceptionExpression();
    return 0;
}

ExpressionRaw::Fragment::Fragment()
{
    _raw = "";
    _expression = 0;
}

ExpressionRaw::Fragment::Fragment(QString raw)
{
    _raw = raw;
    _expression = 0;
}

ExpressionRaw::Fragment::Fragment(Expression *expression)
{
    _raw = "";
    _expression = expression;
}
