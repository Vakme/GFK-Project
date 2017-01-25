#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication *a = new QApplication(argc, argv);
    MainWindow *b = MainWindow::main();
    b->show();
    int ret = a->exec();
    delete a;
    delete b;
    return ret;
}
