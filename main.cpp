#include <clocale>
#include <QApplication>
#include <QIcon>
#include "core/MainWindow.h"

int main(int argc, char *argv[]) {
    setlocale(LC_NUMERIC, "C");
    setlocale(LC_ALL, "C");

    QApplication app(argc, argv);

    MainWindow win;
    win.setWindowIcon(QIcon(":/assets/icon.svg"));
    win.show();

    return app.exec();
}