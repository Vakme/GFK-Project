/*! \file  shapeslist.h
    \brief Contains list of shapes ouside of canvas.
*/

#ifndef SHAPESLIST_H
#define SHAPESLIST_H

#include <QListWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include "utils.h"
#include "element.h"

//! Container to hold all the puzzles
class ShapesList : public QListWidget {
    Q_OBJECT
public:

    //! Sets up the shapes list.
    /*!
      \param   parent  parent object for shapes list
     */
    explicit ShapesList(QWidget *parent = 0);

    //! Accepts the ownership of a puzzle into list.
    /*!
      \param   el  ownership to be transferred
      \see addElement
     */
    void acceptForList(std::unique_ptr<Element>&& el);

public slots:
    //! Sends list item's puzzle piece to canvas with placement mode.
    /*!
      \param  item  list item to be passed
     */
    void sendToDrag(QListWidgetItem* item);

private:
    //! All the puzzle pieces on shapes list.
    utils::unique_vector<Element> elementsOnList;

    //! Adds a puzzle piece to the list.
    /*!
      \param   el  ownership to be transferred
     */
    void addElement(std::unique_ptr<Element>&& el);
};

#endif // SHAPESLIST_H
