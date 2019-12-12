#include "hive.h"
#include <QPainter>
#include <random>

Hive::Hive(float x, float y) : _x(x), _y(y) {
    GenerateColorOfThis();
}

Hive::~Hive() {
    delete _color;
}

float Hive::GetX() { return _x;}

float Hive::GetY() { return _y; }

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
    painter->drawRect(static_cast<int>(_x-_size/2), static_cast<int>(_y-_size/2),
                      static_cast<int>(_size),static_cast<int>(_size));
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Hive::Work()
{

}
