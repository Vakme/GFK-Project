#ifndef CANVAS_H
#define CANVAS_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include "utils.h"
#include "element.h"
#include "dragdrop.h"
#include "panel.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);

protected:
    void paintEvent(QPaintEvent *event);

public:
    void keyPressEvent(QKeyEvent *event);

    utils::unique_vector<Element> elementsOnCanvas;

signals:

public slots:

private:
    Element* actualEl;
    void SaveXMLFile();
    Panel *panel;
    void compare();
};

#endif // CANVAS_H
