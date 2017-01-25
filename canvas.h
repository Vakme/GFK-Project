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

enum class CursorMode { None, Element, Space };

/*!
 *  \class  Canvas
 *  \brief  Class responsible for main part of the game: drawing, moving elements, handling events. This is the place where you solve the puzzles.
 */
class Canvas : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \fn      Canvas
     * \brief   Constructor needed to set up Widget
     * \param   parent  Parent widget for canvas (decides where to put widget in the main window)
     */
    explicit Canvas(QWidget *parent = 0);

    bool elementPositionValid(const Element & nel);

protected:
    /*!
     * \fn      paintEvent
     * \brief   Function handling paint event and drawing using QPainter
     * \param   event  QPaintEvent to handle
     */
    void paintEvent(QPaintEvent *event);

public:
    /*!
     * \fn      keyPressEvent
     * \brief   Function handling key events and calling adequate functions
     * \param   event  QKeyEvent to handle
     */
    void keyPressEvent(QKeyEvent *event);

    void acceptForDrag(std::unique_ptr<Element>&& el);

    utils::unique_vector<Element> elementsOnCanvas;

signals:

public slots:
    void saveFile();

private:

    /*!
     * \fn      SaveXMLFile
     * \brief   Function saving current state of canvas to a XML file
     */
    void SaveXMLFile();

    Panel *panel;

    Element* actualEl;
    std::unique_ptr<Element> dragEl;  // use std::optional in c++17
    CursorMode cursorMode;
    QPointF dragPos;
    QPointF dragDiffVec;
    void startDrag(std::unique_ptr<Element>& el, QPointF pos);
    void revertElemToShapeList();
    void sendToShapeList(Element* to_send);

    void resetCursorMode();

    void selectRect(const QRectF & rec);

    void getBestFit();
    static qreal getAngle(const QVector2D & a, const QVector2D & b);
    qreal findMinLen(Element **them, QPolygonF *their_poly, int *their_idx,
                                     QPolygonF *our_poly,   int *our_idx  ) const;

    /*!
     * \fn      mousePressEvent
     * \brief   Function handling pressing mouse event (drag&drop)
     * \param   event  QMouseEvent to handle
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \fn      mouseMoveEvent
     * \brief   Function handling moving mouse (drag&drop)
     * \param   event  QMouseEvent to handle
     */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
     * \fn      mouseReleaseEvent
     * \brief   Function handling relasing mouse event (drag&drop)
     * \param   event  QMouseEvent to handle
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
     * \fn      wheelEvent
     * \brief   Function handling scrolling (rotate while drag&drop)
     * \param   event  QWheelEvent to handle
     */
    void wheelEvent(QWheelEvent *event);
};

#endif // CANVAS_H
