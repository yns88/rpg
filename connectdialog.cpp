#include "connectdialog.h"
#include "ui_connectdialog.h"
#include "mainwindow.h"
#include "client.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * ConnectDialog
  * Dialog window for connecting to a server
  *
  */

ConnectDialog::ConnectDialog(MainWindow *win, QWidget *parent) :
    QDialog(parent), window(win),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

/**
  * On accept, connect to the server and close this window
  *
  */
void ConnectDialog::on_buttonBox_accepted()
{
    window->startClient(ui->addrEdit->text(),static_cast<quint16>(ui->portEdit->text().toInt()),ui->nameEdit->text());
    this->deleteLater();
}

/**
  * On reject, close this window
  */
void ConnectDialog::on_buttonBox_rejected()
{
    this->deleteLater();
}
