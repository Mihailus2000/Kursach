#ifndef HIVE_H
#define HIVE_H

#include "iobjects.h"
#include <QColor>
#include <QObject>


class World;

class Hive :  public QObject, public IObjects
{
    Q_OBJECT
//    Q_INTERFACES(QGraphicsItem)
public:
    Hive(float x, float y, World* worldPtr);
    virtual ~Hive() override;
    virtual float GetX() override;
    virtual float GetY() override;
    virtual void SetCoordinates(float x, float y) override;
    QColor* GetColor();
    unsigned GetSize();
private:
    float _x;
    float _y;
    unsigned _size = 40;
    QColor* _color = nullptr;
    float _scaleX, _scaleY;

    void GenerateColorOfThis();
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // IObjects interface
public:
    void Work();

};

#endif // HIVE_H
