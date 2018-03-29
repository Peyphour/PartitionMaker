#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(920,455);
    w.setWindowFlags(w.windowFlags() |= Qt::MSWindowsFixedSizeDialogHint);
    w.show();

    return a.exec();
}
