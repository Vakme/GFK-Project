#include <memory>
#include<QObject>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QFileDialog>
#include "utils.h"
#include "canvas.h"
#include "panel.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    elementsOnCanvas(utils::make_unique_vector<Element>(
      std::move(std::make_unique<Square>       (150,  50)),
      std::move(std::make_unique<TriangleMid>  (150, 200)),
      std::move(std::make_unique<TriangleSmall>(150, 300)),
      std::move(std::make_unique<Rhombus>      (450,  50))
    )) {
    panel = this->parent()->findChild<Panel*>(QString("panel"));
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    qDebug() << "NOSZRYSUJ";
    for(auto& element : elementsOnCanvas) {
        element->draw(painter);
    }
    delete painter;
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    for(auto& v : elementsOnCanvas) {
        qDebug() << "(next)";
        if(v->contains(event->pos())) {
            actualEl = &*v;
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
    if(event->key() == Qt::Key_O)
        SaveXMLFile();
    else if(event->key() == Qt::Key_P) {
        panel->setCanvasSize(this->width(), this->height());
        panel->ReadXMLFile();
    }
    else if(event->key() == Qt::Key_C) {
        compare();
    }
    else if(actualEl == nullptr)
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
            actualEl -> mirrorEl();       //Funkcja do odwracania jest gotowa, ale nie stestowana, a ja ledwo na oczy patrzę, jutro skończymy
            break;
        default:
            break;
        }
        repaint();
    }
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
        xmlWriter.writeStartElement("element");
        xmlWriter.writeTextElement("Type", QString::number(static_cast<int>(el->typ) ));
        xmlWriter.writeStartElement("CenterPoint");
        xmlWriter.writeTextElement("x", QString::number(el->centerPoint.x()));
        xmlWriter.writeTextElement("y", QString::number(el->centerPoint.y()));
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("Color");
        xmlWriter.writeTextElement("r", QString::number(el->color.red()));
        xmlWriter.writeTextElement("g", QString::number(el->color.green()));
        xmlWriter.writeTextElement("b", QString::number(el->color.blue()));
        xmlWriter.writeEndElement();
        xmlWriter.writeTextElement("Rotation",  QString::number(el->getRot()));
        xmlWriter.writeTextElement("Mirror",  QString::number(el->getMir()));

        xmlWriter.writeStartElement("Points");
        for(auto & point : el ->getPoly())
        {
            xmlWriter.writeStartElement("Point");
            xmlWriter.writeTextElement("x", QString::number(point.x()));
            xmlWriter.writeTextElement("y", QString::number(point.y()));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

        file.close();
}

void compareElement(Element * cElement, Element * pElement) {
    if(cElement->getMir()!=pElement->getMir()) {
        qDebug() << "Złe odbicie";
        return;
    }
    if(fabs(cElement->getRot()-pElement->getRot()) > 10) {
        qDebug() << "Zły kąt";
        return;
    }
    if(fabs(cElement->centerPoint.manhattanLength()-pElement->centerPoint.manhattanLength()) > 50) {
        qDebug() << "Prawdopodobnie złe położenie";
        return;
    }
}

void Canvas::compare() {

    if(elementsOnCanvas.size() == panel->elementsOnPanel.size()) {
        qDebug() << "Ten sam rozmiar, idziemy dalej";
        for (auto& cElement : elementsOnCanvas) {
            for (auto& pElement : panel->elementsOnPanel) {
                if(cElement->typ == pElement.typ) {
                    qDebug() << "W tym samym elemencie";
                    compareElement(cElement.get(), &pElement);
                }
                else {
                    //Tutaj zaczyna się nieoptymalnie, mogę to jeszcze zmienić
                    for (auto& cElementToCmp : elementsOnCanvas) {
                        for (auto& pElementToCmp : panel->elementsOnPanel) {
                               if(fabs((cElement->centerPoint-cElementToCmp->centerPoint).manhattanLength()
                                       - (pElement.centerPoint-pElementToCmp.centerPoint).manhattanLength()) < 10)
                                  qDebug() << "Różne odległości od siebie";
                        }
                    }
                }
            }
        }
    }
}

