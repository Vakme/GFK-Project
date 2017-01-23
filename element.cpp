#include "element.h"
#include <QTransform>
#include<memory>
#include <QStringList>
#include <QTextStream>
#include <QXmlStreamReader>

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


Element::Element(int typ, QPointF centerPoint, const QPolygonF points,
                 qreal rotation, bool mirror, bool mirrorable, qreal rotation_max) :
    typ(static_cast<ElType>(typ)), centerPoint(centerPoint), points(points), rotation(rotation), mirror(mirror), mirrorable(mirrorable), rotation_max(rotation_max) {
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
    isChanged = true;
}

void Element::rotateRight() {
    rotation += 1;
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
    if(mirrorable)
        mirror = !mirror;

    isChanged = true;
}



std::unique_ptr<Element> Element::checkXML(QXmlStreamReader &Rxml) {

    qreal rotation = 0, rotation_max = 360;
    bool mirror = false, mirrorable = false;
    int typ = 1;
    float x = 0, y = 0;
    QVector<QPointF> pointVec;

    QXmlStreamAttributes attrs;

        qDebug() << "Inside element";
    if(Rxml.name() == "element") {
        attrs = Rxml.attributes();
        if(attrs.hasAttribute("Type") && attrs.hasAttribute("Rotation") && attrs.hasAttribute("x") && attrs.hasAttribute("y"))
        {
            typ = attrs.value("Type").toInt();
            rotation = attrs.value("Rotation").toDouble();
            x = attrs.value("x").toInt();
            y = attrs.value("y").toInt();
            if(attrs.hasAttribute("Mirror"))
                mirror = Rxml.readElementText().toInt();
            else
                mirror = false;
        }
        else
            qDebug() << "Invalid file";

        Rxml.readNext();
        qDebug() << Rxml.name();
        /*while(!Rxml.isEndElement()) {
            qDebug() << Rxml.attributes().value("x") << " " << Rxml.attributes().value("y");
            if(Rxml.name() == "Point" && Rxml.attributes().hasAttribute("x") &&  Rxml.attributes().hasAttribute("y"))
                pointVec.push_back(QPointF(Rxml.attributes().value("x").toFloat(), Rxml.attributes().value("y").toFloat()));
            Rxml.readNext();
        }*/
    }
    if(static_cast<ElType>(typ) == ElType::TRIANGLE_BIG) {
        auto elPtr = std::move(std::make_unique<TriangleBig>(x, y));
        elPtr -> rotation = rotation;
        return elPtr;
    }
    else if(static_cast<ElType>(typ) == ElType::TRIANGLE_MID) {
        auto elPtr = std::move(std::make_unique<TriangleMid>(x, y));
        elPtr -> rotation = rotation;
        return elPtr;
    }
    else if(static_cast<ElType>(typ) == ElType::TRIANGLE_SMALL) {
        auto elPtr = std::move(std::make_unique<TriangleSmall>(x, y));
        elPtr -> rotation = rotation;
        return elPtr;
    }
    else if(static_cast<ElType>(typ) == ElType::SQUARE) {
        auto elPtr = std::move(std::make_unique<Square>(x, y));
        elPtr -> rotation = rotation;
        return elPtr;
    }
    else if(static_cast<ElType>(typ) == ElType::RHOMBUS) {
        auto elPtr = std::move(std::make_unique<Rhombus>(x, y));
        elPtr -> rotation = rotation;
        elPtr -> mirror = mirror;
        return elPtr;
    }
}
