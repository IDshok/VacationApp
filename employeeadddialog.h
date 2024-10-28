#ifndef EMPLOYEEADDDIALOG_H
#define EMPLOYEEADDDIALOG_H

#include <QDialog>

namespace Ui {
class EmployeeAddDialog;
}

class EmployeeAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeAddDialog(QWidget *parent = nullptr);
    ~EmployeeAddDialog();
    int employeeId() const;
    QString employeeName() const;
    QString employeePart() const;
    QString employeeSurname() const;
    QString formatName() const;

private slots:
    void on_btn_save_clicked();

    void on_btn_close_clicked();

private:
    Ui::EmployeeAddDialog *ui;
    int m_employeeId;
    QString m_employeeName;
    QString m_employeePart;
    QString m_employeeSurname;
};

#endif // EMPLOYEEADDDIALOG_H
