#include "vacationscene.h"
#include "vacationrectitem.h"
#include "horizontalheaderitem.h"
#include "verticalheaderitem.h"
#include "employeeadddialog.h"
#include "vacationeditdialog.h"

#include <QSqlError>
#include <QMenu>
#include <QAction>

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
                    // qreal controlWidth = (duration) * (qreal)m_cellSize.width() / newStartDate.daysInMonth();//?
                    // if (newFinisDate.month() != newStartDate.month()) {
                    //     // Если отпуск продолжается в следующий месяц, добавляем к ширине часть следующего месяца
                    //     controlWidth += newFinisDate.day() * (qreal)m_cellSize.width() / newFinisDate.daysInMonth();
                    // }

                    qreal controlWidth = duration * (qreal)m_cellSize.width()/((qreal)(newStartDate.daysInMonth() + newFinisDate.daysInMonth())/2);
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
    if (item && item->type() == VerticalHeaderItem::Type)
    {
        if (auto castItem = dynamic_cast<VerticalHeaderItem*>(item))
        {
            QScopedPointer<EmployeeAddDialog> dialog(new EmployeeAddDialog());
            dialog->setValue(castItem->employeeId(), castItem->employeeName(), castItem->employeePart(), castItem->employeeSurname(), castItem->post(), castItem->subunit());
            if(dialog->exec() == QDialog::Accepted)
            {
                // Заполняем данные в VerticalHeaderItem
                castItem->setId(dialog->employeeId());
                castItem->setName(dialog->employeeName());
                castItem->setPart(dialog->employeePart());
                castItem->setSurname(dialog->employeeSurname());
                castItem->setSubunit(dialog->employeePost());
                castItem->setPost(dialog->employeeSubunit());
                castItem->setText(QString("%1. %2").arg(QString::number(dialog->employeeId()), dialog->formatName()));
                // QGraphicsTextItem *employeeText = scene->addText(QString::number(dialog->employeeId()) +". " + dialog->formatName());
                // employeeText->setPos(5, cellSize.height()*dialog->employeeId());
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
                countQuery.prepare("SELECT COUNT(DISTINCT e.id) AS current_month_count FROM employees e JOIN DateOfVacation dov ON e.id = dov.employee_id WHERE EXTRACT(MONTH FROM dov.vacation_start) = :month OR EXTRACT(MONTH FROM dov.vacation_finish) = :month");
                countQuery.bindValue(":month", month);
                if(countQuery.exec())
                {
                    if(countQuery.first())
                    {
                        int employeeCount = countQuery.value("current_month_count").toInt();
                        headerItem->updateColor(employeeCount, norm);
                    }
                    else
                        qDebug() << "Ошибка получения данных, для обновления цветовой индикации месяцев!";
                }
                else
                    qDebug() << "Ошибка выполнения запроса обновления цветовой индикации месяцев!" << countQuery.lastError().text();

            }
        }
    }
}

void VacationScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    if (item && item->type() == QGraphicsTextItem::Type)
    {
        item = item->parentItem();
    }
    if (item && item->type() == VacationRectItem::Type)
    {
        if (auto deleteItem = dynamic_cast<VacationRectItem*>(item))
        {
            QMenu *menu = new QMenu();
            QAction *deleteAction = menu->addAction("Удалить");
            connect(deleteAction, &QAction::triggered, [this, deleteItem]() {
                deleteVacationItem(deleteItem);
            });
            menu->exec(event->screenPos());
        }
    }
    if (item && item->type() == VerticalHeaderItem::Type)
    {
        if (auto deleteEmployeeItem = dynamic_cast<VerticalHeaderItem*>(item))
        {
            QMenu *menu = new QMenu();
            QAction *deleteAction = menu->addAction("Удалить сотрудника");
            connect(deleteAction, &QAction::triggered, [this, deleteEmployeeItem]() {
                deleteEmployee(deleteEmployeeItem);
            });
            menu->exec(event->screenPos());
        }
    }
    QGraphicsScene::contextMenuEvent(event); // Передача события в родительский класс
}

void VacationScene::deleteVacationItem(VacationRectItem* item) {
    int vacationId = item->vacationId();
    QSqlQuery query;
    query.prepare("DELETE FROM dateofvacation WHERE id = :vacation_id");
    query.bindValue(":vacation_id", vacationId); // Замените vacationId на фактическое значение ID отпуска

    if (query.exec()) {
        qDebug() << "Блок отпуска удален";
    } else {
        qDebug() << "Ошибка удаления отпуска из базы данных:" << query.lastError().text();
    }
    removeItem(item);
    updateScene();
}

