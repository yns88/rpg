#include "gamepiece.h"
#include <QMouseEvent>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * GamePiece
  * Game Piece displayed on the board
  *
  */

GamePiece::GamePiece(QString imgName, quint16 pieceID, QWidget *parent) :
    QLabel(parent), pid(pieceID), pixName(imgName)
{
    this->setPixmap(QPixmap(imgName));
}

GamePiece::GamePiece(QByteArray serial, QWidget *parent)
    : QLabel(parent)
{
    QDataStream in(&serial,QIODevice::ReadOnly);
    QPoint tempPos;
    in >> pid >> tempPos;
    this->move(tempPos);
}

QString GamePiece::imageName()
{
    return pixName;
}

/**
  * Serialize this piece for saving to a file
  *
  */
QByteArray GamePiece::serialize()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << pid;
    out << this->pos();
    return block;
}

/**
  * Get the id of this piece
  *
  */
quint16 GamePiece::pieceID()
{
    return pid;
}

/**
  * Resize the game piece to the given width
  *
  */
void GamePiece::setPixScale(int size)
{
    this->setPixmap(QPixmap(pixName).scaledToWidth(size,Qt::SmoothTransformation));
}

/**
  * On drag, move this game piece to where the mouse cursor is
  *
  */
void GamePiece::mouseMoveEvent(QMouseEvent *ev)
{

    this->move(ev->globalPos()-delta);
}

/**
  * On left click, set this up for dragging.
  * On right click, signal the server to delete this piece.
  */
void GamePiece::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button()==Qt::RightButton) {
        emit deleted(pid);
    }
    else delta = ev->globalPos()-this->pos();

}

/**
  * On mouse release, signal the server to move this piece
  *
  */
void GamePiece::mouseReleaseEvent(QMouseEvent *ev)
{
    emit moved(pid,this->pos());
}

