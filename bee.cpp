#include "bee.h"
#include <QPainter>
#include <QDebug>
#include <QWidget>
#include "world.h"

QRectF Bee::boundingRect() const { return QRectF(0,0,_width,_width); }

void Bee::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    std::mt19937 gen(rand());
//    unsigned hiveSize = _parent->GetSize();
//    std::uniform_int_distribution<int> shift(-static_cast<int>(hiveSize/3),static_cast<int>(hiveSize/3));

    painter->setPen(Qt::black);
    painter->setBrush(*_color);
//    auto scaleX = widget->width();
//            scaleY=
    int newX = static_cast<int>(_x*_scaleX-_width/2/* + _scaleX/2*/);//+shift(gen);
    int newY = static_cast<int>(_y*_scaleY-_height/2/* + _scaleY/2*/);//+shift(gen);

    if(newX < 0 )
        newX = 0;
    if(newY < 0 )
        newY = 0;

//    painter->drawRect(static_cast<int>(_x*_scaleX),static_cast<int>(_y*_scaleY),static_cast<int>(_scaleX), static_cast<int>(_scaleY));
    painter->drawEllipse(newX,newY,static_cast<int>(_width),static_cast<int>(_height));
    painter->drawText(newX+_width,newY,QString::number(_containsNectar));
//    QString coord =
//    painter->drawEllipse(static_cast<int>(_x*_scaleX-_width/2),static_cast<int>(_y*_scaleY-_height/2), _width, _height);
//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Draw bee   :  (" << newX << "x" << newY << ")";

    //    widget->repaint();

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Bee::Work()
{
    _beelife -= 0.5f;
    if(_beeState == FLY){
        Move();

        if(_beelife <= _MAX_LIFE_LEVEL * 0.1f){
            _beeState = FLY_BACK;
            return;
        }
        emit ToCollect(this);
        if(IfFull()){
            _beeState = FLY_BACK;
            _firstStep = true;
        }
        return;
    }
    if(_beeState == FLY_BACK){
        MoveHome();
        //NOTHING
    }if(_beeState == AT_HOME){
        // TODO
        EatHonny();
    }

}

Bee::Bee(Hive* parent, World* worldPtr, float capacityOfNectar, float takeFoodAtTime,float lifeLevel) :
    _MAX_CAPACITY_OF_NECTAR (capacityOfNectar), _TAKE_FOOD_AT_TIME(takeFoodAtTime), _MAX_LIFE_LEVEL(lifeLevel)
{
    _beelife = _MAX_LIFE_LEVEL;

    _scaleY = worldPtr->_scaleY;
    _scaleX = worldPtr->_scaleX;
    _x = parent->GetX();
    _y = parent->GetY();


    _parent = parent;
    _color = parent->GetColor();

//    std::random_device device;
    _gen.seed(rand());
    _beeState = FLY;
//    std::uniform_real_distribution<int> speed(0, 0.5);


}

void Bee::SetCoordinates(float x, float y)
{
    _x = x;
    _y = y;
}

float Bee::GetX() { return _x; }

float Bee::GetY(){ return _y; }

bool Bee::IfFull()
{
    if(_containsNectar >= _MAX_CAPACITY_OF_NECTAR)
        return true;
    else
        return false;
}

void Bee::AddNectar(float nectar)
{
    if(_containsNectar+nectar >= _MAX_CAPACITY_OF_NECTAR){
        _containsNectar = _MAX_CAPACITY_OF_NECTAR;
    }
    else
        _containsNectar += nectar;
}

std::tuple<float, float, float> Bee::GetGeneticParametrs()
{
    return {_MAX_CAPACITY_OF_NECTAR, _TAKE_FOOD_AT_TIME,_MAX_LIFE_LEVEL};
}

void Bee::Move()
{
    std::uniform_real_distribution<float> step(-0.2,0.2);// диапазон шага

//    QDebug(QtMsgType::QtInfoMsg) << "INFO: Bee Move";
    emit WantToMove(step(_gen),step(_gen), this);
}

