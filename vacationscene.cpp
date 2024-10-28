#include "vacationscene.h"
#include "vacationrectitem.h"
#include "horizontalheaderitem.h"
#include "vacationeditdialog.h"

void VacationScene::addRow() {
    // Получаем текущую высоту сцены
    int currentHeight = sceneRect().height();

    // Добавляем новую строку
    for (int x = 0; x <= width(); x += m_cellSize.width()) {
        addLine(x, currentHeight, x, currentHeight + m_cellSize.height(), m_gridPen);
    }
    addLine(0, currentHeight + m_cellSize.height(), width(), currentHeight + m_cellSize.height(), m_gridPen);

    // Обновляем высоту сцены
    setSceneRect(0, 0, width(), currentHeight + m_cellSize.height());
}

void VacationScene::drawTable(int width, int height)
{
    for (int x = 0; x <= width; x += m_cellSize.width()) {
        addLine(x, 0, x, height, m_gridPen);
    }
    for (int y = 0; y <= height; y += m_cellSize.height()) {
        addLine(0, y, width, y, m_gridPen);
    }
}

void VacationScene::setCellSize(QSize size)
{
    m_cellSize = size;
}

void VacationScene::setGridPen(QPen gridPen)
{
    m_gridPen = gridPen;
}

QPen VacationScene::gridPen() const
{
    return m_gridPen;
}

void VacationScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // Получаем элемент, по которому был двойной щелчок
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    if (item && item->type() == QGraphicsTextItem::Type)
    {
        item = item->parentItem();
    }
    if (item && item->type() == VacationRectItem::Type)
    {
        if (auto castItem = dynamic_cast<VacationRectItem*>(item))
        {
            int vacationId = castItem->vacationId();
            int employeeId = castItem->employeeId();
            QDate startDate = castItem->startDate();
            QDate finishDate = castItem->finishDate();
            if (startDate.isValid()) {
                VacationEditDialog *dialog = new VacationEditDialog;
                dialog->setVacatrionId(vacationId);
                dialog->setValue(employeeId, startDate, finishDate);
                connect(dialog, &VacationEditDialog::vacationChanged, this, &VacationScene::updateScene);
                if(dialog->exec()==QDialog::Accepted)
                {
                    QDate newStartDate = dialog->startDate();
                    QDate newFinisDate = dialog->finishDate();
                    int duration = dialog->duration();
                    int startMonth = newStartDate.month() - 1;
                    int startDay = newStartDate.day() - 1;
                    qreal x = startMonth * m_cellSize.width() + startDay * ((qreal)m_cellSize.width()/newStartDate.daysInMonth()) + m_cellSize.width();
                    qreal controlWidth = (duration-1) * (qreal)m_cellSize.width() / newStartDate.daysInMonth();
                    if (newFinisDate.month() != newStartDate.month()) {
                        // Если отпуск продолжается в следующий месяц, добавляем к ширине часть следующего месяца
                        controlWidth += newFinisDate.day() * (qreal)m_cellSize.width() / newFinisDate.daysInMonth();
                    }
                    //qreal controlWidth = duration * (qreal)m_cellSize.width()/((qreal)(newStartDate.daysInMonth() + newFinisDate.daysInMonth())/2);
                    castItem->updateElement(x, controlWidth, QString::number(duration));
                    castItem->setVacationId(vacationId);
                    castItem->setEmployeeId(employeeId);
                    castItem->setStartDate(newStartDate);
                    castItem->setFinishDate(newFinisDate);
                    castItem->setToolTip(QString("Начало отпуска: %1 \nОкончание отпуска: %2").arg(castItem->startDate().toString("dd.MM.yyyy"), castItem->finishDate().toString("dd.MM.yyyy")));
                }
            }
        }
        // Вызываем событие двойного щелчка родительского класса
        QGraphicsScene::mouseDoubleClickEvent(event);
    }
}
void VacationScene::updateScene()
{
    for (auto anyItem : items())
    {
        if (anyItem->type() == HorizontalHeaderItem::Type)
        {
            if (auto headerItem = dynamic_cast<HorizontalHeaderItem*>(anyItem))
            {
                int month = headerItem->data(1).toInt(); // Получаем номер месяца из блока
                int norm = headerItem->data(0).toInt(); // Получаем норму для текущего месяца
                // Запрос для подсчета сотрудников с отпуском в текущем месяце
                QSqlQuery countQuery;
                countQuery.exec(QString("SELECT COUNT(DISTINCT e.id) FROM employees e JOIN DateOfVacation dov ON e.id = dov.employee_id WHERE EXTRACT(MONTH FROM dov.vacation_start) = %1 OR EXTRACT(MONTH FROM dov.vacation_finish) = %1").arg(month));
                countQuery.first();
                int employeeCount = countQuery.value(0).toInt();
                headerItem->updateColor(employeeCount, norm);
            }
        }
    }
}
