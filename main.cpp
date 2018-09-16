#include "maineditor.h"
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>

int main(int argc, char** argv)
{
    QApplication MainApp(argc, argv);

    TextEdit m;
    m.resize(600, 480);
    m.show();
    return MainApp.exec();
}
