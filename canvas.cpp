#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    elementsOnCanvas({
      new Square       (150,  50),
      new TriangleMid  (150, 200),
      new TriangleSmall(150, 300),
      new Rhombus      (450,  50)
    }) {}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    qDebug() << "NOSZRYSUJ";
    for(Element *element : elementsOnCanvas) {
        element->draw(painter);
    }
    delete painter;
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    for(auto& v : elementsOnCanvas) {
        qDebug() << "(next)";
        if(v->contains(event->pos())) {
            actualEl = v;
            //qDebug() << event->pos() << " on " << static_cast<int>(v->typ);
            DragDrop drop(this);
            drop.mousePressEvent(event, actualEl, QString("canvas"));
            return;
        }
    }
    actualEl = nullptr;
    qDebug() << "NoPe! " << event->pos();

    DragDrop drop(this);
    drop.mousePressEvent(event, actualEl, QString("canvas"));
}


void Canvas::keyPressEvent(QKeyEvent *event) {
    if(actualEl == nullptr)
        qDebug() << "nope!";
    else {
        switch (event->key()) {
        case Qt::Key_R:
            qDebug() << "R";
            actualEl -> rotateRight();
            break;
        case Qt::Key_E:
            qDebug() << "E";
            actualEl -> rotateLeft();
            break;
        case Qt::Key_W:
            qDebug() << "W";
            actualEl -> moveUp();
            break;
        case Qt::Key_S:
            qDebug() << "S";
            actualEl -> moveDown();
            break;
        case Qt::Key_A:
            qDebug() << "A";
            actualEl -> moveLeft();
            break;
        case Qt::Key_D:
            qDebug() << "D";
            actualEl -> moveRight();
            break;
        case Qt::Key_M:
            qDebug() << "M";
           // actualEl -> mirrorEl();       //Funkcja do odwracania jest gotowa, ale nie stestowana, a ja ledwo na oczy patrzę, jutro skończymy
            break;
        default:
            break;
        }
        repaint();
    }
}
