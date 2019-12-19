#include "hive.h"
#include <QPainter>
#include <random>
#include "world.h"
#include "bee.h"

void Hive::GenerateNewBee()
{
    //std::tuple<float,float,float> capacityAndTakeFoodAndLifeLevel;
    float sumCapacityOfNectar = 0.f;
    float sumTakeFoodTime = 0.f;
    float sumMaxLifeLevel = 0.f;
    for(auto bee : _poolOfBees){
        float p1,p2,p3;
        std::tie(p1,p2,p3) = bee->GetGeneticParametrs();
        sumCapacityOfNectar += p1;
        sumTakeFoodTime += p2;
        sumMaxLifeLevel += p3;
    }

    int amountOfBees = _poolOfBees.size();
    float averageP1 = sumCapacityOfNectar / amountOfBees;
    float averageP2 = sumTakeFoodTime / amountOfBees;
    float averageP3 = sumMaxLifeLevel / amountOfBees;

    Bee* newBee = new Bee(this, _ptrToWorld, averageP1, averageP2, averageP3);
    emit WantGenerateNewBee(newBee);

}

Hive::Hive(float x, float y, World* worldPtr) : _x(x+0.5f), _y(y+0.5f) {
    GenerateColorOfThis();
    _scaleY = worldPtr->_scaleY;
    _scaleX = worldPtr->_scaleX;
    _ptrToWorld = worldPtr;
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &Hive::GenerateNewBee);
    timer->start(15000);
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

void Hive::AddNectar(float addedNectar)
{
    if(_containsNectar + addedNectar >= _MAX_CAPACITY_OF_NECTAR){
        _containsNectar = _MAX_CAPACITY_OF_NECTAR;
    }
    else
        _containsNectar += addedNectar;
}

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

float Hive::GiveHonny(float howMuchTakeHonny)
{
    if(_containsHonny - howMuchTakeHonny >= 0.0f){
        _containsHonny -= howMuchTakeHonny;
        return howMuchTakeHonny;
    }
    else{
        float tmp = _containsHonny;
        _containsHonny = 0.f;
        return tmp;
    }
}

void Hive::AddBeeToMemory(Bee *bee){ _poolOfBees.insert(bee); }

void Hive::RemoveBeeFromMemory(Bee *bee){ _poolOfBees.remove(bee); }

QRectF Hive::boundingRect() const { return QRectF(0,0,_size,_size); }

void Hive::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(*_color);
    painter->drawRect(static_cast<int>(_x*_scaleX-_size/2/* + _scaleX/2*/), static_cast<int>(_y*_scaleY-_size/2/*+_scaleY/2*/),
                      static_cast<int>(_size),static_cast<int>(_size));
    painter->drawText((_x*_scaleX)+_size/2,_y*_scaleY,/*QString::number(_x)+"x"+QString::number(_y)*/QString::number(_containsNectar) + " | " + QString::number(_containsHonny));

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Hive::Work()
{
    std::mt19937 generator(rand());
    std::uniform_real_distribution<float> generatePobabilityOfCreating(0.f,1.0f);
    float checkingAmount = 5.00001f;
    float efficiency = 0.85f;
    float probabilityOfSuccess = 0.9f;
//     QDebug(QtMsgType::QtInfoMsg) << "INFO: Hive work";
    // Take 5 of Nectar and with 90% posibility turn into Honny (With 85% efficiency)

    if (_containsNectar >= checkingAmount){
        float probability = generatePobabilityOfCreating(generator);
        if(probability <= probabilityOfSuccess){
            float addedHonny = efficiency * checkingAmount;
            _containsNectar -= checkingAmount;
        }
    }
}
