#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QObject>
#include <QString>
#include <QtCore/qmath.h>
#include <QException>
#include <QLinkedList>
#include <QMutableLinkedListIterator>

class Expression : public QObject
{
public:
    virtual double getValue() const = 0;
};

class ExpressionNumber : public Expression
{
    double _number;
public:
    explicit ExpressionNumber(QString stringNumber);

    virtual double getValue() const;
};

class ExpressionOperation : public Expression
{
    Expression* _operandLeft;
    Expression* _operandRight;
    QChar _operatorSymbol;

    void check();
public:
    ExpressionOperation(Expression* operandLeft, QChar operatorSymbol, Expression* operandRight);
    ExpressionOperation(QChar operatorSymbol, Expression* operandRight);
    ~ExpressionOperation();

    virtual double getValue() const;
};

class ExpressionRaw : public Expression
{
    void extractParentheses();
    void extractNumbers();
    void extractNegation();
    void extractPower();
    void extractMultiplication();
    void extractAddition();
    void extractBinaryOperations(QString symbols);

    struct Fragment
    {
        Expression* _expression;
        QString _raw;
        Fragment();
        Fragment(QString raw);
        Fragment(Expression* expression);
    };

    QLinkedList<Fragment> fragments;
    Expression* mainExpression;

public:
    explicit ExpressionRaw(QString string);
    ~ExpressionRaw();

    virtual double getValue() const;
};

/* *** Exceptions *** */

class ExceptionExpression : public QException
{
public:
    void raise() const { throw *this; }
    ExceptionExpression *clone() const  { return new ExceptionExpression(*this); }
};

class ExceptionExpressionNumberInvalid : public ExceptionExpression
{
public:
    void raise() const { throw *this; }
    ExceptionExpressionNumberInvalid *clone() const  { return new ExceptionExpressionNumberInvalid(*this); }
};

class ExceptionExpressionOperationInvalid : public ExceptionExpression
{
public:
    void raise() const { throw *this; }
    ExceptionExpressionOperationInvalid *clone() const  { return new ExceptionExpressionOperationInvalid(*this); }
};

class ExceptionExpressionOperationDivisionByZero : public ExceptionExpression
{
public:
    void raise() const { throw *this; }
    ExceptionExpressionOperationDivisionByZero *clone() const  { return new ExceptionExpressionOperationDivisionByZero(*this); }
};

class ExceptionExpressionOperationNegativeBase : public ExceptionExpression
{
public:
    void raise() const { throw *this; }
    ExceptionExpressionOperationNegativeBase *clone() const  { return new ExceptionExpressionOperationNegativeBase(*this); }
};

class ExceptionExpressionOperationPowerOfZero : public ExceptionExpression
{
public:
    void raise() const { throw *this; }
    ExceptionExpressionOperationPowerOfZero *clone() const  { return new ExceptionExpressionOperationPowerOfZero(*this); }
};

#endif // EXPRESSION_H
