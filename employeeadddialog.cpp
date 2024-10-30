#include "employeeadddialog.h"
#include "ui_employeeadddialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

EmployeeAddDialog::EmployeeAddDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmployeeAddDialog)
{
    ui->setupUi(this);
    setWindowTitle("Добавление сотрудника в базу");
}

EmployeeAddDialog::~EmployeeAddDialog()
{
    delete ui;
}

int EmployeeAddDialog::employeeId() const
{
    return m_employeeId;
}

QString EmployeeAddDialog::employeePart() const
{
    return m_employeePart;
}

QString EmployeeAddDialog::employeeSurname() const
{
    return m_employeeSurname;
}

QString EmployeeAddDialog::formatName() const
{
    return QString("%1 %2. %3.").arg(m_employeeSurname, m_employeeName.left(1), m_employeePart.left(1));
}

QString EmployeeAddDialog::employeeName() const
{
    return m_employeeName;
}

void EmployeeAddDialog::on_btn_save_clicked()
{
    for (auto lineEdit : findChildren<QLineEdit*>())
    {
        if (lineEdit->text().isEmpty())
        {
            QMessageBox::warning(this,"Предупреждение", "Заполните все поля!");
            return;
        }
    }
    QSqlQuery query;
    QString insertQuery =
        QString("INSERT INTO employees(name, patronymic, surname, post, subunit) VALUES('%1','%2','%3','%4','%5') RETURNING id;")
                .arg(ui->le_name->text(),
                   ui->le_part->text(),
                   ui->le_surname->text(),
                   ui->le_post->text(),
                   ui->le_subunit->text());
    m_employeeName = ui->le_name->text();
    m_employeePart = ui->le_part->text();
    m_employeeSurname = ui->le_surname->text();
    if (query.exec(insertQuery))
    {
        query.first();
        m_employeeId = query.value("id").toInt();
        qDebug() << "Новый сотрудник добавлен с ID:" << m_employeeId;
    }
    else
    {
        qDebug() << "Ошибка добавления данных: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Ошибка при добавлении сотрудника: " + query.lastError().text());
        return;
    }
    m_employeeId = query.value(0).toInt();
    accept();
}


void EmployeeAddDialog::on_btn_close_clicked()
{
    reject();
}

