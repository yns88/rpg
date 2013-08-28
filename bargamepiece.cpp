#include "bargamepiece.h"
#include "gameview.h"
#include "gamepiece.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * BarGamePiece
  * A Game Piece which can be dropped onto the gameView to create a new game piece
  *
  */

BarGamePiece::BarGamePiece(QString imageName, GameView * view, QWidget *parent)
    : BarItem(imageName,parent), gameView(view)
{
}

/**
  * Create a new piece when dropped
  *
  */
void BarGamePiece::dropAction(QPoint p)
{

    if (gameView) {
        //GamePiece * piece = new GamePiece(this->parentWidget());
        //piece->setPixmap(this->image);
        //piece->resize(piece->pixmap()->size());
        //gameView->addGamePiece(image,mapToGlobal(p));
        gameView->sendPieceCreated(imgPath,mapToGlobal(p));
    }



}

