#include "shapeslist.h"

ShapesList::ShapesList(QWidget *parent) : QListWidget(parent) {
    addElement(std::move(std::make_unique<TriangleMid>(150, 150)));
    addElement(std::move(std::make_unique<TriangleBig>(250, 250)));
    addElement(std::move(std::make_unique<TriangleBig>(350, 350)));
}

void ShapesList::addElement(std::unique_ptr<Element>&& el) {
    QIcon icon = el->getBitmap();
    elementsOnList.push_back(std::move(el));
    QListWidgetItem *wl = new QListWidgetItem();
    wl->setIcon(QIcon(icon));
    addItem(wl);
}
