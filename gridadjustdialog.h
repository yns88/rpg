#ifndef GRIDADJUSTDIALOG_H
#define GRIDADJUSTDIALOG_H

#include <QDialog>

namespace Ui {
class GridAdjustDialog;
}

class GridAdjustDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GridAdjustDialog(int scale=100, int offh=0, int offv=0, QWidget *parent = 0);
    ~GridAdjustDialog();

signals:
    void gridAdjusted(int size, int h, int v);
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::GridAdjustDialog *ui;
};

#endif // GRIDADJUSTDIALOG_H
