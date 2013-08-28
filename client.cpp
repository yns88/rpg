#include "client.h"
#include <QHostAddress>
#include "constants.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHostInfo>

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * Client
  * Encapsulates all networking code on the client-side
  *
  */

Client::Client(QTcpSocket *conn, QUdpSocket *recv, QObject *parent)
    : QObject(parent), socket(conn), isConnected(false), receiver(recv), alert(NULL), nextBlockSize(0)
{
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(catchError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(processSocket()));
    //connect(receiver,SIGNAL(readyRead()),this,SLOT(processBroadcast()));
}

/**
  * Connects to the server at the given address with the given username
  *
  */
void Client::connectToServer(QString addrName, quint16 port, quint16 clientport, QString username) {
    if (isConnected) {
        disconnectFromServer();
        isConnected = false;
    }
    QHostInfo info = QHostInfo::fromName(addrName);
    if (!info.addresses().empty()) {
        QHostAddress addr = info.addresses().value(0);
        serverAddress = addr;
        serverPort = port;
        clientPort = clientport;
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(10000);
        }
        socket->connectToHost(serverAddress,serverPort);
        if (socket->waitForConnected(10000)) {
            sendGreeting(username,clientport);
            isConnected = true;
            emit connectState(true);
        }
    }
}

/**
  * Sends a greeting message to the server when a client connects
  *
  */
void Client::sendGreeting(QString username, quint16 port){
    if (socket && socket->state()==QAbstractSocket::ConnectedState) {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('G') << username << port;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Sends a chat message to the server
  *
  */
void Client::sendChatMessage(QString user, QString message) {
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('C') << user << message;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Requests a dice roll from the server
  *
  */
void Client::sendRollDice(QString user, int num, int sides) {
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('R') << user << QString::number(num)+"d"+QString::number(sides);
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Tells the server you are disconnecting, if you are connected
  *
  */
void Client::disconnectFromServer()
{
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('D') << clientPort;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->waitForConnected(3000);
        if (socket->state()==QAbstractSocket::ConnectedState) {
            socket->write(block);
            socket->waitForBytesWritten(3000);
            socket->close();

        }
    }
    isConnected = false;
    emit(connectState(false));
}

/**
  * In the case of an error, pop up an alert window
  *
  */
void Client::catchError(QAbstractSocket::SocketError ) {
    if (!alert) {
        alert = new QWidget();
        alert->setWindowTitle("ERROR");
        QVBoxLayout *layout = new QVBoxLayout(alert);
        QLabel *label = new QLabel(socket->errorString(),alert);
        QPushButton *button = new QPushButton("OK",alert);
        layout->addWidget(label);
        layout->addWidget(button);
        alert->connect(button,SIGNAL(clicked()),alert,SLOT(close()));
        alert->show();
    }
    isConnected = false;
    emit(connectState(false));
}

/**
  * send a new piece position to the server
  *
  */
void Client::sendPieceMoved(quint16 pid,QPoint p)
{
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('P') << QString("move") << pid << p;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Send a new piece created to the server
  *
  */
void Client::sendPieceCreated(QString img, QPoint p)
{
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0);
        out << quint8('P');
        out << QString("create");
        out << img;
        out << p;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Notify the server that a piece is being deleted
  *
  */
void Client::sendPieceDeleted(quint16 pid) {
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('P') << QString("delete") << pid;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Ask the server to clear the board
  *
  */
void Client::sendClearBoard()
{
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0);
        out << quint8('P');
        out << QString("clear");
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Request the server to adjust the grid size and offset
  *
  */
void Client::sendGridAdjusted(int size, QPoint offset)
{
    if (socket && isConnected) {
        //socket->connectToHost(serverAddress,serverPort);
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << quint8('B') << QString("grid") << size << offset;
        out.device()->seek(0);
        out << quint16(block.size()-sizeof(qint16));
        socket->write(block);
        //socket->close();
    }
}

/**
  * Process an incoming packet or set of packets from the server
  *
  */
void Client::processSocket()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_3);

    bool processingSocket = true;


    while (processingSocket) {

        if (nextBlockSize == 0) {
            if (socket->bytesAvailable() < sizeof(quint16))
                return;
            in >> nextBlockSize;
        }
        if (socket->bytesAvailable() < nextBlockSize)
            return;


        quint8 requestType;
        in >> requestType;
        if (requestType == 'C') {
            QString msg;
            in >> msg;
            emit newChatMessage(msg);
        }
        else if (requestType == 'P') {
            QString pAction;

            in >> pAction;

            if (pAction == "move") {
                quint16 pid;
                QPoint p;
                in >> pid >> p;
                emit pieceMoved(pid,p);
            }
            else if (pAction == "create") {
                QString img;
                quint16 pid;
                QPoint p;
                in >> img >> pid >> p;
                emit pieceCreated(pid,img,p);
            }
            else if (pAction == "clear") {
                emit pieceClear();
            }
            else if (pAction == "delete") {
                quint16 pid;
                in >> pid;
                emit pieceDeleted(pid);
            }
        }
        /* B = Board actions */
        else if (requestType == 'B') {
            QString bAction;
            in >> bAction;

            /* adjust the grid */
            if (bAction == "grid") {
                int size;
                QPoint offset;
                in >> size >> offset;
                emit gridAdjusted(size,offset);
            }
        }
        /* U = User connect/disconnect */
        else if (requestType == 'U') {
            QString uAction;
            in >> uAction;
            /* connected */
            if (uAction == "connect") {
                QString uname;
                quint16 uid;
                in >> uname >> uid;
                emit userConnected(uname,uid);
            }
            /* disconnected */
            if (uAction == "disconnect") {
                quint16 uid;
                in >> uid;
                emit userDisconnected(uid);
            }
        }

        if (socket->bytesAvailable() <= 0) processingSocket = false;
        nextBlockSize = 0;
    }
}

void Client::processBroadcast()
{
    /*
    QByteArray datagram;
    do {
        datagram.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(datagram.data(),datagram.size());
    } while (receiver->hasPendingDatagrams());

    QDataStream in(&datagram,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_3);

    quint8 requestType;
    in >> requestType;
    if (requestType == 'C') {
        QString msg;
        in >> msg;
        emit newChatMessage(msg);
    }
    else if (requestType == 'P') {
        QPoint p;
        in >> p;
        emit pieceMoved(p);
    }
    */


}
