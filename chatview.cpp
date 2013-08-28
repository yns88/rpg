#include "chatview.h"
#include "constants.h"
#include "mainwindow.h"
#include "client.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * ChatView
  * Window for displaying and entering chat messages between clients
  *
  */

ChatView::ChatView(QTcpSocket *conn, QUdpSocket *recv, Client *clnt, MainWindow *parent) :
    QWidget(parent), chatDisplay(new QTextBrowser(this)), chatLine(new QLineEdit(this)), chatHistory(new QVector<QString>()),
    layout(new QVBoxLayout(this)), chatLineLayout(new QHBoxLayout()), receiver(recv), connection(conn), window(parent),
    client(clnt), userLabel(new QLabel(this))
{
    setEnabled(false);
    chatLine->setText("<Not Connected>");
    //receiver->bind(Constants::clientport);
    //receiver->bind();

    layout->addWidget(chatDisplay);
    chatLineLayout->addWidget(userLabel);
    chatLineLayout->addWidget(chatLine);
    layout->addLayout(chatLineLayout);
    userLabel->setText("No name:");


    //connect(connection,SIGNAL(readyRead()),this,SLOT(updateChatDisplay()));
    connect(chatLine,SIGNAL(returnPressed()),SLOT(onChatEntered()));
    //connect(receiver,SIGNAL(readyRead()),this,SLOT(updateChatDisplay()));

    QHostAddress addr = QHostAddress("127.0.0.1");
    //connection->connectToHost(addr,Constants::hostport);
}

/**
  * set the connection socket to use
  *
  */
void ChatView::setConnection(QTcpSocket *conn)
{
    if (conn) {
    connection = conn;
    connect(connection,SIGNAL(readyRead()),this,SLOT(updateChatDisplay()));
    }
}

/**
  * sends a new chat message through the client socket
  *
  */
void ChatView::onChatEntered() {
    client->sendChatMessage(window->getUser(),chatLine->text());
    chatLine->clear();
}

/**
  * adds a new chat message to the display
  *
  */
void ChatView::updateChatDisplay(QString msg)
{
    chatHistory->append(msg);
    chatDisplay->append(msg);
    /*
    if (receiver) {
        //char str[256];
        //receiver->readLine(str,255);
        QByteArray datagram;
        do {
            datagram.resize(receiver->pendingDatagramSize());
            receiver->readDatagram(datagram.data(),datagram.size());
        } while (receiver->hasPendingDatagrams());

        QDataStream in(&datagram,QIODevice::ReadOnly);
        QString msg;
        in >> msg;

        chatHistory->append(msg);
        chatDisplay->append(QString::fromUtf8(datagram.data()));
    } */
}


/**
  * Change the state of the chat window between connected (enabled) and disconnected (disabled)
  *
  */
void ChatView::changeConnectedState(bool b)
{
    if (b) {
        setEnabled(true);
        chatLine->setText("");
        userLabel->setText(window->getUser()+":");
        chatDisplay->append("--Connected to Server--");
    }
    else {
        setEnabled(false);
        chatDisplay->append("--Disconnected from Server--");
        chatLine->setText("<Not Connected>");
        userLabel->setText("No name:");
    }
}

void ChatView::paintEvent(QPaintEvent *ev) {

}
