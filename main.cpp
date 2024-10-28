#include "vacationtable.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VacationTable w;
    w.show();
    return a.exec();
}
