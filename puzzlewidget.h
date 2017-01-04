#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QList>
#include <QPoint>
#include <QPixmap>
#include <QWidget>

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;

class PuzzleWidget : public QWidget
{
    Q_OBJECT

public:
    PuzzleWidget(int imageSize, QWidget *parent = 0);
    void clear();

    int pieceSize() const;
    int imageSize() const;
    bool findPiece(const QRect &pieceRect) const;
    bool findPiece(int x, int y) const;

signals:
    void puzzleCompleted();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void paintEvent(QPaintEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
private:
    const QRect targetSquare(const QPoint &position) const;

    QList<QPixmap> piecePixmaps;
    QList<QRect> pieceRects;
    QList<QPolygon> piecePolygons;
    QList<QPoint> pieceLocations;
    QRect highlightedRect;
    int inPlace;
    int m_ImageSize;
};

#endif
