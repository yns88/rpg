#include "gameview.h"
#include "gamepiece.h"
#include "grid.h"
#include <QPainter>
#include <QBitmap>
#include <QFile>
#include <QDialog>
#include <QErrorMessage>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * GameView
  * Window for viewing the game board
  *
  *
  */

GameView::GameView(QWidget *parent) :
    QWidget(parent), back(new QLabel(this)), piece(NULL), gridShow(true), gridSnap(true), pieces(new QVector<GamePiece*>())
{

    QPixmap boardPixmap(":/assets/img/board.jpg");
    back->setPixmap(boardPixmap.scaledToWidth(1000));

    back->resize(back->pixmap()->size());
    back->move(-50,-50);
    this->resize(back->size());

    /*
    pixmap = new QPixmap(this->width(),this->height());
    pixmap->fill(Qt::white);

    grid->setPixmap(*pixmap);
    grid->resize(this->size());
    this->drawGrid(100);
    //pixmap->setMask(pixmap->createMaskFromColor(Qt::white,Qt::MaskInColor));
    grid->raise();
    grid->show();
    */

    grid = new Grid(100,this);
    grid->resize(this->size());
    grid->raise();
    grid->setEnabled(false);

    /*
    piece->setPixmap(QPixmap(":/assets/img/chip.png").scaledToWidth(100,Qt::SmoothTransformation));
    piece->move(30,30);
    piece->resize(100,100);
    piece->raise(); */


    //connect(piece,SIGNAL(moved(quint16,QPoint)),this,SLOT(getPieceMoved(quint16,QPoint)));
}

void GameView::resizeEvent(QResizeEvent *ev)
{

    //back->resize(this->size());
    //back->setPixmap(QPixmap(":/assets/img/board.jpg").scaledToWidth(this->width(),Qt::SmoothTransformation));

}

void GameView::paintEvent(QPaintEvent *)
{
    //
    //grid->raise();
}

bool GameView::getGridShow()
{
    return gridShow;
}

bool GameView::getGridSnap()
{
    return gridSnap;
}

void GameView::setGridShow(bool b)
{
    gridShow = b;
    if (b) grid->show();
    else grid->hide();
}

void GameView::setGridSnap(bool b)
{
    gridSnap = b;
}


/**
  * Save the contents and configuration of the board to a file
  *
  */
void GameView::saveBoard(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << QString("RPG v1.0");
    out << grid->gridSize() << grid->offset();
    out << static_cast<qint32>(pieces->size());
    foreach(GamePiece *p, *pieces) {
        out << p->pieceID();
        out << p->pos();
        out << p->imageName();
    }

    file.close();
}

/**
  * Load the contents and configuration of the board from a file
  *
  */
void GameView::loadBoard(QString filename)
{

    emit clearBoard();
    clearPieces();


    QPoint tempOffset;
    qint32 tempSize;
    qint32 pieceCount;

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    QString version;
    in >> version;
    if (version != "RPG v1.0") {
        QErrorMessage *errorWindow = new QErrorMessage();
        errorWindow->setWindowTitle("Load Error");
        errorWindow->showMessage("Incompatible file format!");
        errorWindow->show();
    }
    else {

        in >> tempSize >> tempOffset;

        adjustGrid(tempSize,tempOffset.x(),tempOffset.y());

        in >> pieceCount;

        for(int i=0;i<pieceCount;i++) {
            QByteArray block;
            quint16 tempPid;
            QPoint tempPos;
            QString pix;
            in >> tempPid;
            in >> tempPos;
            in >> pix;
            //GamePiece *p = new GamePiece(pix,tempPid,this);
            //p->setPixmap(QPixmap(pix));
            //p->move(tempPos);
            //p->show();
            emit pieceCreated(pix,tempPos);
            //connect(p,SIGNAL(moved(quint16,QPoint)),this,SLOT(getPieceMoved(quint16,QPoint)));
            //pieces->append(p);

        }
    }



    //grid->setOffset(tempOffset);
    //grid->setGridSize(tempSize);

    file.close();
}

/**
  * Delete all pieces from the board
  *
  */
void GameView::clearPieces() {
    for(int i=0;i<pieces->size();i++) {
        GamePiece *p = pieces->value(i);
        delete p;
    }
    pieces->clear();
    if (pieces)
        delete pieces;
    pieces = new QVector<GamePiece*>();
}

/**
  * Resize and displace the grid
  *
  */
