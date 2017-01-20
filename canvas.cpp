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
    for(auto& v : elementsOnCanvas) {
        qDebug() << "(next)";
        if(v->contains(event->pos())) {
            actualEl = v;
            //qDebug() << event->pos() << " on " << static_cast<int>(v->typ);
            DragDrop drop(this);
            drop.mousePressEvent(event, actualEl, QString("canvas"));
            return;
        }
    }
    actualEl = nullptr;
    qDebug() << "NoPe! " << event->pos();

    DragDrop drop(this);
    drop.mousePressEvent(event, actualEl, QString("canvas"));
}


void Canvas::keyPressEvent(QKeyEvent *event) {
    DragDrop drop(this);
    drop.keyPressEvent(event);
    if(actualEl == nullptr)
        qDebug() << "nope!";
    else
        qDebug() << static_cast<int>(actualEl->typ);
}
