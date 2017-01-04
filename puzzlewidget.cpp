#include <QtGui>

#include "puzzlewidget.h"

PuzzleWidget::PuzzleWidget(int imageSize, QWidget *parent)
    : QWidget(parent), m_ImageSize(imageSize)
{
    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
    setMaximumSize(m_ImageSize, m_ImageSize);
}

void PuzzleWidget::clear()
{
    pieceLocations.clear();
    piecePixmaps.clear();
    pieceRects.clear();
    highlightedRect = QRect();
    inPlace = 0;
    update();
}

void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void PuzzleWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece") &&
        !findPiece(targetSquare(event->pos()))) {

        QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QRect square = targetSquare(event->pos());
        QPixmap pixmap;
        QPoint location;
        dataStream >> pixmap >> location;

        pieceLocations.append(location);
        piecePixmaps.append(pixmap);
        pieceRects.append(square);

        highlightedRect = QRect();
        update(square);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        if (location == QPoint(square.x()/pieceSize(), square.y()/pieceSize())) {
            inPlace++;
            if (inPlace == 25)
                emit puzzleCompleted();
        }
    } else {
        highlightedRect = QRect();
        event->ignore();
    }
}

void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect.united(targetSquare(event->pos()));
    if (event->mimeData()->hasFormat("image/x-puzzle-piece")
        && !findPiece(targetSquare(event->pos()))) {
        highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        highlightedRect = QRect();
        event->ignore();
    }
    update(updateRect);
}

bool PuzzleWidget::findPiece(const QRect &pieceRect) const
{
    for (int i = 0; i < pieceRects.size(); ++i) {
        if (pieceRects[i].contains(pieceRect)) {
            return true;
        }
    }
    return false;
}

bool PuzzleWidget::findPiece(int x, int y) const {
    for (int i = 0; i < pieceRects.size(); ++i) {
        if (pieceRects[i].contains(x,y)) {
            return true;
        }
    }
    return false;
}

void PuzzleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::white);

    if (highlightedRect.isValid()) {
        painter.setBrush(QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    for (int i = 0; i < pieceRects.size(); ++i) {
        painter.drawPixmap(pieceRects[i], piecePixmaps[i]);
    }
    painter.end();
}

const QRect PuzzleWidget::targetSquare(const QPoint &position) const
{
    return QRect(position.x()-pieceSize()/2., position.y()-pieceSize()/2., pieceSize(), pieceSize());
}

int PuzzleWidget::pieceSize() const
{
    return m_ImageSize / 5;
}

int PuzzleWidget::imageSize() const
{
    return m_ImageSize;
}
