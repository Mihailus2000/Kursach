#include "world.h"
#include <random>
#include <QTimer>
//--------------------------------------------------------------------

World::World()
{
    _width = 1000;
    _height = 600;

    _map = new TMap;
//    connect(_map, &TMap::value, this, &World::RepaintObj);
}

void World::Run()
{
    if(_map->size() == 0){
        AddLife();
    }
    QTimer *timer = new QTimer();


    connect(timer, &QTimer::timeout, this, &World::Redraw);

    timer->start(40);

//    timer(Redraw(),40);
//    t = curTime(); // 100
//    while(true){
//        dt = curTume()-t; //250 - 220 = 30
//        if(dt >40){
//            Redraw();
//            t = curTime(); //220
//        }

//        if(dt>30){

//        }
//   }


}

//void World::RepaintObj(QGraphicsItem *item)
//{
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Signal catched!";
//}

void World::AddLife()
{
        std::mt19937 gen(time(nullptr));
        std::uniform_int_distribution<unsigned> X(0, _width);
        std::uniform_int_distribution<unsigned> Y(0, _height);

        for(int i = 0; i < 30; i++){
            unsigned x = X(gen);
            unsigned y = Y(gen);
            QString xStr = "coordX"+QString::number(x)+"coordY"+QString::number(y);
            if(_map->find(xStr) == _map->end()){
                auto cont = new Container(x,y);
                connect(cont, &Container::RepaintObj, this, &World::RepaintObj);
                connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
                _map->insert(xStr, cont);
                cont->AddFlower();
            }
            else{
                auto cont = *_map->find(xStr);
                cont->AddFlower();
            }

        }

        for(int i = 0; i < 6; i++){
            unsigned x = X(gen);
            unsigned y = Y(gen);
            QString xStr = "coordX"+QString::number(x)+"coordY"+QString::number(y);
            if(_map->find(xStr) == _map->end()){
                auto cont = new Container(x,y);
                connect(cont, &Container::RepaintObj, this, &World::RepaintObj);
                _map->insert(xStr, cont);
                Hive* newHive = cont->AddHive();
                for(int j = 0; j < 5; j++){
                    cont->AddBee(newHive);
                }
            }
            else{
                auto cont = *_map->find(xStr);
                Hive* newHive = cont->AddHive();
                for(int j = 0; j < 5; j++){
                    cont->AddBee(newHive);
                }
            }
        }


   // Redraw();
}

void World::Redraw()
{
    for(auto obj : *_map){
        obj->RedrawObject();
    }
}

void World::Recalc()
{
    for(auto obj : *_map){
//        obj->();
    }
}

void World::MoveObject(float dx, float dy, IObjects *ptr)
{

}

