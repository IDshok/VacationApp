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
#include <QSettings>
#include <QStandardPaths>
#include <QSqlRecord>

VacationTable::VacationTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VacationTable)
{
    ui->setupUi(this);
    setWindowTitle("График отпусков на календарный год");
    bool connected = connectToDatabase();
    if (!connected) {
        QMessageBox::critical(this, "Ошибка", "Отсутствует подключение к базе данных. Приложение будет закрыто!");
        QApplication::quit();
    }

    cellSize = {124, 25};
    int tableHeight = cellSize.height();
    int tableWidth = cellSize.width();

    QSqlQuery query;
    query.prepare(QString("SELECT (SELECT COUNT(id) FROM employees) AS employee_count, (SELECT COUNT(month) FROM norms) AS month_count"));
    if(query.exec())
    {
        QString employee_count = query.record().fieldName(0);
        QString month_count = query.record().fieldName(1);
        if(!query.first())
        {
            qDebug() << "Ошибка получения данных для прорисовки таблицы: " + query.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Ошибка получения данных для прорисовки таблицы");
            return;
        }
        tableHeight += cellSize.height() * (query.value(employee_count).toInt());
        tableWidth += cellSize.width() * (query.value(month_count).toInt());
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

bool VacationTable::connectToDatabase()
{
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/settings.ini", QSettings::IniFormat);

    QString host = settings.value("database/host", "localhost").toString();
    QString dbName = settings.value("database/name", "postgres").toString();
    QString userName = settings.value("database/user", "postgres").toString();
    QString password = settings.value("database/password", "1234").toString();

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return false;
    }
    qDebug() << "Подключение к базе данных успешно";
    return true;
}

void VacationTable::drawHeaders()
{  
    QSqlQuery query;
    query.prepare("SELECT n.month, n.norm, COUNT(DISTINCT e.id) AS employee_count, n.month_number FROM norms n LEFT JOIN DateOfVacation dov ON EXTRACT(MONTH FROM dov.vacation_start) = n.month_number OR EXTRACT(MONTH FROM dov.vacation_finish) = n.month_number LEFT JOIN employees e ON dov.employee_id = e.id GROUP BY n.month ORDER BY n.month_number;");
    if(query.exec())
    {
        int x = cellSize.width();
        if(!query.first())
        {
            qDebug() << "Ошибка получения данных о нормативах из базы данных: " + query.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Ошибка получения данных о нормативах из базы данных");
            return;
        }
        do
        {
            int norm = query.value("norm").toInt();
            int employeeCount = query.value("employee_count").toInt();

            HorizontalHeaderItem *monthRect = new HorizontalHeaderItem(x, 0, cellSize.width(), cellSize.height());
            monthRect->updateColor(employeeCount, norm);
            monthRect->setText(query.value("month").toString());
            monthRect->setData(0, norm);
            monthRect->setData(1, query.value("month_number"));
            scene->addItem(monthRect);
            x += cellSize.width();
        } while(query.next());
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
        if(!query.first())
        {
            qDebug() << "Ошибка получения данных о сотрудниках из базы данных: " + query.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Ошибка получения данных о сотрудниках из базы данных");
            return;
        }
        do {
            QGraphicsTextItem *employeeText = scene->addText(QString("%1. %2").arg(query.value("id").toString(), query.value("fio").toString()));
            employeeText->setPos(5, y);
            y += cellSize.height();
        } while(query.next());
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
        if(!query.first())
        {
            qDebug() << "Ошибка получения данных об отпусках из базы данных: " + query.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Ошибка получения данных об отпусках из базы данных");
            return;
        }
        do
        {
            int vacationId = query.value("dateofvacation.id").toInt();
            int employeeId = query.value("employees.id").toInt();
            QDate startDate = query.value("vacation_start").toDate();
            QDate finishDate = query.value("vacation_finish").toDate();
            // Рассчитываем продолжительность отпуска
            int duration = startDate.daysTo(finishDate) + 1;//?
            int startMonth = startDate.month() - 1;
            int startDay = startDate.day() - 1;
            qreal x = startMonth * cellSize.width() + startDay * ((qreal)cellSize.width()/startDate.daysInMonth()) + xOffset;
            qreal y = (employeeId-1) * cellSize.height() + yOffset;
            // qreal controlWidth = (duration) * (qreal)cellSize.width() / startDate.daysInMonth();//?
            // if (finishDate.month() != startDate.month()) {
            //     // Если отпуск продолжается в следующий месяц, добавляем к ширине часть следующего месяца
            //     controlWidth += finishDate.day() * (qreal)cellSize.width() / finishDate.daysInMonth();
            // }
            qreal controlWidth = duration*((qreal)cellSize.width())/((qreal)(startDate.daysInMonth() + finishDate.daysInMonth())/2);
            VacationRectItem *vacationRect = new VacationRectItem(x, y, controlWidth,  cellSize.height());
            vacationRect->setText(QString::number(duration));
            vacationRect->setVacationId(vacationId);
            vacationRect->setEmployeeId(employeeId);
            vacationRect->setStartDate(startDate);
            vacationRect->setFinishDate(finishDate);
            vacationRect->setToolTip(QString("Начало отпуска: %1 \nОкончание отпуска: %2").arg(vacationRect->startDate().toString("dd.MM.yyyy"), vacationRect->finishDate().toString("dd.MM.yyyy")));
            scene->addItem(vacationRect);
        } while (query.next());
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса: " + query.lastError().text());
        return;
    }
}

void VacationTable::addNewEmployee()
{
    QScopedPointer<EmployeeAddDialog> dialog(new EmployeeAddDialog());
    if(dialog->exec() == QDialog::Accepted)
    {
        scene->addRow();
        QGraphicsTextItem *employeeText = scene->addText(QString::number(dialog->employeeId()) +". " + dialog->formatName());
        employeeText->setPos(5, cellSize.height()*dialog->employeeId());
    }
}
