#include <QGraphicsScene>
#include "container.h"
#include <QTime>
//#include "bee.h"
#include "world.h"
#include <QApplication>

#include <qdebug.h>

void Container::BeeWanToCollect(Bee *bee)
{
    emit BeeCollect(bee, this);
}

void Container::WantToMove(float dx, float dy, IObjects *objPtr)
{
    emit ObjectWantToMove(dx,dy,objPtr, this);
}

void Container::GenerateNewBee(Bee *bee)
{
    AddObject(bee);
}

void Container::DeleteObject(IObjects *obj)
{
    auto deletingBee = dynamic_cast<Bee*>(obj);
    auto deletingFlower = dynamic_cast<Flower*>(obj);
    _mu->lock();
    if(deletingBee){
        Hive* beeHive = deletingBee->GetParent();
        beeHive->RemoveBeeFromMemory(deletingBee);
    }
    if(deletingFlower){
        QDebug(QtMsgType::QtWarningMsg) << "INFO: Deleting flower!";
    }
    _objArr.removeOne(obj);
    _count--;
    QDebug(QtMsgType::QtDebugMsg) << "-----LOCK(DELETE)-----";

//    if(obj->scene() == _scene)
//        _scene->removeItem(obj);
    delete obj;
    obj = NULL;
    QDebug(QtMsgType::QtDebugMsg) << "-----UNLOCK(DELETE)-----";
    _mu->unlock();

    if(_count == 0) {
        if(_objArr.size() == 0){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Delete container :" << _x << "," << _y;
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: left objects :" << _count << "\n\n";
            DeleteContainer(this);
        }
        else{
            QDebug(QtMsgType::QtFatalMsg) << "INFO: left objects :" << _count << " instead of 0!!\n\n";
//            return false;
        }
//        this->deleteLater();
//        QApplication::processEvents();
    }
//    else{
//        return false;
    //    }
}

QString Container::GetCoordinates()
{
    return _coordStr;
}

//void Container::GenerateClone(IObjects *obj)
//{
//    auto clonedFlower = dynamic_cast<Flower*>(obj);
//    if(clonedFlower) {
//        std::uniform_real_distribution<float> step(-0.99f,0.99f);// отклоненение появления нового цветка
//        auto x = clonedFlower->GetX();
//        auto y = clonedFlower->GetY();
//        x += step(_gen);
//        y += step(_gen);
//        Flower* newFlower = new Flower(x,y,_myWorld);
//        connect(newFlower, &Flower::GenerateClone, this, &Container::GenerateClone);
//        connect(newFlower, &Flower::DeleteFlower, this, &Container::RemoveObject);
//        _objArr.append(newFlower);
//        _count++;
//    }
//}

Container::Container(unsigned x, unsigned y, World* ptr, QMutex* mu) : _x(x), _y(y), _myWorld(ptr), _mu(mu) {
     _coordStr = "coordX"+QString::number(x) +"coordY"+QString::number(y);
     _gen.seed(rand());
}

Container::~Container()
{
//    qDeleteAll(_objArr);
    QDebug(QtMsgType::QtInfoMsg) << "INFO: Destructor of Container {";
    QDebug(QtMsgType::QtInfoMsg) << "INFO: CONTAIN :" << _objArr.size() << " elements\n}";

}

unsigned Container::GetX()
{
    return _x;
}

unsigned Container::GetY()
{
    return _y;
}

void Container::AddFlower() {

    std::uniform_real_distribution<float> step(0.f,0.99f);// диапазон шага
    auto newX = _x + step(_gen);
    auto newY = _y + step(_gen);

    Flower* newFlower = new Flower(newX, newY,_myWorld);
    connect(newFlower, &Flower::GenerateClone, this, &Container::GenerateClone);
    connect(newFlower, &Flower::DeleteFlower, this, &Container::DeleteObject);

    _objArr.append(newFlower);
    _count++;
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add flower   :  (" << newX << "x" << newY << ")";
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Now elements: :" << _count << "\n\n";


}

