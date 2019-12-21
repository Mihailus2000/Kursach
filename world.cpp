#include "world.h"
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QDebug>
#include <QList>
//--------------------------------------------------------------------


QList<QString> contToRemove;

World::World(unsigned widgetWidth, unsigned widgetHeight, unsigned worldWidth, unsigned worldHeight)
{
    contAmount = 0;
    _width = worldWidth;
    _height = worldHeight;

    _scaleX = widgetWidth / worldWidth;
    _scaleY = widgetHeight / worldHeight;

    _map = new TMap;
    std::random_device device;
//   std::time(QTimer);
    _gen.seed(QTime::currentTime().msecsSinceStartOfDay());
    //    connect(_map, &TMap::value, this, &World::RepaintObj);
}

World::~World()
{
//    qDeleteAll(*_map);
}


void World::Run()
{
    if(_map->size() == 0){
        AddLife();
    }
    qDebug() << "\n--------------------------------------\n";

//    QThread *th1 = new QThread();
//    QThread *th2 = new QThread();

//    QTimer timer1,timer2;

//    timer2.moveToThread(th2);
//    timer1.moveToThread(th1);

//    connect(&timer1, &QTimer::timeout, this, &World::Redraw);
//    connect(&timer2, &QTimer::timeout, this, &World::Recalc);

//    timer1.start(20);
//    timer2.start(20);

//    th1->start();
//    th2->start();
    QTime time, time1, fps, currT;
//    QTimer FPS;
    time1 = time = QTime::currentTime();


    auto dT = std::abs(time.msecsTo(QTime::currentTime())); //250 - 220 = 30
    auto dT1 = std::abs(time1.msecsTo(QTime::currentTime())); //250 - 220 = 30

//    timer(Redraw(),40);
//    t = curTime(); // 100
    while(true){
        dT = std::abs(time.msecsTo(QTime::currentTime())); //250 - 220 = 30
        dT1 = std::abs(time1.msecsTo(QTime::currentTime())); //250 - 220 = 30
        if(dT > 10){
//            currT.start();
            Redraw();
            time = QTime::currentTime(); //220
//            QDebug(QtMsgType::QtInfoMsg) << "   INFO: DRAW FPS = " << currT.elapsed();
        }
        if(dT1 > 100){
            fps.start();
            Recalc();
            time1 = QTime::currentTime(); //220
            QDebug(QtMsgType::QtInfoMsg) << "       INFO: CALC FPS = " << fps.elapsed();

        }
   }


}

//void World::RepaintObj(QGraphicsItem *item)
//{
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Signal catched!";
//}

void World::AddLife()
{

        std::uniform_int_distribution<unsigned> X(0, _width-1);
        std::uniform_int_distribution<unsigned> Y(0, _height-1);

        for(unsigned i = 0; i < _HIVE_AMOUNT; i++) {
            unsigned x = X(_gen);
            unsigned y = Y(_gen);
            QString xStr = "coordX"+QString::number(x)+"coordY"+QString::number(y);
            if(_map->find(xStr) == _map->end()){
                auto cont = new Container(x,y, this);
                connect(cont, &Container::RepaintObj, this, &World::RepaintObj);
                connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
                connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
                connect(cont, &Container::GenerateClone, this, &World::CloneObject);
                _map->insert(xStr, cont);
                Hive* newHive = cont->AddHive();
                for(unsigned j = 0; j < _BEE_AMOUNT; j++){
                    cont->AddBee(newHive);
                }
            }
            else{
                    auto cont = *_map->find(xStr);
                if(!cont->CheckFromHive()) {
                    Hive* newHive = cont->AddHive();
                    for(unsigned j = 0; j < _BEE_AMOUNT; j++){
                        cont->AddBee(newHive);
                    }
                }
            
            }
        }

        for(unsigned i = 0; i < _FLOWERS_AMOUNT; i++){
            unsigned x = X(_gen);
            unsigned y = Y(_gen);
            QString xStr = "coordX"+QString::number(x)+"coordY"+QString::number(y);
            if(_map->find(xStr) == _map->end()){
                auto cont = new Container(x,y, this);
                connect(cont, &Container::RepaintObj, this, &World::RepaintObj);
                connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
                connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
                connect(cont, &Container::GenerateClone, this, &World::CloneObject);
                _map->insert(xStr, cont);
                cont->AddFlower();
                _amountOfFlowers ++;
            }
            else{
                auto cont = *_map->find(xStr);
                if(!cont->CheckFromHive())
                    cont->AddFlower();
                _amountOfFlowers++;
            }

        }


   // Redraw();
}

void World::Redraw()
{
    auto i = _map->begin();
    QDebug(QtMsgType::QtInfoMsg) << "------------------------ SSSSSstartDrawContainerSSSSS";

    while(i != _map->end()){
//    for(auto obj : *_map){

        (*i)->RedrawObject();
        i++;
    }
    QDebug(QtMsgType::QtInfoMsg) << "------------------------ EEEEEendDrawContainerSSSSS";
}

