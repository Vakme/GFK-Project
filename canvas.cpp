#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    elementsOnCanvas({
      new Square       (150,  50),
      new TriangleMid  (150, 200),
      new TriangleSmall(150, 350),
      new Rhombus      (450,  50)
    })
{
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setWindow(QRect(0, 0, 661, 431));

    for(Element *element : elementsOnCanvas)
    {
        element->draw(painter);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    DragDrop drop(this);
    drop.mousePressEvent(event, QString("canvas"));
}
