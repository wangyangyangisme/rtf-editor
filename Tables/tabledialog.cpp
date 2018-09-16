#include "tabledialog.h"
#include "ui_tabledialog.h"

TableDialog::TableDialog(QTextTable* tb, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    tableFormat = tb->format();
    ui->setupUi(this);
    ui->paddingPin->setValue(tableFormat.cellPadding());
    ui->spacingPin->setValue(tableFormat.cellSpacing());
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::on_buttonBox_accepted()
{
    tableFormat.setCellSpacing((qreal)ui->spacingPin->value());
    tableFormat.setCellPadding((qreal)ui->paddingPin->value());
    close();
}

QTextTableFormat TableDialog::getTableFormat()
{
    return tableFormat;
}
