/********************************************************************************
** Form generated from reading UI file 'mapdialog.ui'
**
** Created: Sun Dec 1 18:04:07 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPDIALOG_H
#define UI_MAPDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_MapDialog
{
public:
    QLabel *map_label;

    void setupUi(QDialog *MapDialog)
    {
        if (MapDialog->objectName().isEmpty())
            MapDialog->setObjectName(QString::fromUtf8("MapDialog"));
        MapDialog->setWindowModality(Qt::NonModal);
        MapDialog->resize(1280, 800);
        map_label = new QLabel(MapDialog);
        map_label->setObjectName(QString::fromUtf8("map_label"));
        map_label->setGeometry(QRect(20, 40, 1236, 732));

        retranslateUi(MapDialog);

        QMetaObject::connectSlotsByName(MapDialog);
    } // setupUi

    void retranslateUi(QDialog *MapDialog)
    {
        MapDialog->setWindowTitle(QApplication::translate("MapDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        map_label->setText(QApplication::translate("MapDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapDialog: public Ui_MapDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPDIALOG_H
