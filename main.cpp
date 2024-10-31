#include "vacationtable.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    if (translator.load("C:\\Users\\Ace\\Documents\\VacationApp\\VacationApp_en_US")) {
        a.installTranslator(&translator);
    }
    VacationTable w;
    w.show();
    return a.exec();
}
