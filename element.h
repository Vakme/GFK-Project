#ifndef ELEMENT_H
#define ELEMENT_H
#include<QPoint>
#include <QColor>
#include <QPolygon>
#include <QPainter>
#include <QBitmap>
#include <QtDebug>
#include<memory>
#include <QXmlStreamReader>
enum class ElType { TRIANGLE_BIG, TRIANGLE_MID, TRIANGLE_SMALL, SQUARE, RHOMBUS };

class Element
{
public:
    Element() = delete;
    Element& operator =(Element && el);
    static Element* copy(const Element & el) { return new Element(el); }
    void swap(Element *other);

    bool contains(const QPointF & point) const;
    void draw(QPainter * painter);

    const ElType typ;

    //był const, nie może być, musisz przesuwać obiekt. Jeśli musi być, to wywal, ale przesuwanie na strzałkach nie działa
    QPointF centerPoint() const;
    void setCenterPoint(QPointF center);
    const QColor color;
    QPolygonF getRealPoly(qreal x, qreal y) const;
    QPolygonF getRealPoly() const;

    bool isValid() const;
    void setValid(bool flag);
    bool intersects(const Element& other) const;
    void rotateLeft (int val = 1);
    void rotateRight(int val = 1);
    void moveUp     (int val = 1);
    void moveDown   (int val = 1);
    void moveLeft   (int val = 1);
    void moveRight  (int val = 1);
    void mirrorEl();
    const QPixmap & getBitmap();
    QPolygonF getPoly() { return points; }
    qreal getRot() { return rotation; }
    bool getMir() { return mirror; }
    bool getMirable() { return mirrorable; }

    //możesz mnie za to zabić.

protected:
    Element(ElType typ, QPointF centerPoint, const QPolygonF& points,
            qreal rotation_max = 360, bool mirrorable = false);
    Element(int typ, QPointF centerPoint, const QPolygonF points, qreal rotation, bool mirror, bool mirrorable, qreal rotation_max);

private:
    Element(const Element & el) = default;

    void updateBitmap();
    void reduceRotation();
    static QColor nextColor();

    QPointF _centerPoint;
    const QPolygonF points;
    const qreal rotation_max;
    qreal rotation;
    const bool mirrorable;
    bool mirror;
    QPixmap bitmap;
    bool valid;
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
