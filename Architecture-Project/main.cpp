#include "PMD.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Our_Window w;
    w.showFullScreen();
    return a.exec();
}
