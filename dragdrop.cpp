#include "dragdrop.h"
#include <Qt>
#include <QtGlobal>
#include <QtDebug>
#include <QMessageBox>
#include <QMargins>

void DragDrop::mousePressEvent(QMouseEvent *event, const std::vector<Element*> & vec, const QString& type) {
    for(auto& v : vec) {
        qDebug() << "(next)";
        if(v->contains(event->pos())) {
            //qDebug() << event->pos() << " on " << static_cast<int>(v->typ);
            break;
        }
    }
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
