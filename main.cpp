// Copyright (C) 2018 Yuxiang Sun
// Email: yxsun@link.cuhk.edu.hk
// MIT License

#include <QtGlobal>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #ifdef Q_OS_LINUX
    a.setStyle("windows");
    a.setFont(QFont("tahoma",8));
    #endif

    MainWindow w;
    w.show();
    return a.exec();
}
