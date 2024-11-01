#include "vacationeditdialog.h"
#include "ui_vacationeditdialog.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateEdit>
#include <QMessageBox>
#include <QTimer>

VacationEditDialog::VacationEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VacationEditDialog)
{
    ui->setupUi(this);
    setWindowTitle("Редактирование отпуска");
    ui->w_startDate->setCalendarPopup(true);
}

VacationEditDialog::~VacationEditDialog()
{
    delete ui;
}

void VacationEditDialog::handleDatabaseError() {
    qDebug() << "Ошибка получения данных из базы данных";
    QMessageBox::critical(this, "Ошибка", "Ошибка получения данных из базы данных! Окно редактирования будет закрыто.");
    reject();
    return;
}

void VacationEditDialog::setValue(const int employeeId, const QDate &startDate, const QDate &finishDate) {
    queryForValidation.prepare("SELECT id, vacation_start, vacation_finish FROM dateofvacation WHERE employee_id = :employeeId");
    queryForValidation.bindValue(":employeeId", employeeId);
    if(queryForValidation.exec())
    {
        qDebug() << "Запрос для проверки получен";
        if(!queryForValidation.first())
        {
            handleDatabaseError();
        }

    }
    else
    {
        qDebug() << "Ошибка выполнения запроса: " << queryForValidation.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса! Окно редактирования будет закрыто.");
        reject();
        return;
    }
    ui->w_startDate->setDate(startDate);
    ui->w_startDate->setMinimumDate(QDate(startDate.year(),1,1));
    ui->w_startDate->setMaximumDate(QDate(startDate.year(),12,31));
    int employeeDays = getEmployeeDays(employeeId);
    if (employeeDays == -1) {
        reject();
        return;
    }
    qDebug() << employeeDays << startDate.daysTo(finishDate.addDays(1));
    ui->w_duration->setMaximum(totalVacationLimit - employeeDays + startDate.daysTo(finishDate.addDays(1)));//?
    ui->w_duration->setMinimum(1);
    ui->w_duration->setValue(startDate.daysTo(finishDate.addDays(1)));//?
    ui->w_duration->setToolTip(QString::number(totalVacationLimit - employeeDays + startDate.daysTo(finishDate.addDays(1))));//?

    m_employeeId = employeeId;
    m_previousStartDate = startDate;
    m_previousDuration = startDate.daysTo(finishDate.addDays(1));//?
}

void VacationEditDialog::setVacatrionId(const int vacationId)
{
    m_vacationId = vacationId;
}

QDate VacationEditDialog::startDate() const {
    return ui->w_startDate->date();
}

QDate VacationEditDialog::finishDate() const {
    return ui->w_startDate->date().addDays(ui->w_duration->value()-1);//?
}

int VacationEditDialog::employeeId() const
{
    return m_employeeId;
}

int VacationEditDialog::vacationId() const
{
    return m_vacationId;
}

int VacationEditDialog::duration() const
{
    return ui->w_duration->value();
}

int VacationEditDialog::getEmployeeDays(int employeeId) {
    QSqlQuery query;
    query.prepare(QString("SELECT SUM(vacation_finish - vacation_start + 1) FROM dateofvacation WHERE employee_id = :employeeId"));
    query.bindValue(":employeeId", employeeId);

    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
        else
        {
            // Обработка ситуации, когда нет данных
            qDebug() << "Нет данных об отпусках для сотрудника с ID:" << employeeId;
            return 0; // Возвращаем 0 или другое значение по умолчанию
        }
    }
    else
    {
        // Обработка ошибки выполнения запроса
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        return -1;
    }
}

void VacationEditDialog::on_btn_save_clicked()
{
    QSqlQuery query;
    query.prepare("UPDATE DateOfVacation SET vacation_start = :startDate, vacation_finish = :finishDate WHERE id = :vacationId");
    query.bindValue(":startDate", ui->w_startDate->date());
    query.bindValue(":finishDate", ui->w_startDate->date().addDays(ui->w_duration->value()-1));//?
    query.bindValue(":vacationId", m_vacationId);
    if (query.exec())
    {
        qDebug() << "Отпуск обновлен";
    }
    else
    {
        qDebug() << "Ошибка обновления данных: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Ошибка при обновлении отпуска: " + query.lastError().text());
        return;
    }
    emit vacationChanged();
    accept();
}


void VacationEditDialog::on_btn_close_clicked()
{
    reject();
}


void VacationEditDialog::on_w_startDate_userDateChanged(const QDate &date)
{
    bool isDateValid = true;
    queryForValidation.first();
    do
    {
        if ((m_vacationId != queryForValidation.value("id").toInt()) &&
            (
                (date >= queryForValidation.value("vacation_start").toDate().addDays(-1) && date <= queryForValidation.value("vacation_finish").toDate().addDays(1))
                // Проверка на начало отпуска на 1 день раньше, отпсука не могут идти день в день (не логично их так разбивать)
                                                                         ))
        {
            isDateValid = false;
            break;
        }
        if ((m_vacationId != queryForValidation.value("id").toInt()) &&
            (
                // Проверка на пересечение с началом и концом отпуска
                (date.addDays(ui->w_duration->value()-1) >= queryForValidation.value("vacation_start").toDate() && date.addDays(ui->w_duration->value()-1)<= queryForValidation.value("vacation_finish").toDate()) ||
                // Проверка на пересечение с началом отпуска
                (date < queryForValidation.value("vacation_start").toDate() && date.addDays(ui->w_duration->value()-1) >= queryForValidation.value("vacation_start").toDate()) ||

                (date.addDays(ui->w_duration->value()-1) == queryForValidation.value("vacation_start").toDate().addDays(-1))
            ))
        {
            ui->w_duration->setValue(date.daysTo(queryForValidation.value("vacation_start").toDate()) - 1);
        }
    } while (queryForValidation.next());

    if (!isDateValid) {
        QTimer::singleShot(300, this, [this]() {QMessageBox::warning(this, "Ошибка", "Выбранная дата попадает в уже имеющийся отпускной период.");
        ui->w_startDate->setDate(m_previousStartDate);});
    } else {
        ui->w_startDate->setDate(date);
        m_previousStartDate = date;
    }
}


void VacationEditDialog::on_w_duration_valueChanged(int arg1)
{
    queryForValidation.first();
    bool isDurationValid = true;
    do
    {
        if ((m_vacationId != queryForValidation.value("id").toInt()) &&
            (
                // Проверка на пересечение с началом и концом отпуска
                (ui->w_startDate->date().addDays(arg1-1) >= queryForValidation.value("vacation_start").toDate() && ui->w_startDate->date().addDays(arg1-1) <= queryForValidation.value("vacation_finish").toDate())||
                // Проверка на пересечение с началом отпуска
                (ui->w_startDate->date().addDays(arg1-1) > queryForValidation.value("vacation_finish").toDate() && ui->w_startDate->date() < queryForValidation.value("vacation_start").toDate()) ||
                (ui->w_startDate->date().addDays(arg1-1) == queryForValidation.value("vacation_start").toDate().addDays(-1))
            ))//?
        {
            isDurationValid = false;
            break;
        }
    } while (queryForValidation.next());

    if (!isDurationValid) {
        QMessageBox::warning(this, "Ошибка", "Выбранная длительность отпуска пересекается, с уже установленным ранее отпуском!");
        ui->w_duration->setValue(m_previousDuration);
    } else {
        ui->w_duration->setValue(arg1);
        m_previousDuration = arg1;
    }
}

