#ifndef DRAGDROP_H
#define DRAGDROP_H

#include <QDrag>
#include <QMouseEvent>
#include <memory>
#include <string>
#include <QString>

class DragDrop : public QDrag
{
public:
    void mousePressEvent(QMouseEvent *event, const QString& type);

    DragDrop(QObject* dragSource) : QDrag(dragSource) {}
};

#endif // DRAGDROP_H
