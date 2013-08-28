#include <QApplication>
#include <QDateTime>
#include "mainwindow.h"
#include "gameview.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

int main(int argc, char *argv[])
{
    qint64 seedTime = QDateTime::currentMSecsSinceEpoch();
    qsrand(seedTime);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //GameView g;
    //g.show();
    
    return a.exec();
}
