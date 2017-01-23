#ifndef PANEL_H
#define PANEL_H


#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include"element.h"
#include "utils.h"

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);
    Panel(int width, int height, QWidget *parent = 0);
    void ReadXMLFile();
    void setCanvasSize(int width, int height) {cwidth = width; cheight = height;}
    utils::unique_vector<Element> elementsOnPanel;

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    int cwidth;
    int cheight;
};


#endif // PANEL_H
