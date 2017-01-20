#ifndef DRAGDROP_H
#define DRAGDROP_H

#include <QDrag>
#include <QMouseEvent>
#include <memory>
#include <string>
#include <QString>
#include "element.h"

class DragDrop : public QDrag
{
public:
    void mousePressEvent(QMouseEvent *event, const Element* vec, const QString& type);
    void keyPressEvent(QKeyEvent *event);

    DragDrop(QWidget* dragSource) : QDrag(reinterpret_cast<QObject*>(dragSource)) {}
};

#endif // DRAGDROP_H
