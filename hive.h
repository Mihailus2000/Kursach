#ifndef HIVE_H
#define HIVE_H

#include "iobjects.h"
#include <QColor>
#include <QObject>
#include <QSet>
#include <QTimer>

class World;
class Bee;

class Hive :  public QObject, public IObjects
{
    Q_OBJECT
//    Q_INTERFACES(QGraphicsItem)
signals:
    void WantGenerateNewBee(Bee *bee);
//eNewBee
private slots:
    void GenerateNewBee();
public:
    Hive(float x, float y, World* worldPtr);
    virtual ~Hive() override;
    virtual float GetX() override;
    virtual float GetY() override;
    virtual void SetCoordinates(float x, float y) override;
    QColor* GetColor();
    unsigned GetSize();
    float AddNectar(float addedNectar);
    float GiveHonny(float howMuchTakeHonny);
    void AddBeeToMemory(Bee* bee);
    void RemoveBeeFromMemory(Bee* bee);
private:
    unsigned _generationNumber = 0;
    float _x;
    float _y;
    unsigned _size = 40;
    QColor* _color = nullptr;
    float _scaleX, _scaleY;
    float _containsNectar = 0.f;
    float _containsHonny = 100.f;
    QSet<Bee*> _poolOfBees;
    QTimer *timer;
    World* _ptrToWorld;
    const float _MAX_CAPACITY_OF_NECTAR = 200.f;
    const float _MAX_CHANGE_OF_PARAMETR = 0.5f;
    const unsigned _MAX_AMOUNT_OF_BEES = 7;
    const int _TIMER_TO_BIRTH = 7000;
    void GenerateColorOfThis();

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // IObjects interface
public:
    void Work();

};

#endif // HIVE_H
