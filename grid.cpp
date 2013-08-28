#include "grid.h"
#include <QPainter>
#include <QPaintEvent>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * Grid
  * Class for displaying a grid on the game view
  *
  */

Grid::Grid(int len, QWidget *parent) :
    QLabel(parent), gridLength(len), resized(true), offsetX(0), offsetY(0)
{
}

/**
  * Render the grid to the game view
  */
void Grid::paintEvent(QPaintEvent *ev)
{
    //if (resized) {

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::black, 1));

        for (int x=offsetX;x<this->width();x+=gridLength) {
            painter.drawLine(QPoint(x,0),QPoint(x,this->height()));
        }
        for (int y=offsetY;y<this->width();y+=gridLength) {
            painter.drawLine(QPoint(0,y),QPoint(this->width(),y));
        }
    //}
    resized = false;
}

void Grid::resizeEvent(QResizeEvent *)
{
    resized = true;
}

void Grid::showEvent(QShowEvent *)
{
    resized = true;
}

qint32 Grid::gridSize()
{
    return gridLength;
}

void Grid::setGridSize(int n)
{
    gridLength = n;
    resized = true;
}

void Grid::setOffset(QPoint p) {
    offsetX = p.x();
    offsetY = p.y();
}

QPoint Grid::offset() {
    return QPoint(offsetX,offsetY);
}
