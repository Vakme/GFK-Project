#ifndef CANVAS_H
#define CANVAS_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include "element.h"
#include "dragdrop.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    std::vector<Element *> elementsOnCanvas;
};

#endif // CANVAS_H
