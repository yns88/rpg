#include "bardice.h"
#include "gameview.h"
#include "statusview.h"
#include <QMouseEvent>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/** BarDice
  *
  * Dice item in the sidebar rolls dice when dropped on the game view
  *
  */


BarDice::BarDice(QString imageName, GameView *view, StatusView *sview, QWidget *parent)
    : BarItem(imageName,parent), gameView(view), statusView(sview)
{
}


void BarDice::dropAction(QPoint p)
{
}

/**
  * Overrides parent's mouseReleaseEvent
  * Requests a server dice roll if dropped in gameView or chatView,
  * or does a local, private dice roll if dropped in statusView
  */
void BarDice::mouseReleaseEvent(QMouseEvent *ev)
{
    movingLabel->hide();
    //int y = ev->globalPos().y();
    //int gametop = gameView->mapToGlobal(QPoint(0,0)).y();
    //int gamebottom = gameView->mapToGlobal(QPoint(0,gameView->height())).y();
    if (ev->globalPos().y() < (statusView->mapToGlobal(QPoint(0,0))).y()) {
        emit rollDice(2,6,true);
    }
    else emit rollDice(2,6,false);
}
