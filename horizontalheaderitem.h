#ifndef HORIZONTALHEADERITEM_H
#define HORIZONTALHEADERITEM_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QSqlQuery>
#include <QPair>

class HorizontalHeaderItem : public QGraphicsRectItem
{
public:
    HorizontalHeaderItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    void setText(QString text);

    void updateColor(int employeeCount, int norm);
private:
    QGraphicsTextItem *vacationText;
};

#endif // HORIZONTALHEADERITEM_H
