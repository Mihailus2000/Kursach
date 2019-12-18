#include "flower.h"
#include <QPainter>
#include <QGraphicsView>
#include "world.h"

Flower::Flower(float x, float y, World *worldPtr) : QObject()
{
    _x = x;
    _y = y;

    _scaleY = worldPtr->_scaleY;
    _scaleX = worldPtr->_scaleX;
}

float Flower::GiveNectar()
{
    if(_containsNectar > 0){
        if(_containsNectar - _COLLECT_SPEED >= 0){
            _containsNectar -=_COLLECT_SPEED;
            return _COLLECT_SPEED;
        }
        else{
            float givingNectar = _containsNectar;
            _containsNectar = 0.f;
            // TODO : add signal to destroi flower after creating new
            return givingNectar;
        }
    }
    else{
        // TODO : add signal to destroi flower after creating new
        return 0.f;
    }
}


QRectF Flower::boundingRect() const
{
    return QRectF(0,0,_flowerSize,_flowerSize);
}

void Flower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
//    if(_lifeLevel >= 50){
//        if(_bloomed)
//            painter->setBrush(Qt::yellow);
//        else
//            painter->setBrush(Qt::GlobalColor::green);
//    }
//    if(_lifeLevel >= 25 && _lifeLevel < 50){
//        painter->setBrush(Qt::darkGreen);
//    }
//    if(_lifeLevel >= 10 && _lifeLevel < 25){
//        painter->setBrush(Qt::darkGray);
//    }

    if(_firstDraw){
//        std::mt19937 gen(rand());
//        std::uniform_int_distribution<int> shiftX(-static_cast<int>(_scaleX/2),static_cast<int>(_scaleX/2));
//        std::uniform_int_distribution<int> shiftY(-static_cast<int>(_scaleY/2),static_cast<int>(_scaleY/2));
        _drawingX = static_cast<int>(_x*_scaleX/*+_scaleX/2 *//*+ shiftX(gen) */);
        _drawingy = static_cast<int>(_y*_scaleY/*+_scaleY/2 *//*+ shiftY(gen) */);
        _firstDraw = false;
    }

    if(_lifeLevel < 10)
        painter->setBrush(Qt::black);
//    painter->drawRect(static_cast<int>(_x*_scaleX/*+_scaleX/2*/),static_cast<int>(_y*_scaleY/*+_scaleY/2*/),static_cast<int>(_scaleX), static_cast<int>(_scaleY));
    painter->drawEllipse( _drawingX, _drawingy, _flowerSize, _flowerSize);
    painter->drawText(static_cast<int>(_drawingX+/*_scaleX/2*/_flowerSize),static_cast<int>(_drawingy+_flowerSize),QString::number(_containsNectar));
//    widget->repaint();
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Flower::Work()
{

}

void Flower::SetCoordinates(float x, float y)
{
    _x = x;
    _y = y;
}

float Flower::GetX(){ return _x; }

float Flower::GetY(){ return _y; }
