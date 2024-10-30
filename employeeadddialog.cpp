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
void EmployeeAddDialog::setValue(const int employeeId, const QString& name, const QString& patronymic, const QString& surname, const QString& post, const QString& subunit) {
    // Устанавливаем значения в lineEdit-ы
    m_employeeId = employeeId;
    ui->le_name->setText(name);
    ui->le_part->setText(patronymic);
    ui->le_surname->setText(surname);
    ui->le_post->setText(post);
    ui->le_subunit->setText(subunit);
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

QString EmployeeAddDialog::employeePost() const
{
    return m_employeePost;
}

QString EmployeeAddDialog::employeeSubunit() const
{
    return m_employeeSubunit;
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
    // QString insertQuery =
    //     QString("INSERT INTO employees(name, patronymic, surname, post, subunit) VALUES('%1','%2','%3','%4','%5') RETURNING id;")
    //             .arg(ui->le_name->text(),
    //                ui->le_part->text(),
    //                ui->le_surname->text(),
    //                ui->le_post->text(),
    //                ui->le_subunit->text());
    QString insertQuery = QString("INSERT INTO employees(name, patronymic, surname, post, subunit) VALUES(:name, :patronymic, :surname, :post, :subunit) RETURNING id;");
    QString updateQuery = QString("UPDATE employees SET name=:name, patronymic=:patronymic, surname=:surname, post=:post, subunit=:subunit WHERE id=:id;");

    query.prepare(m_employeeId == 0 ? insertQuery : updateQuery);

    query.bindValue(":name", ui->le_name->text());
    query.bindValue(":patronymic", ui->le_part->text());
    query.bindValue(":surname", ui->le_surname->text());
    query.bindValue(":post", ui->le_post->text());
    query.bindValue(":subunit", ui->le_subunit->text());

    if (m_employeeId != 0) {
        query.bindValue(":id", m_employeeId);
    }
    m_employeeName = ui->le_name->text();
    m_employeePart = ui->le_part->text();
    m_employeeSurname = ui->le_surname->text();
    m_employeePost = ui->le_post->text();
    m_employeeSubunit = ui->le_subunit->text();
    if (query.exec())
    {
        if (m_employeeId == 0) {
            query.first();
            m_employeeId = query.value("id").toInt();
            qDebug() << "Новый сотрудник добавлен с ID:" << m_employeeId;
        } else {
            qDebug() << "Сотрудник с ID:" << m_employeeId << "обновлен";
        }
    }
    else
    {
        qDebug() << "Ошибка добавления данных: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Ошибка при добавлении сотрудника: " + query.lastError().text());
        return;
    }
    //m_employeeId = query.value(0).toInt();
    accept();
}


void EmployeeAddDialog::on_btn_close_clicked()
{
    reject();
}

