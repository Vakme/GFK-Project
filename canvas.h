#ifndef CANVAS_H
#define CANVAS_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include "utils.h"
#include "element.h"

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
    bool onDrag;
    QPointF dragPos;
    QPointF dragDiffVec;
    void startDrag(std::unique_ptr<Element>& el, QPointF pos);
    void revertElemToShapeList();
    void sendToShapeList(Element* to_send);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // CANVAS_H