void GameView::adjustGrid(int size, QPoint offset)
{
    foreach(GamePiece *piece, *pieces) {
        //piece->setPixmap(piece->pixmap()->scaledToWidth(size,Qt::SmoothTransformation));
        piece->setPixScale(size);
        piece->resize(piece->pixmap()->size());
        piece->move(piece->pos()-grid->offset());
        piece->move((size*piece->pos())/grid->gridSize());
        piece->move(piece->pos()+offset);

    }

    grid->setGridSize(size);
    grid->setOffset(offset);
}

/**
  * Delete a piece from the board
  *
  */
void GameView::removePiece(quint16 pid)
{
    foreach(GamePiece *piece, *pieces) {
        if (piece->pieceID()==pid) {
            pieces->remove(pieces->indexOf(piece));
            delete piece;
            break;
        }
    }
}

/**
  * Add a piece to the board
  *
  */
void GameView::addGamePiece(quint16 pid, QString imgName, QPoint pos)
{
    /*
    GamePiece * p = new GamePiece(this);
    p->setPixmap(image);
    p->move(mapFromGlobal(pos));
    p->show();
    pieces->append(p);
    */

    GamePiece *p = new GamePiece(imgName,pid,this);
    p->setPixScale(grid->gridSize());
    //p->setPixmap(QPixmap(imgName).scaledToWidth(100,Qt::SmoothTransformation));
    p->move(pos);
    p->show();
    pieces->append(p);
    connect(p,SIGNAL(moved(quint16,QPoint)),this,SLOT(getPieceMoved(quint16,QPoint)));
    connect(p,SIGNAL(deleted(quint16)),this,SLOT(getPieceDeleted(quint16)));

}

/**
  * signal the server to add a piece to the board
  */
void GameView::sendPieceCreated(QString img, QPoint pos)
{
    emit pieceCreated(img,mapFromGlobal(pos));
}

/**
  * Move a piece on the board, then signal the server to move it
  */
void GameView::getPieceMoved(quint16 pid, QPoint p)
{
    if (gridSnap) {
        p = snapToGrid(p);
        //piece->move(p);
        foreach (GamePiece *piece, *pieces) {
            if (piece->pieceID()==pid) {
                piece->move(p);
                break;
            }
        }
    }
    emit pieceMoved(pid,p);
}

/**
  * Signal the server to delete a piece
  */
void GameView::getPieceDeleted(quint16 pid)
{
    emit pieceDeleted(pid);
}

/**
  * Move a piece to the specified position
  */
void GameView::updatePiece(quint16 pid, QPoint p)
{
    foreach (GamePiece *piece, *pieces) {
        if (piece->pieceID()==pid) {
            piece->move(p);
            break;
        }
    }
    //piece->move(p);
}

/**
  * Signal the server to adjust the grid
  */
void GameView::adjustGrid(int size, int width, int height)
{
    /*
    foreach(GamePiece *piece, *pieces) {
        //piece->setPixmap(piece->pixmap()->scaledToWidth(size,Qt::SmoothTransformation));
        piece->setPixScale(size);
        piece->resize(piece->pixmap()->size());
        piece->move(piece->pos()-grid->offset());
        piece->move((size*piece->pos())/grid->gridSize());
        piece->move(piece->pos()+QPoint(width,height));

    }

    grid->setGridSize(size);
    grid->setOffset(QPoint(width,height)); */
    emit gridAdjusted(size,QPoint(width,height));

}

int GameView::gridSize() {
    return grid->gridSize();
}

QPoint GameView::gridOffset() {
    return grid->offset();
}

/**
  * Draw the grid on the screen
  */
void GameView::drawGrid(int scale)
{
    QPainter painter(pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::SolidPattern);

    painter.fillRect(0,0,1000,1000,painter.brush());


    for (int x=0;x<this->width();x+=scale) {
        painter.drawLine(QPoint(x,0),QPoint(x,200));
    }
    for (int y=0;y<this->width();y+=scale) {
        painter.drawLine(QPoint(0,y),QPoint(this->width(),y));
    }
    this->update();

}

/**
  * Snap a point to the grid
  */
QPoint GameView::snapToGrid(QPoint p)
{
    QPoint check = p-grid->offset();

    double midpoint = grid->gridSize() / 2.0;

    int newx,newy;

    int overx = check.x()%(grid->gridSize());
    int overy = check.y()%(grid->gridSize());
    if (overx < midpoint)
        newx = check.x() - overx;
    else
        newx = check.x() + (grid->gridSize()-overx);
    if (overy < midpoint)
        newy = check.y() - overy;
    else
        newy = check.y() + (grid->gridSize()-overy);

    return QPoint(newx,newy)+grid->offset();
}


