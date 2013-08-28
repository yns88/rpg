#include "gridadjustdialog.h"
#include "ui_gridadjustdialog.h"

/**
  * Author: Yunus Rahbar
  * Date: 6/10/13
  *
  */

/**
  * GridAdjustDialog
  * Dialog window for adjusting the game view grid
  */

GridAdjustDialog::GridAdjustDialog(int scale, int offh, int offv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GridAdjustDialog)
{
    ui->setupUi(this);

    connect(ui->sizeSlider,SIGNAL(valueChanged(int)),ui->sizeSpinbox,SLOT(setValue(int)));
    connect(ui->sizeSpinbox,SIGNAL(valueChanged(int)),ui->sizeSlider,SLOT(setValue(int)));
    connect(ui->offsetHSlider,SIGNAL(valueChanged(int)),ui->offsetHSpinbox,SLOT(setValue(int)));
    connect(ui->offsetHSpinbox,SIGNAL(valueChanged(int)),ui->offsetHSlider,SLOT(setValue(int)));
    connect(ui->offsetVSlider,SIGNAL(valueChanged(int)),ui->offsetVSpinbox,SLOT(setValue(int)));
    connect(ui->offsetVSpinbox,SIGNAL(valueChanged(int)),ui->offsetVSlider,SLOT(setValue(int)));

    ui->sizeSlider->setValue(scale);
    ui->offsetHSlider->setValue(offh);
    ui->offsetVSlider->setValue(offv);
}

GridAdjustDialog::~GridAdjustDialog()
{
    delete ui;
}

void GridAdjustDialog::on_buttonBox_accepted()
{
    emit gridAdjusted(ui->sizeSlider->value(),ui->offsetHSlider->value(),ui->offsetVSlider->value());
    this->close();
}

void GridAdjustDialog::on_buttonBox_rejected()
{
    this->close();
}
