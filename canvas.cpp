#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    elementsOnCanvas({
      new Square       (150,  50),
      new TriangleMid  (150, 200),
      new TriangleSmall(150, 300),
      new Rhombus      (450,  50)
    }) {}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    for(Element *element : elementsOnCanvas) {
        element->draw(painter);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    DragDrop drop(this);
    drop.mousePressEvent(event, *&elementsOnCanvas, QString("canvas"));
}


void Canvas::keyPressEvent(QKeyEvent *event) {
    DragDrop drop(this);
    drop.keyPressEvent(event);
}
