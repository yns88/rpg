#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

class MainWindow;

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectDialog(MainWindow *win, QWidget *parent=0);
    ~ConnectDialog();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ConnectDialog *ui;
    MainWindow *window;
};

#endif // CONNECTDIALOG_H
