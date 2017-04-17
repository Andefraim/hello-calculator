#include "widgetcalculator.h"
#include "ui_widgetcalculator.h"

WidgetCalculator::WidgetCalculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetCalculator)
{
    ui->setupUi(this);
    lineExpression = findChild<QLineEdit*>(tr("lineExpression"));
    lineResult = findChild<QLineEdit*>(tr("lineResult"));
}

WidgetCalculator::~WidgetCalculator()
{
    delete ui;
}

void WidgetCalculator::appendExpression(QString string)
{
    lineExpression->setText(lineExpression->text().append(string));
}

void WidgetCalculator::clearExpression()
{
    lineExpression->setText(tr(""));
}

void WidgetCalculator::backspaceExpression()
{
    lineExpression->setText(lineExpression->text().left(lineExpression->text().length() - 1));
}

void WidgetCalculator::calculateExpression()
{
    QString input = lineExpression->text();
    if (input.length() == 0)
    {
        lineResult->setText(tr(""));
        return;
    }
    double result;
    Expression *expression = 0;
    try
    {
        expression = new ExpressionRaw(input);
        result = expression->getValue();
        delete expression;
        lineResult->setText(QString::number(result, 'g', 9));
    }
    catch (ExceptionExpression e)
    {
        if (expression)
            delete expression;
        lineResult->setText(tr("Error"));
    }
}

void WidgetCalculator::on_button0_clicked()
{
    appendExpression(tr("0"));
}

void WidgetCalculator::on_button1_clicked()
{
    appendExpression(tr("1"));
}

void WidgetCalculator::on_button2_clicked()
{
    appendExpression(tr("2"));
}

void WidgetCalculator::on_button3_clicked()
{
    appendExpression(tr("3"));
}

void WidgetCalculator::on_button4_clicked()
{
    appendExpression(tr("4"));
}

void WidgetCalculator::on_button5_clicked()
{
    appendExpression(tr("5"));
}

void WidgetCalculator::on_button6_clicked()
{
    appendExpression(tr("6"));
}

void WidgetCalculator::on_button7_clicked()
{
    appendExpression(tr("7"));
}

void WidgetCalculator::on_button8_clicked()
{
    appendExpression(tr("8"));
}

void WidgetCalculator::on_button9_clicked()
{
    appendExpression(tr("9"));
}

void WidgetCalculator::on_buttonPlus_clicked()
{
    appendExpression(tr("+"));
}

void WidgetCalculator::on_buttonMinus_clicked()
{
    appendExpression(tr("-"));
}

void WidgetCalculator::on_buttonMultiply_clicked()
{
    appendExpression(tr("*"));
}

void WidgetCalculator::on_buttonDivide_clicked()
{
    appendExpression(tr("/"));
}

void WidgetCalculator::on_buttonPower_clicked()
{
    appendExpression(tr("^"));
}

void WidgetCalculator::on_buttonParenthesisLeft_clicked()
{
    appendExpression(tr("("));
}

void WidgetCalculator::on_buttonParenthesisRight_clicked()
{
    appendExpression(tr(")"));
}

void WidgetCalculator::on_buttonClear_clicked()
{
    clearExpression();
}

void WidgetCalculator::on_buttonBackspace_clicked()
{
    backspaceExpression();
}

void WidgetCalculator::on_buttonEqual_clicked()
{
    calculateExpression();
}

void WidgetCalculator::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    switch(key)
    {
    case Qt::Key_0:
        on_button0_clicked(); break;
    case Qt::Key_1:
        on_button1_clicked(); break;
    case Qt::Key_2:
        on_button2_clicked(); break;
    case Qt::Key_3:
        on_button3_clicked(); break;
    case Qt::Key_4:
        on_button4_clicked(); break;
    case Qt::Key_5:
        on_button5_clicked(); break;
    case Qt::Key_6:
        on_button6_clicked(); break;
    case Qt::Key_7:
        on_button7_clicked(); break;
    case Qt::Key_8:
        on_button8_clicked(); break;
    case Qt::Key_9:
        on_button9_clicked(); break;
    case Qt::Key_Plus:
        on_buttonPlus_clicked(); break;
    case Qt::Key_Minus:
        on_buttonMinus_clicked(); break;
    case Qt::Key_Asterisk:
        on_buttonMultiply_clicked(); break;
    case Qt::Key_Slash:
        on_buttonDivide_clicked(); break;
    case Qt::Key_AsciiCircum:
        on_buttonPower_clicked(); break;
    case Qt::Key_ParenLeft:
        on_buttonParenthesisLeft_clicked(); break;
    case Qt::Key_ParenRight:
        on_buttonParenthesisRight_clicked(); break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        on_buttonEqual_clicked(); break;
    case Qt::Key_Escape:
        on_buttonClear_clicked(); break;
    case Qt::Key_Backspace:
        on_buttonBackspace_clicked(); break;
    }
}
