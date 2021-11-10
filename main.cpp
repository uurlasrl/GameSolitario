//#include "mainwindow.h"

#include <QApplication>
#include "GameCards.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        new GameCards();
//    MainWindow w;
//    w.show();
    return a.exec();
}
