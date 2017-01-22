#include "element.h"
#include <QTransform>

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
    if(mirrorable) {
        mirror = !mirror;
        isChanged = true;
    }
}



Element *Element::checkXML(QXmlStreamReader &Rxml) {

    qreal rotation = 0, rotation_max = 360;
    bool mirror = false, mirrorable = false;
    int typ = 1;
    float x = 0, y = 0;
    QPointF centPoint = QPointF(0,0);
    int r = 0, g = 0, b = 0;
    QVector<QPointF> pointVec;

    QXmlStreamAttributes attrs;

    while((Rxml.name() != "element" && !Rxml.isEndElement())) {
//    if(Rxml.name() == "element")
    if(Rxml.name() == "Rotation") {
            rotation = Rxml.readElementText().toDouble();
            qDebug() << QString::number(rotation);
    }
    else if(Rxml.name() == "Mirror") {
            mirror = Rxml.readElementText().toInt();
            qDebug() << QString::number(mirror);
    }
    else if(Rxml.name() == "Type") {
            typ = Rxml.readElementText().toInt();
            qDebug() << QString::number(typ);
    }
    else if(Rxml.name() == "CenterPoint") {
        Rxml.readNext();
        while(Rxml.name() != "CenterPoint" && !Rxml.isEndElement()) {
            if(Rxml.name() == "x")
                x = Rxml.readElementText().toFloat();
            else if(Rxml.name() == "y")
                y = Rxml.readElementText().toFloat();

            Rxml.readNext();
        }
        centPoint.setX(x);
        centPoint.setY(y);
        qDebug() << "CPoint(" << QString::number(x) << ", " << QString::number(y) << ")";
    }
    else if(Rxml.name() == "Color") {
        Rxml.readNext();
        while(Rxml.name() != "Color" && !Rxml.isEndElement()) {
            if(Rxml.name() == "r")
                r = Rxml.readElementText().toInt();
            else if(Rxml.name() == "g")
                g = Rxml.readElementText().toInt();
            else if(Rxml.name() == "b")
                b = Rxml.readElementText().toInt();

            Rxml.readNext();
        }
        qDebug() << "Color(" << QString::number(r) << ", " << QString::number(g) << ", " << QString::number(b) << ")";

    }

    else if(Rxml.name() == "Points") {
        pointVec.clear();
        Rxml.readNext();
        while(Rxml.name() != "Points" && !Rxml.isEndElement()) {
            if(Rxml.name() == "Point") {
                        Rxml.readNext();
                        while(Rxml.name() != "Point" && !Rxml.isEndElement()) {
                            if(Rxml.name() == "x")
                                x = Rxml.readElementText().toFloat();
                            else if(Rxml.name() == "y")
                                y = Rxml.readElementText().toFloat();

                            Rxml.readNext();
                        }
                        pointVec.push_back(QPointF(x, y));
                        qDebug() << "Point(" << QString::number(x) << ", " << QString::number(y) << ")";
                    }

            Rxml.readNext();
        }
    }
    Rxml.readNext();
    }
    if(typ == 5) {
        mirrorable = true;
        rotation_max = 180;
    }
    else if(typ == 4) {
        mirrorable = false;
        rotation_max = 90;
    }
    else {
        rotation_max = 360;
        mirrorable = false;
    }
    return new Element(typ, centPoint, QPolygonF(pointVec), rotation, mirror, mirrorable, rotation_max);
}
