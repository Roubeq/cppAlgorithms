#include "camp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Camp w;
    w.show();
    return a.exec();
}
