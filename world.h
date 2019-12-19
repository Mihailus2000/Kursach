#ifndef WORLD_H
#define WORLD_H
#include <QMap>
#include <QDebug>
//#include "iobjects.h"
#include <QObject>
#include "container.h"
#include <QGraphicsItem>
#include <random>


class World : public QObject
{
    Q_OBJECT

signals:
    void RepaintObj(QGraphicsItem* item);

public slots:
    void Run();
    void Redraw();
    void Recalc();
    void MoveObject(float dx,float dy, IObjects* ptr, Container* contPtr);
    void BeeCollectFromFlower(Bee* bee, Container* itsCont);
public:
    using TMap = QMap<QString, Container*>;

    unsigned contAmount;
    World(unsigned widgetWidth, unsigned widgetHeight, unsigned worldWidth, unsigned worldHeight);
    void AddLife();
    float _scaleX, _scaleY;
    unsigned _width;
    unsigned _height;
    QString _delimer = "X";
    std::mt19937 _gen;
//    std::random_device* _device;
    TMap* _map;

private:
    const unsigned _BEE_AMOUNT = 3;
    const unsigned _HIVE_AMOUNT = 1;
    const unsigned _FLOWERS_AMOUNT = 100;

};

#endif // WORLD_H
