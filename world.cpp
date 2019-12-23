#include <QGraphicsScene>
#include "world.h"
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QDebug>
#include <QList>
#include <QtConcurrent/QtConcurrent>
//--------------------------------------------------------------------


QList<QString> contToRemove;

World::World(unsigned widgetWidth, unsigned widgetHeight, unsigned worldWidth, unsigned worldHeight, QMutex* mut)
{
    _mutex = mut;
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
    QDebug(QtMsgType::QtInfoMsg) << "\n--------------------------------------\n";

//    QTime time, time1, fps, currT;

//    time1 = time = QTime::currentTime();

//    auto dT = std::abs(time.msecsTo(QTime::currentTime())); //250 - 220 = 30
//    auto dT1 = std::abs(time1.msecsTo(QTime::currentTime())); //250 - 220 = 30


//    while(true){
//        dT = std::abs(time.msecsTo(QTime::currentTime())); //250 - 220 = 30
//        dT1 = std::abs(time1.msecsTo(QTime::currentTime())); //250 - 220 = 30
//        if(dT > 20){
////            currT.start();
//            Redraw();
//            time = QTime::currentTime(); //220
//            QDebug(QtMsgType::QtInfoMsg) << "   INFO: DRAW FPS = " << currT.elapsed();
//        }
//        if(dT1 > 100){
////            fps.start();
//            Recalc();
////            QApplication::processEvents();
//            time1 = QTime::currentTime(); //220
////            QDebug(QtMsgType::QtInfoMsg) << "       INFO: CALC FPS = " << fps.elapsed();

//        }
//   }
    QEventLoop loop;

    QTimer t1,t2;

    connect(&t1, &QTimer::timeout, this, &World::Redraw, static_cast<Qt::ConnectionType>(/*Qt::UniqueConnection | */Qt::QueuedConnection));
    connect(&t2, &QTimer::timeout, this, &World::Recalc, static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::DirectConnection));
    t1.start(/*50*/50);
    t2.start(50);
    loop.exec();
//    int cc = 0;
//    while (_inter <= Maxiteration) {
//        //wait infiftini
//        if(cc == 0){
//            QApplication::processEvents();
//            cc++;
//        }

