/********************************************************************************
** Form generated from reading UI file 'vacationadddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VACATIONADDDIALOG_H
#define UI_VACATIONADDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VacationAddDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *w_employeeBox;
    QGridLayout *gridLayout;
    QDateEdit *w_startDate;
    QLabel *label;
    QSpinBox *w_duration;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_save;
    QPushButton *btn_close;

    void setupUi(QDialog *VacationAddDialog)
    {
        if (VacationAddDialog->objectName().isEmpty())
            VacationAddDialog->setObjectName("VacationAddDialog");
        VacationAddDialog->resize(384, 231);
        verticalLayout = new QVBoxLayout(VacationAddDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(VacationAddDialog);
        label_2->setObjectName("label_2");

        horizontalLayout_2->addWidget(label_2);

        w_employeeBox = new QComboBox(VacationAddDialog);
        w_employeeBox->setObjectName("w_employeeBox");

        horizontalLayout_2->addWidget(w_employeeBox);


        verticalLayout->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        w_startDate = new QDateEdit(VacationAddDialog);
        w_startDate->setObjectName("w_startDate");

        gridLayout->addWidget(w_startDate, 0, 1, 1, 1);

        label = new QLabel(VacationAddDialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        w_duration = new QSpinBox(VacationAddDialog);
        w_duration->setObjectName("w_duration");

        gridLayout->addWidget(w_duration, 1, 1, 1, 1);

        label_3 = new QLabel(VacationAddDialog);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btn_save = new QPushButton(VacationAddDialog);
        btn_save->setObjectName("btn_save");

        horizontalLayout->addWidget(btn_save);

        btn_close = new QPushButton(VacationAddDialog);
        btn_close->setObjectName("btn_close");

        horizontalLayout->addWidget(btn_close);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 4);
        verticalLayout->setStretch(2, 1);

        retranslateUi(VacationAddDialog);

        QMetaObject::connectSlotsByName(VacationAddDialog);
    } // setupUi

    void retranslateUi(QDialog *VacationAddDialog)
    {
        VacationAddDialog->setWindowTitle(QCoreApplication::translate("VacationAddDialog", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("VacationAddDialog", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\260:", nullptr));
        label->setText(QCoreApplication::translate("VacationAddDialog", "\320\224\320\260\321\202\320\260 \320\275\320\260\321\207\320\260\320\273\320\260 \320\276\321\202\320\277\321\203\321\201\320\272\320\260:", nullptr));
        label_3->setText(QCoreApplication::translate("VacationAddDialog", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214 \320\276\321\202\320\277\321\203\321\201\320\272\320\260", nullptr));
        btn_save->setText(QCoreApplication::translate("VacationAddDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        btn_close->setText(QCoreApplication::translate("VacationAddDialog", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VacationAddDialog: public Ui_VacationAddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VACATIONADDDIALOG_H
