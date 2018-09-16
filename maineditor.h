#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include <QMainWindow>
#include <QSettings>

class QAction;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QComboBox;
class QFontComboBox;
class QLineEdit;

class TextEdit : public QMainWindow
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = 0);

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    void setupFileActions();
    void setupEditActions();
    void setupTextActions();
    void setupTableActions();
    void setupSettingActions();
    void setupHelpActions();

    bool load(const QString &f);
    bool maybeSave();
    void setCurrentFileName(const QString &fileName);

private slots:

    void cursorPositionChanged();

    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();

    void textBold();
    void textItalic();
    void textUnderline();
    void textOverline();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(int styleIndex);
    void textColor();
    void textBackgroundColor();
    void textAlign(QAction *);

    void textFind();
    void textFindStr(const QString& str);


    void tableInsert();
    void tableEdit();
    void tableInsertRow();
    void tableInsertColumn();
    void tableDelete();
    void tableDeleteRow();
    void tableDeleteColumn();
    void tableDeleteCurrentRow();
    void tableDeleteCurrentColumn();

    void currentCharFormatChanged(const QTextCharFormat &format);

    void about();

private:

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void backgroundColorChanged(const QColor &c);

    QAction *actionSave;
    QAction *actionTextBold;
    QAction *actionTextItalic;
    QAction *actionTextUnderline;
    QAction *actionTextOverline;
    QAction *actionTextColor;
    QAction *actionTextBackgroundColor;
    QAction *actionUndo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;

    QAction *actionAlignRight;
    QAction *actionAlignLeft;
    QAction *actionAlignCenter;
    QAction *actionAlignJustify;

    QAction *actionInsertTable;
    QAction *actionEditTable;
    QAction *actionInsertRow;
    QAction *actionInsertColumn;
    QAction *actionDeleteTable;
    QAction *actionDeleteRow;
    QAction *actionDeleteColumn;
    QAction *actionDeleteCurrentRow;
    QAction *actionDeleteCurrentColumn;

    QLineEdit* searchLine;
    QLineEdit* replaceLine;

    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

    QToolBar *tb;
    QString fileName;
    QTextEdit *textEdit;
};

#endif
