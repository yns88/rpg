#ifndef STARTSERVERDIALOG_H
#define STARTSERVERDIALOG_H

#include <QDialog>

class MainWindow;

namespace Ui {
class StartServerDialog;
}

class StartServerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartServerDialog(MainWindow *win, QWidget *parent = 0);
    ~StartServerDialog();
    
private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::StartServerDialog *ui;
    MainWindow *window;
};

#endif // STARTSERVERDIALOG_H
