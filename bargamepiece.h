#ifndef BARGAMEPIECE_H
#define BARGAMEPIECE_H

#include "baritem.h"

class GameView;

class BarGamePiece : public BarItem
{
    Q_OBJECT
private:
    GameView *gameView;
protected:
    void dropAction(QPoint p);
public:
    BarGamePiece(QString imageName, GameView * view, QWidget *parent=0);
signals:
    void createPiece(QString,QPoint);
};

#endif // BARGAMEPIECE_H
