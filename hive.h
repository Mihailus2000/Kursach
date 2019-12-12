#ifndef HIVE_H
#define HIVE_H

#include "iobjects.h"
#include <QColor>
#include <QObject>

class Hive :  public QObject, public IObjects
{
    Q_OBJECT
//    Q_INTERFACES(QGraphicsItem)
public:
    Hive(float x, float _y);
    virtual ~Hive() override;
    virtual float GetX() override;
    virtual float GetY() override;
    virtual void SetCoordinates(float x, float _y) override;
    QColor* GetColor();
    unsigned GetSize();
private:
    float _x;
    float _y;
    unsigned _size = 40;
    QColor* _color = nullptr;

    void GenerateColorOfThis();
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // IObjects interface
public:
    void Work();


#endif // HIVE_H
