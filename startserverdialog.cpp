#include "startserverdialog.h"
#include "ui_startserverdialog.h"
#include "mainwindow.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * StartServerDialog
  * Dialog Window for starting a server
  */

StartServerDialog::StartServerDialog(MainWindow *win, QWidget *parent) :
    QDialog(parent), window(win),
    ui(new Ui::StartServerDialog)
{
    ui->setupUi(this);
}

StartServerDialog::~StartServerDialog()
{
    delete ui;
}

void StartServerDialog::on_buttonBox_rejected()
{
    this->deleteLater();
}

void StartServerDialog::on_buttonBox_accepted()
{
    quint16 port = static_cast<quint16>(ui->portEdit->text().toInt());
    window->startServer(port);
    window->startClient("localhost",port,ui->nameEdit->text());
    this->deleteLater();
}
