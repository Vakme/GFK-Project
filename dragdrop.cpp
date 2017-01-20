#include "dragdrop.h"
#include <Qt>
#include <QtGlobal>
#include <QtDebug>
#include <QMessageBox>
#include <QMargins>

void DragDrop::mousePressEvent(QMouseEvent *event, const Element* el, const QString& type) {
    qDebug() << "Ok " << event->pos();
}

void DragDrop::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Ok ";
    switch (event->key()) {
    case Qt::Key_R:
        qDebug() << "You pressed R";
        break;
    case Qt::Key_E:
        qDebug() << "You pressed E";
        break;
    default:
        break;
    }
}
