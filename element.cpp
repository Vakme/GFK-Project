#include "element.h"

Element::Element(const ElType &typ, QPoint centerPoint, QColor color) : typ(typ), centerPoint(centerPoint), color(color)
{
    bitmap = QBitmap(300, 300);
    if(typ == ElType::SQUARE)
        points.setPoints(4, centerPoint.x()-50, centerPoint.y()-50, centerPoint.x()+50, centerPoint.y()-50, centerPoint.x()+50, centerPoint.y()+50, centerPoint.x()-50, centerPoint.y()+50);
    mirror = false;
    rotation = 0;
    QPainter *paint = new QPainter(&bitmap);
    paint -> setPen(QPen(QColor(255, 255, 255)));
    paint -> setBrush(QBrush(color));
    paint -> drawPolygon(points, Qt::OddEvenFill);
    isChanged = false;
}

bool Element::contains(const QPoint & point)
{
    return points.containsPoint(point, Qt::OddEvenFill);
}

void Element::draw(QPainter & painter)
{
    if(isChanged)
    {
        isChanged = false;
    }

    painter.drawPixmap(centerPoint, bitmap);

}

