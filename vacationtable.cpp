#include "vacationrectitem.h"
#include "horizontalheaderitem.h"
#include "employeeadddialog.h"
#include "vacationtable.h"
#include "./ui_vacationtable.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>
#include <QDate>
#include <QVariant>
#include <QMouseEvent>
#include <QMessageBox>
#include <QToolBar>

VacationTable::VacationTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VacationTable)
{
    ui->setupUi(this);
    setWindowTitle("График отпусков на календарный год");
    connectToDatabase();

    cellSize = {124, 25};
    int tableHeight = cellSize.height();
    int tableWidth = cellSize.width();

    QSqlQuery query;
    query.prepare(QString("SELECT (SELECT COUNT(*) FROM employees) AS employee_count, (SELECT COUNT(*) FROM norms) AS norm_count"));
    if(query.exec())
    {
        query.first();
        tableHeight += cellSize.height() * (query.value(0).toInt());
        tableWidth += cellSize.width() * (query.value(1).toInt());
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса: " + query.lastError().text());
        return;
    }

    auto toolbar = new QToolBar();
    QAction *saveAct = new QAction("Добавить сотрудника", toolbar);
    toolbar->addAction(saveAct);
    toolbar->setStyleSheet("QToolBar {border: 1px solid black }");

    auto layout = new QVBoxLayout();
    layout->setMenuBar(toolbar);

    // Создаем сцену
    scene = new VacationScene(this);
    scene->setCellSize(cellSize);
    scene->setGridPen(QPen(Qt::black,1));
    // Создаем виджет для отображения сцены
    view = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, tableWidth, tableHeight); // Размер виджета
    // Рисуем таблицу
    scene->drawTable(tableWidth, tableHeight);
    drawHeaders();
    drawVacations();

    layout->addWidget(view);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    connect(saveAct, &QAction::triggered, this, &VacationTable::addNewEmployee);
}

VacationTable::~VacationTable()
{
    delete ui;
}

void VacationTable::connectToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("1234");

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return;
    }
    qDebug() << "Подключение к базе данных успешно";
}

void VacationTable::drawHeaders()
{  
    QSqlQuery query;
    query.prepare("SELECT n.month, n.norm, COUNT(DISTINCT e.id), n.month_number AS employee_count FROM norms n LEFT JOIN DateOfVacation dov ON EXTRACT(MONTH FROM dov.vacation_start) = n.month_number OR EXTRACT(MONTH FROM dov.vacation_finish) = n.month_number LEFT JOIN employees e ON dov.employee_id = e.id GROUP BY n.month ORDER BY n.month_number;");
    if(query.exec())
    {
        int x = cellSize.width();
        for(int i = 0; query.next(); i++)
        {
            int norm = query.value(1).toInt();
            int employeeCount = query.value(2).toInt();

            HorizontalHeaderItem *monthRect = new HorizontalHeaderItem(x, 0, cellSize.width(), cellSize.height());
            monthRect->updateColor(employeeCount, norm);
            monthRect->setText(query.value(0).toString());
            monthRect->setData(0, norm);
            monthRect->setData(1, query.value(3));
            scene->addItem(monthRect);
            x += cellSize.width();
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса: " + query.lastError().text());
        return;
    }


    query.prepare("SELECT id, concat(surname, ' ', substr(name, 1, 1), '. ', substr(patronymic, 1, 1), '.') AS fio FROM employees;");
    if(query.exec())
    {
        int y = cellSize.height();
        while(query.next()) {
            QGraphicsTextItem *employeeText = scene->addText(query.value(1).toString());
            employeeText->setPos(5, y);
            y += cellSize.height();
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса: " + query.lastError().text());
        return;
    }
}

void VacationTable::drawVacations()
{
    QSqlQuery query;
    query.prepare("SELECT e.id, d.vacation_start, d.vacation_finish, d.id FROM employees e JOIN dateofvacation d ON e.id = d.employee_id");
    if(query.exec())
    {
        int xOffset = cellSize.width();
        int yOffset = cellSize.height();
        //QMap<int, QString> employeeFios; // Мапа для хранения ФИО сотрудников e.surname, e.patronymic, e.name,
        while (query.next())
        {
            int vacationId = query.value(3).toInt();
            int employeeId = query.value(0).toInt();
            QDate startDate = query.value(1).toDate();
            QDate finishDate = query.value(2).toDate();
            // Рассчитываем продолжительность отпуска
            int duration = startDate.daysTo(finishDate); // duration > 0, если finishDate > startDate
            int startMonth = startDate.month() - 1;
            int startDay = startDate.day() - 1;
            // int monthDifference = abs(finishDate.daysInMonth() - startDate.daysInMonth());
            qreal x = startMonth * cellSize.width() + startDay * ((qreal)cellSize.width()/startDate.daysInMonth()) + xOffset;
            qreal y = (employeeId-1) * cellSize.height() + yOffset;
            qreal controlWidth = (duration-1) * (qreal)cellSize.width() / startDate.daysInMonth();
            if (finishDate.month() != startDate.month()) {
                // Если отпуск продолжается в следующий месяц, добавляем к ширине часть следующего месяца
                controlWidth += finishDate.day() * (qreal)cellSize.width() / finishDate.daysInMonth();
            }
            //qreal controlWidth = duration*((qreal)cellSize.width())/((qreal)(startDate.daysInMonth() + finishDate.daysInMonth())/2);
            VacationRectItem *vacationRect = new VacationRectItem(x, y, controlWidth,  cellSize.height());
            vacationRect->setText(QString::number(duration));
            vacationRect->setVacationId(vacationId);
            vacationRect->setEmployeeId(employeeId);
            vacationRect->setStartDate(startDate);
            vacationRect->setFinishDate(finishDate);
            vacationRect->setToolTip(QString("Начало отпуска: %1 \nОкончание отпуска: %2").arg(vacationRect->startDate().toString("dd.MM.yyyy")).arg(vacationRect->finishDate().toString("dd.MM.yyyy")));
            scene->addItem(vacationRect);
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса: " + query.lastError().text());
        return;
    }
}

void VacationTable::addNewEmployee()
{
    EmployeeAddDialog *dialog = new EmployeeAddDialog();
    if(dialog->exec() == QDialog::Accepted)
    {
        scene->addRow();
        QGraphicsTextItem *employeeText = scene->addText(dialog->formatName());
        employeeText->setPos(5, cellSize.height()*dialog->employeeId());
    }
}
