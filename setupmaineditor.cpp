/****************************************************************************
**
** For license see LICENSE
** 05/06/2016 Holmuk
**
****************************************************************************/

#include "maineditor.h"
#include "Tables/tablewindow.h"
#include "Tables/tabledialog.h"
#include "Tables/insertrowdialog.h"

//ABC is ok.
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QCloseEvent>
#include <QMessageBox>
#include <QString>

const QString iconPath = ":/icons";
const QString langPath = ":/Translations";

void TextEdit::setupFileActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setMovable(false);

    tb->setWindowTitle(tr("File Actions"));
    tb->setFixedHeight(24);
    tb->setStyleSheet("spacing: 0px");
    addToolBar(tb);

    QMenu *menu = new QMenu(tr("File"), this);
    menuBar()->addMenu(menu);

    const QIcon newIcon = QIcon(iconPath + "/new.png");
    QAction *a = new QAction(newIcon, tr("New"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
    tb->addAction(a);
    menu->addAction(a);

    const QIcon openIcon = QIcon(iconPath + "/folder.png");
    a = new QAction(openIcon, tr("Open..."), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    const QIcon saveIcon = QIcon(iconPath + "/diskette.png");
    a = new QAction(saveIcon, tr("Save"), this);
    actionSave = a;
    connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(tr("Save As..."), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    menu->addAction(a);
    menu->addSeparator();

    a = new QAction(tr("Quit"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
    menu->addAction(a);
}

void TextEdit::setupEditActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Strange Actions"));
    tb->setFixedHeight(24);
    tb->setMovable(false);
    addToolBar(tb);
    QMenu *menu = new QMenu(tr("Edit"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = actionUndo = new QAction(tr("Undo"), this);
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    a = actionCut = new QAction(tr("Cut"), this);
    tb->addAction(a);
    menu->addAction(a);

    a = actionCopy = new QAction(tr("Copy"), this);
    tb->addAction(a);
    menu->addAction(a);

    a = actionPaste = new QAction(tr("Paste"), this);
    tb->addAction(a);
    menu->addAction(a);
}

void TextEdit::setupTextActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Text Actions"));
    tb->setFixedHeight(24);
    tb->setMovable(false);
    addToolBar(tb);

    QMenu *menu = new QMenu(tr("Format"), this);
    menuBar()->addMenu(menu);

    tb->addSeparator();

    QMenu *formatMenu = menu->addMenu("Font");
    actionTextBold = new QAction(tr("Bold"), this);
    actionTextBold->setIconText("B");
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    tb->addAction(actionTextBold);
    formatMenu->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(tr("Italic"), this);
    actionTextItalic->setIconText("I");
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    tb->addAction(actionTextItalic);
    formatMenu->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(tr("Underline"), this);
    actionTextUnderline->setIconText("U");
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    tb->addAction(actionTextUnderline);
    formatMenu->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    actionTextOverline = new QAction(tr("Overline"), this);
    actionTextOverline->setIconText("O");
    QFont overline;
    overline.setOverline(true);
    actionTextOverline->setFont(overline);
    connect(actionTextOverline, SIGNAL(triggered()), this, SLOT(textOverline()));
    tb->addAction(actionTextOverline);
    formatMenu->addAction(actionTextOverline);
    actionTextOverline->setCheckable(true);

    tb->addSeparator();
    menu->addSeparator();

    actionAlignLeft = new QAction(tr("Left"), this);
    actionAlignLeft->setCheckable(true);

    actionAlignCenter = new QAction(tr("Center"), this);
    actionAlignCenter->setCheckable(true);

    actionAlignRight = new QAction(tr("Right"), this);
    actionAlignRight->setCheckable(true);

    const QIcon jIcon = QIcon(iconPath + "/align_j.png");
    actionAlignJustify = new QAction(jIcon, tr("Justify"), this);
    actionAlignJustify->setCheckable(true);

    QActionGroup *alignGroup = new QActionGroup(this);
    connect(alignGroup, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

    alignGroup->addAction(actionAlignLeft);
    alignGroup->addAction(actionAlignCenter);
    alignGroup->addAction(actionAlignRight);
    alignGroup->addAction(actionAlignJustify);

    tb->addActions(alignGroup->actions());
    menu->addActions(alignGroup->actions());

    //Is it a good idea to put a sweet square?
    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, tr("Color..."), this);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    tb->addAction(actionTextColor);
    menu->addAction(actionTextColor);

    pix.fill(Qt::white);
    actionTextBackgroundColor = new QAction(pix, tr("Color..."), this);
    connect(actionTextBackgroundColor, SIGNAL(triggered()), this, SLOT(textBackgroundColor()));
    tb->addAction(actionTextBackgroundColor);
    menu->addAction(actionTextBackgroundColor);

    tb = new QToolBar(this);
    tb->setMaximumHeight(24);
    tb->setAllowedAreas(Qt::TopToolBarArea);
    tb->setWindowTitle(tr("Format Actions"));
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);

    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Point)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alphabet)");
    comboStyle->addItem("Ordered List (Roman)");
    connect(comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, SIGNAL(activated(QString)),
            this, SLOT(textFamily(QString)));

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);
    comboSize->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)),
            this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));
}

