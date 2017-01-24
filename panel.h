#ifndef PANEL_H
#define PANEL_H


#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include"element.h"
#include "utils.h"

/*! \class  Panel
 *  \brief  Displays the sample image in the top left corner
 */
class Panel : public QWidget
{
    Q_OBJECT
public:

    /*!
     * \fn      Panel
     * \brief   Constructor for setting up the panel
     * \param   parent  Parent object for panel
     */
    explicit Panel(QWidget *parent = 0);

    /*!
     * \fn      Panel
     * \brief   Constructor for setting up the panel
     * \param   parent  Parent object for panel
     * \param   width  Canvas width to scale image on Panel
     * \param   height  Canvas height to scale image on Panel
     */
    Panel(int width, int height, QWidget *parent = 0);

    /*!
     * \fn      ReadXMLFile
     * \brief   Opens a file dialog, reads XML file and saves elements to elementsOnPanel
     */
    void ReadXMLFile();

    /*!
     * \fn      setCanvasSize
     * \brief   setter for generated bitmap size properties
     * \param   width  Canvas width to scale image on Panel
     * \param   height  Canvas height to scale image on Panel
     */
    void setCanvasSize(int width, int height) {cwidth = width; cheight = height;}

    utils::unique_vector<Element> elementsOnPanel;

protected:

    /*!
     * \fn      paintEvent
     * \brief   Creates bitmap containing all elements and draws it on widget using QPainter
     * \param   event  QPaintEvent to handle
     */
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    int cwidth;
    int cheight;
};


#endif // PANEL_H
