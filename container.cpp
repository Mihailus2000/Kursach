#include "container.h"
#include "flower.h"
#include "bee.h"

#include <qdebug.h>

Container::Container(unsigned x, unsigned y) : _x(x), _y(y) { }

void Container::AddFlower() {
    Flower* newFlower = new Flower(_x, _y);
//            connect(newFlower, &Flower::RepaintObject, this, &Container::RepaintObj);
    _objArr.insert(++_count,newFlower);
    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add flower : (" << _x << "x" << _y << ")";

}

void Container::AddBee(Hive* parent)
{
    Bee* newBee = new Bee(_x,_y, parent);
    connect(newBee, &Bee::WantToMove, this, &Container::ObjectWantToMove);
    _objArr.insert(++_count,newBee);
    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add bee   :  (" << _x << "x" << _y << ")";
}

Hive *Container::AddHive()
{
    Hive* newHive = new Hive(_x,_y);
    _objArr.insert(++_count,newHive);
    QDebug(QtMsgType::QtInfoMsg) << "INFO: Add hive   :  (" << _x << "x" << _y << ")";
    return newHive;
}



void Container::RedrawObject()
{
        for(auto obj : _objArr){
            emit RepaintObj(dynamic_cast<QGraphicsItem*>(obj));
        }

}

void Container::Recalc()
{
    for(auto obj : _objArr){

    }
}
