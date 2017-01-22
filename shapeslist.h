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
    void addElement(std::unique_ptr<Element>&& el);

protected:

signals:

public slots:

private:
    utils::unique_vector<Element> elementsOnList;
};

#endif // SHAPESLIST_H
