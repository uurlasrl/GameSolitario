//#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include "GameCards.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        new GameCards();
//    MainWindow w;
//    w.show();

/*    //QGuiApplication::applicationStateChanged()
    QObject::connect(qGuiApp, &QGuiApplication::applicationStateChanged, [](Qt::ApplicationState state){
        qDebug()<<"** evento:" <<state;
    });*/
    return a.exec();
}
