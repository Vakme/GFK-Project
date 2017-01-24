#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QPainter>
#include "canvas.h"
#include "shapeslist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static MainWindow * main();
    Canvas *canvas() const;
    ShapesList *shapesList() const;

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    static std::unique_ptr<MainWindow> _main;
    Ui::MainWindow *ui;
    Canvas *_canvas;
    ShapesList *_shapesList;
};

#endif // MAINWINDOW_H
