#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QWidget>
#include <QPolygon>

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;

class PuzzleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PuzzleWidget(int imageSize, QWidget *parent = 0);
    void clear();

    int pieceSize() const;
    int imageSize() const;
    void rotatePolygon(QPolygon &poly, int angle);

signals:
    void puzzleCompleted();

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int findPiece(const QRect &pieceRect) const;
    int findPiece(int x, int y) const;
    int findPiece(const QPoint &point) const;
    const QRect targetSquare(const QPoint &position) const;

    QList<QPixmap> piecePixmaps;
    QList<QRect> pieceRects;
    QList<QPoint> pieceLocations;
    QList<QPolygon> piecePolygons;
    QRect highlightedRect;
    int inPlace;
    int m_ImageSize;
    QPoint mouseOnRectPos;
};

#endif // PUZZLEWIDGET_H
