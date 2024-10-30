#ifndef VACATIONEDITDIALOG_H
#define VACATIONEDITDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QDate>

namespace Ui {
class VacationEditDialog;
}

class VacationEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VacationEditDialog(QWidget *parent = nullptr);
    ~VacationEditDialog();

    void setValue(const int employeeId, const QDate &startDate, const QDate &finishDate);
    void setVacatrionId(const int vacationId);
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

    void on_w_startDate_userDateChanged(const QDate &date);

    void on_w_duration_valueChanged(int arg1);

private:
    void handleDatabaseError();

    int getEmployeeDays(int employeeId);
    QSqlQuery queryForValidation;
    Ui::VacationEditDialog *ui;
    const int totalVacationLimit = 28;
    int m_employeeId;
    int m_vacationId;
    QDate m_previousStartDate;
    int m_previousDuration;
};

#endif // VACATIONEDITDIALOG_H
