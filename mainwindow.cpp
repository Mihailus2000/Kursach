#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>


MainWindow::MainWindow(QWidget *parent, unsigned width, unsigned height)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _widgetWidth(width), _widgetHeigth(height)
{
    ui->setupUi(this);
    _scene = new QGraphicsScene;
    this->showMaximized();
    ui->graphicsView->setScene(_scene);

//    _scene->setSceneRect(this->rect());
//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);                // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключаем скроллбар по горизонтали

    ui->graphicsView->setGeometry(ui->centralwidget->rect());

    _scene->setSceneRect(ui->graphicsView->rect());

    QColor sceneColor;
    sceneColor.setRgb(213,255,201);
    _scene->addRect(_rect->boundingRect(),QPen(Qt::GlobalColor::green),QBrush(sceneColor));
    ui->graphicsView->show();

    RepaintScene(_rect);
    QApplication::processEvents();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::GetWorldRef(World *worldPtr)
{
    _refToWorld = worldPtr;
    connect(_refToWorld,&World::RepaintObj, this, &MainWindow::RepaintScene);
}

void MainWindow::RepaintScene(QGraphicsItem * item)
{
//    _scene->addEllipse(rand()%1000,rand()%500,10,10, QPen(Qt::gray));

    if(item != nullptr) {
        if(item->scene() == _scene)
            item->scene()->removeItem(item);
        _scene->addItem(item);
    }
    _scene->update();
    QApplication::processEvents();
}

void MainWindow::StartAlgorithm()
{

//    _algorithm = new QThread();
//    _refToWorld->moveToThread(_algorithm);

//    connect(_algorithm, &QThread::started, _refToWorld, &World::Run);
//    _algorithm->start();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << "CHHAHHAHAH";
    qDebug() << "---\nvuew: " << ui->graphicsView->rect();
    qDebug() << "widgeeet: " << ui->centralwidget->rect();
    qDebug() << "seceneee: " << _scene->sceneRect();
    _scene->setSceneRect(0,0,_widgetWidth, _widgetHeigth);
    if(_rect == nullptr){
        _rect = new QGraphicsRectItem(_scene->sceneRect());
    }
    _scene->update();
//    QApplication::processEvents(/*QEventLoop::ProcessEventsFlag::WaitForMoreEvents*/);
}
