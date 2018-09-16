/****************************************************************************
**
** For license see LICENSE
** 04/06/2016 Holmuk
**
****************************************************************************/

#include "maineditor.h"

#include <QAction>
#include <QApplication>
#include <QColorDialog>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QToolBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <QBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QTextDocumentWriter>
#include <QTextEdit>
#include <QTextCodec>

TextEdit::TextEdit(QWidget *parent)
    : QMainWindow(parent)
{
    //setToolButtonStyle(Qt::ToolButtonFollowStyle);
    qApp->setStyleSheet("QToolBar { spacing: 0px; }");
    setupFileActions();
    setupEditActions();
    setupTextActions();
    setupTableActions();
    setupSettingActions();
    setupHelpActions();

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    QDockWidget* dockWidget = new QDockWidget(this);
    dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);

    QLabel* findLabel = new QLabel(tr("Find: "), this);
    QLabel* replaceLabel = new QLabel(tr("Replace: "), this);

    searchLine = new QLineEdit(this);
    replaceLine = new QLineEdit(this);

    QPushButton* findButton = new QPushButton(tr("Find!"), this);
    QPushButton* replaceButton = new QPushButton(tr("Replace"), this);

    QGroupBox *groupBox = new QGroupBox(dockWidget);
    QGridLayout *findBox = new QGridLayout(groupBox);
    findBox->addWidget(findLabel, 0, 0);
    findBox->addWidget(replaceLabel, 1, 0);
    findBox->addWidget(searchLine, 0, 1);
    findBox->addWidget(replaceLine, 1, 1);
    findBox->addWidget(findButton, 0, 2);
    findBox->addWidget(replaceButton, 1, 2);

    dockWidget->setWidget(groupBox);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);

    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

    connect(findButton, SIGNAL(clicked()), this, SLOT(textFind()));
    connect(searchLine, SIGNAL(textChanged(QString)), this, SLOT(textFindStr(QString)));
    //connect(replaceButton, SIGNAL(clicked()), this, SLOT(textReplace()));

    textEdit = new QTextEdit(this);
    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));

    connect(textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));


    setCentralWidget(textEdit);
    textEdit->setFocus();
    setCurrentFileName(QString());

    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    backgroundColorChanged(textEdit->textBackgroundColor());

    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            actionSave, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    connect(textEdit->document(), SIGNAL(undoAvailable(bool)),
            actionUndo, SLOT(setEnabled(bool)));

    setWindowModified(textEdit->document()->isModified());
    actionSave->setEnabled(textEdit->document()->isModified());

    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));

    actionCut->setEnabled(false);
    connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));

    actionCopy->setEnabled(false);
    connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));

    connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

    fileNew();
}

void TextEdit::closeEvent(QCloseEvent *e)
{
    if (maybeSave())
        e->accept();
    else
        e->ignore();
}

void TextEdit::cursorPositionChanged()
{
    if (textEdit->textCursor().currentTable() != 0)
    {
        actionEditTable->setEnabled(true);
        actionInsertRow->setEnabled(true);
        actionInsertColumn->setEnabled(true);
        actionDeleteTable->setEnabled(true);
        actionDeleteColumn->setEnabled(true);
        actionDeleteRow->setEnabled(true);
        actionDeleteCurrentColumn->setEnabled(true);
        actionDeleteCurrentRow->setEnabled(true);
    } else
    {
        actionEditTable->setDisabled(true);
        actionInsertRow->setDisabled(true);
        actionInsertColumn->setDisabled(true);
        actionDeleteTable->setDisabled(true);
        actionDeleteColumn->setDisabled(true);
        actionDeleteRow->setDisabled(true);
        actionDeleteCurrentColumn->setDisabled(true);
        actionDeleteCurrentRow->setDisabled(true);
    }

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.clear();
    textEdit->setExtraSelections(extraSelections);

}

bool TextEdit::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str))
        textEdit->setHtml(str);
    else
    {
        str = QString::fromLocal8Bit(data);
        textEdit->setPlainText(str);
    }

    setCurrentFileName(f);
    return true;
}

bool TextEdit::maybeSave()
{
    if ((!textEdit->document()->isModified()) || (fileName.startsWith(QLatin1String(":/"))))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your eternal works?"),
                               QMessageBox::Save | QMessageBox::Discard
                               | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void TextEdit::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(shownName);
    setWindowModified(false);
}

void TextEdit::fileNew()
{
    if (maybeSave())
    {
        textEdit->clear();
        setCurrentFileName(QString());
    }
}

void TextEdit::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("All Files (*)"));
    if (!fn.isEmpty())
        load(fn);
}

bool TextEdit::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success)
        textEdit->document()->setModified(false);
    return success;
}

bool TextEdit::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("ODF files (*.odt);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (!(fn.endsWith(".odt", Qt::CaseInsensitive)))
        fn += ".odt"; // default
    setCurrentFileName(fn);
    return fileSave();
}


void TextEdit::about()
{
    QMessageBox::about(this, tr("About"), tr("Are you ready to know?"));
}
