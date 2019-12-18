#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "world.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, unsigned width = 0 , unsigned height = 0);
    ~MainWindow();
    void GetWorldRef(World* worldPtr);
    void RepaintScene(QGraphicsItem * item);
    void StartAlgorithm();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *_scene = nullptr;
    World* _refToWorld = nullptr;
    QThread* _algorithm = nullptr;
    QGraphicsItem* _rect = nullptr;
    unsigned _widgetWidth, _widgetHeigth;
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};
#endif // MAINWINDOW_H
