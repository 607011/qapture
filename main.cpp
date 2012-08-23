/*
 Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
*/

#include <QtGui/QApplication>
#include <QDesktopWidget>

#include "GraphicsView.h"
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow m;
    m.show();
    return a.exec();
}
