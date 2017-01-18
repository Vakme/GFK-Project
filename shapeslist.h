#ifndef SHAPESLIST_H
#define SHAPESLIST_H

#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include"element.h"
#include "dragdrop.h"

class ShapesList : public QWidget
{
    Q_OBJECT
public:
    explicit ShapesList(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    std::vector<Element *> elementsOnList;
};

#endif // SHAPESLIST_H