void Bee::MoveHome()
{

//    auto CalculateYCoord = [](float xBeeCoord, float yBeeCoord,float xHiveCoord,float yHiveCoord, float newXCoord) -> float {
//        int xBeeCoordInt = static_cast<int>(xBeeCoord);
//        int yBeeCoordInt = static_cast<int>(yBeeCoord);
//        int xHiveCoordInt = static_cast<int>(xHiveCoord);
//        int yHiveCoordInt = static_cast<int>(yHiveCoord);
//        int newXCoordInt = static_cast<int>(newXCoord);
//        int yCoord = 0;
//        yCoord = static_cast<int>(round(yBeeCoordInt - (xBeeCoordInt - newXCoordInt)*(static_cast<int>(round(yHiveCoordInt-yBeeCoordInt)/(xHiveCoordInt-xBeeCoordInt)))));
//        if(yCoord >= 0)
//            return static_cast<unsigned>(yCoord);
//        else
//            QDebug(QtMsgType::QtWarningMsg) << "WRN: Y coordinate < 0!!";
//        return 0.f; // TODO do
//    };

    if(_firstStep) {


        auto deltaDx = _parent->GetX() - _x;
        auto deltaDy = _parent->GetY() - _y;
        float z=0;
        if(deltaDx != 0.f && deltaDy != 0.f){
            z = std::max(abs(deltaDx/deltaDy),abs(deltaDy/deltaDx));
        } else {
            if(deltaDx == 0.f && deltaDy != 0.f)
                z = abs(deltaDy);
            if(deltaDy == 0.f && deltaDx != 0.f)
                z = abs(deltaDx);

        }
        if(z != 0.f){
            _dx = deltaDx/z * 0.1f;
            _dy = deltaDy/z * 0.1f;
            _firstStep = false;
        }
        else{
            _beeState = AT_HOME;
            _parent->AddNectar(_containsNectar);
            _containsNectar = 0.f;
            return;
        }
     }
     if(floor(_x) == floor(_parent->GetX()) && floor(_y) == floor(_parent->GetY()) ) {
         _beeState = AT_HOME;
         _parent->AddNectar(_containsNectar);
         _containsNectar = 0.f;
         return;
     }

    emit WantToMove(_dx,_dy,this);

}

void Bee::EatHonny()
{
    float efficientyOfEating = 0.7f;
    _beelife += _parent->GiveHonny(_TAKE_FOOD_AT_TIME) * efficientyOfEating;
}
//        _firstStep = false;
//    }

//    if(_x < _parent->GetX()){

//    }

//    float b = std::abs(((_y-_parent->GetY()))) / (_x-_parent->GetX()) ;

//    if(a >= b){

//    }

//    float newYCoord = CalculateYCoord(_x,_y,_parent->GetX(),_parent->GetY(),_x+dx);
//    float dy = (_y-_parent->GetY())/(_x-_parent->GetX());
//    if(_x < _parent->GetX()) {
//        float newXCoord = _x+0.1f;
//        unsigned newYCoord = CalculateYCoord(_x,_y,_parent->GetX(),_parent->GetY(),newXCoord);
//        emit WantToMove(0.1f, -0.1f,this);
//        return;
//    }
//    if(_x > _parent->GetX()){
//        float newXCoord = _x-0.1f;
//        unsigned newYCoord = CalculateYCoord(_x,_y,_parent->GetX(),_parent->GetY(),newXCoord);
//        emit WantToMove(newXCoord-_x,newYCoord-_y,this);
//        return;
//    }
//    if(_x == _parent->GetX()){
//        if(_y < _parent->GetY()) {
//            emit WantToMove(0.f, 0.1, this);
//        }
//        else
//            emit WantToMove(0.f, -0.1, this);
//        return;
//    }
//    emit WantToMove(dx, newYCoord-_y, this);

