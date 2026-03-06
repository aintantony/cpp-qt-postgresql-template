#include <QApplication>
#include "../gui/mainwindow.h"
#include "../config/env.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Env::load("../.env");

    MainWindow window;
    window.show();

    return app.exec();
}