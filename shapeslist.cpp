#include "shapeslist.h"
#include "mainwindow.h"

ShapesList::ShapesList(QWidget *parent) : QListWidget(parent) {
    addElement(std::move(std::make_unique<TriangleMid>  (150, 150)));
    addElement(std::move(std::make_unique<TriangleSmall>(250, 250)));
    addElement(std::move(std::make_unique<TriangleBig>  (350, 350)));
    addElement(std::move(std::make_unique<TriangleBig>  (450, 450)));
    addElement(std::move(std::make_unique<TriangleSmall>(550, 550)));
    addElement(std::move(std::make_unique<Rhombus>      (650, 650)));

    connect(this, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(sendToDrag(QListWidgetItem*)));
}

void ShapesList::addElement(std::unique_ptr<Element>&& el) {
    QIcon icon = el->getBitmap();
    elementsOnList.push_back(std::move(el));
    QListWidgetItem *wl = new QListWidgetItem();
    wl->setIcon(QIcon(icon));
    addItem(wl);
}

void ShapesList::sendToDrag(QListWidgetItem* item) {
    int idx = row(item);
    delete (item->listWidget()->takeItem(idx));
    std::unique_ptr<Element> el(std::move(elementsOnList[idx]));
    elementsOnList.erase(elementsOnList.begin() + idx);
    MainWindow::main()->canvas()->acceptForDrag(std::move(el));
}

void ShapesList::acceptForList(std::unique_ptr<Element>&& el) {
    addElement(std::move(el));
}
