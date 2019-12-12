#ifndef IOBJECTS_H
#define IOBJECTS_H
#include <QGraphicsItem>

class IObjects : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    IObjects();
    ~IObjects();
    virtual void SetCoordinates(float x, float _y) = 0;
    virtual float GetX() = 0;
    virtual float GetY() = 0;
    virtual void Work();
//    virtual void Redraw() = 0;
};

#endif // IOBJECTS_H
