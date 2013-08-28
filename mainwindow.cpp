#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatview.h"
#include "statusview.h"
#include "gameview.h"
#include <QSplitter>
#include "constants.h"
#include "server.h"
#include "client.h"
#include "connectdialog.h"
#include "startserverdialog.h"
#include <QScrollArea>
#include <QPoint>
#include <QMouseEvent>
#include "gridadjustdialog.h"
#include "baritem.h"
#include "bargamepiece.h"
#include "bardice.h"
#include <QFileDialog>
#include "aboutdialog.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * Main Window
  * Contains all views and main menu items
  *
  */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), server(NULL), socket(new QTcpSocket()), receiver(new QUdpSocket()), sideBar(new QVBoxLayout()), topLevelLayout(new QHBoxLayout())
{

    client = new Client(socket,receiver,this);
    statusView = new StatusView(this);
    ui->setupUi(this);
    QScrollArea *area = new QScrollArea(this);

    gameView = new GameView(area);
    area->setWidget(gameView);

    username = "guest";

    //mainview = new QMdiArea(this);
    //mainview->move(0,this->menuBar()->size().height());
    //mainview->tileSubWindows();

    receiver->bind();

    //socket = new QTcpSocket();

    mainLayout = new QVBoxLayout();

    QScrollArea *sideBarScrollArea = new QScrollArea(this);
    QWidget *sideBarContents = new QWidget(this);
    sideBarContents->move(sideBarContents->x(),30);
    sideBarContents->setLayout(sideBar);

    //this->setWindowTitle(QString::number(sideBarContents->pos().x()));



    BarItem *testlabel = new BarGamePiece(":/assets/img/chip.png",gameView,this);
    //testlabel->setPixmap(QPixmap(":/assets/img/chip.png").scaledToWidth(100));
    //testlabel->resize(100,100);
    BarItem *testlabel2 = new BarGamePiece(":/assets/img/chip-red.png",gameView,this);
    BarDice *dicelabel = new BarDice(":/assets/img/dice.png",gameView,statusView,this);



    sideBar->addWidget(testlabel,0);
    sideBar->addWidget(testlabel2,0);
    sideBar->addWidget(dicelabel,0);
    sideBar->setAlignment(Qt::AlignTop);


    sideBarScrollArea->setMaximumWidth(105);
    sideBarScrollArea->setMinimumWidth(105);
    topLevelLayout->addWidget(sideBarScrollArea);
    sideBarContents->resize(105,this->height());
    //sideBarScrollArea->resize(100,this->height());

    //sideBar->setGeometry(QRect(0,0,100,300));

    topLevelLayout->addLayout(mainLayout,1);



    split = new QSplitter(this);
    split->setOrientation(Qt::Vertical);
    hsplit = new QSplitter(this);
    hsplit->setOrientation(Qt::Horizontal);


    chatview = new ChatView(socket,receiver,client,this);
    //chatview->setConnection(socket);
    //ChatView *chat2 = new ChatView(this);




    split->addWidget(hsplit);
    split->addWidget(statusView);
    split->show();




    hsplit->addWidget(area);
    hsplit->addWidget(chatview);
    hsplit->show();

    QList<int> sizeList = QList<int>();
    sizeList.append(450);
    sizeList.append(150);
    split->setSizes(sizeList);

    QList<int> sizeList2 = QList<int>();
    sizeList2.append(500);
    sizeList2.append(300);
    hsplit->setSizes(sizeList2);



    mainLayout->addWidget(split);
    //setLayout(mainLayout);
    centralWidget()->setLayout(topLevelLayout);

    connect(dicelabel,SIGNAL(rollDice(int,int,bool)),this,SLOT(getDiceRoll(int,int,bool)));
    connect(client,SIGNAL(connectState(bool)),chatview,SLOT(changeConnectedState(bool)));
    connect(client,SIGNAL(connectState(bool)),statusView,SLOT(changeConnectedState(bool)));
    connect(gameView,SIGNAL(pieceMoved(quint16,QPoint)),client,SLOT(sendPieceMoved(quint16,QPoint)));
    connect(gameView,SIGNAL(pieceCreated(QString,QPoint)),client,SLOT(sendPieceCreated(QString,QPoint)));
    connect(gameView,SIGNAL(clearBoard()),client,SLOT(sendClearBoard()));
    connect(gameView,SIGNAL(gridAdjusted(int,QPoint)),client,SLOT(sendGridAdjusted(int,QPoint)));
    connect(gameView,SIGNAL(pieceDeleted(quint16)),client,SLOT(sendPieceDeleted(quint16)));
    connect(client,SIGNAL(newChatMessage(QString)),chatview,SLOT(updateChatDisplay(QString)));
    connect(client,SIGNAL(pieceMoved(quint16,QPoint)),gameView,SLOT(updatePiece(quint16,QPoint)));
    connect(client,SIGNAL(pieceCreated(quint16,QString,QPoint)),gameView,SLOT(addGamePiece(quint16,QString,QPoint)));
    connect(client,SIGNAL(pieceDeleted(quint16)),gameView,SLOT(removePiece(quint16)));
    connect(client,SIGNAL(pieceClear()),gameView,SLOT(clearPieces()));
    connect(client,SIGNAL(gridAdjusted(int,QPoint)),gameView,SLOT(adjustGrid(int,QPoint)));
    connect(client,SIGNAL(userConnected(QString,quint16)),statusView,SLOT(addUser(QString,quint16)));
    connect(client,SIGNAL(userDisconnected(quint16)),statusView,SLOT(removeUser(quint16)));

    //gameView->move(110,30);
    //area->move(sideBarContents->width(),ui->menuBar->height());
    //this->setWindowTitle(QString::number(gameView->mapToGlobal(area->pos()).x()));
    setWindowTitle("RPG Platform");
}

