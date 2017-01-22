#ifndef PANEL_H
#define PANEL_H


#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include"element.h"

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);
    Panel(int width, int height, QWidget *parent = 0);
    void ReadXMLFile();
    void setCanvasSize(int width, int height) {cwidth = width; cheight = height;}

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    std::vector<Element> elementsOnPanel;
    int cwidth;
    int cheight;
};


#endif // PANEL_H
