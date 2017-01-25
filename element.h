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
enum class ElType { SQUARE, TRIANGLE_BIG, TRIANGLE_MID, TRIANGLE_SMALL, RHOMBUS };

/*! \class  Element
 *  \brief  Basic puzzle piece
 */
class Element
{
public:

    /*!
     * \fn      Element
     * \brief
     */
    Element() = delete;
    Element& operator =(Element && el);
    static Element* copy(const Element & el) { return new Element(el); }
    void swap(Element *other);

    bool contains(const QPointF & point) const;
    void draw(QPainter * painter);

    const ElType typ;

    /*!
     * \fn      checkXML
     * \brief   Matches an XML describin a single element into object (Element)
     * \param   Rxml  XMLReader Object - current position in XML file
     * \return  unique_ptr ready to put into elements on panel vector
     * \see     ReadXmlFile (class Panel)
     */
    static std::unique_ptr<Element> checkXML(QXmlStreamReader &Rxml);


    /*!
     * \fn      centerPoint
     * \brief   Getter for _centerPoint
     * \return  copy of _centerPoint
     */
    QPointF centerPoint() const;

    /*!
     * \fn      setCenterPoint
     * \brief   Setter for _centerPoint
     * \param   center  Setting point
     */
    void setCenterPoint(QPointF center);
    const QColor color;
    QPolygonF getRealPoly(qreal x, qreal y) const;
    QPolygonF getRealPoly() const;

    bool isValid() const;
    void setValid(bool flag);
    bool intersects(const Element& other) const;

    /*!
     * \fn      rotateLeft
     * \brief   Rotates element left
     * \param   val  Angle value (degrees) - default 1
     */
    void rotateLeft (int val = 1);

    /*!
     * \fn      rotateRight
     * \brief   Rotates element right
     * \param   val  Angle value (degrees) - default 1
     */
    void rotateRight(int val = 1);

    /*!
     * \fn      moveUp
     * \brief   Moves element up
     * \param   val  Translation value (pixels) - default 1
     */
    void moveUp     (int val = 1);

    /*!
     * \fn      moveDown
     * \brief   Moves element down
     * \param   val  Translation value (pixels) - default 1
     */
    void moveDown   (int val = 1);


    /*!
     * \fn      moveLeft
     * \brief   Moves element left
     * \param   val  Translation value (pixels) - default 1
     */
    void moveLeft   (int val = 1);


    /*!
     * \fn      rotateRight
     * \brief   Moves element rigth
     * \param   val  Translation value (pixels) - default 1
     */
    void moveRight  (int val = 1);

    /*!
     * \fn      mirrorEl
     * \brief   Changes mirror value (makes mirrored version of element)
     */
    void mirrorEl();

    const QPixmap & getBitmap();

    /*!
     * \fn      getPoly
     * \brief   Getter for polygon containing all points
     * \return  copy of points QPolygonF
     */
    QPolygonF getPoly() { return points; }

    /*!
     * \fn      getRot
     * \brief   Gets rotation value (degrees)
     * \return  copy of rotation (qreal)
     */
    qreal getRot() { return rotation; }

    /*!
     * \fn      getMir
     * \brief   Gets mirror value
     * \return  copy of mirror value (bool)
     */
    bool getMir() { return mirror; }

    /*!
     * \fn      getMirable
     * \brief   Gets mirrorable value (saying if element may be mirrored)
     * \return  true if element may be mirrored, false if not
     */
    bool getMirable() { return mirrorable; }

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
