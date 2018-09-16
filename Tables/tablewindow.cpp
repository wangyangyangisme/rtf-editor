#include "tablewindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

tableWindow::tableWindow(QWidget *parent) :
    QDialog(parent)
{
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel* rowLabel = new QLabel(tr("Row: "));
    QLabel* columnLabel = new QLabel(tr("Column:"));

    rowEdit = new QLineEdit("4");
    columnEdit = new QLineEdit("4");

    mainLayout->addWidget(rowLabel);
    mainLayout->addWidget(rowEdit);
    mainLayout->addWidget(columnLabel);
    mainLayout->addWidget(columnEdit);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

QPair<int, int> tableWindow::getValues()
{
    return QPair<int, int>(rowEdit->text().toInt(), columnEdit->text().toInt());
}
