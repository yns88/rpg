#ifndef CHATCONNECTION_H
#define CHATCONNECTION_H

#include <QTcpSocket>
#include <QPixmap>
#include <QString>


class Server;

class ChatConnection : public QTcpSocket
{
    Q_OBJECT
private:
    Server *server;
    quint16 nextBlockSize;
public:
    explicit ChatConnection(Server *serv, QObject *parent = 0);
    void sendChatMessage(QString msg);
    
    void sendPieceMoved(quint16 pieceID, QPoint p);
    void createPiece(QString img,QPoint p, bool broadcast=true);
    void deletePiece(quint16 pid);
    void clearPieces();
    void sendGridAdjust(int size, QPoint offset);
    void rollDice(QString uname, int num, int sides);
signals:
    
public slots:
    void processChat();
    bool isGreeting(char *str);
    
};

#endif // CHATCONNECTION_H
