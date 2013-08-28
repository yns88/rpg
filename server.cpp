#include "server.h"
#include "chatconnection.h"
#include "constants.h"
#include <QUdpSocket>
#include <QPoint>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * Server
  * Handles server-side user and board management
  *
  */

Server::Server(QObject *parent) :
    QTcpServer(parent), users(new QVector<user>()), nextpid(1), pieces(new QVector<struct serverGamePiece>()), nextuid(1),
    gridSize(100), gridOffset(QPoint(0,0))
{
}

/**
  * Adds a user to the server's userlist,
  * and broadcasts that the user has connected
  */
void Server::addUser(QString name, QHostAddress addr, quint16 port, ChatConnection *conn)
{
    // send the current user list to the guy who just connected
    foreach(user us, *users) {
        QByteArray block2;
        QDataStream out2(&block2,QIODevice::WriteOnly);
        out2.setVersion(QDataStream::Qt_4_3);
        out2 << quint16(0) << quint8('U') << QString("connect") << us.username << us.uid;
        out2.device()->seek(0);
        out2 << quint16(block2.size()-sizeof(qint16));
        conn->write(block2);
    }

    struct user u;
    u.addr = addr;
    u.username = name;
    u.port = port;
    u.conn = conn;
    if (users->size()==0) u.gm = true;
    else u.gm = false;
    u.uid = nextuid++;
    users->append(u);

    //QUdpSocket socket;
    QString message = name + "@" + addr.toString() + " has connected";

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << quint8('C') << message;
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    broadcastData(block);

    QByteArray block2;
    QDataStream out2(&block2,QIODevice::WriteOnly);
    out2.setVersion(QDataStream::Qt_4_3);
    out2 << quint16(0) << quint8('U') << QString("connect") << u.username << u.uid;
    out2.device()->seek(0);
    out2 << quint16(block2.size()-sizeof(qint16));
    broadcastData(block2);



    //broadcastChat(message);
    /*
    foreach (user us, *users) {
        socket.writeDatagram(message.toUtf8(),us.addr,us.port);
    } */
}

/**
  * Adds a game piece to the piece list
  */
void Server::addPiece(QString img, QPoint p) {
    struct serverGamePiece piece;
    piece.img = img;
    piece.p = p;
    piece.pid = nextpid;
    pieces->append(piece);
}

/**
  * Removes a game piece from the piece list
  */
void Server::removePiece(quint16 pid)
{
    for(int i=0;i<pieces->size();i++) {
        if (pieces->value(i).pid == pid) {
            pieces->remove(i);
        }
    }
}

/**
  * Updates a game piece to a new position
  */
void Server::movePiece(quint16 id, QPoint p) {
    for(int i=0;i<pieces->size();i++) {
        struct serverGamePiece piece = pieces->value(i);
        if (piece.pid == id) {
            piece.p = p;
            pieces->replace(i,piece);
        }
    }
}

/**
  * Removes all game pieces
  */
void Server::clearPieces() {
    //pieces->clear();
    if (pieces) delete pieces;
    pieces = new QVector<serverGamePiece>();
}

/**
  * Checks if a user is the GM
  */
bool Server::isGM(QHostAddress addr)
{
    return true;
    /*
    foreach (user u, *users) {
        if ((u.addr == addr) && u.gm) return true;
    } */
}

void Server::setGridSize(int size)
{
    gridSize = size;
}

void Server::setGridOffset(QPoint off)
{
    gridOffset = off;
}


/**
  * Sends the current state of the board to a client
  *
  */
void Server::sendBoardData(ChatConnection *conn) {

    QByteArray block0;
    QDataStream out0(&block0,QIODevice::WriteOnly);
    out0.setVersion(QDataStream::Qt_4_3);
    out0 << quint16(0) << quint8('B') << QString("grid") << gridSize << gridOffset;
    out0.device()->seek(0);
    out0 << quint16(block0.size()-sizeof(qint16));
    conn->write(block0);
    conn->waitForBytesWritten(10000);

    foreach(serverGamePiece piece, *pieces) {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('P') << QString("create") << piece.img << piece.pid << piece.p;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        conn->write(block);
        conn->waitForBytesWritten(10000);
    }
}

void Server::sendMessage(QString uname, QString message) {
    QString outStr = "<"+uname+"> "+message;
    broadcastChat(outStr);
    /*
    foreach(user u, *users) {
        socket.writeDatagram(outStr.toUtf8(),u.addr,u.port);
    } */
}

/**
  * Remove the user with the given username
  */
void Server::removeUser(QHostAddress addr, quint16 port)
{
    for(int i=0;i<users->size();i++) {
        user u = users->value(i);
        if ((u.addr == addr) && u.port == port) {
            users->remove(i);

            QString message = u.username+"@"+u.addr.toString()+" has disconnected";

            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_3);
            out << quint16(0) << quint8('C') << message;
            out.device()->seek(0);
            out << quint16(block.size()-sizeof(qint16));
            broadcastData(block);

            QByteArray block2;
            QDataStream out2(&block2,QIODevice::WriteOnly);
            out2.setVersion(QDataStream::Qt_4_3);
            out2 << quint16(0) << quint8('U') << QString("disconnect") << u.uid;
            out2.device()->seek(0);
            out2 << quint16(block2.size()-sizeof(qint16));
            broadcastData(block2);

            //broadcastChat(u.username+"@"+u.addr.toString()+" has disconnected");
        }
    }

}

void Server::sendPiecePosition(QPoint p)
{
    /*
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint8('P') << p;
    //out.device()->seek(0);
    //out << quint16(block.size()-sizeof(qint16));
    broadcastData(block);

    foreach(user u, *users) {
        if (u.conn) u.conn->sendPieceMoved(p);
    }
    */
}

void Server::broadcastChat(QString msg) {
    /*
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint8('C') << msg;
    //out.device()->seek(0);
    //out << quint16(block.size()-sizeof(qint16));
    foreach(user u, *users) {
        socket.writeDatagram(block,u.addr,u.port);
    } */

    foreach(user u, *users) {
        if (u.conn)
            u.conn->sendChatMessage(msg);
    }
}

/**
  * Broadcasts a packet to all users
  */
void Server::broadcastData(QByteArray block)
{
    foreach(user u, *users) {
        if (u.conn)
            u.conn->write(block);
            u.conn->flush();

        //socket.writeDatagram(block,u.addr,u.port);
    }
}

/**
  * Get the next piece ID
  */
quint16 Server::nextPieceID()
{
    return nextpid++;
}


/**
  * When the server receives a socket connection, it creates a ChatConnection socket to handle it
  */
void Server::incomingConnection(int handle){
    ChatConnection * conn = new ChatConnection(this);
    conn->setSocketDescriptor(handle);
    //this->addPendingConnection(conn);

}
