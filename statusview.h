#ifndef STATUSVIEW_H
#define STATUSVIEW_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QListView>
#include <QSplitter>
#include <QListWidget>

class StatusView : public QWidget
{
    Q_OBJECT
private:
    QTextBrowser * textView;
    QHBoxLayout * layout;
    //QListView * userList;
    QListWidget * userList;
    QVector<quint16> * userIdList;
    QSplitter * split;
public:
    explicit StatusView(QWidget *parent = 0);
    

signals:
    
public slots:
    void addUser(QString username, quint16 id);
    void removeUser(quint16 id);
    void clearUsers();
    void addMessage(QString message);
    void changeConnectedState(bool b);
    
};

#endif // STATUSBAR_H
