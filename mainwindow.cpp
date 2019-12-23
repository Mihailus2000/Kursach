#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>


MainWindow::MainWindow(QWidget *parent, unsigned width, unsigned height, QApplication *app)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _app(app), _widgetWidth(width), _widgetHeigth(height)
{
    ui->setupUi(this);
    _scene = new QGraphicsScene;
    this->showMaximized();
    _scene->setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);
    _scene->setSortCacheEnabled(true);


    ui->graphicsView->setScene(_scene);
//    ui->graphicsView->setOptimizationFlags(QGraphicsView::OptimizationFlag::DontSavePainterState);
//    ui->graphicsView->setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::NoViewportUpdate);

//    _scene->setSceneRect(this->rect());
//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);                // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключаем скроллбар по горизонтали

    ui->graphicsView->setGeometry(ui->centralwidget->rect());

    _scene->setSceneRect(ui->graphicsView->rect());
    ui->graphicsView->setInteractive(false);
    QColor sceneColor;
    sceneColor.setRgb(213,255,201);
    _scene->addRect(_rect->boundingRect(),QPen(Qt::GlobalColor::green),QBrush(sceneColor));
    ui->graphicsView->show();

//    RepaintScene(_rect);
    QApplication::processEvents();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
    delete _refToWorld;
    delete _rect;

}

void MainWindow::GetWorldRef(World *worldPtr)
{
    _refToWorld = worldPtr;
    _refToWorld->_scenePtr = _scene;

}

void MainWindow::RepaintScene(IObjects *item, QThread* threadToSleep)
{
//    _scene->addEllipse(rand()%1000,rand()%500,10,10, QPen(Qt::gray));

    if(dynamic_cast<Bee*>(item)){
//                QDebug(QtMsgType::QtInfoMsg) << "INFO: Bee DRAW!";
    }
    else{
        if(dynamic_cast<Flower*>(item)){
//                    QDebug(QtMsgType::QtInfoMsg) << "INFO: Flower DRAW!";
        }
        else{
            if(dynamic_cast<Hive*>(item)){
//                        QDebug(QtMsgType::QtInfoMsg) << "INFO: Hive DRAW!";
            }
            else{
//                QDebug(QtMsgType::QtFatalMsg) << "INFO: UNKNOWN DRAW! Type : " << typeid(item).name();
            }
        }
    }
//    _mutex->lock();

    if(dynamic_cast<QGraphicsItem*>(item)) {
        if(thread()->currentThread()->currentThreadId() != threadToSleep->currentThreadId())
//            QDebug(QtMsgType::QtCriticalMsg) << "QTFTFTTFT";
//        if(item->scene() == _scene) {
////            _workThread->blockSignals(true);
//            item->scene()->removeItem(item);
//            _mutex->lock();
//            QDebug(QtMsgType::QtDebugMsg) << "-----LOCK(DRAW1)-----";
//            _scene->addItem(item);
//            QDebug(QtMsgType::QtDebugMsg) << "-----UNLOCK(DRAW1)-----";
//            _mutex->unlock();

////            threadToSleep->blockSignals(false);
//        }
//        else{
//            threadToSleep->blockSignals(true);
//            item->scene()->removeItem(item);

            _mutex->lock();
//            QDebug(QtMsgType::QtDebugMsg) << "-----LOCK(DRAW2)-----";

            auto bee = dynamic_cast<Bee*>(item);
            auto hive = dynamic_cast<Hive*>(item);
            auto flower = dynamic_cast<Flower*>(item);

            if(bee!=nullptr || hive!=nullptr || flower != nullptr){
//                if(item != nullptr){
                if(item->scene() == _scene)
                    _scene->removeItem(item);
                _scene->addItem(item);
            }
            else
//                QDebug(QtMsgType::QtWarningMsg) << "-----DRAW_FAIL-----";
            _mutex->unlock();
//            QDebug(QtMsgType::QtDebugMsg) << "-----UNLOCK(DRAW2)-----";

//            threadToSleep->blockSignals(false);
//        }
//            QApplication::processEvents();
        _scene->update();
//        ui->graphicsView->update();
    }


}

void MainWindow::StartAlgorithm()
{
    _mutex = new QMutex();
    _refToWorld->_mutex = _mutex;
    _workThread = new QThread;

    _refToWorld->moveToThread(_workThread);
    connect(_refToWorld,&World::RepaintObj, this, &MainWindow::RepaintScene, Qt::QueuedConnection);
    connect(_workThread,&QThread::started, _refToWorld, &World::Run);
    connect(_workThread,&QThread::finished, _workThread,&QThread::deleteLater);
    _workThread->start();
//    _algorithm = new QThread();
//    _refToWorld->moveToThread(_algorithm);

//    connect(_algorithm, &QThread::started, _refToWorld, &World::Run);
//    _algorithm->start();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
//    QDebug(QtMsgType::QtInfoMsg) << "RESIZE";/*
//    QDebug(QtMsgType::QtInfoMsg) << "---\nvuew: " << ui->graphicsView->rect();
//    QDebug(QtMsgType::QtInfoMsg) << "widgeeet: " << ui->centralwidget->rect();
//    QDebug(QtMsgType::QtInfoMsg) << "seceneee: " << _scene->sceneRect();*/
    _scene->setSceneRect(0,0,_widgetWidth, _widgetHeigth);
    if(_rect == nullptr){
        _rect = new QGraphicsRectItem(_scene->sceneRect());
    }
//    QDebug(QtMsgType::QtInfoMsg) << "\n---";
//    _scene->update();
//    QApplication::processEvents(/*QEventLoop::ProcessEventsFlag::WaitForMoreEvents*/);
}

void MainWindow::on_actionStop_triggered()
{
    _app->closeAllWindows();
    delete this;
    _app->exit();
    QApplication::processEvents();
}
