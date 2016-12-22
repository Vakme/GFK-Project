#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDrag>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>

class PiecesList;
class PuzzleWidget;
class QListWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openImage(const QString &path = QString());
    void setupPuzzle();

private slots:
    void setCompleted();

private:
    Ui::MainWindow *ui;
    void setupMenus();
    void setupWidgets();

    QPixmap puzzleImage;
    PiecesList *piecesList;
    PuzzleWidget *puzzleWidget;
};

#endif // MAINWINDOW_H
