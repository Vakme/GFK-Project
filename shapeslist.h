#ifndef SHAPESLIST_H
#define SHAPESLIST_H

#include <QWidget>
#include <QPainter>
#include <QtGui>

class ShapesList : public QWidget
{
    Q_OBJECT
public:
    explicit ShapesList(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // SHAPESLIST_H
