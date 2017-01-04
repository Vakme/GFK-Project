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

