/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionDelay_Time;
    QAction *actionSet_Linux_Passward;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *scan_pushButton;
    QTableView *AP_tableView;
    QCustomPlot *plotRSSI_widget;
    QPlainTextEdit *measurementInfo_plainTextEdit;
    QProgressBar *measurement_progressBar;
    QSpinBox *scanTimes_spinBox;
    QRadioButton *plotRSSI_radioButton;
    QPushButton *viewMap_pushButton;
    QRadioButton *writeDatabase_radioButton;
    QPushButton *stopScan_pushButton;
    QGroupBox *scanSettings_groupBox;
    QGroupBox *scanCoordinates_groupBox;
    QSpinBox *X_spinBox;
    QLabel *X_label;
    QLabel *Y_label;
    QSpinBox *Y_spinBox;
    QGroupBox *dataSelection_groupBox;
    QComboBox *dataSelection_comboBox;
    QWidget *tab_2;
    QPushButton *readDatabase_pushButton;
    QSpinBox *readX_spinBox;
    QSpinBox *readY_spinBox;
    QPlainTextEdit *readmeasurementInfo_plainTextEdit;
    QCustomPlot *readplotRSSI_widget;
    QPushButton *saveAs_pushButton;
    QPushButton *creatTrainingDataset_pushButton;
    QComboBox *trainingDatasetType_comboBox;
    QPushButton *getSparseDatabase_pushButton;
    QComboBox *readdataSelection_comboBox;
    QWidget *tab_3;
    QGraphicsView *map_graphicsView;
    QLineEdit *locationX_lineEdit;
    QPushButton *locateStop_PushButton;
    QPushButton *locate_PushButton;
    QPlainTextEdit *localizationInfo_plainTextEdit;
    QRadioButton *showGrids_radioButton;
    QSlider *zoom_verticalSlider;
    QFrame *scale_line;
    QLabel *scale_label;
    QRadioButton *showAP_radioButton;
    QLineEdit *locationY_lineEdit;
    QRadioButton *showWCS_radioButton;
    QLineEdit *locationError_lineEdit;
    QComboBox *methodSelection_comboBox;
    QPushButton *methodSettings_pushButton;
    QGroupBox *methodSettings_groupBox;
    QGroupBox *groundTruthSettings_groupBox;
    QLineEdit *groundTruthX_lineEdit;
    QComboBox *groundTruthSelection_comboBox;
    QLineEdit *groundTruthY_lineEdit;
    QRadioButton *showGroundTruth_radioButton;
    QLineEdit *locationPossible_lineEdit;
    QGroupBox *localizationResults_groupBox;
    QGroupBox *localizationControl_groupBox;
    QWidget *tab_4;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(845, 775);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionDelay_Time = new QAction(MainWindow);
        actionDelay_Time->setObjectName(QStringLiteral("actionDelay_Time"));
        actionSet_Linux_Passward = new QAction(MainWindow);
        actionSet_Linux_Passward->setObjectName(QStringLiteral("actionSet_Linux_Passward"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 825, 720));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        scan_pushButton = new QPushButton(tab);
        scan_pushButton->setObjectName(QStringLiteral("scan_pushButton"));
        scan_pushButton->setGeometry(QRect(20, 40, 60, 22));
        AP_tableView = new QTableView(tab);
        AP_tableView->setObjectName(QStringLiteral("AP_tableView"));
        AP_tableView->setGeometry(QRect(10, 210, 800, 150));
        AP_tableView->setSortingEnabled(true);
        plotRSSI_widget = new QCustomPlot(tab);
        plotRSSI_widget->setObjectName(QStringLiteral("plotRSSI_widget"));
        plotRSSI_widget->setGeometry(QRect(10, 370, 800, 320));
        plotRSSI_widget->setStyleSheet(QStringLiteral(""));
        measurementInfo_plainTextEdit = new QPlainTextEdit(tab);
        measurementInfo_plainTextEdit->setObjectName(QStringLiteral("measurementInfo_plainTextEdit"));
        measurementInfo_plainTextEdit->setGeometry(QRect(410, 20, 400, 180));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        measurementInfo_plainTextEdit->setFont(font);
        measurement_progressBar = new QProgressBar(tab);
        measurement_progressBar->setObjectName(QStringLiteral("measurement_progressBar"));
        measurement_progressBar->setGeometry(QRect(10, 170, 395, 23));
        measurement_progressBar->setValue(0);
        scanTimes_spinBox = new QSpinBox(tab);
        scanTimes_spinBox->setObjectName(QStringLiteral("scanTimes_spinBox"));
        scanTimes_spinBox->setGeometry(QRect(100, 40, 65, 22));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setBold(true);
        font1.setWeight(75);
        scanTimes_spinBox->setFont(font1);
        plotRSSI_radioButton = new QRadioButton(tab);
        plotRSSI_radioButton->setObjectName(QStringLiteral("plotRSSI_radioButton"));
        plotRSSI_radioButton->setGeometry(QRect(100, 82, 71, 17));
        viewMap_pushButton = new QPushButton(tab);
        viewMap_pushButton->setObjectName(QStringLiteral("viewMap_pushButton"));
        viewMap_pushButton->setGeometry(QRect(20, 120, 60, 22));
        writeDatabase_radioButton = new QRadioButton(tab);
        writeDatabase_radioButton->setObjectName(QStringLiteral("writeDatabase_radioButton"));
        writeDatabase_radioButton->setGeometry(QRect(100, 122, 71, 17));
        stopScan_pushButton = new QPushButton(tab);
        stopScan_pushButton->setObjectName(QStringLiteral("stopScan_pushButton"));
        stopScan_pushButton->setGeometry(QRect(20, 80, 60, 22));
        stopScan_pushButton->setFlat(false);
        scanSettings_groupBox = new QGroupBox(tab);
        scanSettings_groupBox->setObjectName(QStringLiteral("scanSettings_groupBox"));
        scanSettings_groupBox->setGeometry(QRect(10, 15, 170, 140));
        scanCoordinates_groupBox = new QGroupBox(tab);
        scanCoordinates_groupBox->setObjectName(QStringLiteral("scanCoordinates_groupBox"));
        scanCoordinates_groupBox->setGeometry(QRect(200, 15, 170, 85));
        X_spinBox = new QSpinBox(scanCoordinates_groupBox);
        X_spinBox->setObjectName(QStringLiteral("X_spinBox"));
        X_spinBox->setGeometry(QRect(70, 22, 80, 20));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        X_spinBox->setFont(font2);
        X_label = new QLabel(scanCoordinates_groupBox);
        X_label->setObjectName(QStringLiteral("X_label"));
        X_label->setGeometry(QRect(25, 24, 31, 16));
        X_label->setFont(font2);
        Y_label = new QLabel(scanCoordinates_groupBox);
        Y_label->setObjectName(QStringLiteral("Y_label"));
        Y_label->setGeometry(QRect(25, 56, 31, 16));
        Y_label->setFont(font2);
        Y_spinBox = new QSpinBox(scanCoordinates_groupBox);
        Y_spinBox->setObjectName(QStringLiteral("Y_spinBox"));
        Y_spinBox->setGeometry(QRect(70, 55, 80, 20));
        Y_spinBox->setFont(font2);
        dataSelection_groupBox = new QGroupBox(tab);
        dataSelection_groupBox->setObjectName(QStringLiteral("dataSelection_groupBox"));
        dataSelection_groupBox->setGeometry(QRect(200, 105, 170, 51));
        dataSelection_comboBox = new QComboBox(dataSelection_groupBox);
        dataSelection_comboBox->setObjectName(QStringLiteral("dataSelection_comboBox"));
        dataSelection_comboBox->setGeometry(QRect(15, 20, 140, 22));
        tabWidget->addTab(tab, QString());
        scanSettings_groupBox->raise();
        scan_pushButton->raise();
        AP_tableView->raise();
        plotRSSI_widget->raise();
        measurementInfo_plainTextEdit->raise();
        measurement_progressBar->raise();
        scanTimes_spinBox->raise();
        plotRSSI_radioButton->raise();
        viewMap_pushButton->raise();
        writeDatabase_radioButton->raise();
        stopScan_pushButton->raise();
        scanCoordinates_groupBox->raise();
        dataSelection_groupBox->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        readDatabase_pushButton = new QPushButton(tab_2);
        readDatabase_pushButton->setObjectName(QStringLiteral("readDatabase_pushButton"));
        readDatabase_pushButton->setGeometry(QRect(10, 30, 75, 23));
        readX_spinBox = new QSpinBox(tab_2);
        readX_spinBox->setObjectName(QStringLiteral("readX_spinBox"));
        readX_spinBox->setGeometry(QRect(130, 30, 42, 22));
        readY_spinBox = new QSpinBox(tab_2);
        readY_spinBox->setObjectName(QStringLiteral("readY_spinBox"));
        readY_spinBox->setGeometry(QRect(130, 70, 42, 22));
        readmeasurementInfo_plainTextEdit = new QPlainTextEdit(tab_2);
        readmeasurementInfo_plainTextEdit->setObjectName(QStringLiteral("readmeasurementInfo_plainTextEdit"));
        readmeasurementInfo_plainTextEdit->setGeometry(QRect(410, 20, 400, 180));
        readmeasurementInfo_plainTextEdit->setFont(font);
        readplotRSSI_widget = new QCustomPlot(tab_2);
        readplotRSSI_widget->setObjectName(QStringLiteral("readplotRSSI_widget"));
        readplotRSSI_widget->setGeometry(QRect(10, 370, 800, 320));
        saveAs_pushButton = new QPushButton(tab_2);
        saveAs_pushButton->setObjectName(QStringLiteral("saveAs_pushButton"));
        saveAs_pushButton->setGeometry(QRect(10, 70, 75, 23));
        creatTrainingDataset_pushButton = new QPushButton(tab_2);
        creatTrainingDataset_pushButton->setObjectName(QStringLiteral("creatTrainingDataset_pushButton"));
        creatTrainingDataset_pushButton->setGeometry(QRect(10, 180, 75, 23));
        trainingDatasetType_comboBox = new QComboBox(tab_2);
        trainingDatasetType_comboBox->setObjectName(QStringLiteral("trainingDatasetType_comboBox"));
        trainingDatasetType_comboBox->setGeometry(QRect(110, 180, 91, 22));
        getSparseDatabase_pushButton = new QPushButton(tab_2);
        getSparseDatabase_pushButton->setObjectName(QStringLiteral("getSparseDatabase_pushButton"));
        getSparseDatabase_pushButton->setGeometry(QRect(10, 140, 75, 23));
        readdataSelection_comboBox = new QComboBox(tab_2);
        readdataSelection_comboBox->setObjectName(QStringLiteral("readdataSelection_comboBox"));
        readdataSelection_comboBox->setGeometry(QRect(220, 50, 140, 22));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        map_graphicsView = new QGraphicsView(tab_3);
        map_graphicsView->setObjectName(QStringLiteral("map_graphicsView"));
        map_graphicsView->setGeometry(QRect(10, 210, 800, 470));
        locationX_lineEdit = new QLineEdit(tab_3);
        locationX_lineEdit->setObjectName(QStringLiteral("locationX_lineEdit"));
        locationX_lineEdit->setGeometry(QRect(30, 125, 80, 25));
        QFont font3;
        font3.setFamily(QStringLiteral("Arial"));
        font3.setPointSize(16);
        font3.setBold(true);
        font3.setWeight(75);
        locationX_lineEdit->setFont(font3);
        locationX_lineEdit->setReadOnly(true);
        locateStop_PushButton = new QPushButton(tab_3);
        locateStop_PushButton->setObjectName(QStringLiteral("locateStop_PushButton"));
        locateStop_PushButton->setGeometry(QRect(30, 65, 60, 22));
        locateStop_PushButton->setFlat(false);
        locate_PushButton = new QPushButton(tab_3);
        locate_PushButton->setObjectName(QStringLiteral("locate_PushButton"));
        locate_PushButton->setGeometry(QRect(30, 35, 60, 22));
        localizationInfo_plainTextEdit = new QPlainTextEdit(tab_3);
        localizationInfo_plainTextEdit->setObjectName(QStringLiteral("localizationInfo_plainTextEdit"));
        localizationInfo_plainTextEdit->setGeometry(QRect(410, 20, 400, 180));
        localizationInfo_plainTextEdit->setFont(font);
        localizationInfo_plainTextEdit->setReadOnly(true);
        showGrids_radioButton = new QRadioButton(tab_3);
        showGrids_radioButton->setObjectName(QStringLiteral("showGrids_radioButton"));
        showGrids_radioButton->setGeometry(QRect(110, 20, 111, 17));
        showGrids_radioButton->setChecked(false);
        zoom_verticalSlider = new QSlider(tab_3);
        zoom_verticalSlider->setObjectName(QStringLiteral("zoom_verticalSlider"));
        zoom_verticalSlider->setGeometry(QRect(35, 345, 30, 200));
        zoom_verticalSlider->setContextMenuPolicy(Qt::DefaultContextMenu);
        zoom_verticalSlider->setMinimum(65);
        zoom_verticalSlider->setMaximum(300);
        zoom_verticalSlider->setOrientation(Qt::Vertical);
        zoom_verticalSlider->setTickPosition(QSlider::NoTicks);
        zoom_verticalSlider->setTickInterval(0);
        scale_line = new QFrame(tab_3);
        scale_line->setObjectName(QStringLiteral("scale_line"));
        scale_line->setGeometry(QRect(750, 650, 30, 10));
        scale_line->setFrameShadow(QFrame::Plain);
        scale_line->setLineWidth(4);
        scale_line->setMidLineWidth(0);
        scale_line->setFrameShape(QFrame::HLine);
        scale_label = new QLabel(tab_3);
        scale_label->setObjectName(QStringLiteral("scale_label"));
        scale_label->setGeometry(QRect(746, 630, 46, 13));
        scale_label->setStyleSheet(QStringLiteral(""));
        showAP_radioButton = new QRadioButton(tab_3);
        showAP_radioButton->setObjectName(QStringLiteral("showAP_radioButton"));
        showAP_radioButton->setGeometry(QRect(110, 40, 111, 17));
        locationY_lineEdit = new QLineEdit(tab_3);
        locationY_lineEdit->setObjectName(QStringLiteral("locationY_lineEdit"));
        locationY_lineEdit->setGeometry(QRect(120, 125, 80, 25));
        locationY_lineEdit->setFont(font3);
        locationY_lineEdit->setReadOnly(true);
        showWCS_radioButton = new QRadioButton(tab_3);
        showWCS_radioButton->setObjectName(QStringLiteral("showWCS_radioButton"));
        showWCS_radioButton->setGeometry(QRect(110, 60, 111, 17));
        locationError_lineEdit = new QLineEdit(tab_3);
        locationError_lineEdit->setObjectName(QStringLiteral("locationError_lineEdit"));
        locationError_lineEdit->setGeometry(QRect(30, 160, 80, 25));
        locationError_lineEdit->setFont(font3);
        locationError_lineEdit->setReadOnly(true);
        methodSelection_comboBox = new QComboBox(tab_3);
        methodSelection_comboBox->setObjectName(QStringLiteral("methodSelection_comboBox"));
        methodSelection_comboBox->setGeometry(QRect(250, 135, 130, 22));
        methodSettings_pushButton = new QPushButton(tab_3);
        methodSettings_pushButton->setObjectName(QStringLiteral("methodSettings_pushButton"));
        methodSettings_pushButton->setGeometry(QRect(250, 165, 130, 23));
        methodSettings_groupBox = new QGroupBox(tab_3);
        methodSettings_groupBox->setObjectName(QStringLiteral("methodSettings_groupBox"));
        methodSettings_groupBox->setGeometry(QRect(240, 115, 150, 80));
        groundTruthSettings_groupBox = new QGroupBox(tab_3);
        groundTruthSettings_groupBox->setObjectName(QStringLiteral("groundTruthSettings_groupBox"));
        groundTruthSettings_groupBox->setGeometry(QRect(240, 10, 150, 81));
        groundTruthX_lineEdit = new QLineEdit(groundTruthSettings_groupBox);
        groundTruthX_lineEdit->setObjectName(QStringLiteral("groundTruthX_lineEdit"));
        groundTruthX_lineEdit->setGeometry(QRect(10, 50, 60, 22));
        groundTruthX_lineEdit->setFont(font2);
        groundTruthSelection_comboBox = new QComboBox(groundTruthSettings_groupBox);
        groundTruthSelection_comboBox->setObjectName(QStringLiteral("groundTruthSelection_comboBox"));
        groundTruthSelection_comboBox->setGeometry(QRect(10, 20, 130, 22));
        groundTruthY_lineEdit = new QLineEdit(groundTruthSettings_groupBox);
        groundTruthY_lineEdit->setObjectName(QStringLiteral("groundTruthY_lineEdit"));
        groundTruthY_lineEdit->setGeometry(QRect(80, 50, 60, 22));
        QFont font4;
        font4.setFamily(QStringLiteral("Arial"));
        font4.setPointSize(14);
        font4.setBold(true);
        font4.setWeight(75);
        groundTruthY_lineEdit->setFont(font4);
        showGroundTruth_radioButton = new QRadioButton(tab_3);
        showGroundTruth_radioButton->setObjectName(QStringLiteral("showGroundTruth_radioButton"));
        showGroundTruth_radioButton->setGeometry(QRect(110, 80, 82, 17));
        locationPossible_lineEdit = new QLineEdit(tab_3);
        locationPossible_lineEdit->setObjectName(QStringLiteral("locationPossible_lineEdit"));
        locationPossible_lineEdit->setGeometry(QRect(120, 160, 80, 25));
        locationPossible_lineEdit->setFont(font3);
        locationPossible_lineEdit->setReadOnly(true);
        localizationResults_groupBox = new QGroupBox(tab_3);
        localizationResults_groupBox->setObjectName(QStringLiteral("localizationResults_groupBox"));
        localizationResults_groupBox->setGeometry(QRect(20, 105, 190, 90));
        localizationControl_groupBox = new QGroupBox(tab_3);
        localizationControl_groupBox->setObjectName(QStringLiteral("localizationControl_groupBox"));
        localizationControl_groupBox->setGeometry(QRect(20, 10, 190, 90));
        tabWidget->addTab(tab_3, QString());
        localizationResults_groupBox->raise();
        localizationControl_groupBox->raise();
        groundTruthSettings_groupBox->raise();
        methodSettings_groupBox->raise();
        map_graphicsView->raise();
        locationX_lineEdit->raise();
        locateStop_PushButton->raise();
        locate_PushButton->raise();
        localizationInfo_plainTextEdit->raise();
        showGrids_radioButton->raise();
        zoom_verticalSlider->raise();
        scale_line->raise();
        scale_label->raise();
        showAP_radioButton->raise();
        locationY_lineEdit->raise();
        showWCS_radioButton->raise();
        locationError_lineEdit->raise();
        methodSelection_comboBox->raise();
        methodSettings_pushButton->raise();
        showGroundTruth_radioButton->raise();
        locationPossible_lineEdit->raise();
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        tabWidget->addTab(tab_4, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 845, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(scan_pushButton, AP_tableView);
        QWidget::setTabOrder(AP_tableView, measurementInfo_plainTextEdit);
        QWidget::setTabOrder(measurementInfo_plainTextEdit, saveAs_pushButton);
        QWidget::setTabOrder(saveAs_pushButton, viewMap_pushButton);
        QWidget::setTabOrder(viewMap_pushButton, plotRSSI_radioButton);
        QWidget::setTabOrder(plotRSSI_radioButton, writeDatabase_radioButton);
        QWidget::setTabOrder(writeDatabase_radioButton, stopScan_pushButton);
        QWidget::setTabOrder(stopScan_pushButton, X_spinBox);
        QWidget::setTabOrder(X_spinBox, Y_spinBox);
        QWidget::setTabOrder(Y_spinBox, readDatabase_pushButton);
        QWidget::setTabOrder(readDatabase_pushButton, readX_spinBox);
        QWidget::setTabOrder(readX_spinBox, readY_spinBox);
        QWidget::setTabOrder(readY_spinBox, readmeasurementInfo_plainTextEdit);
        QWidget::setTabOrder(readmeasurementInfo_plainTextEdit, tabWidget);
        QWidget::setTabOrder(tabWidget, scanTimes_spinBox);
        QWidget::setTabOrder(scanTimes_spinBox, map_graphicsView);
        QWidget::setTabOrder(map_graphicsView, groundTruthSelection_comboBox);
        QWidget::setTabOrder(groundTruthSelection_comboBox, groundTruthX_lineEdit);
        QWidget::setTabOrder(groundTruthX_lineEdit, groundTruthY_lineEdit);
        QWidget::setTabOrder(groundTruthY_lineEdit, locateStop_PushButton);
        QWidget::setTabOrder(locateStop_PushButton, locate_PushButton);
        QWidget::setTabOrder(locate_PushButton, localizationInfo_plainTextEdit);
        QWidget::setTabOrder(localizationInfo_plainTextEdit, showGrids_radioButton);
        QWidget::setTabOrder(showGrids_radioButton, zoom_verticalSlider);
        QWidget::setTabOrder(zoom_verticalSlider, showAP_radioButton);
        QWidget::setTabOrder(showAP_radioButton, locationY_lineEdit);
        QWidget::setTabOrder(locationY_lineEdit, showWCS_radioButton);
        QWidget::setTabOrder(showWCS_radioButton, locationError_lineEdit);
        QWidget::setTabOrder(locationError_lineEdit, methodSelection_comboBox);
        QWidget::setTabOrder(methodSelection_comboBox, methodSettings_pushButton);
        QWidget::setTabOrder(methodSettings_pushButton, creatTrainingDataset_pushButton);
        QWidget::setTabOrder(creatTrainingDataset_pushButton, trainingDatasetType_comboBox);
        QWidget::setTabOrder(trainingDatasetType_comboBox, locationX_lineEdit);
        QWidget::setTabOrder(locationX_lineEdit, showGroundTruth_radioButton);
        QWidget::setTabOrder(showGroundTruth_radioButton, locationPossible_lineEdit);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionDelay_Time);
        menuFile->addAction(actionSet_Linux_Passward);
        menuAbout->addAction(actionAbout);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionDelay_Time->setText(QApplication::translate("MainWindow", "Set Delay Time", 0));
        actionSet_Linux_Passward->setText(QApplication::translate("MainWindow", "Set Linux Password", 0));
        scan_pushButton->setText(QApplication::translate("MainWindow", "Scan", 0));
        plotRSSI_radioButton->setText(QApplication::translate("MainWindow", "Plot RSSI", 0));
        viewMap_pushButton->setText(QApplication::translate("MainWindow", "View Map", 0));
        writeDatabase_radioButton->setText(QApplication::translate("MainWindow", "Write DB", 0));
        stopScan_pushButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        scanSettings_groupBox->setTitle(QApplication::translate("MainWindow", "Scan Settings", 0));
        scanCoordinates_groupBox->setTitle(QApplication::translate("MainWindow", "Scan Coordinates", 0));
        X_label->setText(QApplication::translate("MainWindow", "X", 0));
        Y_label->setText(QApplication::translate("MainWindow", "Y", 0));
        dataSelection_groupBox->setTitle(QApplication::translate("MainWindow", "Data Selection", 0));
        dataSelection_comboBox->clear();
        dataSelection_comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "RSSI", 0)
         << QApplication::translate("MainWindow", "Quality", 0)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Calibration", 0));
        readDatabase_pushButton->setText(QApplication::translate("MainWindow", "Read DB", 0));
        saveAs_pushButton->setText(QApplication::translate("MainWindow", "Save as", 0));
        creatTrainingDataset_pushButton->setText(QApplication::translate("MainWindow", "Training Data", 0));
        trainingDatasetType_comboBox->clear();
        trainingDatasetType_comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Raw Data", 0)
         << QApplication::translate("MainWindow", "Mean Data", 0)
         << QApplication::translate("MainWindow", "Mode Data", 0)
        );
        getSparseDatabase_pushButton->setText(QApplication::translate("MainWindow", "Sparse DB", 0));
        readdataSelection_comboBox->clear();
        readdataSelection_comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "RSSI", 0)
         << QApplication::translate("MainWindow", "Quality", 0)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Data Viewer", 0));
        locationX_lineEdit->setText(QString());
        locateStop_PushButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        locate_PushButton->setText(QApplication::translate("MainWindow", "Start", 0));
        showGrids_radioButton->setText(QApplication::translate("MainWindow", "Show Grids", 0));
        scale_label->setText(QApplication::translate("MainWindow", "46.2 cm", 0));
        showAP_radioButton->setText(QApplication::translate("MainWindow", "Show Routers", 0));
        locationY_lineEdit->setText(QString());
        showWCS_radioButton->setText(QApplication::translate("MainWindow", "Show CS", 0));
        locationError_lineEdit->setText(QString());
        methodSelection_comboBox->clear();
        methodSelection_comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "KNN", 0)
         << QApplication::translate("MainWindow", "Improved KNN", 0)
         << QApplication::translate("MainWindow", "GPR(Bayesian Filter Simulation)", 0)
         << QApplication::translate("MainWindow", "GPR(Bayesian Filter)", 0)
         << QApplication::translate("MainWindow", "Collaborative(simulation)", 0)
        );
        methodSettings_pushButton->setText(QApplication::translate("MainWindow", "Settings", 0));
        methodSettings_groupBox->setTitle(QApplication::translate("MainWindow", "Method Settings", 0));
        groundTruthSettings_groupBox->setTitle(QApplication::translate("MainWindow", "Ground Truth Settings", 0));
        groundTruthX_lineEdit->setText(QApplication::translate("MainWindow", "0", 0));
        groundTruthSelection_comboBox->clear();
        groundTruthSelection_comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Input Manually", 0)
         << QApplication::translate("MainWindow", "Visual SLAM", 0)
        );
        groundTruthY_lineEdit->setText(QApplication::translate("MainWindow", "0", 0));
        showGroundTruth_radioButton->setText(QApplication::translate("MainWindow", "Show Truth", 0));
        localizationResults_groupBox->setTitle(QApplication::translate("MainWindow", "Localization Results", 0));
        localizationControl_groupBox->setTitle(QApplication::translate("MainWindow", "Control Area", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Localization", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Robot Control", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuAbout->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
