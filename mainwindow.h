// Copyright (C) 2018 Yuxiang Sun
// Email: yxsun@link.cuhk.edu.hk
// MIT License

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sleep();
    
private slots:
    void on_scan_pushButton_clicked();
    void on_actionAbout_triggered();
    void on_viewMap_pushButton_clicked();
    void on_readDatabase_pushButton_clicked();
    void on_actionDelay_Time_triggered();
    void on_actionSet_Linux_Passward_triggered();
    void on_zoom_verticalSlider_valueChanged(int value);
    void on_showGrids_radioButton_toggled(bool checked);
    void on_showAP_radioButton_toggled(bool checked);
    void on_locate_PushButton_clicked();
    void on_showWCS_radioButton_toggled(bool checked);
    void on_saveAs_pushButton_clicked();
    void on_methodSettings_pushButton_clicked();
    void on_creatTrainingDataset_pushButton_clicked();
    void on_showGroundTruth_radioButton_toggled(bool checked);
    void on_getSparseDatabase_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    //table 1
    QStandardItemModel *model;

    //table 3
    QGraphicsScene *mapScence;
    QGraphicsItemGroup *coordinateAxisItemGroup;
    QGraphicsItemGroup *crossItemGroup;
    QGraphicsItemGroup *APEllipseItemGroup;
    QGraphicsRectItem *groundTruthRectItem;

    //some variables
    int delayTime;
    QString linuxPassword;
    int k_knn;
    int variationThreshold_eknn;
    int localizationTimes;
};

#endif // MAINWINDOW_H
