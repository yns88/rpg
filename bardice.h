#ifndef BARDICE_H
#define BARDICE_H

#include <QString>
#include "baritem.h"

class GameView;
class StatusView;

class BarDice : public BarItem
{
    Q_OBJECT
private:
    GameView *gameView;
    StatusView *statusView;
protected:
    void dropAction(QPoint p);
    void mouseReleaseEvent(QMouseEvent *ev);
public:
    explicit BarDice(QString imageName, GameView * view, StatusView *sview, QWidget *parent=0);
signals:
    void rollDice(int,int,bool);
};

#endif // BARDICE_H
