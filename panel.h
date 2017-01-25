/*! \file  panel.h
    \brief Contains panel for drawing shape to solve.
*/

#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include"element.h"
#include "utils.h"

//! Displays the sample image in the top left corner.
class Panel : public QWidget {
    Q_OBJECT
public:

    //! Sets up the panel.
    /*!
      \param   parent  parent object for panel
     */
    explicit Panel(QWidget *parent = 0);

    //! Sets up the panel with width and height.
    /*!
      \param   parent  parent object for panel
      \param   width   canvas width to scale image on panel
      \param   height  canvas height to scale image on panel
     */
    Panel(int width, int height, QWidget *parent = 0);

    //! Opens a file dialog and then reads XML file and loads elements.
    void ReadXMLFile();

    //! Sets generated bitmap size properties.
    /*!
      \param   width   canvas width to scale image on panel
      \param   height  canvas height to scale image on panel
     */
    void setCanvasSize(int width, int height) { cwidth = width; cheight = height; }

    //! All the puzzle pieces on panel.
    utils::unique_vector<Element> elementsOnPanel;

protected:

    //! Draws the blackened shape to solve.
    /*!
      \param   event  event to handle
     */
    void paintEvent(QPaintEvent *event);

private:
    //! Width of the canvas to resize into panel.
    int cwidth;

    //! Height of the canvas to resize into panel.
    int cheight;
};


#endif // PANEL_H
