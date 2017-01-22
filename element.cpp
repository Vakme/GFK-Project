#include <cmath>
#include <typeinfo>
#include <QTransform>
#include "element.h"


Element::Element(ElType typ, QPointF center, const QPolygonF& points,
                 qreal rotation_max, bool mirrorable) :
    typ(typ), color(nextColor()), _centerPoint(center), points(points),
    rotation_max(rotation_max), mirrorable(mirrorable), valid(true)
{
    rotation = 0;
    mirror = false;
    isChanged = true;
    bitmap = QPixmap(350, 350);
    bitmap.fill(QColor(0,0,0,0));
    updateBitmap();
}

void Element::swap(Element *other) {
    Element tmp = Element(*this);
    *this = Element(*other);
    *other = std::move(tmp);
}

Element& Element::operator =(Element && el) {
    if(typ != el.typ) {
        throw std::bad_cast();
    }
    rotation     = el.rotation;
    _centerPoint = el.centerPoint();
    isChanged = true;
    return *this;
}

QPolygonF Element::getRealPoly(qreal x, qreal y) const {
    QTransform trans;
    trans.translate(x,y);
    trans.rotate(rotation);
    if(mirror) {
        trans = QTransform(-1, 0, 0, 0, 1, 0, 0, 0, 1) * trans;
    }
    return trans.map(points);
}

QPolygonF Element::getRealPoly() const {
    return getRealPoly(_centerPoint.x(), _centerPoint.y());
}

void Element::updateBitmap() {
    if(isChanged) {
        bitmap.fill(QColor(0,0,0,0));

        QPainter *paint = new QPainter(&bitmap);
        paint -> setRenderHints(QPainter::Antialiasing);

        QColor colPen, colBrush;
        Qt::BrushStyle styleBrush;
        if(valid) {
            colPen = QColor(0,0,0);
            colBrush = color;
            styleBrush = Qt::SolidPattern;
        }
        else {
            colPen = QColor(255,0,0, 127);
            colBrush = QColor(255, 0, 0);
            styleBrush = Qt::Dense6Pattern;
        }
        paint->setPen(QPen(colPen));
        paint->setBrush(QBrush(colBrush, styleBrush));

        paint -> drawPolygon(getRealPoly(175,175));
        isChanged = false;
        delete paint;
    }
}


template<>
Square::ElementKind(QPointF centerPoint) :
    ElementKindBase(centerPoint, QVector<QPointF>{
                        QPointF( 50, -50),
                        QPointF(-50, -50),
                        QPointF(-50,  50),
                        QPointF( 50,  50)
                    }, 90) {}

template<>
TriangleSmall::ElementKind(QPointF centerPoint) :
    ElementKindBase(centerPoint, QVector<QPointF>{
                        QPointF( -35, -12),
                        QPointF(  35, -12),
                        QPointF(   0,  23)
                    }) {}

template<>
TriangleMid::ElementKind(QPointF centerPoint) :
    ElementKindBase(centerPoint, QVector<QPointF>{
                        QPointF( -71, -23),
                        QPointF(  71, -23),
                        QPointF(   0,  47)
                    }) {}

template<>
TriangleBig::ElementKind(QPointF centerPoint) :
    ElementKindBase(centerPoint, QVector<QPointF>{
                        QPointF(-141, -47),
                        QPointF( 141, -47),
                        QPointF(   0,  94)
                    }) {}

template<>
Rhombus::ElementKind(QPointF centerPoint) :
    ElementKindBase(centerPoint, QVector<QPointF>{
                        QPointF( -75, -25),
                        QPointF(  25, -25),
                        QPointF(  75,  25),
                        QPointF( -25,  25)
                    }, 180, true) {}


QColor Element::nextColor() {
    static int color_num = 0;
    static QColor colors[] = {
                               QColor(255, 31,  0),
                               QColor(  0,255, 63),
                               QColor(  0,127,255),
                               QColor(  0,255,127),
                               QColor(255,158,  0)
                             };
    color_num = (color_num + 1) % (sizeof(colors)/sizeof(colors[0]));
    return colors[color_num];
}


QPointF Element::centerPoint() const {
    return _centerPoint;
}

void Element::setCenterPoint(QPointF center) {
    _centerPoint = center;
    isChanged = true;
}

bool Element::isValid() const {
    return valid;
}

void Element::setValid(bool flag) {
    valid = flag;
    isChanged = true;
}

bool Element::contains(const QPointF & point) const {
    return getRealPoly().containsPoint(point, Qt::OddEvenFill);
}

void Element::draw(QPainter * painter) {
    updateBitmap();
    painter->drawPixmap(_centerPoint - QPoint(175,175), bitmap);
}

const QPixmap & Element::getBitmap() {
    updateBitmap();
    return bitmap;
}

bool Element::intersects(const Element& other) const {
    return ! getRealPoly().intersected(other.getRealPoly()).isEmpty();
}

void Element::rotateLeft(int val) {
    rotation -= val;
    reduceRotation();
    isChanged = true;
}

void Element::rotateRight(int val) {
    rotation += val;
    reduceRotation();
    isChanged = true;
}

void Element::moveUp(int val) {
    _centerPoint -= QPointF(0, val);
    isChanged = true;
}

void Element::moveDown(int val) {
    _centerPoint += QPointF(0, val);
    isChanged = true;
}

void Element::moveLeft(int val) {
    _centerPoint -= QPointF(val, 0);
    isChanged = true;
}
void Element::moveRight(int val) {
    _centerPoint += QPointF(val, 0);
    isChanged = true;
}

void Element::mirrorEl() {
    if(mirrorable) {
        mirror = !mirror;
        isChanged = true;
    }
}

void Element::reduceRotation() {
    rotation = fmod(rotation, rotation_max);
    // fmod returns x in range <-rotation_max, rotation_max>
    if(rotation < 0) {
        rotation = rotation + rotation_max;
    }
}