void TextEdit::setupTableActions()
{
    QMenu* menu = new QMenu(tr("Table"), this);
    menuBar()->addMenu(menu);

    QMenu* insertMenu = menu->addMenu(tr("Insert"));
    actionInsertTable = new QAction(tr("Table"), this);
    connect(actionInsertTable, SIGNAL(triggered()), this, SLOT(tableInsert()));
    insertMenu->addAction(actionInsertTable);
    actionInsertRow = new QAction(tr("Row"), this);
    connect(actionInsertRow, SIGNAL(triggered()), this, SLOT(tableInsertRow()));
    insertMenu->addAction(actionInsertRow);
    actionInsertRow->setDisabled(true);
    actionInsertColumn = new QAction(tr("Column"), this);
    connect(actionInsertColumn, SIGNAL(triggered()), this, SLOT(tableInsertColumn()));
    insertMenu->addAction(actionInsertColumn);
    actionInsertColumn->setDisabled(true);

    QMenu* deleteMenu = menu->addMenu(tr("Delete"));
    actionDeleteTable = new QAction(tr("Table"), this);
    connect(actionDeleteTable, SIGNAL(triggered()), this, SLOT(tableDelete()));
    deleteMenu->addAction(actionDeleteTable);
    actionDeleteTable->setDisabled(true);

    actionDeleteRow = new QAction(tr("Row"), this);
    connect(actionDeleteRow, SIGNAL(triggered()), this, SLOT(tableDeleteRow()));
    deleteMenu->addAction(actionDeleteRow);
    actionDeleteRow->setDisabled(true);

    actionDeleteColumn = new QAction(tr("Column"), this);
    connect(actionDeleteColumn, SIGNAL(triggered()), this, SLOT(tableDeleteColumn()));
    deleteMenu->addAction(actionDeleteColumn);
    actionDeleteColumn->setDisabled(true);

    actionDeleteCurrentRow = new QAction(tr("Current Row"), this);
    connect(actionDeleteCurrentRow, SIGNAL(triggered()), this, SLOT(tableDeleteCurrentRow()));
    deleteMenu->addAction(actionDeleteCurrentRow);
    actionDeleteRow->setDisabled(true);
    actionDeleteCurrentColumn = new QAction(tr("Current Column"), this);
    connect(actionDeleteCurrentColumn, SIGNAL(triggered()), this, SLOT(tableDeleteCurrentColumn()));
    deleteMenu->addAction(actionDeleteCurrentColumn);
    actionDeleteCurrentColumn->setDisabled(true);


    actionEditTable = new QAction(tr("Properties"), this);
    connect(actionEditTable, SIGNAL(triggered()), this, SLOT(tableEdit()));
    menu->addAction(actionEditTable);

    actionEditTable->setDisabled(true);
}

void TextEdit::setupSettingActions()
{
    QMenu* menu = new QMenu(tr("Settings"), this);
    menuBar()->addMenu(menu);
}

void TextEdit::setupHelpActions()
{
    QMenu *helpMenu = new QMenu(tr("Help"), this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(tr("About"), this, SLOT(about()));
}
