#include "hive.h"
#include <QPainter>
#include <random>
#include "world.h"
#include "bee.h"
#include <QVector>

void Hive::GenerateNewBee()
{
    //std::tuple<float,float,float> capacityAndTakeFoodAndLifeLevel;
    float sumCapacityOfNectar = 0.f;
    float sumTakeFoodTime = 0.f;
    float sumMaxLifeLevel = 0.f;
    QVector<int> gen,avgGen;
    bool firstReservation = true;
    int amountOfBees = _poolOfBees.size();
    if(amountOfBees > 0){
        for(auto bee : _poolOfBees){
            gen = bee->GetGeneticParametrs();
//                avgGen.reserve(gen.size());
            for(auto i =0;i < gen.size();i++){
                if(firstReservation){
                    avgGen.push_back(gen.at(i));
                }
                else{
                    avgGen[i] += gen.at(i);
                }
            }
            firstReservation = false;
        }

        for(auto i =0; i < avgGen.size();i++)
            avgGen[i] = round(avgGen[i]/(amountOfBees));

//        float averageP1 = sumCapacityOfNectar / amountOfBees;
//        float averageP2 = sumTakeFoodTime / amountOfBees;
//        float averageP3 = sumMaxLifeLevel / amountOfBees;

        if(_generationNumber % 1 == 0 && _poolOfBees.size() <= _MAX_AMOUNT_OF_BEES){
            std::mt19937 generator(rand());
            std::uniform_int_distribution<int> mutationGenValue(0,255);
            std::uniform_int_distribution<int> genIndex(0,avgGen.size()-1);
            avgGen[genIndex(generator)] = mutationGenValue(generator);
        }
        Bee* newBee = new Bee(this, _ptrToWorld, avgGen);
        emit WantGenerateNewBee(newBee);
        _generationNumber++;
    }
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

float Hive::AddNectar(float addedNectar)
{
    if(_containsNectar < _MAX_CAPACITY_OF_NECTAR){
        if(_containsNectar + addedNectar >= _MAX_CAPACITY_OF_NECTAR){
            auto leftToAdd = _MAX_CAPACITY_OF_NECTAR - (_containsNectar + addedNectar);
            _containsNectar = _MAX_CAPACITY_OF_NECTAR;
            return leftToAdd;
        }
        else{
            _containsNectar += addedNectar;
            return 0.f;
        }
    }
    else
        return false;
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

    if(howMuchTakeHonny <= _containsHonny){
        _containsHonny -= howMuchTakeHonny;
        return howMuchTakeHonny;
    }
    else {
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
