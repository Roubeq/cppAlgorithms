#include <QApplication>
#include <QMainWindow>
#include "mochikorowidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    MochikoroWidget *widget = new MochikoroWidget(&window);
    window.setCentralWidget(widget);
    window.resize(500, 500);
    window.show();
    return app.exec();
}
