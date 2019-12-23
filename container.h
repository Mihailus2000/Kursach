#ifndef CONTAINER_H
#define CONTAINER_H

#include <QMutex>
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
   /* AbstractFlower* flowerPtr = nullptr;
    Hive* hivePtr = nullptr;
    BeeSimulator* beePtr = nullptr;*/
    int _count = 0;
    std::mt19937 _gen;
    World* _myWorld;
    QMutex* _mu;
signals:
    void RepaintObj(IObjects* item, QThread* work);
    void ObjectWantToMove(float dx,float dy, IObjects* objPtr, Container* contPtr);
    void BeeCollect(Bee* bee, Container* thisCont);
    void GenerateClone(IObjects* obj);
    void DeleteContainer(Container* cont);
private slots:
    void BeeWanToCollect(Bee* bee);
    void WantToMove(float dx,float dy, IObjects* objPtr);
    void GenerateNewBee(Bee *bee);
    void DeleteObject(IObjects* obj);
public:
    QList<IObjects*> _objArr;
    QString GetCoordinates();
    Container(unsigned x, unsigned y, World* ptr, QMutex* mu);
    virtual ~Container();
    unsigned GetX();
    unsigned GetY();
    void RedrawObject(QThread* workThread);
    void Recalc();
    unsigned _x;
    unsigned _y;
    QSet<Flower *> GetFlowers();
    bool CheckFromHive();
    QString _coordStr;
    QGraphicsScene* _scene;


/*
    bool ContainsFlower() {return (flowerPtr) ? true : false; }
    bool ContainsHive() {return (hivePtr) ? true : false; }
    bool ContainsBee() {return (beePtr) ? true : false; }

    AbstractFlower* GetFlower(){
        if(flowerPtr)
            return flowerPtr;
        else{
//            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
            //TODO Exception
        }
    }
    Hive* GetHive(){
        if(hivePtr)
            return hivePtr;
        else{
//            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning hive is null!";
            //TODO Exception
        }
    }
    BeeSimulator* GetBee(){
        if(beePtr)
            return beePtr;
        else{
//            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning bee is null!";
            //TODO Exception
        }
    }*/

    void AddFlower(/*World* worldPtr*/);
    void AddObject(IObjects* obj);
    bool RemoveObject(IObjects *obj);
    void AddBee(Hive* parrent/*, World* worldPtr*/);
    Hive* AddHive(/*World* worldPtr*/);
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
//            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
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
//            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
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
//            QDebug(QtMsgType::QtWarningMsg) << "WRN: Returning flower is null!";
            //TODO Exception
        }
    }
*/

};

#endif // CONTAINER_H
