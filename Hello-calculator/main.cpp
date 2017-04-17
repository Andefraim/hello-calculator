#include "widgetcalculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WidgetCalculator w;
    w.show();

    return a.exec();
}
