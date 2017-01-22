#include <cmath>
#include "element.h"
#include <QTransform>


Element::Element(ElType typ, QPointF centerPoint, const QPolygonF& points,
                 qreal rotation_max, bool mirrorable) :
    typ(typ), centerPoint(centerPoint), color(nextColor()),
    rotation_max(rotation_max), mirrorable(mirrorable), points(points)
{
    rotation = 0;
    mirror = false;
    isChanged = true;
    bitmap = QPixmap(350, 350);
    bitmap.fill(QColor(0,0,0,0));
    updateBitmap();
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

void Element::updateBitmap() {
    if(isChanged) {
        bitmap.fill(QColor(0,0,0,0));

        QPainter *paint = new QPainter(&bitmap);
        paint -> setRenderHints(QPainter::Antialiasing);
        paint -> setPen(QPen());
        paint -> setBrush(QBrush(color));

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


bool Element::contains(const QPointF & point) const {
    return getRealPoly(centerPoint.x(), centerPoint.y()).containsPoint(point, Qt::OddEvenFill);
}

void Element::draw(QPainter * painter) {
    updateBitmap();
    painter->drawPixmap(centerPoint - QPoint(175,175), bitmap);
}


void Element::rotateLeft() {
    rotation -= 1;
    reduceRotation();
    isChanged = true;
}

void Element::rotateRight() {
    rotation += 1;
    reduceRotation();
    isChanged = true;
}

void Element::moveUp() {
    centerPoint -= QPointF(0, 1);
    isChanged = true;
}

void Element::moveDown() {
    centerPoint += QPointF(0, 1);
    isChanged = true;
}

void Element::moveLeft() {
    centerPoint -= QPointF(1, 0);
    isChanged = true;
}
void Element::moveRight() {
    centerPoint += QPointF(1, 0);
    isChanged = true;
}

void Element::mirrorEl() {
    if(mirrorable) {
        mirror = !mirror;
    }
    else {
        rotation *= -1;
        reduceRotation();
    }
    isChanged = true;
}

void Element::reduceRotation() {
    rotation = fmod(rotation, rotation_max);
    // fmod returns x in range <-rotation_max, rotation_max>
    if(rotation < 0) {
        rotation = rotation + rotation_max;
    }
}
