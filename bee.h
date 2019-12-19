#ifndef BEE_H
#define BEE_H
#include <QGraphicsItem>
#include <QColor>
#include "iobjects.h"
#include <QObject>
#include "hive.h"
#include <random>


class World;

class Bee : public QObject/*public QGraphicsItem*/, public IObjects
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    enum State {
        FLY,
        COLLECT,
        FLY_BACK,
        AT_HOME
    };

signals:
    void WantToMove(float dx,float dy, IObjects* ptr);
    void ToCollect(Bee* beePtr);

public:

    Bee(Hive* parent, World* worldPtr, float capacityOfNectar = 50.0f, float takeFoodAtTime = 5.f , float lifeLevl = 100.f);
    virtual ~Bee() override = default ;
    virtual void SetCoordinates(float x, float y) override;
    virtual float GetX() override;
    virtual float GetY() override;
    bool IfFull();
    void AddNectar(float nectar);
    std::tuple<float,float,float> GetGeneticParametrs();
private:

    void Move();
    void MoveHome();
    void EatHonny();
    State _beeState;
    std::mt19937 _gen;
    float _beelife;
    float _x;
    float _y;
    unsigned _width = 11;
    unsigned _height = 5;
//    unsigned _containsPoolen = 0;
//    unsigned _maxCapacityOfNectar = 10;
    float _containsNectar = 0.0;
    QColor * _color = nullptr;
    Hive* _parent = nullptr;
    float _scaleX, _scaleY;
    bool _firstStep = true;
    float _dx;
    float _dy;

    const float _SPEED = 0.5f;
    const float _MAX_CAPACITY_OF_NECTAR;
    const float _TAKE_FOOD_AT_TIME;
    const float _MAX_LIFE_LEVEL;



    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    // IObjects interface
public:
    void Work();
};
#endif // BEE_H
