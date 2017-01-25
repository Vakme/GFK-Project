#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _canvas = findChild<Canvas*>("canvas");
    _shapesList = findChild<ShapesList*>("shapesList");
}

MainWindow::~MainWindow() {
    delete ui;
}

std::unique_ptr<MainWindow> MainWindow::_main;

MainWindow * MainWindow::main() {
    if(_main == nullptr) {
        _main = std::unique_ptr<MainWindow>(std::move(new MainWindow()));
    }
    return _main.get();
}
Canvas * MainWindow::canvas() const {
    return _canvas;
}
ShapesList * MainWindow::shapesList() const {
    return _shapesList;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    _canvas->keyPressEvent(event);
}
