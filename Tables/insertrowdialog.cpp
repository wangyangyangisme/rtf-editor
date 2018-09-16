#include "insertrowdialog.h"
#include "ui_insertrowdialog.h"

insertRowDialog::insertRowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::insertRowDialog)
{
    ui->setupUi(this);
    position = POSITION_BEFORE_CURSOR;
}

insertRowDialog::~insertRowDialog()
{
    delete ui;
}

void insertRowDialog::on_buttonBox_accepted()
{
    close();
}

int insertRowDialog::getAmount()
{
    return ui->amountSpin->value();
}

int insertRowDialog::getPosition()
{
    return position;
}

void insertRowDialog::on_afterRadio_clicked()
{
    position = POSITION_AFTER_CURSOR;
    ui->beforeRadio->setDown(false);
}

void insertRowDialog::on_beforeRadio_clicked()
{
    position = POSITION_BEFORE_CURSOR;
    ui->afterRadio->setDown(false);
}
