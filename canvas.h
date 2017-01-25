/*! \file  canvas.h
    \brief Contains canvas and assocciated entities.
*/

#ifndef CANVAS_H
#define CANVAS_H

#include <QMainWindow>
#include <QWidget>

#include <QPainter>
#include <QtGui>
#include <memory>
#include "utils.h"
#include "element.h"
#include "panel.h"


//! Cursor action mode, may imply transaction.
enum class CursorMode {
    None,     /*!< Neutral mode, no transaction. */
    Element,  /*!< Single puzzle piece active, transactional operations for one piece. */
    Space     /*!< (FUTURE) Multiple puzzle pieces active, transactional operations for multiple pieces. */
};

//! Place to solve the puzzle in.
/*!
  Responsible for main part of the game: drawing, moving elements.
  It also sends messages to other modules to execute more elaborate
  actions (ex. check whenever the puzzle is solved).
 */
class Canvas : public QWidget
{
    Q_OBJECT
public:
    //!Creates basic Canvas Widget
    /*!
      \param   parent  parent widget for canvas, decides where to put widget in the main window.
     */
    explicit Canvas(QWidget *parent = 0);

    //! Checks whenever the element's position is correct considering other elements on canvas.
    /*!
      \param   nel  puzzle piece to be checked
      \return  true if element's position is valid, false if not
     */
    bool elementPositionValid(const Element & nel);

protected:
    //! Handles paint event and drawing using QPainter.
    /*!
      \param   event   event to handle
     */
    void paintEvent(QPaintEvent *event);

public:
    //! Handles key pressing.
    /*!
      \param   event  event to handle
     */
    void keyPressEvent(QKeyEvent *event);

    //! Accepts the ownership of a puzzle and starts its placing procedure.
    /*!
      \param   el puzzle piece being delivered
     */
    void acceptForDrag(std::unique_ptr<Element>&& el);

    //! All the puzzle pieces on canvas.
    utils::unique_vector<Element> elementsOnCanvas;

public slots:

    //! Handles save file button signal.
    void saveFile();

private:
    //! Saves current state of canvas to an XML file.
    void SaveXMLFile();

    //! Weak reference to the panel.
    Panel *panel;

    //! Borrowing of the puzzle piece in current transaction.
    Element* actualEl;

    //! Safe copy to operate during transaction.
    std::unique_ptr<Element> dragEl;  // use std::optional in c++17

    //! Mode the cursor is in, may imply a transaction.
    CursorMode cursorMode;

    //! Point the transaction started in.
    QPointF dragPos;

    //! Shift between transaction starting point and the center of the manipulated group.
    QPointF dragDiffVec;

    //! Starts a drag mode transaction.
    /*!
      \param   el   puzzle piece to use inside of transaction.
      \param   pos  position to start drag from
     */
    void startDrag(std::unique_ptr<Element>& el, QPointF pos);

    //! Aborts transaction for puzzle piece from shapelist.
    /*!
      \see     sendToShapeList
      \see     resetCursorMode
     */
    void revertElemToShapeList();

    //! Transfers puzzle piece to shapelist.
    void sendToShapeList(Element* to_send);

    //! Ends any transaction and sets cursor mode to neutral.
    void resetCursorMode();

    //! (FUTURE) Select rectangular space.
    void selectRect(const QRectF & rec);

    //! Finds best position fit for puzzle piece operated.
    void getBestFit();

    //! Finds directed angle between vectors in 2D.
    /*!
      \param   a  vector starting the angle
      \param   b  vector ending the angle
      \returns directed angle value in degrees
     */
    static qreal getAngle(const QVector2D & a, const QVector2D & b);

    //! Finds minimal distance between puzzle piece point and the point from one of the other pieces.
    /*!
      \param[in]   them        puzzle piece considered
      \param[out]  their_poly  polygon of the puzzle containing the other point
      \param[out]  their_idx   index of the other point in its puzzle
      \param[out]  our_poly    polygon of the puzzle considered
      \param[out]  our_idx     index of the point in its puzzle
      \returns distance found
     */
    qreal findMinLen(Element **them, QPolygonF *their_poly, int *their_idx,
                                     QPolygonF *our_poly,   int *our_idx  ) const;

    //! Handles mouse pressing event (drag&drop)
    /*!
      \param   event  event to handle
     */
    void mousePressEvent(QMouseEvent *event);

    //! handles mouse moving (drag&drop)
    /*!
      \param   event  event to handle
     */
    void mouseMoveEvent(QMouseEvent *event);

    //! handles mouse relasing (drag&drop)
    /*!
      \param   event  event to handle
     */
    void mouseReleaseEvent(QMouseEvent *event);

    //! handles scrolling (rotate while drag&drop)
    /*!
      \param   event  event to handle
     */
    void wheelEvent(QWheelEvent *event);
};

#endif // CANVAS_H
