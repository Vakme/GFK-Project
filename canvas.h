#ifndef CANVAS_H
#define CANVAS_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include "utils.h"
#include "element.h"

enum class CursorMode { None, Element, Space };

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);
    bool elementPositionValid(const Element & nel);

protected:
    void paintEvent(QPaintEvent *event);

public:
    void keyPressEvent(QKeyEvent *event);
    void acceptForDrag(std::unique_ptr<Element>&& el);

signals:

public slots:

private:
    utils::unique_vector<Element> elementsOnCanvas;
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

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // CANVAS_H
