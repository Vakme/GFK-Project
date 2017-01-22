#include <QDrag>
#include <QMouseEvent>
#include <memory>
#include "utils.h"
#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    elementsOnCanvas(utils::make_unique_vector<Element>(
      std::move(std::make_unique<Square>       (150,  50)),
      std::move(std::make_unique<TriangleMid>  (150, 200)),
      std::move(std::make_unique<TriangleSmall>(150, 300)),
      std::move(std::make_unique<Rhombus>      (450,  50))
    )) {

    onDrag = false;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    for(auto& element : elementsOnCanvas) {
        element->draw(painter);
    }
    delete painter;
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if(event->button() & Qt::MidButton  &&  cursorMode == CursorMode::Element) {
        dragEl->mirrorEl();
        repaint();
        return;
    }

    if(event->button() == Qt::LeftButton) {
        resetCursorMode();
        for(auto& el : elementsOnCanvas) {
            if(el->contains(event->pos())) {
                actualEl = el.get();
                dragPos = event->pos();
                dragDiffVec = actualEl->centerPoint() - dragPos;
                onDrag = true;
                return;
            }
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if( !(event->button() & Qt::MidButton  &&  cursorMode == CursorMode::Element) ) {
        resetCursorMode();
    }
}

void Canvas::resetCursorMode() {
    switch(cursorMode) {
        case CursorMode::None:
            return;
        case CursorMode::Element:
            if(dragEl->isValid()) {
                actualEl->swap(dragEl.get());
            }
            else if( !(actualEl->isValid()) ) {
                revertElemToShapeList();
                return;
            }
            actualEl = nullptr;
            break;
        case CursorMode::Space:
            //TODO: implement
            break;
    }
    cursorMode = CursorMode::None;
    repaint();
>>>>>>> dd145be... (poprawienie scrolla)
}

bool Canvas::elementPositionValid(Element *nel) {
    bool is_ok = true;
    for(auto& el : elementsOnCanvas) {
        if(el.get() != nel && nel->intersects(*el)) {
            is_ok = false;
            break;
        }
    }
    return is_ok;
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if(onDrag) {
        dragPos = event->pos();
        QPointF oldPos = actualEl->centerPoint();
        actualEl->setCenterPoint(event->pos() + dragDiffVec);

        if(elementPositionValid(actualEl)) {
            repaint();
        }
        else {
            actualEl->setCenterPoint(oldPos);
            dragDiffVec = actualEl->centerPoint() - dragPos;
        }
    }
}

void Canvas::wheelEvent(QWheelEvent *event) {
    if(actualEl != nullptr) {
        int rot_val = event->delta() / 8;

        actualEl->rotateRight(rot_val);
        if(event->buttons() | Qt::MidButton) {
            actualEl->mirrorEl();
        }

        if(elementPositionValid(actualEl)) {
            repaint();
        }
        else {
            actualEl->rotateLeft(rot_val);
            if(event->buttons() | Qt::MidButton) {
                actualEl->mirrorEl();
            }
        }
    }
}

void Canvas::keyPressEvent(QKeyEvent *event) {
    if(actualEl != nullptr) {
        QPointF oldPos = actualEl->centerPoint();
        actualEl->setCenterPoint(dragPos + dragDiffVec);
        switch (event->key()) {
            case Qt::Key_R:  actualEl->rotateRight();  break;
            case Qt::Key_E:  actualEl->rotateLeft ();  break;
            case Qt::Key_W:  actualEl->moveUp     ();  break;
            case Qt::Key_S:  actualEl->moveDown   ();  break;
            case Qt::Key_A:  actualEl->moveLeft   ();  break;
            case Qt::Key_D:  actualEl->moveRight  ();  break;
            case Qt::Key_M:  actualEl->mirrorEl   ();  break;
        }

        if(elementPositionValid(actualEl)) {
            repaint();
            dragDiffVec = actualEl->centerPoint() - dragPos;
        }
        else {
            actualEl->setCenterPoint(oldPos);
            switch (event->key()) {
                case Qt::Key_R:  actualEl->rotateLeft ();  break;
                case Qt::Key_E:  actualEl->rotateRight();  break;
                case Qt::Key_W:  actualEl->moveDown   ();  break;
                case Qt::Key_S:  actualEl->moveUp     ();  break;
                case Qt::Key_A:  actualEl->moveRight  ();  break;
                case Qt::Key_D:  actualEl->moveLeft   ();  break;
                case Qt::Key_M:  actualEl->mirrorEl   ();  break;
            }
        }
    }
}



void Canvas::startDrag(QPoint pos, std::unique_ptr<Element>& el) {
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setText("Kabum!");
    drag->setMimeData(mimeData);
    drag->setPixmap(el->getBitmap());
    drag->setHotSpot((pos - el->centerPoint() + QPoint(175,175)).toPoint());

    Qt::DropAction dropAction = drag->exec();
}
