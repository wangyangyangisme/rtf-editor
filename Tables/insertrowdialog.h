#ifndef INSERTROWDIALOG_H
#define INSERTROWDIALOG_H

#include <QDialog>

namespace Ui {
class insertRowDialog;
}

enum __position
{
    POSITION_AFTER_CURSOR,
    POSITION_BEFORE_CURSOR
};

class insertRowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit insertRowDialog(QWidget *parent = 0);
    ~insertRowDialog();

    int getAmount();
    int getPosition();

private slots:
    void on_buttonBox_accepted();

    void on_afterRadio_clicked();

    void on_beforeRadio_clicked();

private:
    Ui::insertRowDialog *ui;
    int position;

};

#endif // INSERTROWDIALOG_H