void World::Recalc()
{
    auto i = _map->begin();
//    QTime currT;
    while(i != _map->end()){
//        if(dynamic_cast<Bee*>(*i)){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Bee CALC!";
//        }
//        if(dynamic_cast<Flower*>(*i)){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Flower CALC!";
//        }
//        if(dynamic_cast<Hive*>(*i)){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Hive CALC!";
//        }
//        currT.start();
        (*i)->Recalc();
//        QDebug(QtMsgType::QtInfoMsg) << "       INFO: CALC FPS = " << currT.elapsed();
        i++;

    }
//    auto delit = contToRemove.begin();
//    while(delit != contToRemove.end()){
//        auto delitIt = _map->find(*delit);
//        _map->erase(delitIt);
//        delit = contToRemove.erase(delit);
//    }
//    contToRemove.clear();

}



void World::MoveObject(float dx, float dy, IObjects *ptr, Container *contPtr)
{
    auto objY = ptr->GetY();
    auto objX = ptr->GetX();


    if(objX+dx > 0 && objY+dy > 0 && objX+dx < _scaleX*(_width-1) &&  objY+dy < _scaleY*(_height-1)) {

        unsigned newX = static_cast<unsigned>(std::floor(static_cast<double>(objX+dx)));
        unsigned newY = static_cast<unsigned>(std::floor(static_cast<double>(objY+dy)));

        if(( newX <= _width-1 && newX >= 0 )&&( newY <= _height-1 && newY >= 0)) {
            if(newX != contPtr->GetX() || newY != contPtr->GetY()) {
                QString coordStr = "coordX"+QString::number(newX) +"coordY"+QString::number(newY);

                if(_map->find(coordStr) == _map->end()){
                    auto cont = new Container(newX,newY, this);
                    connect(cont, &Container::RepaintObj, this, &World::RepaintObj);
                    connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
                    connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
                    connect(cont, &Container::GenerateClone, this, &World::CloneObject);
                    _map->insert(coordStr, cont);
                    if(contPtr->RemoveObject(ptr)){
//                        contToRemove.push_back(contPtr->coordStr);
//                        _map->remove(contPtr->coordStr);
                    }
                    cont->AddObject(ptr);
                }
                else{
                    auto cont = *_map->find(coordStr);
                    if(contPtr->RemoveObject(ptr)){
//                        contToRemove.push_back(contPtr->coordStr);
                    }
                    cont->AddObject(ptr);
                }
            }
            ptr->SetCoordinates(objX+dx,objY+dy);
        }
    }
}

void World::BeeCollectFromFlower(Bee *bee, Container *itsCont)
{
    auto flowers = itsCont->GetFlowers();
    if(flowers.size() != 0){
        for(auto flower : flowers){
            if(!bee->IfFull()){
                bee->AddNectar(flower->GiveNectar());
            }
            else
                break;
        }
    }
}

void World::CloneObject(IObjects *obj)
{
    auto clonedFlower = dynamic_cast<Flower*>(obj);
    if(clonedFlower) {
        std::uniform_real_distribution<float> stepRange(0.f, 1.f);
        float range = stepRange(_gen);
        std::uniform_real_distribution<float> step(-range,range);// отклоненение появления нового цветка
        auto x = clonedFlower->GetX();
        auto y = clonedFlower->GetY();
        bool findNewCoordinates = false;
        while(!findNewCoordinates){
            auto newX  = x + step(_gen);
            auto newY  = y + step(_gen);
            if(( floor(newX) <= _width-1 && floor(newX) >= 0 )&&( floor(newY) <= _height-1 && floor(newY) >= 0)) {
                x = newX;
                y = newY;
                findNewCoordinates = true;
            }
        }
        unsigned intX = std::floor(x);
        unsigned intY =  std::floor(y);
        QString xStr = "coordX"+QString::number(intX)+"coordY"+QString::number(intY);
        if(_map->find(xStr) == _map->end()){
            auto cont = new Container(x,y, this);
            connect(cont, &Container::RepaintObj, this, &World::RepaintObj);
            connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
            connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
            connect(cont, &Container::GenerateClone, this, &World::CloneObject);
            _map->insert(xStr, cont);
            Flower* newFlower = new Flower(x,y,this);
            cont->AddObject(newFlower);
        }
        else{
            auto cont = *_map->find(xStr);
            auto flowers = cont->GetFlowers();
            if(!cont->CheckFromHive() && static_cast<unsigned>(flowers.size()) <= _MAX_AMOUNT_OF_FLOWERS_IN_COORDINATE){
                Flower* newFlower = new Flower(x,y,this);
                cont->AddObject(newFlower);
            }
            else{
                // TODO
            }
        }
//        Flower* newFlower = new Flower(x,y,_myWorld);
//        connect(newFlower, &Flower::GenerateClone, this, &Container::GenerateClone);
//        connect(newFlower, &Flower::DeleteFlower, this, &Container::RemoveObject);
//        _objArr.append(newFlower);
//        _count++;
    }
}

void World::DeleteContainer(Container *cont)
{
    QString coord = cont->GetCoordinates();
    if(_map->find(coord) != _map->end()){
        Container* cont = *_map->find(coord);
        int cnt = _map->remove(coord);
        if(cnt > 1 || cnt == 0){
            QDebug(QtMsgType::QtFatalMsg) << "FATAL: Remove NOT ONE container!";
        }
        delete cont;
        cont = nullptr;
    }
}


