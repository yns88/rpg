#include "chatconnection.h"
#include "server.h"
#include <QPoint>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * ChatConnection
  * Socket class for server-side communication with clients
  * Each ChatConnection has a client socket counterpart
  */

ChatConnection::ChatConnection(Server *serv, QObject *parent) :
    QTcpSocket(parent), server(serv)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(processChat()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    nextBlockSize = 0;

}

/**
  * Broadcast a chat message to all clients
  *
  */
void ChatConnection::sendChatMessage(QString msg)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << quint8('C') << msg;
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    server->broadcastData(block);
}

/**
  * Broadcast a new piece position to all clients
  *
  */
void ChatConnection::sendPieceMoved(quint16 pieceID, QPoint p) {
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << quint8('P') << QString("move") << pieceID << p;
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    server->movePiece(pieceID,p);
    server->broadcastData(block);
}

/**
  * Broadcast a created piece to all clients
  *
  */
void ChatConnection::createPiece(QString img, QPoint p, bool broadcast) {

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << quint8('P') << QString("create") << img << server->nextPieceID() << p;
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    server->broadcastData(block);

}

/**
  * Broadcast a deleted piece to all clients
  *
  */
void ChatConnection::deletePiece(quint16 pid)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << quint8('P') << QString("delete") << pid;
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    server->broadcastData(block);
}

/**
  * Broadcast a clear board to all clients
  *
  */
void ChatConnection::clearPieces() {
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << quint8('P') << QString("clear");
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    server->broadcastData(block);
}

/**
  * Broadcast an adjusted grid to all clients
  *
  */
void ChatConnection::sendGridAdjust(int size, QPoint offset) {
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint16(0) << quint8('B') << QString("grid") << size << offset;
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    server->broadcastData(block);
}

/**
  * Broadcast a dice roll to all clients
  *
  */
void ChatConnection::rollDice(QString uname, int num, int sides)
{
    QString result = "<FONT COLOR=darkred>* " + uname + " rolls "+ QString::number(num) + "d" + QString::number(sides) + ": ";
    if (num > 0) {
        result += QString::number((qrand()%sides)+1);
        num--;
    }
    while (num > 0) {
        result += ", " + QString::number((qrand()%sides)+1);
        num--;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << quint16(0) << quint8('C') << result;
    out.device()->seek(0);
    out << quint16(block.size()-sizeof(qint16));
    server->broadcastData(block);
}

/**
  * Process an incoming packet or set of packets from the client socket
  *
  */
void ChatConnection::processChat()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_3);

    bool processing = true;

    while (processing) {

        if (nextBlockSize == 0) {
            if (bytesAvailable() < sizeof(quint16))
                return;
            in >> nextBlockSize;
        }
        if (bytesAvailable() < nextBlockSize)
            return;


        quint8 requestType;
        in >> requestType;

        /* G=Greeting, add new user to server */
        if (requestType=='G') {
            QString username;
            quint16 port;
            in >> username >> port;
            server->addUser(username,this->peerAddress(),port,this);
            server->sendBoardData(this);
        }
        /* C=Chat message, broadcast chat to all users */
        else if (requestType=='C') {
            QString username;
            QString message;
            in >> username >> message;

            // If message is /roll XXX, do a dice roll
            QStringList list = message.split(" ",QString::KeepEmptyParts,Qt::CaseInsensitive);
            if (list.size() > 1 && list.value(0) == "/roll") {
                QStringList list2 = list.value(1).split("d",QString::KeepEmptyParts,Qt::CaseInsensitive);
                if (list2.size() == 2) {
                    int num = list2.value(0).toInt();
                    int sides = list2.value(1).toInt();
                    if (num <= 0) num = 1;
                    if (sides > 0) this->rollDice(username,num,sides);
                }
            }
            else {

                QString outStr = "<"+username+"> "+message;
                this->sendChatMessage(outStr);
            }
            //write(outStr.toUtf8());
            //server->sendMessage(username,message);
        }
        /* D=Disconnect, remove this user */
        else if (requestType=='D') {
            quint16 port;
            in >> port;
            server->removeUser(this->peerAddress(),port);
        }
        /* P=Piece, send piece actions */
        else if (requestType=='P') {
            QString pAction;

            in >> pAction;
            if (pAction == "move") {
                quint16 pid;
                QPoint p;
                in >> pid >> p;
                this->sendPieceMoved(pid,p);
            }
            else if (pAction == "create") {
                QString img;
                QPoint p;
                in >> img >> p;
                server->addPiece(img,p);
                this->createPiece(img,p);
            }
            else if (pAction == "clear") {
                if (server->isGM(this->peerAddress())) {
                    server->clearPieces();
                    this->clearPieces();
                }
            }
            else if (pAction == "delete") {
                quint16 pid;
                in >> pid;
                server->removePiece(pid);
                this->deletePiece(pid);
            }
            //server->sendPiecePosition(p);
        }
        /* B=Board */
        else if (requestType == 'B') {
            QString bAction;
            in >> bAction;

            if (bAction == "grid") {
                int size;
                QPoint offset;
                in >> size >> offset;
                server->setGridSize(size);
                server->setGridOffset(offset);
                this->sendGridAdjust(size,offset);
            }
        }
        /* R=Dice Roll */
        else if (requestType == 'R') {
            QString user;
            QString dice;
            in >> user >> dice;
            QStringList list = dice.split("d",QString::KeepEmptyParts,Qt::CaseInsensitive);
            if (list.size() == 2) {
                int num = list.value(0).toInt();
                int sides = list.value(1).toInt();
                if (num <= 0) num = 1;
                if (sides > 0) this->rollDice(user,num,sides);
            }
        }

        if (this->bytesAvailable() <= 0) processing = false;
        nextBlockSize = 0;
        //this->close();
    }
}

bool ChatConnection::isGreeting(char *str)
{
    return false;
}

