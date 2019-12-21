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
        AT_HOME,
        DIE
    };

signals:
    void WantToMove(float dx,float dy, IObjects* ptr);
    void ToCollect(Bee* beePtr);
    void DeleteBee(Bee* bee);
public:
// gen {1-255}
    //takeFoodAtTime {0-10} 255-10 1-0 255/10 = k = 25.5
//    MAXcOPACIRYD= 100;
//    K_capacityOfNEctar = MAXcOPACIRYD/255;
//    float capacityOfNectar = 50.0f, float
//    takeFoodAtTime = 5.f , float lifeLevel = 100.f
    Bee(Hive* parent, World* worldPtr, QVector<int> gen = {128,128,128});
    virtual ~Bee() override;
    virtual void SetCoordinates(float x, float y) override;
    virtual float GetX() override;
    virtual float GetY() override;
    bool IfFull();
    void AddNectar(float nectar);
    QVector<int> GetGeneticParametrs();
    Hive* GetParent();
private:
    Hive* _parent = nullptr;
    QVector<int> _gen;
    void Move();
    void MoveHome();
    void EatHonny();
    std::mt19937 _generator;
    State _beeState;
    float _BeeHealth;
    float _x;
    //    unsigned _containsPoolen = 0;
    //    unsigned _maxCapacityOfNectar = 10;

    float _y;    unsigned _height = 5;
    unsigned _width = 11;
    float _containsNectar = 0.0;
    float _scaleX, _scaleY;
    bool _firstStep = true;
    float _dx;
    float _dy;

    float _capacityOfNectar;
    float _currentTakingFoodAtTime;
    float _beeLife;

    QColor * _color = nullptr;

    const float _SPEED = 0.5f;
    const float _MAX_CAPACITY_OF_NECTAR = 70.f; // for ideal bee
    const float _TAKE_FOOD_AT_TIME = 3.f;// for ideal bee
    const float _MAX_LIFE_LEVEL = 60.f;// for ideal bee
    const float _MAX_WIDTH = 23.3;
    const float _MAX_HEIGHT = 10;

    const float _MIN_WIDTH = 7.;
    const float _MIN_HEIGHT = 3.;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    // IObjects interface
public:
    void Work();
};
#endif // BEE_H
