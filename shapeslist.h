#ifndef SHAPESLIST_H
#define SHAPESLIST_H

#include <QListWidget>
#include <QPainter>
#include <QtGui>
#include <memory>
#include "utils.h"
#include "element.h"

class ShapesList : public QListWidget
{
    Q_OBJECT
public:
    explicit ShapesList(QWidget *parent = 0);
    void acceptForList(std::unique_ptr<Element>&& el);

protected:

signals:

public slots:
    void sendToDrag(QListWidgetItem* item);

private:
    utils::unique_vector<Element> elementsOnList;
    void addElement(std::unique_ptr<Element>&& el);
};

#endif // SHAPESLIST_H
