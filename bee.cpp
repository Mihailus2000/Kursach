#include "bee.h"
#include <QPainter>
#include <random>



QRectF Bee::boundingRect() const { return QRectF(0,0,_width,_width); }

void Bee::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    std::mt19937 gen(rand());
    unsigned hiveSize = _parent->GetSize();
    std::uniform_int_distribution<int> shift(-static_cast<int>(hiveSize/3),static_cast<int>(hiveSize/3));

    painter->setPen(Qt::black);
    painter->setBrush(*_color);
    int newX = static_cast<int>(_x-_width/2)+shift(gen);
    int newY = static_cast<int>(_y-_height/2)+shift(gen);

    if(newX < 0 )
        newX = 0;
    if(newY < 0 )
        newY = 0;
    painter->drawEllipse(newX,newY,_width,_height);
//    widget->repaint();

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Bee::Work()
{
    std::mt19937 gen(rand());
    std::uniform_real_distribution<float> step(-1,1);
//    std::uniform_real_distribution<float> dy(-1,1);

   emit WantToMove(step(gen),step(gen), this);
}

Bee::Bee(float x, float y, Hive* parent)
{
    _x = x;
    _y = y;
    _parent = parent;
    _color = parent->GetColor();
}

void Bee::SetCoordinates(float x, float y)
{
    _x = x;
    _y = y;
}
