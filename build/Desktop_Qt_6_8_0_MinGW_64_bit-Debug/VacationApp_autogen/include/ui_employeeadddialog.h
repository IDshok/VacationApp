/********************************************************************************
** Form generated from reading UI file 'employeeadddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYEEADDDIALOG_H
#define UI_EMPLOYEEADDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmployeeAddDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *le_name;
    QLabel *label_2;
    QLineEdit *le_part;
    QLabel *label_3;
    QLineEdit *le_surname;
    QLabel *label_4;
    QLineEdit *le_post;
    QLabel *label_5;
    QLineEdit *le_subunit;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_save;
    QPushButton *btn_close;

    void setupUi(QDialog *EmployeeAddDialog)
    {
        if (EmployeeAddDialog->objectName().isEmpty())
            EmployeeAddDialog->setObjectName("EmployeeAddDialog");
        EmployeeAddDialog->resize(400, 246);
        verticalLayout = new QVBoxLayout(EmployeeAddDialog);
        verticalLayout->setObjectName("verticalLayout");
        label_6 = new QLabel(EmployeeAddDialog);
        label_6->setObjectName("label_6");

        verticalLayout->addWidget(label_6);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(EmployeeAddDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        le_name = new QLineEdit(EmployeeAddDialog);
        le_name->setObjectName("le_name");

        formLayout->setWidget(0, QFormLayout::FieldRole, le_name);

        label_2 = new QLabel(EmployeeAddDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        le_part = new QLineEdit(EmployeeAddDialog);
        le_part->setObjectName("le_part");

        formLayout->setWidget(1, QFormLayout::FieldRole, le_part);

        label_3 = new QLabel(EmployeeAddDialog);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        le_surname = new QLineEdit(EmployeeAddDialog);
        le_surname->setObjectName("le_surname");

        formLayout->setWidget(2, QFormLayout::FieldRole, le_surname);

        label_4 = new QLabel(EmployeeAddDialog);
        label_4->setObjectName("label_4");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        le_post = new QLineEdit(EmployeeAddDialog);
        le_post->setObjectName("le_post");

        formLayout->setWidget(3, QFormLayout::FieldRole, le_post);

        label_5 = new QLabel(EmployeeAddDialog);
        label_5->setObjectName("label_5");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        le_subunit = new QLineEdit(EmployeeAddDialog);
        le_subunit->setObjectName("le_subunit");

        formLayout->setWidget(4, QFormLayout::FieldRole, le_subunit);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btn_save = new QPushButton(EmployeeAddDialog);
        btn_save->setObjectName("btn_save");

        horizontalLayout->addWidget(btn_save);

        btn_close = new QPushButton(EmployeeAddDialog);
        btn_close->setObjectName("btn_close");

        horizontalLayout->addWidget(btn_close);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 4);
        verticalLayout->setStretch(2, 2);

        retranslateUi(EmployeeAddDialog);

        QMetaObject::connectSlotsByName(EmployeeAddDialog);
    } // setupUi

    void retranslateUi(QDialog *EmployeeAddDialog)
    {
        EmployeeAddDialog->setWindowTitle(QCoreApplication::translate("EmployeeAddDialog", "Dialog", nullptr));
        label_6->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\264\320\260\320\275\320\275\321\213\320\265 \320\275\320\276\320\262\320\276\320\263\320\276 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\260:", nullptr));
        label->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\230\320\274\321\217:", nullptr));
        label_2->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\236\321\202\321\207\320\265\321\201\321\202\320\262\320\276:", nullptr));
        label_3->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\244\320\260\320\274\320\270\320\273\320\270\321\217:", nullptr));
        label_4->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214:", nullptr));
        label_5->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\237\320\276\320\264\321\200\320\260\320\267\320\264\320\265\320\273\320\265\320\275\320\270\320\265:", nullptr));
        btn_save->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        btn_close->setText(QCoreApplication::translate("EmployeeAddDialog", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmployeeAddDialog: public Ui_EmployeeAddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYEEADDDIALOG_H
