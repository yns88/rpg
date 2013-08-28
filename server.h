#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <gamepiece.h>

class ChatConnection;

struct user {
    QString username;
    QHostAddress addr;
    quint16 port;
    ChatConnection * conn;
    bool gm;
    quint16 uid;
};

struct serverGamePiece {
    QString img;
    QPoint p;
    quint16 pid;
};




class Server : public QTcpServer
{
    Q_OBJECT

private:
    QVector<user> *users;
    QUdpSocket socket;
    quint16 nextpid;
    quint16 nextuid;
    QVector<struct serverGamePiece> *pieces;
    int gridSize;
    QPoint gridOffset;

    void broadcastChat(QString msg);

protected:
    void incomingConnection(int handle);
public:
    explicit Server(QObject *parent = 0);
    void addUser(QString name, QHostAddress addr, quint16 port, ChatConnection *conn);
    void removeUser(QHostAddress addr, quint16 port);
    void sendPiecePosition(QPoint p);
    void broadcastData(QByteArray block);
    quint16 nextPieceID();
    
    void sendMessage(QString uname, QString message);

    void addPiece(QString img, QPoint p);
    void removePiece(quint16 pid);
    void sendBoardData(ChatConnection *conn);
    void movePiece(quint16 id, QPoint p);
    void clearPieces();
    bool isGM(QHostAddress addr);

    void setGridSize(int size);
    void setGridOffset(QPoint off);
signals:
    
public slots:
    
};

#endif // SERVER_H
