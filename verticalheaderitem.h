#ifndef VERTICALHEADERITEM_H
#define VERTICALHEADERITEM_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QSqlQuery>

class VerticalHeaderItem : public QGraphicsRectItem
{
public:
    VerticalHeaderItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    void setText(QString text);
    void setId(const int id);
    void setName(const QString &name);
    void setPart(const QString &part);
    void setSurname(const QString &surname);
    void setPost(const QString &post);
    void setSubunit(const QString &subunit);

    int employeeId() const;
    QString employeeName() const;
    QString employeePart() const;
    QString employeeSurname() const;
    QString post() const;
    QString subunit() const;
private:
    QGraphicsTextItem *employeeText;
    int m_employeeId;
    QString m_employeeName;
    QString m_employeePart;
    QString m_employeeSurname;
    QString m_post;
    QString m_subunit;
};

#endif // VERTICALHEADERITEM_H
