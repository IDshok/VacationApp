#include "horizontalheaderitem.h"

HorizontalHeaderItem::HorizontalHeaderItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent)
{
    setBrush(Qt::white); // Устанавливаем белый фон для всей панели
    vacationText = new QGraphicsTextItem(this);
    vacationText->setParentItem(this);

}

void HorizontalHeaderItem::setText(QString text)
{
    vacationText->setPlainText(text);
    vacationText->setPos(boundingRect().x() + boundingRect().width()/2 - vacationText->boundingRect().width() / 2, boundingRect().y() + boundingRect().height() / 2 - vacationText->boundingRect().height() / 2);
}

void HorizontalHeaderItem::updateColor(int employeeCount, int norm)
{
    this->setBrush(employeeCount > norm ? Qt::red : Qt::green);
}
