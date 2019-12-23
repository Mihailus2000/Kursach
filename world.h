#ifndef WORLD_H
#define WORLD_H
#include <QMap>
#include <QDebug>
//#include "iobjects.h"
#include <QObject>
#include "container.h"
#include <QGraphicsItem>
#include <random>

#include <QMutex>


class World : public QObject
{
    Q_OBJECT

signals:
    void RepaintObj(IObjects* item, QThread* workTh);

public slots:
//    void DebugFPS();
    void Run();
    void Redraw();
    void Recalc();
    void MoveObject(float dx,float dy, IObjects* ptr, Container* contPtr);
    void BeeCollectFromFlower(Bee* bee, Container* itsCont);
    void CloneObject(IObjects* obj);
    void DeleteContainer(Container* cont);
public:
    using TMap = QMap<QString, Container*>;

    unsigned contAmount;
//    void CloseProgram();
    World(unsigned widgetWidth, unsigned widgetHeight, unsigned worldWidth, unsigned worldHeight, QMutex* mut = nullptr);
    virtual ~World();

    void AddLife();
    float _scaleX, _scaleY;
    unsigned _width;
    unsigned _height;
    QString _delimer = "X";
    std::mt19937 _gen;
    QMutex* _mutex = nullptr;
//    std::random_device* _device;
    TMap* _map;
    QGraphicsScene* _scenePtr;
private:
    unsigned _amountOfFlowers = 0;
    unsigned _inter = 0;
    unsigned Maxiteration = 9999999;
    const unsigned _BEE_AMOUNT = 2;
    const unsigned _HIVE_AMOUNT = 3;
    const unsigned _FLOWERS_AMOUNT = 30  ;
    const unsigned _MAX_AMOUNT_OF_FLOWERS_IN_COORDINATE = 5;

};

#endif // WORLD_H
