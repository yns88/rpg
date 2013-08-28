#include "baritem.h"
#include <QMouseEvent>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * BarItem
  *
  * An abstract class for items in the sidebar, which can be dragged into the gameView.
  */

BarItem::BarItem(QString imageName, QWidget *parent) :
    QLabel(parent), movingLabel(new QLabel(parent)), delta(QPoint(0,0)), imgPath(imageName)
{
    offset = QPoint(0,30);
    image = QPixmap(imageName).scaledToWidth(sideLength,Qt::SmoothTransformation);
    setPixmap(image);
    resize(image.size());
    movingLabel->setPixmap(image);
    movingLabel->resize(image.size());
    movingLabel->hide();
}

BarItem::~BarItem()
{
    //delete image;
    delete movingLabel;
}

void BarItem::mouseMoveEvent(QMouseEvent *ev)
{
    movingLabel->move(ev->globalPos() + delta + offset);
}

void BarItem::mousePressEvent(QMouseEvent *ev)
{
    delta = this->pos() - ev->globalPos();
    movingLabel->move(this->pos() + offset);
    movingLabel->show();

}

void BarItem::mouseReleaseEvent(QMouseEvent *ev)
{
    movingLabel->hide();
    //move(movingLabel->pos());
    dropAction(ev->globalPos()+delta-QPoint(0,this->pos().y()));
}
