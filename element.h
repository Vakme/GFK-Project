/*! \file  element.h
    \brief Contains puzzle piece class hierarchy.
*/

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

//! Puzzle piece type
enum class ElType {
    SQUARE,          /*!< Square composed of two small triangles */
    TRIANGLE_BIG,    /*!< Halfsquare being a quarter of the puzzlebox */
    TRIANGLE_MID,    /*!< Halfsquare being two times smaller than half of the puzzlebox */
    TRIANGLE_SMALL,  /*!< Halfsquare being two times smaller than big triangle */
    RHOMBUS          /*!< Parallelogram composed of two small triangles */
};

//! Puzzle piece
/*! Contains all the characteristics needed to distinguish puzzle piece shapes. */
class Element {
public:
    //! Default constructor is unwanted as it does not specify type.
    Element() = delete;

    //! Moves the puzzle piece, i.e. changes its ownership.
    /*!
      \param   puzzle piece to be moved
      \return  reference to the moved puzzle piece
     */
    Element& operator =(Element && el);

    //! Explicitly makes a deep copy of the puzzle.
    /*!
      \param   reading borrow for making the copy
      \return  made copy
     */
    static Element* copy(const Element & el) { return new Element(el); }

    //! Swaps puzzle pieces of the same type.
    /*! Elements have to be of the same type. Color and other non-describtive
        attributes will not be swapped.
     */
    void swap(Element *other);

    //! Checks whenever given point is contained in the puzzle piece.
    /*!
      \param   point  point to be checked
      \return  true if the point is contained in the puzzle piece, false otherwise
     */
    bool contains(const QPointF & point) const;

    //! Draws the puzzle piece.
    /*!
      \param   painter  painter to be used in drawing
     */
    void draw(QPainter * painter);

    //! Puzzle piece type.
    const ElType typ;

    //! Parses XML describing puzzle piece.
    /*!
      \param   Rxml  XMLReader Object -- current position in XML file
      \return  puzzle piece described in parsed XML
      \see     Panel::ReadXmlFile
     */
    static std::unique_ptr<Element> checkXML(QXmlStreamReader &Rxml);


    //! Gets center point coordinates.
    /*!
      \return  coordinates
     */
    QPointF centerPoint() const;

    //! Sets center point coordinates.
    /*!
      \param   center  point value to be set
     */
    void setCenterPoint(QPointF center);

    //! Puzzle piece color.
    const QColor color;

    //! Returns puzzle piece's polygon after all the transformation.
    /*!
      \return  puzzle piece's polygon
      \see getRealPoly(qreal x, qreal y)
     */
    QPolygonF getRealPoly() const;

    //! Returns puzzle piece's polygon after all the transformation.
    /*!
      \param   x  polygon horizontal shift
      \param   y  polygon vertical shift
      \return  puzzle piece's polygon
     */
    QPolygonF getRealPoly(qreal x, qreal y) const;

    //! Checks whenever puzzle piece has been marked as valid.
    /*! i.e. getter for valid.
      \return  true if has been marked as valid, false otherwise
     */
    bool isValid() const;

    //! Marks puzzle as either valid or invalid.
    /*! i.e. setter for valid. */
    void setValid(bool flag);

    //! Checks whenever two puzzle pieces intersect
    /*!
      \param   other  puzzle piece to check intersection with
      \return  true if the puzzle pieces intersect, false otherwise
     */
    bool intersects(const Element& other) const;

    //! Rotates puzzle piece left
    /*!
      \param   val  angle value in degrees, defaults to 1
     */
    void rotateLeft (qreal val = 1.0);

    //! Rotates puzzle piece right
    /*!
      \param   val  angle value in degrees, defaults to 1
     */
    void rotateRight(qreal val = 1.0);

    //! Translates puzzle piece up
    /*!
     * \param   val distance value in pixels, defaults to 1
     */
    void moveUp     (qreal val = 1.0);

    //! Translates puzzle piece down
    /*!
     * \param   val distance value in pixels, defaults to 1
     */
    void moveDown   (qreal val = 1.0);

    //! Translates puzzle piece left
    /*!
     * \param   val distance value in pixels, defaults to 1
     */
    void moveLeft   (qreal val = 1.0);

