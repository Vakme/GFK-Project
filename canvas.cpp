#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent), elementsOnCanvas({new Element(ElType::SQUARE, QPoint(100, 100), QColor(255, 0, 0))})
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