//    }

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
            bool findCoord = false;
            while(!findCoord) {
                unsigned x = X(_gen);
                unsigned y = Y(_gen);
                QString xStr = "coordX"+QString::number(x)+"coordY"+QString::number(y);
                if(_map->find(xStr) == _map->end()){
                    auto cont = new Container(x,y, this, _mutex);
                    connect(cont, &Container::RepaintObj, this, &World::RepaintObj, Qt::QueuedConnection);
                    connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
                    connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
                    connect(cont, &Container::GenerateClone, this, &World::CloneObject);
                    connect(cont, &Container::DeleteContainer, this, &World::DeleteContainer, Qt::QueuedConnection);
                    cont->_scene = _scenePtr;

                    _map->insert(xStr, cont);
                    Hive* newHive = cont->AddHive();
                    for(unsigned j = 0; j < _BEE_AMOUNT; j++){
                        cont->AddBee(newHive);
                    }
                    findCoord = true;
                }
                else{
                        auto cont = *_map->find(xStr);
                    if(!cont->CheckFromHive()) {
                        Hive* newHive = cont->AddHive();
                        for(unsigned j = 0; j < _BEE_AMOUNT; j++){
                            cont->AddBee(newHive);
                        }
                        findCoord = true;
                    }
                }
            
            }
        }

        for(unsigned i = 0; i < _FLOWERS_AMOUNT; i++){
            unsigned x = X(_gen);
            unsigned y = Y(_gen);
            QString xStr = "coordX"+QString::number(x)+"coordY"+QString::number(y);
            if(_map->find(xStr) == _map->end()){
                auto cont = new Container(x,y, this, _mutex);
                connect(cont, &Container::RepaintObj, this, &World::RepaintObj, Qt::QueuedConnection);
                connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
                connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
                connect(cont, &Container::GenerateClone, this, &World::CloneObject);
                connect(cont, &Container::DeleteContainer, this, &World::DeleteContainer, Qt::QueuedConnection);
                cont->_scene = _scenePtr;

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
//    _scenePtr->clear();
    auto i = _map->begin();
    QDebug(QtMsgType::QtInfoMsg) << "------------------------ SSSSSstartDrawContainerSSSSS";

    while(i != _map->end()){
//    for(auto obj : *_map){
      _mutex->lock();
      QDebug(QtMsgType::QtDebugMsg) << "-----LOCK(REDRAW)-----";
            (*i)->RedrawObject(this->thread()->currentThread());
            i++;
        QDebug(QtMsgType::QtDebugMsg) << "-----UNLOCK(REDRAW)-----";
      _mutex->unlock();
//        QApplication::processEvents();
    }
    QDebug(QtMsgType::QtInfoMsg) << "------------------------ EEEEEendDrawContainerSSSSS";
}

void World::Recalc()
{
    _inter++;
    auto i = _map->begin();
    QTime currT;

//    auto Draw = [](Container* cont, QThread* workThread){
//        cont->RedrawObject(workThread);
//    };

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
//        _mutex->lock();
        (*i)->Recalc();
//        _mutex->unlock();
//        currT.start();
//        auto thread = this->thread();
//        QtConcurrent::run(Draw,*i, thread);
//        (*i)->RedrawObject();
//        QDebug(QtMsgType::QtInfoMsg) << "   INFO: DRAW FPS = " << currT.elapsed() << "\n----";
//        this->thread()->usleep(15000);

//        QDebug(QtMsgType::QtInfoMsg) << "       INFO: CALC FPS = " << currT.elapsed();
        i++;

    }
    int cnt = 0;
    auto delit = contToRemove.begin();
    while(delit != contToRemove.end()){
        cnt++;
        auto delitIt = _map->find(*delit);
        if((*delitIt)->_objArr.size() > 0){
//            for(auto el : (*delitIt)->_objArr){
//                if(dynamic_cast<Bee*>(el)){
//                        QDebug(QtMsgType::QtInfoMsg) << "\n-------------------\nDELETE BEE\n-------------------\n";
//                        auto smth = dynamic_cast<Bee*>(el);
//                        qDebug() << "lol";
//                }
//                else{
//                    if(dynamic_cast<Flower*>(el)){
//                            QDebug(QtMsgType::QtInfoMsg) << "\n-------------------\nDELETE FLOWER\n-------------------\n";
//                            auto smth = dynamic_cast<Flower*>(el);
//                            qDebug() << "lol";
//                    }
//                    else{
//                        if(dynamic_cast<Hive*>(el)){
//                                QDebug(QtMsgType::QtInfoMsg) << "\n-------------------\nDELETE HIVE\n-------------------\n";
//                                auto smth = dynamic_cast<Hive*>(el);
//                                qDebug() << "lol";
//                        }
//                        else{
//                            QDebug(QtMsgType::QtFatalMsg) << "EEEEEEEEERRRRRRRRRRRROOOOOOOOOOOOOOORRRRRRRRRRRRRRRRR   INFO: UNKNOWN DRAW! Type : " << typeid(el).name();
//                        }
//                    }
//                }
//            }
//            QDebug(QtMsgType::QtCriticalMsg) << "EDLDELDLELLELLEELLDLELLELDLELDLELLDLE";
            delit = contToRemove.erase(delit);
        }
        else{
            _map->erase(delitIt);
            delit = contToRemove.erase(delit);
            _mutex->lock();
            QDebug(QtMsgType::QtDebugMsg) << "-----LOCK(RECALC)-----";
            delete *delitIt;
            *delitIt = NULL;
            QDebug(QtMsgType::QtDebugMsg) << "-----UNLOCK(RECALC)-----";
            _mutex->unlock();

        }
    }
    QDebug(QtMsgType::QtDebugMsg) << "Maybe deleted " << cnt << " containers!";
    contToRemove.clear();

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
                    auto cont = new Container(newX,newY, this, _mutex);
                    connect(cont, &Container::RepaintObj, this, &World::RepaintObj);
                    connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
                    connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
                    connect(cont, &Container::GenerateClone, this, &World::CloneObject);
                    connect(cont, &Container::DeleteContainer, this, &World::DeleteContainer, Qt::QueuedConnection);
                    cont->_scene = _scenePtr;

                    _map->insert(coordStr, cont);
                    if(contPtr->RemoveObject(ptr)){
                        contToRemove.push_back(contPtr->GetCoordinates());
//                        _map->remove(contPtr->coordStr);
                    }
                    cont->AddObject(ptr);
                }
                else{
                    auto cont = *_map->find(coordStr);
                    if(contPtr->RemoveObject(ptr)){
                        contToRemove.push_back(contPtr->GetCoordinates());
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
        int cntTry = 0;
        while(!findNewCoordinates && cntTry <=15){
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
            auto cont = new Container(x,y, this, _mutex);
            connect(cont, &Container::RepaintObj, this, &World::RepaintObj, Qt::QueuedConnection);
            connect(cont, &Container::ObjectWantToMove, this, &World::MoveObject);
            connect(cont, &Container::BeeCollect, this, &World::BeeCollectFromFlower);
            connect(cont, &Container::GenerateClone, this, &World::CloneObject);
            connect(cont, &Container::DeleteContainer, this, &World::DeleteContainer, Qt::QueuedConnection);
            cont->_scene = _scenePtr;
            _mutex->lock();
            _map->insert(xStr, cont);
            Flower* newFlower = new Flower(x,y,this);
            cont->AddObject(newFlower);
            _mutex->unlock();
        }
        else{
            auto cont = *_map->find(xStr);
            auto flowers = cont->GetFlowers();
            if(!cont->CheckFromHive() && static_cast<unsigned>(flowers.size()) <= _MAX_AMOUNT_OF_FLOWERS_IN_COORDINATE){
                Flower* newFlower = new Flower(x,y,this);
                _mutex->lock();
                cont->AddObject(newFlower);
                _mutex->unlock();
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
//    _mutex->lock();
//    QDebug(QtMsgType::QtDebugMsg) << "-----LOCK(DELETE_CONTAINER)-----";
    if(_map->find(coord) != _map->end()){
        Container* cont = *_map->find(coord);
        int cnt = _map->remove(coord);
        if(cnt > 1 || cnt == 0){
            QDebug(QtMsgType::QtFatalMsg) << "FATAL: Remove NOT ONE container!";
        }

        delete cont;
        cont = NULL;
        QDebug(QtMsgType::QtWarningMsg) << "WRN: Delete container!";
    }
//    QDebug(QtMsgType::QtDebugMsg) << "-----UNLOCK(DELETE_CONTAINER)-----";
//    _mutex->unlock();
}


