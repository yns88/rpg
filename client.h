#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include <QPoint>
#include <QUdpSocket>
#include <QPixmap>

class Client : public QObject
{
    Q_OBJECT
private:


    QTcpSocket * socket;
    QUdpSocket * receiver;
    QWidget *alert;
    QHostAddress serverAddress;
    quint16 serverPort;
    quint16 clientPort;
    bool isConnected;
    quint16 nextBlockSize;

    void sendGreeting(QString username, quint16 port);

public:
    explicit Client(QTcpSocket *conn, QUdpSocket *recv, QObject *parent=0);
    virtual ~Client(){}
    void connectToServer(QString addrName, quint16 port, quint16 clientport, QString username);
    void sendChatMessage(QString user, QString message);
    void disconnectFromServer();

signals:
    void connectState(bool);
    void newChatMessage(QString);
    void pieceMoved(quint16,QPoint);
    void pieceCreated(quint16,QString,QPoint);
    void pieceClear();
    void gridAdjusted(int,QPoint);
    void userConnected(QString,quint16);
    void userDisconnected(quint16);
    void pieceDeleted(quint16);


public slots:
    void catchError(QAbstractSocket::SocketError);
    void sendPieceMoved(quint16 pid,QPoint p);
    void sendPieceCreated(QString img,QPoint p);
    void sendPieceDeleted(quint16 pid);
    void sendClearBoard();
    void sendGridAdjusted(int size, QPoint offset);
    void sendRollDice(QString user,int num, int sides);
    void processSocket();
    void processBroadcast();
};

#endif // CLIENT_H
