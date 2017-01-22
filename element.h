#ifndef ELEMENT_H
#define ELEMENT_H
#include<QPoint>
#include <QColor>
#include <QPolygon>
#include <QPainter>
#include <QBitmap>
#include <QtDebug>

enum class ElType { TRIANGLE_BIG, TRIANGLE_MID, TRIANGLE_SMALL, SQUARE, RHOMBUS };

class Element
{
public:
    Element() = delete;
    bool contains(const QPointF & point) const;
    void draw(QPainter * painter);

    const ElType typ;

    //był const, nie może być, musisz przesuwać obiekt. Jeśli musi być, to wywal, ale przesuwanie na strzałkach nie działa
    QPointF centerPoint;
    const QColor color;
    QPolygonF getRealPoly(qreal x, qreal y) const;
    void rotateLeft();
    void rotateRight();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void mirrorEl();

    QPolygonF getPoly() { return points; }
    qreal getRot() { return rotation; }
    bool getMir() { return mirror; }

    //możesz mnie za to zabić.
    Element(int typ, QPointF centerPoint, const QPolygonF points, qreal rotation, bool mirror, bool mirrorable, qreal rotation_max);

protected:
    Element(ElType typ, QPointF centerPoint, const QPolygonF& points,
            qreal rotation_max = 360, bool mirrorable = false);

private:
    void updateBitmap();
    static QColor nextColor();

    const QPolygonF points;
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