void VacationScene::deleteEmployee(VerticalHeaderItem *item) {
    int employeeId = item->employeeId();

    // Удаление сотрудника из базы данных
    QSqlQuery query;
    // query.prepare("DELETE FROM dateofvacation WHERE employee_id = :employee_id;");
    // query.bindValue(":employee_id", employeeId);
    // if (!query.exec()) {
    //     qDebug() << "Ошибка удаления связанных отпусков:" << query.lastError().text();
    // }
    // query.prepare("UPDATE dateofvacation SET employee_id = employee_id - 1 WHERE employee_id > :employee_id");
    // query.bindValue(":employee_id", employeeId);
    // if (!query.exec()) {
    //     qDebug() << "Ошибка обновления id сотрудников в отпусках:" << query.lastError().text();
    //     return;
    // }

    query.prepare("DELETE FROM employees WHERE id = :employee_id;");
    query.bindValue(":employee_id", employeeId);
    if (!query.exec()) {
        qDebug() << "Ошибка удаления сотрудника из базы данных:" << query.lastError().text();
        return;
    }

    removeItem(item);


    query.prepare("UPDATE employees SET id = id - 1 WHERE id > :employee_id");
    query.bindValue(":employee_id", employeeId);
    if (!query.exec()) {
        qDebug() << "Ошибка обновления id сотрудников:" << query.lastError().text();
        return;
    }
    int maxId;
    query.prepare("SELECT MAX(id) FROM employees");
    if (query.exec() && query.next()) {
        maxId = query.value(0).toInt();
        if(!query.exec(QString("ALTER SEQUENCE employees_id_seq RESTART WITH %1").arg(maxId + 1)))
        {
            qDebug() << "Ошибка обновления счетчика первичного ключа:" << query.lastError().text();
        }
    } else {
        qDebug() << "Ошибка получения максимального id:" << query.lastError().text();
    }
    // обновляем индексацию в БД


    // Удаление элементов отпуска со сцены
    QList<VacationRectItem*> itemsToRemove;
    for (auto anyItem : items()) {
        if (anyItem->type() == VacationRectItem::Type) {
            if(auto vacationItem = dynamic_cast<VacationRectItem*>(anyItem))
            {
                if (vacationItem->employeeId() == employeeId) {
                    itemsToRemove.append(vacationItem);
                }
            }
        }
    }

    for (auto itemToRemove : itemsToRemove) {
        removeItem(itemToRemove);
    }

    for (auto anyItem : items()) {
        if (anyItem->type() == VerticalHeaderItem::Type) {
            if (auto headerItem = dynamic_cast<VerticalHeaderItem*>(anyItem)) {
                if (headerItem->employeeId() > employeeId) {
                    headerItem->setId(headerItem->employeeId()-1);
                    headerItem->setText(QString("%1. %2").arg(QString::number(headerItem->employeeId()), QString("%1 %2. %3.").arg(headerItem->employeeSurname(), headerItem->employeeName().left(1), headerItem->employeePart().left(1))));
                    // Сдвиг  y-координаты  для  элементов  после  удаленного
                    headerItem->setPos(headerItem->x(), headerItem->y() - m_cellSize.height());
                }
            }
        }
        if (anyItem->type() == VacationRectItem::Type) {
            if (auto vacationItem = dynamic_cast<VacationRectItem*>(anyItem)) {
                if (vacationItem->employeeId() > employeeId) {
                    vacationItem->setEmployeeId(vacationItem->employeeId()-1);
                    // Сдвиг  y-координаты  для  элементов  ниже  удаленного
                    vacationItem->updatePosition(vacationItem->x(), vacationItem->y()-m_cellSize.height());
                }
            }
        }
        if (anyItem->type() == QGraphicsLineItem::Type) {
            // Проверяем, является ли линия нижней границей
            if (auto line = dynamic_cast<QGraphicsLineItem*>(anyItem)){
                    if (line->line().y2() == sceneRect().height()) {
                    removeItem(line);
                }
            }
        }
    }
    setSceneRect(0, 0, sceneRect().width(), sceneRect().height() - m_cellSize.height());
    updateScene();
    update();
}
