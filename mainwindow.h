#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "world.h"
#include <QThread>
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, unsigned width = 0 , unsigned height = 0, QApplication* app = nullptr);
    ~MainWindow();
    void GetWorldRef(World* worldPtr);
    void RepaintScene(IObjects * item, QThread *threadToSleep);
    void StartAlgorithm();


private:
    Ui::MainWindow *ui;
    QThread* _workThread;
    QGraphicsScene *_scene = nullptr;
    World* _refToWorld = nullptr;
    QMutex* _mutex;
//    QThread* _algorithm = nullptr;
    QGraphicsItem* _rect = nullptr;
    QApplication* _app;
    unsigned _widgetWidth, _widgetHeigth;
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void on_actionStop_triggered();
};
#endif // MAINWINDOW_H
