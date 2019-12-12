#include "flower.h"
#include <QPainter>
#include <QGraphicsView>
Flower::Flower(float x, float y) : QObject()
{
    _x = x;
    _y = y;
}


QRectF Flower::boundingRect() const
{
    return QRectF(0,0,_flowerSize,_flowerSize);
}

void Flower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    if(_lifeLevel >= 50){
        if(_bloomed)
            painter->setBrush(Qt::yellow);
        else
            painter->setBrush(Qt::GlobalColor::green);
    }
    if(_lifeLevel >= 25 && _lifeLevel < 50){
        painter->setBrush(Qt::darkGreen);
    }
    if(_lifeLevel >= 10 && _lifeLevel < 25){
        painter->setBrush(Qt::darkGray);
    }
    if(_lifeLevel < 10)
        painter->setBrush(Qt::black);

    painter->drawEllipse(static_cast<int>(_x),static_cast<int>(_y), _flowerSize, _flowerSize);
//    widget->repaint();
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void Flower::Work()
{

}

////void Flower::Redraw(unsigned x, unsigned y)
//{
////    _x = x;
////    _y = y;
////    QGraphicsItem* item;
////    item = new QGraphicsItem(this);
//   // emit RepaintObj(this);
////    auto graphicsView = new QGraphicsView();
////    auto scene = new QGraphicsScene();
////    graphicsView->setScene(scene);

////    auto parent1 = new QGraphicsRectItem(0, 0, 100, 200);
////    parent1->setBrush(QBrush(QColor(Qt::cyan)));

////    scene->addItem(parent1);

////    auto subRect1 = new QGraphicsRectItem(10, 10, 50, 50, parent1);
////    subRect1->setBrush(QBrush(QColor(Qt::yellow)));
////    graphicsView->render(Q)
//}
