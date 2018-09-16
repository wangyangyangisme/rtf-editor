#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QTextTableFormat>
#include <QTextTable>

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QTextTable* tb, QWidget *parent = 0);
    ~TableDialog();

    QTextTableFormat getTableFormat();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::TableDialog *ui;
    QTextTable* table;
    QTextTableFormat tableFormat;

};

#endif // TABLEDIALOG_H
