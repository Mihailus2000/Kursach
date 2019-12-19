#include "container.h"

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

Container::Container(unsigned x, unsigned y) : _x(x), _y(y) {
     coordStr = "coordX"+QString::number(x) +"coordY"+QString::number(y);
     _gen.seed(rand());
}

unsigned Container::GetX()
{
    return _x;
}

unsigned Container::GetY()
{
    return _y;
}

void Container::AddFlower(World* worldPtr) {

    std::uniform_real_distribution<float> step(0.f,0.99f);// диапазон шага
    auto newX = _x + step(_gen);
    auto newY = _y + step(_gen);

    Flower* newFlower = new Flower(newX, newY, worldPtr);
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
    if(_objArr.removeOne(obj)){
//        connect(obj, &Bee::WantToMove, this, &Container::WantToMove);
        auto deletingFlower = dynamic_cast<Flower*>(obj);
        auto deletingHive = dynamic_cast<Hive*>(obj);
        auto deletingBee = dynamic_cast<Bee*>(obj);
//        if(deletingFlower){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Delete flower! : " << obj->GetX() << "," << obj->GetY();
//            // TODO : Add here all disconnects!
//        }
//        if(deletingHive){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Delete hive! : " << obj->GetX() << "," << obj->GetY();
//            // TODO : Add here all disconnects!
//        }
        if(deletingBee){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Delete bee! : " << obj->GetX() << "," << obj->GetY();
            disconnect(deletingBee, &Bee::WantToMove, this, &Container::WantToMove);
            disconnect(deletingBee, &Bee::ToCollect, this, &Container::BeeWanToCollect);
        }
//        QDebug(QtMsgType::QtInfoMsg) << "INFO: delete from :" << _x << "," << _y;

        _count--;
    }

    if(_count == 0){
        if(_objArr.size() == 0){
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: Delete container :" << _x << "," << _y;
//            QDebug(QtMsgType::QtInfoMsg) << "INFO: left objects :" << _count << "\n\n";
            return true;
        }
        else{
//            QDebug(QtMsgType::QtFatalMsg) << "INFO: left objects :" << _count << " instead of 0!!\n\n";
            return false;
        }
//        this->deleteLater();
//        QApplication::processEvents();
    }
    else{
        return false;
    }
}

void Container::AddBee(Hive* parent, World *worldPtr)
{
    Bee* newBee = new Bee(/*_x+0.5f,_y+0.5f, */parent, worldPtr);
    connect(newBee, &Bee::WantToMove, this, &Container::WantToMove);
    connect(newBee, &Bee::ToCollect, this, &Container::BeeWanToCollect);
    _objArr.append(newBee);
    parent->AddBeeToMemory(newBee);
    ++_count;
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add bee   :  (" << _x+0.5f << "x" << _y+0.5f << ")";
}

Hive *Container::AddHive(World *worldPtr)
{
    Hive* newHive = new Hive(_x,_y, worldPtr);
    connect(newHive, &Hive::WantGenerateNewBee, this, &Container::GenerateNewBee);
    _objArr.append(newHive);
    ++_count;
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add hive to contaner: (" << _x << "x" << _y << ")";
    return newHive;
}



void Container::RedrawObject()
{
    auto i = _objArr.begin();
    while(i!=_objArr.end()){

//        for(auto obj : _objArr){
        if(*i != nullptr)
            emit RepaintObj(dynamic_cast<QGraphicsItem*>(*i));
//        else
  //          QDebug(QtMsgType::QtWarningMsg) << "WRN: Phantom object!";
        i++;
    }

}

void Container::Recalc()
{
//    auto i = _objArr.begin();
//    while(i != _objArr.end()){
    for(int i = 0; i < _objArr.size(); i++){
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
        _objArr.at(i)->Work();
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
