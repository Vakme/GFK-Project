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
    bool contains(const QPointF & point) const;
    void draw(QPainter & painter);

    const ElType typ;
    const QPointF centerPoint;
    const QPolygonF points;
    const QColor color;

protected:
    Element(ElType typ, QPointF centerPoint, const QPolygonF& points,
            qreal rotation_max = 360, bool mirrorable = false);

private:
    void updateBitmap();
    static QColor nextColor();

    const qreal rotation_max;
    qreal rotation;
    const bool mirrorable;
    bool mirror;
    QPixmap bitmap;
    bool isChanged;
};



template<ElType _typ>
class ElementKindBase : public Element {
public:
    template<typename... Args>
    ElementKindBase(Args... args) : Element(_typ, std::forward<Args>(args)...) {}
};

template<ElType _typ>
class ElementKind : public ElementKindBase<_typ> {
public:
    ElementKind(QPointF centerPoint);
    ElementKind(double x, double y) : ElementKind(QPointF(x, y)) {}
};

using Square        = ElementKind<ElType::SQUARE>;
using TriangleBig   = ElementKind<ElType::TRIANGLE_BIG>;
using TriangleMid   = ElementKind<ElType::TRIANGLE_MID>;
using TriangleSmall = ElementKind<ElType::TRIANGLE_SMALL>;
using Rhombus       = ElementKind<ElType::RHOMBUS>;


#endif // ELEMENT_H
