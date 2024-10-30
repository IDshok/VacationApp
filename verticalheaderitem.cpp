#include "verticalheaderitem.h"

VerticalHeaderItem::VerticalHeaderItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    :QGraphicsRectItem(x, y, width, height, parent)
{
    employeeText = new QGraphicsTextItem(this);
    employeeText->setParentItem(this);
}

void VerticalHeaderItem::setText(QString text)
{
    employeeText->setPlainText(text);
    employeeText->setPos(boundingRect().x() + boundingRect().width()/2 - employeeText->boundingRect().width() / 2, boundingRect().y() + boundingRect().height() / 2 - employeeText->boundingRect().height() / 2);
}

void VerticalHeaderItem::setId(const int id)
{
    m_employeeId = id;
    setData(0, id);
}

void VerticalHeaderItem::setName(const QString &name)
{
    m_employeeName = name;
    setData(1, name);
}

void VerticalHeaderItem::setPart(const QString &part)
{
    m_employeePart = part;
    setData(2, part);
}

void VerticalHeaderItem::setSurname(const QString &surname)
{
    m_employeeSurname = surname;
    setData(3, surname);
}

void VerticalHeaderItem::setPost(const QString &post)
{
    m_post = post;
    setData(4, post);
}

void VerticalHeaderItem::setSubunit(const QString &subunit)
{
    m_subunit = subunit;
    setData(5, subunit);
}

int VerticalHeaderItem::employeeId() const
{
    return m_employeeId;
}

QString VerticalHeaderItem::employeePart() const
{
    return m_employeePart;
}

QString VerticalHeaderItem::employeeSurname() const
{
    return m_employeeSurname;
}

QString VerticalHeaderItem::post() const
{
    return m_post;
}

QString VerticalHeaderItem::subunit() const
{
    return m_subunit;
}

QString VerticalHeaderItem::employeeName() const
{
    return m_employeeName;
}
