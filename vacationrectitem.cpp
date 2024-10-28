#include "vacationrectitem.h"

#include <QBrush>

VacationRectItem::VacationRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent)
{
    setBrush(QBrush(Qt::cyan));
    vacationText = new QGraphicsTextItem(this);
    vacationText->setParentItem(this);
}

void VacationRectItem::setText(QString text)
{
    vacationText->setPlainText(text);
    vacationText->setPos(boundingRect().x() + boundingRect().width()/2 - vacationText->boundingRect().width() / 2, boundingRect().y() + boundingRect().height() / 2 - vacationText->boundingRect().height() / 2);
}

void VacationRectItem::updatePosition(qreal x, qreal y)
{
    setPos(x, y);
    vacationText->setPos(boundingRect().x() + boundingRect().width()/2 - vacationText->boundingRect().width() / 2, boundingRect().y() + boundingRect().height() / 2 - vacationText->boundingRect().height() / 2);
}

void VacationRectItem::updateElement(qreal x, qreal width, QString text)
{
    setRect(x, rect().y(), width, rect().height());
    vacationText->setPlainText(text);
    vacationText->setPos(boundingRect().x() + boundingRect().width()/2 - vacationText->boundingRect().width() / 2,
                         boundingRect().y() + boundingRect().height()/2 - vacationText->boundingRect().height() / 2);
}

void VacationRectItem::setStartDate(const QDate &startDate)
{
    m_startDate = startDate;
    setData(1, startDate);
}

void VacationRectItem::setFinishDate(const QDate &finishDate)
{
    m_finishDate = finishDate;
    setData(2, finishDate);
}

void VacationRectItem::setEmployeeId(const int employeeId)
{
    m_employeeId = employeeId;
    setData(0, employeeId);
}

void VacationRectItem::setVacationId(const int vacationId)
{
    m_vacationId = vacationId;
    setData(3, vacationId);
}

QDate VacationRectItem::startDate() const
{
    return m_startDate;
}

QDate VacationRectItem::finishDate() const
{
    return m_finishDate;
}

int VacationRectItem::employeeId() const
{
    return m_employeeId;
}

int VacationRectItem::vacationId() const
{
    return m_vacationId;
}
