#ifndef IOBJECTS_H
#define IOBJECTS_H
#include <QGraphicsItem>
//#include "container."

class IObjects : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
//    IObjects(unsigned width, unsigned height) : worldXSize(width), worldYSize(height) {};
    IObjects();
    virtual ~IObjects();
    virtual void SetCoordinates(float x, float _y) = 0;
    virtual float GetX() = 0;
    virtual float GetY() = 0;
    virtual void Work() = 0;
//    virtual void Redraw() = 0;
protected:
    unsigned worldXSize;
    unsigned worldYSize;
};

#endif // IOBJECTS_H
