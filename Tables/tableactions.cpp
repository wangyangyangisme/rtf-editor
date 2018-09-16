#include "../maineditor.h"
#include "insertrowdialog.h"
#include "tablewindow.h"
#include "tabledialog.h"

#include <QTextEdit>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>

void TextEdit::tableInsert()
{
    tableWindow* dlg = new tableWindow;

    if (dlg->exec())
    {
        QPair<int, int> rInfo = dlg->getValues();
        textEdit->textCursor().insertTable(rInfo.first, rInfo.second);
    }
}

void TextEdit::tableEdit()
{
    QTextTable* tbl = textEdit->textCursor().currentTable();
    TableDialog* dlg = new TableDialog(tbl);
    dlg->exec();
    tbl->setFormat(dlg->getTableFormat());
}

void TextEdit::tableInsertRow()
{
    insertRowDialog* dlg = new insertRowDialog;
    dlg->exec();

    QTextTable* tbl = textEdit->textCursor().currentTable();
    QTextTableCell cell = tbl->cellAt(textEdit->textCursor());

    if (dlg->getPosition() == POSITION_BEFORE_CURSOR)
    {
        tbl->insertRows(cell.row(), dlg->getAmount());
    } else
    {
        int am = dlg->getAmount();
        int off = tbl->rows() - cell.row();
        if (am > off)
        {
            tbl->appendRows(am - off);
            am = am - off;
        }
        tbl->insertRows(cell.row() + am, am);
    }
}

void TextEdit::tableInsertColumn()
{
    insertRowDialog* dlg = new insertRowDialog;
    dlg->exec();

    QTextTable* tbl = textEdit->textCursor().currentTable();
    QTextTableCell cell = tbl->cellAt(textEdit->textCursor());

    if (dlg->getPosition() == POSITION_BEFORE_CURSOR)
    {
        tbl->insertColumns(cell.column(), dlg->getAmount());
    } else
    {
        int am = dlg->getAmount();
        int off = tbl->columns() - cell.column();
        if (am > off)
        {
            tbl->appendColumns(am - off);
            am = am - off;
        }
        tbl->insertColumns(cell.row() + am, am);
    }
}

void TextEdit::tableDelete()
{
    QTextTable* tbl = textEdit->textCursor().currentTable();
    tbl->removeColumns(0, tbl->columns());
}

void TextEdit::tableDeleteRow()
{
    insertRowDialog* dlg = new insertRowDialog;
    dlg->exec();

    QTextTable* tbl = textEdit->textCursor().currentTable();
    QTextTableCell cell = tbl->cellAt(textEdit->textCursor());

    int am = dlg->getAmount();
    if (dlg->getPosition() == POSITION_BEFORE_CURSOR)
    {
        int delta = cell.row();
        if (am > delta)
            am = delta;
        if (am != 0)
            tbl->removeRows(cell.row()-am, am);

    } else
    {
        int delta = tbl->rows() - cell.row() - 1;
        if (am > delta)
            am = delta;
        if (am != 0)
            tbl->removeRows(cell.row()+1, am);
    }
}

void TextEdit::tableDeleteColumn()
{
    insertRowDialog* dlg = new insertRowDialog;
    dlg->exec();

    QTextTable* tbl = textEdit->textCursor().currentTable();
    QTextTableCell cell = tbl->cellAt(textEdit->textCursor());

    int am = dlg->getAmount();
    if (dlg->getPosition() == POSITION_BEFORE_CURSOR)
    {
        int delta = cell.column();
        if (am > delta)
            am = delta;
        if (am != 0)
            tbl->removeColumns(cell.column()-am, am);

    } else
    {
        int delta = tbl->columns() - cell.column() - 1;
        if (am > delta)
            am = delta;
        if (am != 0)
            tbl->removeColumns(cell.row()+1, am);
    }
}

void TextEdit::tableDeleteCurrentRow()
{
    QTextTable* tbl = textEdit->textCursor().currentTable();
    tbl->removeRows(tbl->cellAt(textEdit->textCursor()).row(), 1);
}

void TextEdit::tableDeleteCurrentColumn()
{
    QTextTable* tbl = textEdit->textCursor().currentTable();
    tbl->removeColumns(tbl->cellAt(textEdit->textCursor()).column(), 1);
}
