#include "vacationadddialog.h"
#include "ui_vacationadddialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QMessageBox>

VacationAddDialog::VacationAddDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VacationAddDialog)
{
    ui->setupUi(this);
    setWindowTitle("Добавление отпуска");

    ui->w_startDate->setDate(QDate::currentDate());
    ui->w_startDate->setMinimumDate(QDate(QDate::currentDate().year(),1,1));
    ui->w_startDate->setMaximumDate(QDate(QDate::currentDate().year(),12,31));

    ui->w_startDate->setCalendarPopup(true);
    ui->w_employeeBox->addItem("Выберите сотрудника", QVariant(-1));
    QSqlQuery query;
    query.prepare("SELECT id, concat(surname, ' ', substr(name, 1, 1), '. ', substr(patronymic, 1, 1), '.') AS fio FROM employees ORDER BY id ASC;");

    if (query.exec()) {
        if (!query.first()) {
            qDebug() << "Ошибка получения данных о сотрудниках из базы данных: " + query.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Ошибка получения данных о сотрудниках из базы данных");
            return;
        }

        do {
            int id = query.value("id").toInt();
            QString fio = query.value("fio").toString();

            ui->w_employeeBox->addItem(fio, QVariant(id));

        } while (query.next());
    } else {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса: " + query.lastError().text());
        return;
    }
    m_previousStartDate = ui->w_startDate->date();
    m_previousDuration = 0;
}

VacationAddDialog::~VacationAddDialog()
{
    delete ui;
}

