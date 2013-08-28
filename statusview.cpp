#include "statusview.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * StatusView
  * View for displaying status messages and the connected user list
  */

StatusView::StatusView(QWidget *parent) :
    QWidget(parent), textView(new QTextBrowser(this)), layout(new QHBoxLayout(this)), userList(new QListWidget(this)),
    split(new QSplitter(this)), userIdList(new QVector<quint16>())
{

    layout->addWidget(split);
    split->addWidget(textView);
    split->addWidget(userList);

    QList<int> sizes;
    sizes.append(600);
    sizes.append(200);
    split->setSizes(sizes);


    //textView->append(QString::number(this->sizeHint().height()));
    //textView->append(QString::number(this->minimumHeight()));





    //this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //textView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

/**
  * Appends a message to the text view
  */
void StatusView::addMessage(QString message) {
    textView->append(message);
}

/**
  * Sets state to connected or disconnected
  */
void StatusView::changeConnectedState(bool b)
{
    if (b) {
        textView->append("Connected to server.");
        userList->setEnabled(b);
    }
    else {
        textView->append("Disconnected from server.");
        clearUsers();
        userList->setEnabled(b);
    }
}

/**
  * Adds a username to the user list
  */
void StatusView::addUser(QString username, quint16 id)
{
    userList->addItem(username);
    userIdList->append(id);

}

/**
  * Removes the user with the given user id from the user list
  */
void StatusView::removeUser(quint16 id)
{
    quint16 uid = userIdList->indexOf(id);
    if (uid >= 0) {
        QListWidgetItem *item = userList->item(uid);
        userList->removeItemWidget(item);
        delete item;
    }
}

/**
  * Removes all users
  */
void StatusView::clearUsers()
{
    userList->clear();
}
