#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

class PuzzleWidget;
class PiecesModel;
class QListView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void openImage();
    void loadImage(const QString &path);
    void setupPuzzle();

private slots:
    void setCompleted();

private:
    void setupMenus();
    void setupWidgets();

    QPixmap puzzleImage;
    QListView *piecesList;
    PuzzleWidget *puzzleWidget;
    PiecesModel *model;
};

#endif // MAINWINDOW_H