int VacationAddDialog::getEmployeeDays(int employeeId) {
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

QDate VacationAddDialog::startDate() const
{
    return ui->w_startDate->date();
}

QDate VacationAddDialog::finishDate() const
{
    return ui->w_startDate->date().addDays(ui->w_duration->value()-1);
}

int VacationAddDialog::employeeId() const
{
    return m_employeeId;
}

int VacationAddDialog::vacationId() const
{
    return m_vacationId;
}

int VacationAddDialog::duration() const
{
    return ui->w_duration->value();
}

void VacationAddDialog::on_btn_save_clicked()
{
    int id = ui->w_employeeBox->itemData(ui->w_employeeBox->currentIndex()).toInt();

    // Проверяем, что выбран сотрудник
    if (id == -1) {
        QMessageBox::warning(this, "Ошибка", "Выберите сотрудника!");
        return;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO DateOfVacation (employee_id, vacation_start, vacation_finish) VALUES (:employeeId, :startDate, :finishDate) RETURNING id;");
    query.bindValue(":startDate", ui->w_startDate->date());
    query.bindValue(":finishDate", ui->w_startDate->date().addDays(ui->w_duration->value()-1));
    query.bindValue(":employeeId", m_employeeId);
    if (query.exec())
    {
        qDebug() << "Отпуск добавлен";
        query.first();
        m_vacationId = query.value("id").toInt();
    }
    else
    {
        qDebug() << "Ошибка добавления данных: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Ошибка при добавлении отпуска: " + query.lastError().text());
        return;
    }
    emit vacationChanged();
    accept();
}


void VacationAddDialog::on_btn_close_clicked()
{
    reject();
}


void VacationAddDialog::on_w_employeeBox_currentIndexChanged(int index)
{
    int id = ui->w_employeeBox->itemData(index).toInt(); // Используем itemData() для извлечения QVariant(id)
    int employeeDays = getEmployeeDays(id);
    if (employeeDays == -1) {
        reject();
        return;
    }
    if(totalVacationLimit == employeeDays)
    {
        QMessageBox::warning(this, "Предупреждение", "Нельзя добавить отпуск. Нет свободных дней для распределения.");
        ui->w_employeeBox->setCurrentIndex(0); // Индекс 0 - значение по умолчанию
        return;
    }
    else
    {
        m_employeeId = id;
        queryForValidation.prepare("SELECT vacation_start, vacation_finish FROM dateofvacation WHERE employee_id = :employeeId");
        queryForValidation.bindValue(":employeeId", id);
        if(queryForValidation.exec())
        {
            qDebug() << "Запрос для проверки получен";
            hasRecords = queryForValidation.first();
            if(hasRecords)
            {
                do
                {
                    if ((ui->w_startDate->date() >= queryForValidation.value("vacation_start").toDate().addDays(-1) && ui->w_startDate->date() <= queryForValidation.value("vacation_finish").toDate().addDays(1))
                        )
                    {
                            ui->w_startDate->setDate(queryForValidation.value("vacation_finish").toDate().addDays(2) < ui->w_startDate->maximumDate() ?
                                    queryForValidation.value("vacation_finish").toDate().addDays(2) : queryForValidation.value("vacation_start").toDate().addDays(-2));

                    }
                    if ((ui->w_startDate->date().addDays(ui->w_duration->value()-1) >= queryForValidation.value("vacation_start").toDate() && ui->w_startDate->date().addDays(ui->w_duration->value()-1)<= queryForValidation.value("vacation_finish").toDate()) ||
                        // Проверка на пересечение с началом отпуска
                        (ui->w_startDate->date() < queryForValidation.value("vacation_start").toDate() && ui->w_startDate->date().addDays(ui->w_duration->value()-1) >= queryForValidation.value("vacation_start").toDate()) ||

                        (ui->w_startDate->date().addDays(ui->w_duration->value()-1) == queryForValidation.value("vacation_start").toDate().addDays(-1))//?
                        )
                    {
                        ui->w_duration->setValue(ui->w_startDate->date().daysTo(queryForValidation.value("vacation_start").toDate()) - 1);//?
                    }
                } while (queryForValidation.next());
            }

        }
        else
        {
            qDebug() << "Ошибка выполнения запроса: " << queryForValidation.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса! Окно редактирования будет закрыто.");
            reject();
            return;
        }
        ui->w_duration->setMaximum(totalVacationLimit - employeeDays);
        ui->w_duration->setMinimum(1);
        ui->w_duration->setValue(totalVacationLimit - employeeDays);
        ui->w_duration->setToolTip(QString::number(totalVacationLimit - employeeDays));
    }

}


void VacationAddDialog::on_w_startDate_userDateChanged(const QDate &date)
{
    bool isDateValid = true;
    if(hasRecords)
    {
        queryForValidation.first();
        do
        {
            if ((date >= queryForValidation.value("vacation_start").toDate().addDays(-1) && date <= queryForValidation.value("vacation_finish").toDate().addDays(1))
                )
            {
                isDateValid = false;
                break;
            }
            if ((date.addDays(ui->w_duration->value()-1) >= queryForValidation.value("vacation_start").toDate() && date.addDays(ui->w_duration->value()-1)<= queryForValidation.value("vacation_finish").toDate()) ||
                    // Проверка на пересечение с началом отпуска
                    (date < queryForValidation.value("vacation_start").toDate() && date.addDays(ui->w_duration->value()-1) >= queryForValidation.value("vacation_start").toDate()) ||

                    (date.addDays(ui->w_duration->value()-1) == queryForValidation.value("vacation_start").toDate().addDays(-1))//?
                    )
            {
                ui->w_duration->setValue(date.daysTo(queryForValidation.value("vacation_start").toDate()) - 1);//?
            }
        } while (queryForValidation.next());
    }
    if (!isDateValid) {
        QTimer::singleShot(300, this, [this]() {QMessageBox::warning(this, "Ошибка", "Выбранная дата попадает в уже имеющийся отпускной период.");
        ui->w_startDate->setDate(m_previousStartDate);});
    } else {
        ui->w_startDate->setDate(date);
        m_previousStartDate = date;
    }
}


void VacationAddDialog::on_w_duration_valueChanged(int arg1)
{
    bool isDurationValid = true;
    if(hasRecords)
    {
        queryForValidation.first();
        do
        {
            if ((ui->w_startDate->date().addDays(arg1-1) >= queryForValidation.value("vacation_start").toDate().addDays(-1) && ui->w_startDate->date().addDays(arg1-1) <= queryForValidation.value("vacation_finish").toDate())||
                    // Проверка на пересечение с началом отпуска
                    (ui->w_startDate->date().addDays(arg1-1) > queryForValidation.value("vacation_finish").toDate() && ui->w_startDate->date() < queryForValidation.value("vacation_start").toDate())
                    )
            {
                isDurationValid = false;
                break;
            }
        } while (queryForValidation.next());
    }
    if (!isDurationValid) {
        QMessageBox::warning(this, "Ошибка", "Выбранная длительность отпуска пересекается, с уже установленным ранее отпуском!");
        ui->w_duration->setValue(m_previousDuration);
    } else {
        ui->w_duration->setValue(arg1);
        m_previousDuration = arg1;
    }
}

