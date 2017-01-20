#include "shapeslist.h"

ShapesList::ShapesList(QWidget *parent) : QWidget(parent)
{

}

void ShapesList::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //a simple line
    painter.drawLine(1,1,50,50);

}

void ShapesList::mousePressEvent(QMouseEvent *event)
{
    DragDrop drop(this);
    //drop.mousePressEvent(event, QString("shapesList"));
}
