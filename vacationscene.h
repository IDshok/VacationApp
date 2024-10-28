#ifndef VACATIONSCENE_H
#define VACATIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class VacationScene : public QGraphicsScene
{Q_OBJECT
public:
    explicit VacationScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}
    void addRow();
    void drawTable(int width, int height);
    void setCellSize(QSize size);
    void setGridPen(QPen gridPen);
    QPen gridPen() const;
    void updateScene();
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QSize m_cellSize;
    QPen m_gridPen;
};

#endif // VACATIONSCENE_H
