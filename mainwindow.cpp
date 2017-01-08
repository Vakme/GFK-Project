#include "mainwindow.h"
#include "piecesmodel.h"
#include "puzzlewidget.h"

#include <QtWidgets>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenus();
    setupWidgets();
    model = new PiecesModel(puzzleWidget->pieceSize(), this);
    piecesList->setModel(model);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("Tangram"));

    ////////////////////////////////////////////////////////////////

    int imgSize = puzzleWidget->imageSize();

    QPolygon poly;
    poly    << QPoint(0,0)
            << QPoint(imgSize/4,0)
            << QPoint(imgSize/8,imgSize/8)
            << QPoint(0,0);

    QPixmap tan(imgSize/4,imgSize/4);
    tan.fill(QColor(Qt::white));
    QPainter paintTan(&tan);
    QPen pen(Qt::black);
    paintTan.setPen(pen);
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    QPainterPath path;
    path.addPolygon(poly);
    paintTan.drawPolygon(poly);
    paintTan.fillPath(path, brush);

    model->addPiece(tan.createMaskFromColor(Qt::white), QPoint(0,0));
////
    poly.clear();
    tan.fill(QColor(Qt::white));
    path = QPainterPath();
    poly    << QPoint(0,0)
            << QPoint(0,imgSize/4)
            << QPoint(imgSize/8,imgSize/8)
            << QPoint(0,0);
    path.addPolygon(poly);
    paintTan.drawPolygon(poly);
    paintTan.fillPath(path, brush);
    model->addPiece(tan.createMaskFromColor(Qt::white), QPoint(0,0));
////
    poly.clear();
    tan.fill(QColor(Qt::white));
    path = QPainterPath();
    poly    << QPoint(imgSize/8,imgSize/8)
            << QPoint(3*imgSize/16,imgSize/16)
            << QPoint(3*imgSize/16,3*imgSize/16)
            << QPoint(imgSize/8,imgSize/8);
    path.addPolygon(poly);
    paintTan.drawPolygon(poly);
    paintTan.fillPath(path, brush);
    model->addPiece(tan.createMaskFromColor(Qt::white), QPoint(0,0));
////
    poly.clear();
    tan.fill(QColor(Qt::white));
    path = QPainterPath();
    poly    << QPoint(3*imgSize/16,imgSize/16)
            << QPoint(imgSize/4,0)
            << QPoint(imgSize/4,imgSize/8)
            << QPoint(3*imgSize/16,3*imgSize/16)
            << QPoint(3*imgSize/16,imgSize/16);
    path.addPolygon(poly);
    paintTan.drawPolygon(poly);
    paintTan.fillPath(path, brush);
    model->addPiece(tan.createMaskFromColor(Qt::white), QPoint(0,0));
////

    poly.clear();
    tan.fill(QColor(Qt::white));
    path = QPainterPath();
    poly    << QPoint(imgSize/8,imgSize/8)
            << QPoint(3*imgSize/16,3*imgSize/16)
            << QPoint(imgSize/8,imgSize/4)
            << QPoint(imgSize/16,3*imgSize/16)
            << QPoint(imgSize/8,imgSize/8);
    path.addPolygon(poly);
    paintTan.drawPolygon(poly);
    paintTan.fillPath(path, brush);
    model->addPiece(tan.createMaskFromColor(Qt::white), QPoint(0,0));
////
    poly.clear();
    tan.fill(QColor(Qt::white));
    path = QPainterPath();
    poly    << QPoint(0,imgSize/4)
            << QPoint(imgSize/16,3*imgSize/16)
            << QPoint(imgSize/8,imgSize/4)
            << QPoint(0,imgSize/4);
    path.addPolygon(poly);
    paintTan.drawPolygon(poly);
    paintTan.fillPath(path, brush);
    model->addPiece(tan.createMaskFromColor(Qt::white), QPoint(0,0));
////

    poly.clear();
    tan.fill(QColor(Qt::white));
    path = QPainterPath();
    poly    << QPoint(imgSize/8,imgSize/4)
            << QPoint(imgSize/4,imgSize/8)
            << QPoint(imgSize/4,imgSize/4)
            << QPoint(imgSize/8,imgSize/4);
    path.addPolygon(poly);
    paintTan.drawPolygon(poly);
    paintTan.fillPath(path, brush);
    model->addPiece(tan.createMaskFromColor(Qt::white), QPoint(0,0));
////

    ///////////////////////////////////////////////////////////////
}

void MainWindow::openImage()
{
    const QString fileName =
        QFileDialog::getOpenFileName(this,
                                     tr("Open Image"), QString(),
                                     tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
        loadImage(fileName);
}

void MainWindow::loadImage(const QString &fileName)
{
    QPixmap newImage;
    if (!newImage.load(fileName)) {
        QMessageBox::warning(this, tr("Open Image"),
                             tr("The image file could not be loaded."),
                             QMessageBox::Cancel);
        return;
    }
    puzzleImage = newImage;
    setupPuzzle();
}

void MainWindow::setCompleted()
{
    QMessageBox::information(this, tr("Puzzle Completed"),
                             tr("Congratulations! You have completed the puzzle!\n"
                                "Click OK to start again."),
                             QMessageBox::Ok);

    setupPuzzle();
}

void MainWindow::setupPuzzle()
{
    int size = qMin(puzzleImage.width(), puzzleImage.height());
    puzzleImage = puzzleImage.copy((puzzleImage.width() - size) / 2,
        (puzzleImage.height() - size) / 2, size, size).scaled(puzzleWidget->imageSize(),
            puzzleWidget->imageSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    qsrand(QCursor::pos().x() ^ QCursor::pos().y());

    model->addPieces(puzzleImage);
    puzzleWidget->clear();
}

void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);

    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcuts(QKeySequence::Quit);

    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));

    QAction *restartAction = gameMenu->addAction(tr("&Restart"));

    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);
    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(restartAction, &QAction::triggered, this, &MainWindow::setupPuzzle);
}

void MainWindow::setupWidgets()
{
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);

    puzzleWidget = new PuzzleWidget(400);

    piecesList = new QListView;
    piecesList->setDragEnabled(true);
    piecesList->setViewMode(QListView::IconMode);
    //piecesList->setIconSize(QSize(puzzleWidget->pieceSize() - 20, puzzleWidget->pieceSize() - 20));
    piecesList->setIconSize(QSize(puzzleWidget->pieceSize(), puzzleWidget->pieceSize()));
    piecesList->setGridSize(QSize(puzzleWidget->pieceSize(), puzzleWidget->pieceSize()));
    piecesList->setSpacing(10);
    piecesList->setMovement(QListView::Snap);
    piecesList->setAcceptDrops(true);
    piecesList->setDropIndicatorShown(true);

    PiecesModel *model = new PiecesModel(puzzleWidget->pieceSize(), this);
    piecesList->setModel(model);

    connect(puzzleWidget, &PuzzleWidget::puzzleCompleted,
            this, &MainWindow::setCompleted, Qt::QueuedConnection);

    frameLayout->addWidget(piecesList);
    frameLayout->addWidget(puzzleWidget);
    setCentralWidget(frame);
}
