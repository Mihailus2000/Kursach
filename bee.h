#ifndef BEE_H
#define BEE_H
#include <QGraphicsItem>
#include <QColor>
#include "iobjects.h"
#include <QObject>
#include "hive.h"

class Bee : public QObject/*public QGraphicsItem*/, public IObjects
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

signals:
    void WantToMove(float dx,float dy, IObjects* ptr);

public:
    Bee(float x, float y, Hive* parent);
    virtual ~Bee() override = default ;
    virtual void SetCoordinates(float x, float _y) override;
    virtual float GetX() override;
    virtual float GetY() override;
private:
    unsigned _beelife = 100;
    float _x;
    float _y;
    unsigned _width = 11;
    unsigned _height = 5;
    unsigned _containsPoolen = 0;
    unsigned _containsNectar = 0;
    QColor * _color = nullptr;
    Hive* _parent = nullptr;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    // IObjects interface
public:
    void Work();
};
#endif // BEE_H
