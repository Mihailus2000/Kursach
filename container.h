#ifndef CONTAINER_H
#define CONTAINER_H

#include "iobjects.h"
#include <QMap>
#include <QGraphicsItem>
#include <QObject>
#include "hive.h"
#include <QList>
#include "bee.h"
#include "flower.h"


class World;

class Container : public QObject{
    Q_OBJECT

private:
    QList<IObjects*> _objArr;
   /* AbstractFlower* flowerPtr = nullptr;
    Hive* hivePtr = nullptr;
    BeeSimulator* beePtr = nullptr;*/
    int _count = 0;
    std::mt19937 _gen;
signals:
    void RepaintObj(QGraphicsItem* item);
    void ObjectWantToMove(float dx,float dy, IObjects* objPtr, Container* contPtr);
    void BeeCollect(Bee* bee, Container* thisCont);
private slots:
    void BeeWanToCollect(Bee* bee);
    void WantToMove(float dx,float dy, IObjects* objPtr);
    void GenerateNewBee(Bee *bee);
public:
    Container(unsigned x, unsigned y);
    virtual ~Container() = default;
    unsigned GetX();
    unsigned GetY();
    void RedrawObject();
    void Recalc();
    unsigned _x;
    unsigned _y;
    QSet<Flower *> GetFlowers();
    bool CheckFromHive();
    QString coordStr;


/*
    bool ContainsFlower() {return (flowerPtr) ? true : false; }
    bool ContainsHive() {return (hivePtr) ? true : false; }
    bool ContainsBee() {return (beePtr) ? true : false; }

    AbstractFlower* GetFlower(){
        if(flowerPtr)
            return flowerPtr;
        else{
            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
            //TODO Exception
        }
    }
    Hive* GetHive(){
        if(hivePtr)
            return hivePtr;
        else{
            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning hive is null!";
            //TODO Exception
        }
    }
    BeeSimulator* GetBee(){
        if(beePtr)
            return beePtr;
        else{
            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning bee is null!";
            //TODO Exception
        }
    }*/

    void AddFlower(World* worldPtr);
    void AddObject(IObjects* obj);
    bool RemoveObject(IObjects *obj);
    void AddBee(Hive* parrent, World* worldPtr);
    Hive* AddHive(World* worldPtr);
/*    void AddHive() {
        _objArr.insert(++_count,new Hive);
    }
    void AddBee() {
        _objArr.insert(++_count,new Bee);
    }

    AbstractFlower* RemoveFlower(){

        if(flowerPtr){
            AbstractFlower* tmpPtr = flowerPtr;
            flowerPtr = nullptr;
            return tmpPtr;
        }
        else{
            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
            //TODO Exception
        }
    }
    Hive* RemoveHive(){
        //        AbstractFlower* tmpPtr = flowerPtr;
        //        flowerPtr = nullptr;
        if(hivePtr){
            Hive* tmpPtr = hivePtr;

            hivePtr = nullptr;
            return tmpPtr;
        }
        else{
            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
            //TODO Exception
        }
    }
    BeeSimulator* RemoveBee(){
        //        AbstractFlower* tmpPtr = flowerPtr;
        //        flowerPtr = nullptr;
        if(beePtr){
            BeeSimulator* tmpPtr = beePtr;
            beePtr = nullptr;
            return tmpPtr;
        }
        else{
            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
            //TODO Exception
        }
    }
*/

};

#endif // CONTAINER_H
