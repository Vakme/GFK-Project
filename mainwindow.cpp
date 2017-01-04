#include <stdlib.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pieceslist.h"
#include "puzzlewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupMenus();
    setupWidgets();

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("Tangram"));

    ///////////////////////////////////////////////////
 /*       QPolygon tan;
        tan << QPoint(-100, -100)
            << QPoint(100, -100)
            << QPoint(0, 0)
            << QPoint(-100, -100);
        //tan << QPoint(0, -100);
        QPixmap pixmap(tan.boundingRect().width()/2+1,
                       tan.boundingRect().height()/2+1);
        pixmap.fill( QColor( Qt::white) );

        //QPixmap alpha = pixmap.createMaskFromColor(QColor( Qt::black ),Qt::MaskOutColor);
        //pixmap.setAlphaChannel( alpha );

        QPainter painter( &pixmap );
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::black);
        QPainterPath path;
        path.addPolygon(tan);
        painter.fillPath(path, brush);

        painter.setPen( QPen( Qt::black) );
        //painter.translate(tan.boundingRect().width()/4, tan.boundingRect().height()/2 );
        //painter.drawPolygon( tan, Qt::WindingFill );
*/



            QPolygon poly;

            /*poly << QPoint(0, 85) << QPoint(75, 75)
                 << QPoint(100, 10) << QPoint(125, 75)
                 << QPoint(200, 85) << QPoint(150, 125)
                 << QPoint(160, 190) << QPoint(100, 150)
                 << QPoint(40, 190) << QPoint(50, 125)
                 << QPoint(0, 85);*/

            poly << QPoint(0,0)
                 << QPoint(200,0)
                 << QPoint(100,100)
                 << QPoint(0,0);

            QPixmap pix(poly.boundingRect().width()+1,
                        poly.boundingRect().height()+1);
            pix.fill( QColor( Qt::white) );
            QPainter painter2(&pix);

            // style(), width(), brush(), capStyle() and joinStyle().
            QPen pen(Qt::black);
            painter2.setPen(pen);

            // Brush
            QBrush brush;
            brush.setColor(Qt::black);
            brush.setStyle(Qt::SolidPattern);

            // Fill polygon
            QPainterPath path;
            path.addPolygon(poly);

            // Draw polygon
            painter2.drawPolygon(poly);
            painter2.fillPath(path, brush);
            piecesList->addPiece(pix, QPoint(0, 0));

    ///////////////////////////////////////////////////
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "", "Image Files (*.png *.jpg *.bmp)");

    if (!fileName.isEmpty()) {
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
    puzzleImage = puzzleImage.copy((puzzleImage.width() - size)/2,
        (puzzleImage.height() - size)/2, size, size).scaled(puzzleWidget->width(),
            puzzleWidget->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    piecesList->clear();

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            int pieceSize = puzzleWidget->pieceSize();
            QPixmap pieceImage = puzzleImage.copy(x * pieceSize, y * pieceSize, pieceSize, pieceSize);
            piecesList->addPiece(pieceImage, QPoint(x, y));
        }
    }



    qsrand(QCursor::pos().x() ^ QCursor::pos().y());

    for (int i = 0; i < piecesList->count(); ++i) {
        if (int(2.0*qrand()/(RAND_MAX+1.0)) == 1) {
            QListWidgetItem *item = piecesList->takeItem(i);
            piecesList->insertItem(0, item);
        }
    }

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

    connect(openAction, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(restartAction, SIGNAL(triggered()), this, SLOT(setupPuzzle()));
}

void MainWindow::setupWidgets()
{
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    puzzleWidget = new PuzzleWidget(260);
#else
    puzzleWidget = new PuzzleWidget(400);
#endif

    piecesList = new PiecesList(puzzleWidget->pieceSize(), this);

    connect(puzzleWidget, SIGNAL(puzzleCompleted()),
            this, SLOT(setCompleted()), Qt::QueuedConnection);

    frameLayout->addWidget(piecesList);
    frameLayout->addWidget(puzzleWidget);
    setCentralWidget(frame);
}
