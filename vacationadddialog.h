#ifndef VACATIONADDDIALOG_H
#define VACATIONADDDIALOG_H

#include <QDate>
#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class VacationAddDialog;
}

class VacationAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VacationAddDialog(QWidget *parent = nullptr);
    ~VacationAddDialog();

    int getEmployeeDays(int employeeId);
    QDate startDate() const;
    QDate finishDate() const;
    int employeeId() const;
    int vacationId() const;
    int duration() const;
signals:
    void vacationChanged();

private slots:
    void on_btn_save_clicked();

    void on_btn_close_clicked();

    void on_w_employeeBox_currentIndexChanged(int index);

    void on_w_startDate_userDateChanged(const QDate &date);

    void on_w_duration_valueChanged(int arg1);

private:
    Ui::VacationAddDialog *ui;
    QSqlQuery queryForValidation;
    bool hasRecords = false;
    const int totalVacationLimit = 28;
    int m_employeeId;
    int m_vacationId;
    QDate m_previousStartDate;
    int m_previousDuration;
};

#endif // VACATIONADDDIALOG_H
