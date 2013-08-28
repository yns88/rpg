#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QWidget>
#include <QTextBrowser>
#include <QLineEdit>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QLabel>

class MainWindow;
class Client;

class ChatView : public QWidget
{
    Q_OBJECT

private:
    QTextBrowser * chatDisplay;
    QLineEdit * chatLine;
    QLabel * userLabel;
    QVector<QString> * chatHistory;
    QVBoxLayout * layout;
    QHBoxLayout * chatLineLayout;
    QTcpSocket * connection;
    QUdpSocket * receiver;
    MainWindow *window;
    Client *client;
public:
    explicit ChatView( QTcpSocket *conn, QUdpSocket *recv, Client *clnt, MainWindow *parent=0);
    void setConnection(QTcpSocket *conn);
    void paintEvent(QPaintEvent *ev);


    void sendChatMessage(QString user, QString message);
signals:
    
public slots:
    void onChatEntered();
    void updateChatDisplay(QString msg);
    void changeConnectedState(bool b);
    
};

#endif // CHATVIEW_H
