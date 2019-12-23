#include "flower.h"
#include <QPainter>
#include <QGraphicsView>
#include "world.h"

Flower::Flower(float x, float y, World *worldPtr) : QObject()
{
    _x = x;
    _y = y;
    std::mt19937 gen(rand());
    std::uniform_real_distribution<float> genHealth(_MAX_START_HEALTH/2,_MAX_START_HEALTH);
    _lifeLevel = genHealth(gen);
    _health = _lifeLevel;
    _scaleY = worldPtr->_scaleY;
    _scaleX = worldPtr->_scaleX;
}

Flower::~Flower()
{
    QDebug(QtMsgType::QtInfoMsg) << "INFO: Destructor of Flower";
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
//    const_cast<Flower*>(this)->prepareGeometryChange();
    return QRectF(0,0,_flowerSize,_flowerSize);
}

void Flower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    if(_lifeLevel >= 0.5f*_lifeLevel){
        if(_posibleToClone)
            painter->setBrush(Qt::yellow);
        else
            painter->setBrush(Qt::GlobalColor::green);
    }
    if(_lifeLevel >= 0.25f*_lifeLevel && _lifeLevel < 0.5f*_lifeLevel){
        painter->setBrush(Qt::darkGreen);
    }
    if(_lifeLevel >= 0.15*_lifeLevel && _lifeLevel < 0.25f*_lifeLevel){
        painter->setBrush(Qt::darkGray);
    }
    if(_health < 0.15*_lifeLevel)
        painter->setBrush(Qt::black);

    if(_firstDraw){
//        std::mt19937 gen(rand());
//        std::uniform_int_distribution<int> shiftX(-static_cast<int>(_scaleX/2),static_cast<int>(_scaleX/2));
//        std::uniform_int_distribution<int> shiftY(-static_cast<int>(_scaleY/2),static_cast<int>(_scaleY/2));
        _drawingX = static_cast<int>(_x*_scaleX/*+_scaleX/2 *//*+ shiftX(gen) */);
        _drawingy = static_cast<int>(_y*_scaleY/*+_scaleY/2 *//*+ shiftY(gen) */);
        _firstDraw = false;
    }

//    painter->drawRect(static_cast<int>(_x*_scaleX/*+_scaleX/2*/),static_cast<int>(_y*_scaleY/*+_scaleY/2*/),static_cast<int>(_scaleX), static_cast<int>(_scaleY));
    painter->drawEllipse( _drawingX, _drawingy, _flowerSize, _flowerSize);

    painter->setPen(Qt::GlobalColor::lightGray);
    auto text = QString::number(_containsNectar,'f',2)+"|"+QString::number(_health,'f',2);
    painter->drawText(static_cast<int>(_drawingX/*_scaleX/2*/-33/*/2*/),static_cast<int>(_drawingy+6/*+_flowerSize*/),text);

//    widget->repaint();
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Flower::Work()
{
    if(_health > 0.f){
        if(_posibleToClone){
            if(_containsNectar== 0.f && _leftToBirth > 0){ // its means that flower is dusted
                emit GenerateClone(this);
                _decreasesHealth *= 2.f;
//                _maxCapacityOfNectar -= 2.f;
                _containsNectar = _maxCapacityOfNectar / 1.26;
                _leftToBirth --;
                if(_leftToBirth <= 0){
                    emit DeleteFlower(this);
                    return;
                }
                _stepsFromLastClone = 0;
                _posibleToClone = false;

            }
            if(_leftToBirth <= 0){
                emit DeleteFlower(this);
                return;
            }
        }
        else{
            _stepsFromLastClone ++;
            if(_stepsFromLastClone >= _STEPS_TO_NEXT_POSIBLE_CLONING){
                _posibleToClone = true;
            }
        }
    }
    else{
        emit DeleteFlower(this);
        return;
    }
    _health -= _decreasesHealth;
//                if(_health <= 0){
//                    emit DeleteFlower(this);
//                    return;
//                }
}

void Flower::SetCoordinates(float x, float y)
{
    _x = x;
    _y = y;
}

float Flower::GetX(){ return _x; }

float Flower::GetY(){ return _y; }
