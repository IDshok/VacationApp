/********************************************************************************
** Form generated from reading UI file 'vacationeditdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VACATIONEDITDIALOG_H
#define UI_VACATIONEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VacationEditDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QSpinBox *w_duration;
    QDateEdit *w_startDate;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_save;
    QPushButton *btn_close;

    void setupUi(QDialog *VacationEditDialog)
    {
        if (VacationEditDialog->objectName().isEmpty())
            VacationEditDialog->setObjectName("VacationEditDialog");
        VacationEditDialog->resize(400, 165);
        verticalLayout = new QVBoxLayout(VacationEditDialog);
        verticalLayout->setObjectName("verticalLayout");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(VacationEditDialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(VacationEditDialog);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        w_duration = new QSpinBox(VacationEditDialog);
        w_duration->setObjectName("w_duration");

        gridLayout->addWidget(w_duration, 1, 1, 1, 1);

        w_startDate = new QDateEdit(VacationEditDialog);
        w_startDate->setObjectName("w_startDate");

        gridLayout->addWidget(w_startDate, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btn_save = new QPushButton(VacationEditDialog);
        btn_save->setObjectName("btn_save");

        horizontalLayout->addWidget(btn_save);

        btn_close = new QPushButton(VacationEditDialog);
        btn_close->setObjectName("btn_close");

        horizontalLayout->addWidget(btn_close);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 1);

        retranslateUi(VacationEditDialog);

        QMetaObject::connectSlotsByName(VacationEditDialog);
    } // setupUi

    void retranslateUi(QDialog *VacationEditDialog)
    {
        VacationEditDialog->setWindowTitle(QCoreApplication::translate("VacationEditDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("VacationEditDialog", "\320\224\320\260\321\202\320\260 \320\275\320\260\321\207\320\260\320\273\320\260 \320\276\321\202\320\277\321\203\321\201\320\272\320\260:", nullptr));
        label_3->setText(QCoreApplication::translate("VacationEditDialog", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214 \320\276\321\202\320\277\321\203\321\201\320\272\320\260", nullptr));
        btn_save->setText(QCoreApplication::translate("VacationEditDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        btn_close->setText(QCoreApplication::translate("VacationEditDialog", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VacationEditDialog: public Ui_VacationEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VACATIONEDITDIALOG_H