void Container::AddObject(IObjects *obj)
{
    _objArr.append(obj);
    if (dynamic_cast<Bee*>(obj)){
        auto bee = dynamic_cast<Bee*>(obj);
        connect(bee, &Bee::WantToMove, this, &Container::WantToMove);
        connect(bee, &Bee::ToCollect, this, &Container::BeeWanToCollect, Qt::DirectConnection);
        connect(bee, &Bee::DeleteBee, this, &Container::DeleteObject);
    }
    if(dynamic_cast<Flower*>(obj)){
        auto flower = dynamic_cast<Flower*>(obj);
        connect(flower, &Flower::GenerateClone, this, &Container::GenerateClone);
        connect(flower, &Flower::DeleteFlower, this, &Container::DeleteObject);
    }
//    if(dynamic_cast<Flower*>(obj))
//        QDebug(QtMsgType::QtInfoMsg) << "INFO: Add flower(object)! : " << obj->GetX() << "," << obj->GetY();
//    if(dynamic_cast<Hive*>(obj))
//        QDebug(QtMsgType::QtInfoMsg) << "INFO: Add hive(object)! : " << obj->GetX() << "," << obj->GetY();
//    if(dynamic_cast<Bee*>(obj))
//        QDebug(QtMsgType::QtInfoMsg) << "INFO: Add bee(object)! : " << obj->GetX() << "," << obj->GetY();
    _count++;
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add to :" << _x << "," << _y;
}

bool Container::RemoveObject(IObjects *obj)
{
    _mu->lock();
   int cnt = _objArr.removeAll(obj);
   _mu->unlock();
   if(cnt > 1){
       QDebug(QtMsgType::QtFatalMsg) << "Maybe deleted more then 1 element!";
   }
   QDebug(QtMsgType::QtInfoMsg) << "INFO: Remove " << cnt << " elements!";
//    connect(obj, &Bee::WantToMove, this, &Container::WantToMove);
    auto deletingFlower = dynamic_cast<Flower*>(obj);
    auto deletingHive = dynamic_cast<Hive*>(obj);
    auto deletingBee = dynamic_cast<Bee*>(obj);

//    if(deletingFlower){
//        QDebug(QtMsgType::QtInfoMsg) << "INFO: Delete flower! : " << obj->GetX() << "," << obj->GetY();
//        // TODO : Add here all disconnects!
//    }
    if(deletingHive){
        QDebug(QtMsgType::QtInfoMsg) << "INFO: REmove hive! : " << obj->GetX() << "," << obj->GetY();
        // TODO : Add here all disconnects!
    }
    if(deletingBee){
          QDebug(QtMsgType::QtInfoMsg) << "INFO: Remove bee! : " << obj->GetX() << "," << obj->GetY();
        disconnect(deletingBee, &Bee::WantToMove, this, &Container::WantToMove);
        disconnect(deletingBee, &Bee::ToCollect, this, &Container::BeeWanToCollect);
        disconnect(deletingBee, &Bee::DeleteBee, this, &Container::DeleteObject);

    }
    if(deletingFlower){
        QDebug(QtMsgType::QtCriticalMsg) << "INFO: REmove flower! : " << obj->GetX() << "," << obj->GetY();
        // TODO : Add here all disconnects!
    }
//    if(deletingFlower){
//        disconnect(deletingFlower, &Flower::GenerateClone, this, &Container::GenerateClone);
//        disconnect(deletingFlower, &Flower::DeleteFlower, this, &Container::DeleteObject);
//        delete deletingFlower;
//        deletingFlower = nullptr;
//    }
//      QDebug(QtMsgType::QtInfoMsg) << "INFO: delete from :" << _x << "," << _y;

    _count--;

    if(_count == 0) {
        if(_objArr.size() == 0){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Delete container :" << _x << "," << _y;
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: left objects :" << _count << "\n\n";
            return true;
        }
        else{
            QDebug(QtMsgType::QtFatalMsg) << "INFO: left objects :" << _count << " instead of 0!!\n\n";
            return false;
        }
//        this->deleteLater();
//        QApplication::processEvents();
    }
    else{
        return false;
    }
}

void Container::AddBee(Hive* parent)
{
    Bee* newBee = new Bee(/*_x+0.5f,_y+0.5f, */parent, _myWorld);
    connect(newBee, &Bee::WantToMove, this, &Container::WantToMove);
    connect(newBee, &Bee::ToCollect, this, &Container::BeeWanToCollect);
    connect(newBee, &Bee::DeleteBee, this, &Container::DeleteObject);

    _objArr.append(newBee);
    parent->AddBeeToMemory(newBee);
    ++_count;
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add bee   :  (" << _x+0.5f << "x" << _y+0.5f << ")";
}