    //! Translates puzzle piece right
    /*!
     * \param   val distance value in pixels, defaults to 1
     */
    void moveRight  (qreal val = 1.0);

    //! Mirrors puzzle piece
    void mirrorEl();

    //! Returns updated bitmap represanting puzzle piece.
    /*! i.e. getter for bitmap
      \return reference to the bitmap
     */
    const QPixmap & getBitmap();

    //! Returns basic polygon for the puzzle piece.
    /*!
      \return  copy of the basic polygon
     */
    QPolygonF getPoly() { return points; }

    //! Gets rotation.
    /*!
      \return  rotation value
     */
    qreal getRot() { return rotation; }

    //! Gets for mirror value.
    /*!
      \return  mirror value
     */
    bool getMir() { return mirror; }

    //! Gets puzzle type's mirrorable attribute, i.e. whenever puzzle piece can be mirrored.
    /*!
      \return  true if puzzle piece can be mirrored, false otherwise
     */
    bool getMirable() { return mirrorable; }

protected:
    //! Creates detailed puzzle piece
    /*!
      \param typ           number representing puzzle type
      \param centerPoint   puzzle center point
      \param points        puzzle shape's polygon
      \param rotation_max  whenever can be mirrored or not
      \param mirrorable    modulus base for rotation
     */
    Element(ElType typ, QPointF centerPoint, const QPolygonF& points,
            qreal rotation_max = 360, bool mirrorable = false);

    //! Creates detailed puzzle piece with state
    /*!
      \param typ           number representing puzzle type
      \param centerPoint   puzzle center point
      \param points        puzzle shape's polygon
      \param rotation      rotation in degrees
      \param mirror        whenever mirrored or not
      \param mirrorable    whenever can be mirrored or not
      \param rotation_max  modulus base for rotation
     */
    Element(int typ, QPointF centerPoint, const QPolygonF points, qreal rotation, bool mirror, bool mirrorable, qreal rotation_max);

private:
    //! Private copy constructor used in explicit deep copying.
    Element(const Element & el) = default;

    //! Lazily updates bitmap.
    void updateBitmap();

    //! Fits rotate value into rotate modulus range.
    void reduceRotation();

    //! Generates puzzle colors infinitely.
    /*!
      \return next generated color
     */
    static QColor nextColor();

    //! Center point of the puzzle shape.
    QPointF _centerPoint;

    //! Puzzle piece shape.
    const QPolygonF points;

    //! Rotation modulus base.
    const qreal rotation_max;

    //! Rotation value in degrees.
    qreal rotation;

    //! Whenever puzzle piece can be mirrored or not.
    const bool mirrorable;

    //! Whenever puzzle piece is mirrored or not.
    bool mirror;

    //! Color bitmap.
    QPixmap bitmap;

    //! Flag for marking the puzzle as either valid or not.
    bool valid;

    //! Flag for marking whenever repaint is needed or not.
    /*! Turns true each time the bitmap should be repaint and set false after repaint. */
    bool isChanged;
};


//! Element with constexpr value injected into typ field.
template<ElType _typ>
class ElementKindBase : public Element {
public:
    //! Perfect forwarding of all the constructors into analogues with typ field injected.
    template<typename... Args>
    ElementKindBase(Args... args) : Element(_typ, std::forward<Args>(args)...) {}
};

//! Element with injected typ field and custom constructor.
template<ElType _typ>
class ElementKind : public ElementKindBase<_typ> {
public:
    //! Custom constructor to be implemented by specializations.
    ElementKind(QPointF centerPoint);

    //! Constructor variant using bare coordinates instead of center point.
    ElementKind(double x, double y) : ElementKind(QPointF(x, y)) {}
};

using Square        = ElementKind<ElType::SQUARE>;          /*!< Square shape puzzle piece.         */
using TriangleBig   = ElementKind<ElType::TRIANGLE_BIG>;    /*!< Big triangle shape puzzle piece.   */
using TriangleMid   = ElementKind<ElType::TRIANGLE_MID>;    /*!< Mid triangle shape puzzle piece.   */
using TriangleSmall = ElementKind<ElType::TRIANGLE_SMALL>;  /*!< Small triangle shape puzzle piece. */
using Rhombus       = ElementKind<ElType::RHOMBUS>;         /*!< Parallelogram shape puzzle piece. */


#endif // ELEMENT_H
