#include <QDrag>
#include <QMouseEvent>
#include <memory>
#include "utils.h"
#include "canvas.h"
#include "mainwindow.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    elementsOnCanvas(utils::make_unique_vector<Element>(
      std::move(std::make_unique<Square>       (150,  50)),
      std::move(std::make_unique<TriangleMid>  (150, 200)),
      std::move(std::make_unique<TriangleSmall>(150, 300)),
      std::move(std::make_unique<Rhombus>      (450,  50))
    )) {
    setMouseTracking(true);
    cursorMode = CursorMode::None;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    for(auto& el : elementsOnCanvas) {
        if(el.get() != actualEl) {
            el->draw(painter);
        }
    }

    if(actualEl != nullptr) {
        dragEl->draw(painter);
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
                startDrag(el, QPointF(event->pos()));
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
}

bool Canvas::elementPositionValid(const Element & nel) {
    // invalid if outside of the canvas
    QRect rec = contentsRect();
    if(!nel.getRealPoly().boundingRect().intersects(rec)) {
        return false;
    }

    // invalid if intersects with others
    bool is_ok = true;
    for(auto& el : elementsOnCanvas) {
        if(el.get() != actualEl && nel.intersects(*el)) {
            is_ok = false;
            break;
        }
    }
    return is_ok;
}

void Canvas::selectRect(const QRectF & rec) {
    //TODO: implement
}

void Canvas::revertElemToShapeList() {
    // if is valid, let it be
    // else send it back to shapeslist
    actualEl->setValid(true);
    sendToShapeList(actualEl);
    actualEl = nullptr;
    cursorMode = CursorMode::None;
    repaint();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if(cursorMode == CursorMode::Element) {
        dragPos = event->pos();
        dragEl->setCenterPoint(event->pos() + dragDiffVec);
        dragEl->setValid(elementPositionValid(*dragEl));
        repaint();
    }
}

void Canvas::wheelEvent(QWheelEvent *event) {
    if(actualEl != nullptr) {
        int rot_val = event->delta() / 16;

        dragEl->rotateRight(rot_val);
        dragEl->setValid(elementPositionValid(*dragEl));
        repaint();
    }
}

void Canvas::keyPressEvent(QKeyEvent *event) {
    if(actualEl != nullptr) {
        if(event->key() == Qt::Key_X) {
            revertElemToShapeList();
            return;
        }

        switch (event->key()) {
            case Qt::Key_R:  dragEl->rotateRight();  break;
            case Qt::Key_E:  dragEl->rotateLeft ();  break;
            case Qt::Key_W:  dragEl->moveUp     ();  break;
            case Qt::Key_S:  dragEl->moveDown   ();  break;
            case Qt::Key_A:  dragEl->moveLeft   ();  break;
            case Qt::Key_D:  dragEl->moveRight  ();  break;
            case Qt::Key_M:  dragEl->mirrorEl   ();  break;
        }

        dragEl->setValid(elementPositionValid(*dragEl));
        dragDiffVec = dragEl->centerPoint() - dragPos;
        repaint();
    }
}


void Canvas::acceptForDrag(std::unique_ptr<Element>&& el) {
    if(cursorMode != CursorMode::None) {
        resetCursorMode();
    }
    el->setValid(false);
    QPointF pos = el->centerPoint();
    elementsOnCanvas.push_back(std::move(el));
    // if there is active drag, it is automatically aborted
    startDrag(elementsOnCanvas.back(), pos);
}

void Canvas::startDrag(std::unique_ptr<Element>& el, QPointF pos) {
    actualEl = el.get();

    std::unique_ptr<Element> dragElNew(Element::copy(*actualEl));
    dragEl.swap(dragElNew);

    dragPos = pos;
    dragDiffVec = actualEl->centerPoint() - dragPos;
    cursorMode = CursorMode::Element;
    setFocus();
}

void Canvas::sendToShapeList(Element* to_send) {
    auto it = std::find_if(elementsOnCanvas.begin(),
                           elementsOnCanvas.end()  ,
                           [to_send](const std::unique_ptr<Element> &el) {
                             return el.get() == to_send;
                           });
    int idx = it - elementsOnCanvas.begin();
    // not checking `it != myVec.end()` as it's a precondition
    std::unique_ptr<Element> el;
    el.swap(elementsOnCanvas[idx]);
    elementsOnCanvas.erase(it);
    MainWindow::main()->shapesList()->acceptForList(std::move(el));
}
