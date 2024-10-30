#ifndef VACATIONTABLE_H
#define VACATIONTABLE_H
#include "vacationscene.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class VacationTable;
}
QT_END_NAMESPACE

class VacationTable : public QWidget
{
    Q_OBJECT

public:
    VacationTable(QWidget *parent = nullptr);
    ~VacationTable();
    bool connectToDatabase();
    void drawHeaders();
    void drawVacations();
private slots:
    void addNewEmployee();
private:
    Ui::VacationTable *ui;
    VacationScene* scene;
    QGraphicsView *view;
    QSize cellSize;

};



#endif // VACATIONTABLE_H
