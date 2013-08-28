#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>

class GamePiece;
class Grid;

class GameView : public QWidget
{
    Q_OBJECT
private:
    QScrollArea *area;
    QLabel *back;
    GamePiece *piece;
    QPixmap *pixmap;
    Grid *grid;
    QVector<GamePiece*> * pieces;

    bool gridShow;
    bool gridSnap;

    void drawGrid(int scale);
    QPoint snapToGrid(QPoint p);

public:
    explicit GameView(QWidget *parent = 0);
    virtual void resizeEvent(QResizeEvent *ev);
    void paintEvent(QPaintEvent *);
    bool getGridShow();
    bool getGridSnap();
    void setGridShow(bool b);
    void setGridSnap(bool b);
    void saveBoard(QString filename);
    void loadBoard(QString filename);

    void sendPieceCreated(QString img, QPoint pos);
    
    QPoint gridOffset();
    int gridSize();
signals:
    void pieceMoved(quint16,QPoint);
    void pieceCreated(QString, QPoint);
    void pieceCreatedPid(QString,QPoint,quint16);
    void clearBoard();
    void gridAdjusted(int,QPoint);
    void pieceDeleted(quint16);
    
public slots:
    void getPieceMoved(quint16 pid,QPoint p);
    void getPieceDeleted(quint16 pid);
    void addGamePiece(quint16 pid,QString imgName,QPoint pos);
    void updatePiece(quint16 pid,QPoint p);
    void adjustGrid(int size, int width, int height);
    void clearPieces();
    void adjustGrid(int size, QPoint offset);
    void removePiece(quint16 pid);
    
};

#endif // GAMEVIEW_H
