#ifndef ELEMENT_H
#define ELEMENT_H
#include<QPoint>
#include <QColor>
#include <QPolygon>
#include <QPainter>
#include <QBitmap>

enum class ElType { TRIANGLE_BIG, TRIANGLE_MID, TRIANGLE_SMALL, SQUARE, RHOMBUS };

class Element
{
public:
    Element() = delete;
    Element(const Element & el) = delete;
    Element(const ElType &typ, QPoint centerPoint, QColor color);
    bool contains(const QPoint & point);
    void draw(QPainter & painter);


private:
    QPoint centerPoint;
    QPolygon points;
    bool mirror;
    qreal rotation;
    QColor color;
    ElType typ;
    QBitmap bitmap;
    bool isChanged;
};

#endif // ELEMENT_H
