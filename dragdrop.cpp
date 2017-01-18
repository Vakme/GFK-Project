#include "dragdrop.h"
#include <Qt>
#include <QtGlobal>
#include <QtDebug>
#include <QMessageBox>

void DragDrop::mousePressEvent(QMouseEvent *event, const QString& type)
{
    qDebug() << "Jestem tu biczys! " << type;
}