void MainWindow::paintEvent(QPaintEvent *ev) {
    //split->resize(this->size().width(),this->size().height()-this->menuBar()->size().height());
}

/**
  * On close, send a disconnect message to the server
  */
void MainWindow::closeEvent(QCloseEvent *ev)
{
    client->disconnectFromServer();
    ev->accept();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionStart_Server_triggered()
{
    StartServerDialog *d = new StartServerDialog(this);
    d->show();

}

/**
  * Create a new server that listens on the specified port
  */
void MainWindow::startServer(quint16 port) {
    if (server) delete server;
    server = new Server();
    server->listen(QHostAddress::Any,port);
}

/**
  * Send a dice roll request to the server, or do a private local roll
  */
void MainWindow::getDiceRoll(int num, int sides, bool b)
{
    if (b) {
        client->sendRollDice(this->username,num,sides);
    }
    else {
        QString result = "roll "+ QString::number(num) + "d" + QString::number(sides) + ": ";
        while (num > 0) {
            result += QString::number((qrand()%sides)+1) + " ";
            num--;
        }
        statusView->addMessage(result);
    }
}

void MainWindow::on_actionConnect_as_Client_triggered()
{
    ConnectDialog *d = new ConnectDialog(this);

    d->show();
}

/**
  * Have the client connect to the specified server
  */
void MainWindow::startClient(QString addr, quint16 port, QString user) {
    username = user;
    client->connectToServer(addr,port,receiver->localPort(),username);
}

QString MainWindow::getUser() {
    return username;
}

void MainWindow::on_actionDisconnect_triggered()
{
    client->disconnectFromServer();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionChange_Name_triggered()
{

}

void MainWindow::on_actionShow_Grid_triggered()
{
    gameView->setGridShow(!gameView->getGridShow());
}

void MainWindow::on_actionSnap_to_Grid_triggered()
{
    gameView->setGridSnap(!gameView->getGridSnap());
}

void MainWindow::on_actionAdjust_Grid_triggered()
{
    GridAdjustDialog *d = new GridAdjustDialog(gameView->gridSize(),gameView->gridOffset().x(),gameView->gridOffset().y());
    d->show();
    connect(d,SIGNAL(gridAdjusted(int,int,int)),gameView,SLOT(adjustGrid(int,int,int)));
}

void MainWindow::on_actionSave_Board_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save File","","Files (*.rpg)");
    gameView->saveBoard(filename);
}

void MainWindow::on_actionLoad_Board_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open File","","Files (*.rpg)");
    gameView->loadBoard(filename);
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog * d = new AboutDialog();
    d->show();
}
