/********************************************************************************
** Form generated from reading UI file 'vacationtable.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VACATIONTABLE_H
#define UI_VACATIONTABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VacationTable
{
public:

    void setupUi(QWidget *VacationTable)
    {
        if (VacationTable->objectName().isEmpty())
            VacationTable->setObjectName("VacationTable");
        VacationTable->resize(800, 600);

        retranslateUi(VacationTable);

        QMetaObject::connectSlotsByName(VacationTable);
    } // setupUi

    void retranslateUi(QWidget *VacationTable)
    {
        VacationTable->setWindowTitle(QCoreApplication::translate("VacationTable", "VacationTable", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VacationTable: public Ui_VacationTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VACATIONTABLE_H
