/*
 * Made by Alexander Iwoh
 * Launches application.
 *
 */
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    printf("(c) 2021 Alexander Iwoh\n");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
