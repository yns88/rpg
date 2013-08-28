#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

class ChatView;
class StatusView;
class GameView;
class Client;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QHBoxLayout * topLevelLayout;
    QVBoxLayout * mainLayout;
    QVBoxLayout * sideBar;
    QMdiArea *mainview;
    ChatView *chatview;
    QSplitter *split;
    QSplitter *hsplit;
    StatusView *statusView;
    GameView *gameView;
    QTcpServer *server;
    QTcpSocket *socket;
    Client *client;
    QUdpSocket *receiver;
    QString username;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent *);
    virtual void closeEvent(QCloseEvent *ev);

    void startClient(QString addr, quint16 port, QString user);
    QString getUser();
    void startServer(quint16 port);
private slots:

    void getDiceRoll(int num, int sides, bool b=true);

    void on_actionStart_Server_triggered();

    void on_actionConnect_as_Client_triggered();

    void on_actionDisconnect_triggered();

    void on_actionExit_triggered();

    void on_actionChange_Name_triggered();

    void on_actionShow_Grid_triggered();

    void on_actionSnap_to_Grid_triggered();

    void on_actionAdjust_Grid_triggered();

    void on_actionSave_Board_triggered();

    void on_actionLoad_Board_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
