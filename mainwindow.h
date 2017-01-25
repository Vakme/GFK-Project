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

/*! \class  MainWindow
 *  \brief  Main Window of the Application. Contains other elements.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \fn      MainWindow
     * \brief   Constructor for setting up the main window
     * \param   parent  Parent object for MainWindow
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow * main();

    Canvas *canvas() const;

    ShapesList *shapesList() const;

protected:

    /*!
     * \fn      keyPressEvent
     * \brief   Redirects QKeyEvent to canvas
     * \param   event  QKeyEvent to handle
     */
    virtual void keyPressEvent(QKeyEvent *event);

private:
    static std::unique_ptr<MainWindow> _main;
    Ui::MainWindow *ui;
    Canvas *_canvas;
    ShapesList *_shapesList;
};

#endif // MAINWINDOW_H
