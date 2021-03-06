/*! \file  mainwindow.h
    \brief Contains main window of the application.
*/

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

//! Main window of the application, contains all the main elements.
/*! Implemented as a singleton. */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Gets the only instance of the class.
    static MainWindow * main();

    //! Releases window's data.
    ~MainWindow();

    //! Forbiddens copying to force uniqueness.
    MainWindow(const MainWindow & other) = delete;

    //! Get the reference to canvas.
    Canvas *canvas() const;

    //! Get the reference to shapes list.
    ShapesList *shapesList() const;

protected:
    //! Redirects key events to canvas.
    /*!
      \param   event  event to redirect
     */
    virtual void keyPressEvent(QKeyEvent *event);

private:
    //! Sets up the main window.
    /*!
      Is private to force single instance.
      \param   parent  parent object for the window
     */
    explicit MainWindow(QWidget *parent = 0);

    //! The only instance of the class.
    static std::unique_ptr<MainWindow> _main;

    //! Autoreference generated by Qt.
    Ui::MainWindow *ui;

    //! Reference to the canvas module.
    Canvas *_canvas;

    //! Reference to the shapes list module.
    ShapesList *_shapesList;
};

#endif // MAINWINDOW_H
