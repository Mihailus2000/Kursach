#include "mainwindow.h"

#include <QApplication>
#include "world.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    unsigned widgetWidth = 1000;
    unsigned widgetHeight = 600;

    unsigned worldWidth = 10;
    unsigned worldHeight = 5;

    MainWindow w(nullptr,widgetWidth,widgetHeight, &a);
    World* world = nullptr;
    world = new World(widgetWidth, widgetHeight, worldWidth, worldHeight);
    w.GetWorldRef(world);
    world->Run();
//    w.StartAlgorithm();
//    w.GetWorldRef(worldPtr);
//    w.StartAlgorithm();
    w.show();
    return a.exec();
}

