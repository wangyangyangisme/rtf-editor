#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

//#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPair>

//class QDialog;
class tableWindow : public QDialog
{
    Q_OBJECT
public:
    tableWindow(QWidget *parent = 0);
    QPair<int, int> getValues();

private:
    QDialogButtonBox *buttonBox;
    QLineEdit *rowEdit;
    QLineEdit *columnEdit;

signals:

public slots:

};

#endif // TABLEWINDOW_H
