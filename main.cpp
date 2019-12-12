#include "mainwindow.h"

#include <QApplication>
#include "world.h"


World* BigBang(){
    World *world = new World();
    return  world;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    World* world = nullptr;
    world = new World();
    w.GetWorldRef(world);
    world->Run();
//    w.StartAlgorithm();
//    w.GetWorldRef(worldPtr);
//    w.StartAlgorithm();
    w.show();
    return a.exec();
}

