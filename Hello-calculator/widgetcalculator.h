#ifndef WIDGETCALCULATOR_H
#define WIDGETCALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QtCore/qmath.h>
#include "expression.h"

namespace Ui {
class WidgetCalculator;
}

class WidgetCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetCalculator(QWidget *parent = 0);
    ~WidgetCalculator();

private slots:
    void on_button0_clicked();

    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();

    void on_button5_clicked();

    void on_button6_clicked();

    void on_button7_clicked();

    void on_button8_clicked();

    void on_button9_clicked();

    void on_buttonPlus_clicked();

    void on_buttonMinus_clicked();

    void on_buttonMultiply_clicked();

    void on_buttonDivide_clicked();

    void on_buttonPower_clicked();

    void on_buttonParenthesisLeft_clicked();

    void on_buttonParenthesisRight_clicked();

    void on_buttonClear_clicked();

    void on_buttonBackspace_clicked();

    void on_buttonEqual_clicked();

    void keyPressEvent(QKeyEvent *event);
    
private:
    void appendExpression(QString string);
    void clearExpression();
    void backspaceExpression();
    void calculateExpression();
    Ui::WidgetCalculator *ui;
    QLineEdit *lineExpression;
    QLineEdit *lineResult;
};

#endif // WIDGETCALCULATOR_H
