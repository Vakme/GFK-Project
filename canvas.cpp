#include <QDrag>
#include <QMouseEvent>
#include <memory>
#include<QObject>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QFileDialog>
#include "utils.h"
#include "canvas.h"
#include "mainwindow.h"
#include "panel.h"
#include "comparator.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    elementsOnCanvas(utils::make_unique_vector<Element>(
      std::move(std::make_unique<Square>       (150,  50)),
      std::move(std::make_unique<TriangleMid>  (150, 200)),
      std::move(std::make_unique<TriangleSmall>(150, 300)),
      std::move(std::make_unique<Rhombus>      (450,  50))
    )) {
    panel = this->parent()->findChild<Panel*>(QString("panel"));
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

        getBestFit();

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
    if(event->key() == Qt::Key_O)
        SaveXMLFile();
    else if(event->key() == Qt::Key_P) {
        panel->setCanvasSize(this->width(), this->height());
        panel->ReadXMLFile();
    }
    else if(event->key() == Qt::Key_C) {
        Comparator::compareElems(panel->elementsOnPanel, elementsOnCanvas);
    }
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



void Canvas::getBestFit() {
    qreal max_jump_len   = 10.0;
    qreal max_jump_angle = 10.0;
    if(elementsOnCanvas.size() < 2) {
        return;
    }

    Element * them  = nullptr;
    QPolygonF their_poly, our_poly;
    int i, j;

    qreal min_len = findMinLen(&them, &their_poly, &i, &our_poly, &j);
    if(min_len > max_jump_len) {
        return;
    }

    int ip, jp, im, jm;

    ip =  (i != their_poly.size()-1) ? i + 1 : 0;
    jp =  (j != our_poly.size()  -1) ? j + 1 : 0;
    im =  (i != 0) ? (i - 1) : their_poly.size()-1;
    jm =  (j != 0) ? (j - 1) : our_poly.size()  -1;

    std::vector<qreal> angle;

    angle.push_back(getAngle(QVector2D(our_poly[j]   - our_poly[jp]  ),
                             QVector2D(their_poly[i] - their_poly[ip])));

    angle.push_back(getAngle(QVector2D(our_poly[j]   - our_poly[jp]  ),
                             QVector2D(their_poly[i] - their_poly[im])));

    angle.push_back(getAngle(QVector2D(our_poly[j]   - our_poly[jm]  ),
                             QVector2D(their_poly[i] - their_poly[ip])));

    angle.push_back(getAngle(QVector2D(our_poly[j]   - our_poly[jm]  ),
                             QVector2D(their_poly[i] - their_poly[im])));

    int pos;
    {
        std::vector<qreal> angle_abs;
        std::transform(angle.begin(), angle.end(),
                       std::back_inserter(angle_abs),
                       [](qreal x)->qreal { return std::fabs(x); });

        pos = std::distance(angle_abs.begin(),
                            std::min_element(angle_abs.begin(), angle_abs.end()));
    }

    qreal ang = angle[pos];
    if(std::fabs(ang) < max_jump_angle) {
        dragEl->rotateRight(ang);
        dragEl->setCenterPoint(dragEl->centerPoint() + their_poly[i] - our_poly[j]);
    }
}

qreal Canvas::getAngle(const QVector2D & a, const QVector2D & b) {
    qreal ang_a = atan(a.x() / a.y());
    if(a.x() * ang_a < 0) { ang_a -= 1.0; }
    qreal ang_b = atan(b.x() / b.y());
    if(b.x() * ang_b < 0) { ang_b -= 1.0; }
    return (ang_a - ang_b) * (180.0 / M_PI);
}

qreal Canvas::findMinLen(Element **them, QPolygonF *their_poly, int *their_idx,
                                         QPolygonF *our_poly,   int *our_idx  ) const {
    qreal min_len = 10000;

    // find min_len
    *our_poly = dragEl->getRealPoly();
    for(auto& el : elementsOnCanvas) {
        if(el.get() != actualEl) {
            QPolygonF i_poly = el->getRealPoly();
            for(int i = 0; i < i_poly.size(); ++i) {
                for(int j = 0; j < our_poly->size(); ++j) {
                    qreal len = QVector2D(i_poly[i] - (*our_poly)[j]).length();
                    if(len < min_len) {
                        min_len = len;
                        *them = el.get();
                        *their_poly = i_poly;
                        *their_idx  = i;
                        *our_idx    = j;
                    }
                }
            }
        }
    }

    return min_len;
}

void Canvas::SaveXMLFile()
{

    QString filename = QFileDialog::getSaveFileName(this,
                                       tr("Save Xml"), ".",
                                       tr("Xml files (*.xml)"));
    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("elements");
    for(auto & el : elementsOnCanvas) {
        xmlWriter.writeEmptyElement("element");
        xmlWriter.writeAttribute("Type", QString::number(static_cast<int>(el->typ) ));
        xmlWriter.writeAttribute("Rotation",  QString::number(el->getRot()));
        xmlWriter.writeAttribute("x", QString::number(el->centerPoint.x()));
        xmlWriter.writeAttribute("y", QString::number(el->centerPoint.y()));
        if(el->getMirable())
            xmlWriter.writeAttribute("Mirror",  QString::number(el->getMir()));
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

        file.close();
}
