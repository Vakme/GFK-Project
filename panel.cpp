#include "panel.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QFileDialog>

Panel::Panel(QWidget *parent) : QWidget(parent)
{
    this->elementsOnPanel = std::vector<Element*>();
}

Panel::Panel(int width, int height, QWidget *parent) : cwidth(width), cheight(height), QWidget(parent)
{
    elementsOnPanel = std::vector<Element*>();
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
        painter -> setBrush(QBrush(element->color));
        painter -> drawPolygon(element->getRealPoly(element->centerPoint.x(),element->centerPoint.y()));
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
    while(!Rxml.atEnd())
    {

        elementsOnPanel.push_back(Element::checkXML(Rxml));
        Rxml.readNext();

    }
    file.close();
    this->repaint();
    qDebug() << "Closed";
}

