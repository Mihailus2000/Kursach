#ifndef WORLD_H
#define WORLD_H
#include <QMap>
#include <QDebug>
//#include "iobjects.h"
#include <QObject>
#include "container.h"
#include <QGraphicsItem>


class World : public QObject
{
    Q_OBJECT

signals:
    void RepaintObj(QGraphicsItem* item);

public slots:
    void Run();
    void Redraw();
    void Recalc();
    void MoveObject(float dx,float dy, IObjects* ptr);
public:
    using TMap = QMap<QString, Container*>;

    World();
    void AddLife();
    unsigned _width;
    unsigned _height;
    QString _delimer = "X";

    TMap* _map;

private:


};

#endif // WORLD_H
