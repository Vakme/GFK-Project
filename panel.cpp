#include "panel.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QFileDialog>

Panel::Panel(QWidget *parent) : QWidget(parent)
{
    this->elementsOnPanel = std::vector<Element>();
}

Panel::Panel(int width, int height, QWidget *parent) : cwidth(width), cheight(height), QWidget(parent)
{
    elementsOnPanel = std::vector<Element>();
}

void Panel::paintEvent(QPaintEvent *event)
{
    qDebug() << cwidth << " " << cheight;
    QPixmap pixmap = QPixmap(cwidth, cheight);
    pixmap.fill(QColor(0,0,0,0));
    QPainter *painter = new QPainter(&pixmap);
    qDebug() << "RYSUJNAPANELU";
    painter -> setRenderHints(QPainter::Antialiasing);
    painter -> setPen(QPen());
    for(auto& element : elementsOnPanel) {
        painter -> setBrush(QBrush(element.color));
        painter -> drawPolygon(element.getRealPoly(element.centerPoint.x(),element.centerPoint.y()));
    }
    float scale = ((float)cwidth)/this->width();
    qDebug() << "SCALE" << scale << " " << (float)cwidth/scale << " " << (float)cheight/scale;
    delete painter;
    QPainter *paintOnPanel = new QPainter(this);
    paintOnPanel -> drawPixmap(0,0,(float)cwidth/scale, (float)cheight/scale, pixmap);
    delete paintOnPanel;
}

void Panel::ReadXMLFile()
{
    qDebug() << "JESZCZE DZIAŁA WEWNĄTRZ";
        QXmlStreamReader Rxml;

        QString filename = QFileDialog::getOpenFileName(this,
                                   tr("Open Xml"), ".",
                                   tr("Xml files (*.xml)"));
    if(!elementsOnPanel.empty())
        elementsOnPanel.clear();
    QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error: Cannot read file " << qPrintable(filename)
                  << ": " << qPrintable(file.errorString());

    }
    Rxml.setDevice(&file);
    Rxml.readNext();

    if (Rxml.hasError())
{
   qDebug() << "Error: Failed to parse file "
             << qPrintable(filename) << ": "
             << qPrintable(Rxml.errorString());
    }
else if (file.error() != QFile::NoError)
{
    qDebug() << "Error: Cannot read file " << qPrintable(filename)
              << ": " << qPrintable(file.errorString());
}
qDebug() << "Opened";
qreal rotation = 0, rotation_max = 360;
bool mirror = false, mirrorable = false;
int typ = 1;
float x = 0, y = 0;
QPointF centPoint = QPointF(0,0);
int r = 0, g = 0, b = 0;
QVector<QPointF> pointVec;
    while(!Rxml.atEnd())
    {
        if(Rxml.name() == "Rotation") {
                rotation = Rxml.readElementText().toDouble();
                qDebug() << QString::number(rotation);
        }
        else if(Rxml.name() == "Mirror") {
                mirror = Rxml.readElementText().toInt();
                qDebug() << QString::number(mirror);
        }
        else if(Rxml.name() == "Type") {
                typ = Rxml.readElementText().toInt();
                qDebug() << QString::number(typ);
        }
        else if(Rxml.name() == "CenterPoint") {
            Rxml.readNext();
            while(Rxml.name() != "CenterPoint" && !Rxml.isEndElement()) {
                if(Rxml.name() == "x")
                    x = Rxml.readElementText().toFloat();
                else if(Rxml.name() == "y")
                    y = Rxml.readElementText().toFloat();

                Rxml.readNext();
            }
            centPoint.setX(x);
            centPoint.setY(y);
            qDebug() << "CPoint(" << QString::number(x) << ", " << QString::number(y) << ")";
        }
        else if(Rxml.name() == "Color") {
            Rxml.readNext();
            while(Rxml.name() != "Color" && !Rxml.isEndElement()) {
                if(Rxml.name() == "r")
                    r = Rxml.readElementText().toInt();
                else if(Rxml.name() == "g")
                    g = Rxml.readElementText().toInt();
                else if(Rxml.name() == "b")
                    b = Rxml.readElementText().toInt();

                Rxml.readNext();
            }
            qDebug() << "Color(" << QString::number(r) << ", " << QString::number(g) << ", " << QString::number(b) << ")";

        }

        else if(Rxml.name() == "Points") {
            pointVec.clear();
            Rxml.readNext();
            while(Rxml.name() != "Points" && !Rxml.isEndElement()) {
                if(Rxml.name() == "Point") {
                            Rxml.readNext();
                            while(Rxml.name() != "Point" && !Rxml.isEndElement()) {
                                if(Rxml.name() == "x")
                                    x = Rxml.readElementText().toFloat();
                                else if(Rxml.name() == "y")
                                    y = Rxml.readElementText().toFloat();

                                Rxml.readNext();
                            }
                            pointVec.push_back(QPointF(x, y));
                            qDebug() << "Point(" << QString::number(x) << ", " << QString::number(y) << ")";
                        }

                Rxml.readNext();
            }
        }
        else if(Rxml.name() == "element" && Rxml.isEndElement()) {
            if(typ == 5) {
                mirrorable = true;
                rotation_max = 180;
            }
            else if(typ == 4) {
                mirrorable = false;
                rotation_max = 90;
            }
            else {
                rotation_max = 360;
                mirrorable = false;
            }

            elementsOnPanel.push_back(Element(typ, centPoint, QPolygonF(pointVec), rotation, mirror, mirrorable, rotation_max));
        }
        Rxml.readNext();

    }
    file.close();
    this->repaint();
    qDebug() << "Closed";
}

