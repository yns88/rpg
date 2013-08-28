#ifndef GAMEPIECE_H
#define GAMEPIECE_H

#include <QLabel>

class GamePiece : public QLabel
{
    Q_OBJECT
private:
    QPoint delta;
    quint16 pid;
    QString pixName;
protected:
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
public:
    explicit GamePiece(QString imgName, quint16 pieceID=0, QWidget *parent = 0);
    explicit GamePiece(QByteArray serial, QWidget *parent = 0);
    QString imageName();
    QByteArray serialize();
    quint16 pieceID();
    void setPixScale(int size);
    
signals:
    void moved(quint16,QPoint);
    void deleted(quint16);
    
public slots:
    
};

#endif // GAMEPIECE_H