Hive *Container::AddHive()
{
    Hive* newHive = new Hive(_x,_y, _myWorld);
    connect(newHive, &Hive::WantGenerateNewBee, this, &Container::GenerateNewBee);
    _objArr.append(newHive);
    ++_count;
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add hive to contaner: (" << _x << "x" << _y << ")";
    return newHive;
}



void Container::RedrawObject(QThread *workThread)
{
    auto i = _objArr.begin();
    QTime currT;
    int cnt = 0;
//    QDebug(QtMsgType::QtInfoMsg) << "------ startDrawContainer";
    while(i!=_objArr.end()){
//        for(auto obj : _objArr){
        if(dynamic_cast<QGraphicsItem*>(*i) != nullptr){
            if(dynamic_cast<Bee*>(*i)){
//                QDebug(QtMsgType::QtInfoMsg) << "INFO: Bee DRAW!";
            }
            else{
                if(dynamic_cast<Flower*>(*i)){
//                    QDebug(QtMsgType::QtInfoMsg) << "INFO: Flower DRAW!";
                }
                else{
                    if(dynamic_cast<Hive*>(*i)){
//                        QDebug(QtMsgType::QtInfoMsg) << "INFO: Hive DRAW!";
                    }
                    else{
                        QDebug(QtMsgType::QtFatalMsg) << "INFO: UNKNOWN DRAW! Type : " << typeid(*i).name();
                    }
                }
            }
//            currT.start();
            auto bee = dynamic_cast<Bee*>(*i);
            auto flower = dynamic_cast<Flower*>(*i);
            auto hive = dynamic_cast<Hive*>(*i);
            if(bee!=nullptr || hive!=nullptr || flower != nullptr){
                emit RepaintObj(*i,workThread);
            }
            else{
                QDebug(QtMsgType::QtFatalMsg) << "-----FALFALFAOFHIAD-----";
            }
//            QApplication::processEvents();
//            QDebug(QtMsgType::QtInfoMsg) << "   INFO: DRAW FPS = " << currT.elapsed() << "\n----";
//            i++;

        }
        else{
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: ???Succesfull redraw elements : " << cnt << "\n-----";
//            QDebug(QtMsgT/ype::QtFatalMsg) << "FATAL: Phantom object!";
        }
        ++i;
        cnt++;
//        QDebug(QtMsgType::QtInfoMsg) << "INFO: Succesfull redraw elements : " << cnt << "\n-----";
    }

}

void Container::Recalc()
{
//    auto i = _objArr.begin();
//    while(i != _objArr.end()){
//    _mu->lock();
//    QDebug(QtMsgType::QtDebugMsg) << "-----LOCKKKKKKKK-----";
    for(int i = 0; i < _objArr.size();){

//    for(auto obj = _objArr.begin(); obj != _objArr.end(); ++obj){
//        if(dynamic_cast<Bee*>(object) != nullptr){
//            auto bee = dynamic_cast<Bee*>(object);
//            bee->Work();
//            continue;
//        }
//        if(dynamic_cast<Hive*>(object) != nullptr){
//            auto hive = dynamic_cast<Hive*>(object);
//            hive->Work();
//            continue;
//        }
//        if(dynamic_cast<Flower*>(object) != nullptr){
//            auto flower = dynamic_cast<Flower*>(object);
//            flower->Work();
//            continue;
//        }
//        qDebug() << "Work...";
        _objArr.at(i)->Work();
//        QDebug(QtMsgType::QtDebugMsg) << "-----UNLOCKKKKKKKKK-----";
//        _mu->unlock();
//        qDebug() << "...Work end";

        i++;

    }
}

QSet<Flower *> Container::GetFlowers()
{
    QSet<Flower*> findedFlowers;
    for(int i = 0; i < _objArr.size(); i++){
        if(dynamic_cast<Flower*>(_objArr.at(i)) != nullptr){
            findedFlowers.insert(dynamic_cast<Flower*>(_objArr.at(i)));
        }
    }
    return findedFlowers;
}

bool Container::CheckFromHive()
{
    for(int i = 0; i < _objArr.size(); i++){
        if(dynamic_cast<Hive*>(_objArr.at(i)) != nullptr){
            return true;
        }
    }
    return false;
}
