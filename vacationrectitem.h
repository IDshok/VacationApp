#ifndef VACATIONRECTITEM_H
#define VACATIONRECTITEM_H

#include <QGraphicsRectItem>
#include <QDate>

class VacationRectItem : public QGraphicsRectItem {

public:
    VacationRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    void setText(QString text);
    void setStartDate(const QDate &startDate);
    void setFinishDate(const QDate &finishDate);
    void setEmployeeId(const int employeeId);
    void setVacationId(const int vacationId);
    QDate startDate() const;
    QDate finishDate() const;
    int employeeId() const;
    int vacationId() const;

    void updatePosition(qreal x, qreal y);
    void updateElement(qreal x, qreal width, QString text);
private:
    QGraphicsTextItem *vacationText;
    int m_vacationId;
    int m_employeeId;
    QDate m_startDate;
    QDate m_finishDate;
};

#endif // VACATIONRECTITEM_H
