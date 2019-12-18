#include "hive.h"
#include <QPainter>
#include <random>
#include "world.h"

Hive::Hive(float x, float y, World* worldPtr) : _x(x+0.5f), _y(y+0.5f) {
    GenerateColorOfThis();
    _scaleY = worldPtr->_scaleY;
    _scaleX = worldPtr->_scaleX;
}

Hive::~Hive() {
    delete _color;
}

float Hive::GetX() { return _x;}

float Hive::GetY() { return _y; }

void Hive::SetCoordinates(float x, float y)
{
    Q_UNUSED(x)
    Q_UNUSED(y)
}

QColor *Hive::GetColor(){ return _color; }

unsigned Hive::GetSize(){ return _size; }

void Hive::GenerateColorOfThis()
{
    int redPart = 0;
    int greenPart = 0;
    int bluePart = 0;
    auto generateColor = [=]() -> int{
        std::mt19937 generator(rand());
        std::uniform_int_distribution<int> genColor(70, 240);
        int tmpColor = 0;
        tmpColor = genColor(generator);
        return tmpColor;
    };

    redPart = generateColor() ;
    greenPart = generateColor();
    bluePart = generateColor();
    _color = new QColor(redPart,greenPart,bluePart);

}


QRectF Hive::boundingRect() const { return QRectF(0,0,_size,_size); }

void Hive::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(*_color);
    painter->drawRect(static_cast<int>(_x*_scaleX-_size/2/* + _scaleX/2*/), static_cast<int>(_y*_scaleY-_size/2/*+_scaleY/2*/),
                      static_cast<int>(_size),static_cast<int>(_size));
//    painter->drawText(_x+_size,_y,QString::number(_x)+"x"+QString::number(_y));

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Hive::Work()
{
//     QDebug(QtMsgType::QtInfoMsg) << "INFO: Hive work";

}
