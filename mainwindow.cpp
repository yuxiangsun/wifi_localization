// Copyright (C) 2018 Yuxiang Sun
// Email: yxsun@link.cuhk.edu.hk
// MIT License

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "localization.h"
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <time.h>
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("WiFi Localization");
    this->setFixedSize(845,775);

    /*-----------The following is the TAB1 of the UI-----------*/

    model = new QStandardItemModel(5,7,this);   //new a model for AP_tableView
    model->setHorizontalHeaderItem(0, new QStandardItem("SSID"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Channel"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Frequency"));
    model->setHorizontalHeaderItem(3, new QStandardItem("RSSI"));
    model->setHorizontalHeaderItem(4, new QStandardItem("Quality"));
    model->setHorizontalHeaderItem(5, new QStandardItem("Encryption"));
    model->setHorizontalHeaderItem(6, new QStandardItem("MAC Address"));
    ui->AP_tableView->setModel(model);  //relate the model with AP_tableView

    ui->AP_tableView->verticalHeader()->hide(); //hide the No. of each row
    ui->AP_tableView->setShowGrid(false);   //hide the grids
    ui->AP_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //set selection mode as the SelectRows
    ui->AP_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);   //all cells is read-only
    ui->AP_tableView->setContextMenuPolicy(Qt::CustomContextMenu);  //right clicked enabled

    ui->AP_tableView->setRowHeight(0,20);
    ui->AP_tableView->setRowHeight(1,20);
    ui->AP_tableView->setRowHeight(2,20);
    ui->AP_tableView->setRowHeight(3,20);
    ui->AP_tableView->setRowHeight(4,20);
    ui->AP_tableView->setColumnWidth(0,90);
    ui->AP_tableView->setColumnWidth(1,90);
    ui->AP_tableView->setColumnWidth(2,120);
    ui->AP_tableView->setColumnWidth(3,90);
    ui->AP_tableView->setColumnWidth(4,150);
    ui->AP_tableView->setColumnWidth(5,90);
    ui->AP_tableView->setColumnWidth(6,165);

    ui->scanTimes_spinBox->setRange(1,10000);    //set each measurement can support up to 10000 scans
    ui->scanTimes_spinBox->setValue(100);

    ui->stopScan_pushButton->setCheckable(true);    //make this button become a toggle button

    ui->X_spinBox->setRange(0,39);
    ui->X_spinBox->setValue(0);

    ui->Y_spinBox->setRange(0,22);
    ui->Y_spinBox->setValue(0);

    ui->measurement_progressBar->setRange(0,ui->scanTimes_spinBox->value());
    ui->measurement_progressBar->setValue(0);

    ui->plotRSSI_radioButton->setChecked(true);
    ui->plotRSSI_radioButton->setAutoExclusive(false);  //make the radioButton become a non AutoExclusive button
    ui->writeDatabase_radioButton->setChecked(true);
    ui->writeDatabase_radioButton->setAutoExclusive(false);

    ui->measurementInfo_plainTextEdit->setReadOnly(true);   //set the plain text as a display

    ui->plotRSSI_widget->xAxis->setLabel("Scanning ID");
    ui->plotRSSI_widget->yAxis->setLabel("RSSI Value (dBm)");
    ui->plotRSSI_widget->xAxis->setRange(1, ui->scanTimes_spinBox->value());
    ui->plotRSSI_widget->yAxis->setRange(-100, 0);
    ui->plotRSSI_widget->setAntialiasedElements(QCP::aeAll);

    /*-----------The following is the TAB2 of the UI-----------*/

    ui->readX_spinBox->setValue(0);
    ui->readY_spinBox->setValue(0);

    ui->readplotRSSI_widget->xAxis->setLabel("Scanning ID");
    ui->readplotRSSI_widget->yAxis->setLabel("RSSI Value (dBm)");
    ui->readplotRSSI_widget->xAxis->setRange(1,1000);
    ui->readplotRSSI_widget->yAxis->setRange(-100,0);
    ui->readplotRSSI_widget->setAntialiasedElements(QCP::aeAll);


    /*-----------The following is the TAB3 of the UI-----------*/

    mapScence = new QGraphicsScene(this);
    mapScence->setSceneRect(0,0,1200,695);

    //draw map
    QGraphicsPixmapItem *mapPixmapItem = new QGraphicsPixmapItem;
    mapPixmapItem->setPixmap(QPixmap(":/images/pics/map_without_AP.png"));
    mapScence->addItem(mapPixmapItem);

    //draw grids
    crossItemGroup = new QGraphicsItemGroup;
    QGraphicsPixmapItem (*crossItem)[40] = new QGraphicsPixmapItem [23][40];
    for ( int i=0; i<=22; i++ )
    {
        for ( int j=0; j<=39; j++ )
        {
            crossItem[i][j].setPixmap(QPixmap(":/images/pics/cross.png")); //the cross pixmap is 11px longth and 11px width
            crossItem[i][j].setPos(15+30*j-5.5,15+30*i-5.5); //the central point of cross is (x, y), so we need move the cross to (x-5.5,y-5.5), 15+30*j-5.5 is width, 15+30*i-5.5 is height
            crossItemGroup->addToGroup(&crossItem[i][j]);
        }
    }
    crossItemGroup->setVisible(false);  //don't show grids as default
    mapScence->addItem(crossItemGroup);

    //draw APs
    APEllipseItemGroup = new QGraphicsItemGroup;
    QGraphicsEllipseItem *APEllipseItem = new QGraphicsEllipseItem [5];
    for ( int i=0; i<=4; i++ )
    {
        if ( i==0 )
            (APEllipseItem+i)->setRect(15+30*13-20/2,15+30*16-20/2,20,20);
        else if ( i==1 )
            (APEllipseItem+i)->setRect(15+30*36-20/2,15+30*1-20/2,20,20);
        else if ( i==2 )
            (APEllipseItem+i)->setRect(15+30*32-20/2,15+30*20-20/2,20,20);
        else if ( i==3 )
            (APEllipseItem+i)->setRect(15+30*3-20/2,15+30*22-20/2,20,20);
        else if ( i==4 )
            (APEllipseItem+i)->setRect(15+30*5-20/2,15+30*0-20/2,20,20);
        (APEllipseItem+i)->setBrush(QBrush(Qt::red));
        (APEllipseItem+i)->setPen(QPen(Qt::NoPen)); //no boundary
        APEllipseItemGroup->addToGroup(APEllipseItem+i);
    }
    APEllipseItemGroup->setVisible(false);
    mapScence->addItem(APEllipseItemGroup);

    //draw coordinate axis
    coordinateAxisItemGroup = new QGraphicsItemGroup;
    QGraphicsPixmapItem *coordinateAxisPixmapItem = new QGraphicsPixmapItem(QPixmap(":/images/pics/coordinate_axis.png"));
    coordinateAxisItemGroup->addToGroup(coordinateAxisPixmapItem);
    QGraphicsSimpleTextItem (*coordinateAxisTextItem)[40] = new QGraphicsSimpleTextItem [2][40];
    for ( int i=0; i<=1; i++ )
    {
        if( i==0 )  //draw X axis value
        {
            for ( int j=1; j<=39; j++ )
            {
                coordinateAxisTextItem[i][j].setText(QString::number(j));
                coordinateAxisTextItem[i][j].setFont(QFont("Helvetica",10));
                coordinateAxisTextItem[i][j].setPen(Qt::NoPen);
                coordinateAxisTextItem[i][j].setBrush(QBrush(Qt::red));
                coordinateAxisTextItem[i][j].setPos(15+j*30-5,18);
                coordinateAxisItemGroup->addToGroup(&coordinateAxisTextItem[i][j]);
            }
        }
        else    //draw Y axis value
        {
            for ( int j=1; j<=22; j++ )
            {
                coordinateAxisTextItem[i][j].setText(QString::number(j));
                coordinateAxisTextItem[i][j].setFont(QFont("Helvetica",10));
                coordinateAxisTextItem[i][j].setPen(Qt::NoPen);
                coordinateAxisTextItem[i][j].setBrush(QBrush(Qt::red));
                coordinateAxisTextItem[i][j].setPos(21,15+j*30-8);
                coordinateAxisItemGroup->addToGroup(&coordinateAxisTextItem[i][j]);
            }
        }
    }
    coordinateAxisItemGroup->setVisible(false);
    mapScence->addItem(coordinateAxisItemGroup);

    //darw ground truth
    groundTruthRectItem = new QGraphicsRectItem;
    groundTruthRectItem->setPen(Qt::NoPen);
    groundTruthRectItem->setBrush(QBrush(QColor(0,153,68,255)));  //pure green
    groundTruthRectItem->setVisible(false); //don't show grids as default
    mapScence->addItem(groundTruthRectItem);

    //put the scene to the graphicsView
    ui->map_graphicsView->setScene(mapScence);

    ui->zoom_verticalSlider->setRange(65,300);  //set the maximum zoom
    ui->zoom_verticalSlider->setValue(65);
    QTransform mapTransform;    //scale the map 65% to fit the GraphicsView
    ui->map_graphicsView->setTransform(mapTransform.scale(ui->zoom_verticalSlider->value()/100.0,ui->zoom_verticalSlider->value()/100.0),false);
    ui->map_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->map_graphicsView->setRenderHint(QPainter::SmoothPixmapTransform,true);

    ui->showAP_radioButton->setChecked(false);
    ui->showAP_radioButton->setAutoExclusive(false);
    ui->showGrids_radioButton->setChecked(false);
    ui->showGrids_radioButton->setAutoExclusive(false);
    ui->showGroundTruth_radioButton->setChecked(false);
    ui->showGroundTruth_radioButton->setAutoExclusive(false);

    ui->locateStop_PushButton->setCheckable(true);  //make it be a toggle button

    /*------------The folowing is the default value of some variables.------------*/

    delayTime = 2000;
    linuxPassword = "sun";
    k_knn = 100;
    variationThreshold_eknn = 7;
    localizationTimes = 500;

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*****************************Menu*************************/

void MainWindow::on_actionDelay_Time_triggered()    //set delay time
{
    QDialog *setDelayTimeDlg = new QDialog(this);
    QLabel *setDelayTimeDlgLabel = new QLabel(setDelayTimeDlg);
    QSpinBox *setDelayTimeDlgSpinBox = new QSpinBox(setDelayTimeDlg);
    QPushButton *setDelayTimeDlgPushButton = new QPushButton(setDelayTimeDlg);
    setDelayTimeDlg->resize(180,90);
    setDelayTimeDlg->setWindowTitle("Set Delay Time...");
    setDelayTimeDlgLabel->setText("Please input the time \nslot(ms) between each scan.");
    setDelayTimeDlgLabel->move(20,10);
    setDelayTimeDlgSpinBox->move(25,50);
    setDelayTimeDlgSpinBox->resize(60,22);
    setDelayTimeDlgSpinBox->setRange(1,10000);
    setDelayTimeDlgSpinBox->setValue(delayTime);
    setDelayTimeDlgPushButton->move(97,50);
    setDelayTimeDlgPushButton->resize(60,22);
    setDelayTimeDlgPushButton->setText("OK");
    connect(setDelayTimeDlgPushButton,SIGNAL(clicked()),setDelayTimeDlg,SLOT(accept()));
    if (setDelayTimeDlg->exec() == QDialog::Accepted)   //use modal dialog to relize the data transfer
    {
        delayTime = setDelayTimeDlgSpinBox->value();
    }
    delete setDelayTimeDlg;
}

void MainWindow::on_actionSet_Linux_Passward_triggered()    //set Linux Password
{
    QDialog *setLinuxPassword = new QDialog(this);
    QLabel *setLinuxPasswordLabel = new QLabel(setLinuxPassword);
    QLineEdit *setLinuxPasswordLineEdit = new QLineEdit(setLinuxPassword);
    QPushButton *setLinuxPasswordPushButton = new QPushButton(setLinuxPassword);
    setLinuxPassword->resize(180,90);
    setLinuxPassword->setWindowTitle("Set Linux Password...");
    setLinuxPasswordLabel->setText("Please input the Linux Password.");
    setLinuxPasswordLabel->move(12,15);
    setLinuxPasswordLineEdit->move(25,50);
    setLinuxPasswordLineEdit->resize(60,22);
    setLinuxPasswordLineEdit->setText("123456");
    setLinuxPasswordLineEdit->setEchoMode(QLineEdit::Password);
    setLinuxPasswordPushButton->move(97,50);
    setLinuxPasswordPushButton->resize(60,22);
    setLinuxPasswordPushButton->setText("OK");
    connect(setLinuxPasswordPushButton,SIGNAL(clicked()),setLinuxPassword,SLOT(accept()));
    if (setLinuxPassword->exec() == QDialog::Accepted)   //use modal dialog to relize the data transfer
    {
        if (setLinuxPasswordLineEdit->isModified())
            linuxPassword = setLinuxPasswordLineEdit->text();
    }
    delete setLinuxPassword;
}

void MainWindow::sleep()
{
    QTime dieTime = QTime::currentTime().addMSecs(delayTime);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this, tr("About"), "  Developed by Yuxiang Sun \nEmail: yxsun@link.cuhk.edu.hk");
    if (reply == QMessageBox::Ok)
        return ;
}


/*****************************Tab 1*************************/

void MainWindow::on_scan_pushButton_clicked()
{
    ui->scan_pushButton->setEnabled(false); //disable the button
    ui->scanTimes_spinBox->setEnabled(false);   //disable the spinbox
    ui->X_spinBox->setEnabled(false);   //disable the spinbox
    ui->Y_spinBox->setEnabled(false);   //disable the spinbox
    ui->plotRSSI_radioButton->setEnabled(false);    //disable the radioButton
    ui->dataSelection_comboBox->setEnabled(false);

    ui->measurement_progressBar->setValue(0);   //reset the processBar
    ui->measurementInfo_plainTextEdit->setPlainText("");  //reset the plain text dispaly
    ui->plotRSSI_widget->xAxis->setRange(1, ui->scanTimes_spinBox->value());
    if ( ui->dataSelection_comboBox->currentIndex()==0 )    //plot RSSI
    {
        ui->plotRSSI_widget->yAxis->setLabel("RSSI Value (dBm)");
        ui->plotRSSI_widget->yAxis->setRange(-100, 0);  //reset the plot
    }
    if ( ui->dataSelection_comboBox->currentIndex()==1 )    //plot Quality
    {
        ui->plotRSSI_widget->yAxis->setLabel("Quality Value");
        ui->plotRSSI_widget->yAxis->setRange(0, 1.15);  //reset the plot
    }
    ui->plotRSSI_widget->clearGraphs(); //This will clear the graphs in the RAM, but the plot still stay on the widget.
    ui->plotRSSI_widget->replot();  //This will clear the plots after the last statement, reset the plot
    int row=0, col=0;   //reset AP_tableView
    for( row=0; row<=4; row++ )
    {
        for( col=0; col<=6; col++ )
        {
            QStandardItem *data = new QStandardItem(" ");
            model->setItem(row,col,data);
        }
    }

    ui->plotRSSI_widget->xAxis->setRange(1, ui->scanTimes_spinBox->value());

    ui->plotRSSI_widget->addGraph()->setName("Loc0"); //display RSSI[0], using red curve; You can not put addGraph() in a loop, otherwise it will become more and more slow, and finally the application will be freeze.
    ui->plotRSSI_widget->graph(0)->setPen(QPen(Qt::red));
    ui->plotRSSI_widget->addGraph()->setName("Loc1"); //display RSSI[1], using darkYellow curve
    ui->plotRSSI_widget->graph(1)->setPen(QPen(Qt::darkYellow));
    ui->plotRSSI_widget->addGraph()->setName("Loc2"); //display RSSI[2], using blue curve
    ui->plotRSSI_widget->graph(2)->setPen(QPen(Qt::blue));
    ui->plotRSSI_widget->addGraph()->setName("Loc3"); //display RSSI[3], using darkCyan curve
    ui->plotRSSI_widget->graph(3)->setPen(QPen(Qt::darkCyan));
    ui->plotRSSI_widget->addGraph()->setName("Loc4"); //display RSSI[4], using black curve
    ui->plotRSSI_widget->graph(4)->setPen(QPen(Qt::black));
    ui->plotRSSI_widget->legend->setVisible(true);
    ui->plotRSSI_widget->legend->setFont(QFont("Helvetica",9));


    QDateTime startMeasurementTime = startMeasurementTime.currentDateTime();
    ui->measurementInfo_plainTextEdit->appendPlainText("Measurement Information:"); //display the measurement information
    ui->measurementInfo_plainTextEdit->appendPlainText("Start Time:");
    ui->measurementInfo_plainTextEdit->appendPlainText(startMeasurementTime.toString( "yyyy-MM-dd hh:mm:ss" ));
    ui->measurementInfo_plainTextEdit->appendPlainText("Location: ");
    ui->measurementInfo_plainTextEdit->insertPlainText("X "+QString::number(ui->X_spinBox->value())+" ");
    ui->measurementInfo_plainTextEdit->insertPlainText("Y "+QString::number(ui->Y_spinBox->value())+" ");

    QString tempAddress[ui->scanTimes_spinBox->value()][5]; //These strings are for databse writing. row is the number of scan, col is the five Address value respectively
    QString tempChannel[ui->scanTimes_spinBox->value()][5];
    QString tempFrequency[ui->scanTimes_spinBox->value()][5];
    QString tempQuality[ui->scanTimes_spinBox->value()][5];
    QString tempRSSI[ui->scanTimes_spinBox->value()][5];
    QString tempEncryption[ui->scanTimes_spinBox->value()][5];
    QString tempESSID[ui->scanTimes_spinBox->value()][5];

    int stoppedFlag = 0;    //this flag = 1 means the scan process(follwing loop) has been stopped, if stopped, do not write database!!!

    for( int scanTimes = 1; scanTimes <= ui->scanTimes_spinBox->value(); scanTimes++ )  //The scanTimes only count the case when all 5 AP are scanned
    {
        if ( ui->stopScan_pushButton->isChecked() ) //break the loop, end the scan
        {
            stoppedFlag = 1;
            ui->stopScan_pushButton->setChecked(false);
            break ;
        }

        MainWindow::sleep();  //set the dalay

        int numScanned = 0;
        QString scanAddress[5] = {"--","--","--","--","--"};   //These strings are for scanning
        QString scanChannel[5]= {"--","--","--","--","--"};
        QString scanFrequency[5]= {"--","--","--","--","--"};
        QString scanQuality[5]= {"--","--","--","--","--"};
        QString scanRSSI[5]= {"--","--","--","--","--"};
        QString scanEncryption[5]= {"--","--","--","--","--"};
        QString scanESSID[5]= {"--","--","--","--","--"};

        CLocalization scanner;
        scanner.scanWifi( linuxPassword, numScanned, scanAddress, scanChannel,
                          scanFrequency, scanQuality, scanRSSI, scanEncryption, scanESSID);
        int i=0, j=0;   //put the parameters into AP_tableView
        for( j=0; j<=6; j++ )
        {
            for( i=0; i<=4; i++ )
            {
                if( j==0 )
                {
                    QStandardItem *data = new QStandardItem(scanESSID[i]);
                    model->setItem(i,j,data);
                }
                else if( j==1 )
                {
                    QStandardItem *data = new QStandardItem(scanChannel[i]);
                    model->setItem(i,j,data);
                }
                else if( j==2 )
                {
                    QStandardItem *data = new QStandardItem(scanFrequency[i]);
                    model->setItem(i,j,data);
                }
                else if( j==3 )
                {
                    QStandardItem *data = new QStandardItem(scanRSSI[i]);
                    model->setItem(i,j,data);
                }
                else if( j==4 )
                {
                    QStandardItem *data = new QStandardItem(scanQuality[i]);
                    model->setItem(i,j,data);
                }
                else if( j==5 )
                {
                    QStandardItem *data = new QStandardItem(scanEncryption[i]);
                    model->setItem(i,j,data);
                }
                else if( j==6 )
                {
                    QStandardItem *data = new QStandardItem(scanAddress[i]);
                    model->setItem(i,j,data);
                }
                model->item(i, j)->setTextAlignment(Qt::AlignCenter);
            }
        }
        if( numScanned != 5 )
        {
            scanTimes--;
        }
        else    //numScanned == 5, display and plot RSSI or Quality, store all the information
        {
            if ( ui->dataSelection_comboBox->currentIndex()==0 ) //display the RSSI Fingerprint
            {
                ui->measurementInfo_plainTextEdit->appendPlainText("Scanning ID:"+QString::number(scanTimes-1)+'\n'+"RSSI Fingerprint:  ");
                for ( int i=0; i<=4; i++)
                {
                    ui->measurementInfo_plainTextEdit->insertPlainText(scanRSSI[i]+"  ");
                }
                if ( ui->plotRSSI_radioButton->isChecked() )    //plot RSSI
                {
                    ui->plotRSSI_widget->graph(0)->addData(scanTimes, scanRSSI[0].toDouble());
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(1)->addData(scanTimes, scanRSSI[1].toDouble());
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(2)->addData(scanTimes, scanRSSI[2].toDouble());
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(3)->addData(scanTimes, scanRSSI[3].toDouble());
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(4)->addData(scanTimes, scanRSSI[4].toDouble());
                    ui->plotRSSI_widget->replot();
                }
            }

            qreal scanQualityValue[5];
            if ( ui->dataSelection_comboBox->currentIndex()==1 ) //display the Quality Fingerprint
            {
                ui->measurementInfo_plainTextEdit->appendPlainText("Scanning ID:"+QString::number(scanTimes-1)+'\n'+"Quality Fingerprint:  ");
                for ( int i=0; i<=4; i++)
                {
                    ui->measurementInfo_plainTextEdit->insertPlainText(scanQuality[i]+"  ");
                    QString scanQualityNumerator = scanQuality[i].split("/").at(0);
                    QString scanQualityDenominator  = scanQuality[i].split("/").at(1);
                    scanQualityValue[i] = scanQualityNumerator.toDouble() / scanQualityDenominator.toDouble();
                }
                if ( ui->plotRSSI_radioButton->isChecked() )    //plot RSSI
                {
                    ui->plotRSSI_widget->graph(0)->addData(scanTimes, scanQualityValue[0]);
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(1)->addData(scanTimes, scanQualityValue[1]);
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(2)->addData(scanTimes, scanQualityValue[2]);
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(3)->addData(scanTimes, scanQualityValue[3]);
                    ui->plotRSSI_widget->replot();
                    ui->plotRSSI_widget->graph(4)->addData(scanTimes, scanQualityValue[4]);
                    ui->plotRSSI_widget->replot();
                }
            }

            for ( int i=0; i<=4; i++)   //store all the scanned information in the 2-D array tempAddress, tempChannel...
            {
                tempAddress[scanTimes-1][i] = scanAddress[i];
                tempChannel[scanTimes-1][i] =scanChannel[i];
                tempFrequency[scanTimes-1][i] = scanFrequency[i];
                tempQuality[scanTimes-1][i] = scanQuality[i];
                tempRSSI[scanTimes-1][i] = scanRSSI[i];
                tempEncryption[scanTimes-1][i] = scanEncryption[i];
                tempESSID[scanTimes-1][i] = scanESSID[i];
            }
        }
        ui->measurement_progressBar->setRange(0,ui->scanTimes_spinBox->value());
        ui->measurement_progressBar->setValue(scanTimes);   //give the progressBar the current scanTimes
    }

    if (stoppedFlag == 1 )
    {
        ui->measurementInfo_plainTextEdit->appendPlainText("The scanning has been stopped!\nThe incomplete measurement data will not be written in the database!");
    }
    QDateTime endMeasurementTime = endMeasurementTime.currentDateTime();
    ui->measurementInfo_plainTextEdit->appendPlainText("End Time:");
    ui->measurementInfo_plainTextEdit->appendPlainText(endMeasurementTime.toString( "yyyy-MM-dd hh:mm:ss" ));

    if ( (ui->writeDatabase_radioButton->isChecked()) && (!stoppedFlag) )   //save data into the XML database
    {
        CLocalization databaseWriter;
        int tempX = ui->X_spinBox->value();
        int tempY = ui->Y_spinBox->value();
        int tempscanTimes = ui->scanTimes_spinBox->value();
        QString tempstartMeasurementTime = startMeasurementTime.toString( "yyyy-MM-dd hh:mm:ss" );
        QString tempendMeasurementTime = endMeasurementTime.toString( "yyyy-MM-dd hh:mm:ss" );
        databaseWriter.writeDatabase( tempX, tempY, tempscanTimes,
                                      tempstartMeasurementTime,tempendMeasurementTime,
                                      tempAddress, tempChannel, tempFrequency, tempQuality, tempRSSI, tempEncryption, tempESSID );
    }

    ui->scan_pushButton->setEnabled(true);  //enable the button
    ui->scanTimes_spinBox->setEnabled(true);    //enable the spinbox
    ui->X_spinBox->setEnabled(true);   //enable the spinbox
    ui->Y_spinBox->setEnabled(true);   //enable the spinbox
    ui->plotRSSI_radioButton->setEnabled(true);    //enable the radioButton
    ui->dataSelection_comboBox->setEnabled(true);

}


void MainWindow::on_viewMap_pushButton_clicked()
{
    QDialog *viewMapDlg = new QDialog(this);
    QLabel *viewMapDlgLabel = new QLabel(viewMapDlg);
    viewMapDlg->setWindowModality(Qt::NonModal);
    viewMapDlg->setWindowTitle("View map...");
    viewMapDlg->resize(1210, 705);
    viewMapDlgLabel->setObjectName(QString::fromUtf8("map_label"));
    viewMapDlgLabel->setGeometry(QRect(5, 5, 1200, 695));
    viewMapDlgLabel->setPixmap(QPixmap(":/images/pics/map.png"));
    viewMapDlg->show();
}

/*****************************Tab 2*************************/

void MainWindow::on_readDatabase_pushButton_clicked()
{
    ui->readmeasurementInfo_plainTextEdit->setPlainText("");  //reset the plain text dispaly

    int tempX, tempY, tempscanTimes;
    QString tempstartMeasurementTime, tempendMeasurementTime;
    QString tempAddress[ui->scanTimes_spinBox->maximum()][5];   //here the ui->scanTimes_spinBox->maximum() is 10000
    QString tempChannel[ui->scanTimes_spinBox->maximum()][5];
    QString tempFrequency[ui->scanTimes_spinBox->maximum()][5];
    QString tempQuality[ui->scanTimes_spinBox->maximum()][5];
    QString tempRSSI[ui->scanTimes_spinBox->maximum()][5];
    QString tempEncryption[ui->scanTimes_spinBox->maximum()][5];
    QString tempESSID[ui->scanTimes_spinBox->maximum()][5];

    CLocalization databaseReader;
    tempX = ui->readX_spinBox->value();
    tempY = ui->readY_spinBox->value();
    int readDatabaseSucessFlag = 0;

    readDatabaseSucessFlag = databaseReader.readDatabase( tempX, tempY, tempscanTimes,
                                                          tempstartMeasurementTime,tempendMeasurementTime,
                                                          tempAddress, tempChannel, tempFrequency, tempQuality, tempRSSI, tempEncryption, tempESSID );

    if ( readDatabaseSucessFlag == 2 )  //The data has been found successfully
    {
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("Measurement Information in the Database:"); //display the read measurement information
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("Location: ");
        QString temp;
        ui->readmeasurementInfo_plainTextEdit->insertPlainText("X "+temp.setNum(tempX)+" ");
        ui->readmeasurementInfo_plainTextEdit->insertPlainText("Y "+temp.setNum(tempY)+" ");
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("No. of Scannings: "+temp.setNum(tempscanTimes));
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("Start Time:");
        ui->readmeasurementInfo_plainTextEdit->appendPlainText(tempstartMeasurementTime);
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("End Time:");
        ui->readmeasurementInfo_plainTextEdit->appendPlainText(tempendMeasurementTime);
        if ( ui->readdataSelection_comboBox->currentIndex()==0 )
        {
            for ( int i=0; i<tempscanTimes; i++)   //display the RSSI Fingerprint
            {
                ui->readmeasurementInfo_plainTextEdit->appendPlainText("Scanning ID:"+temp.setNum(i+1)+'\n'+"RSSI Fingerprint:  ");
                for ( int j=0; j<5; j++ )
                {
                    ui->readmeasurementInfo_plainTextEdit->insertPlainText(tempRSSI[i][j]+"  ");
                }
            }

            QVector<double> x(tempscanTimes), y0(tempscanTimes), y1(tempscanTimes), y2(tempscanTimes), y3(tempscanTimes), y4(tempscanTimes); // initialize with entries 0..100
            for (int i=0; i<tempscanTimes; ++i)
            {
                x[i] = i;
                y0[i] = tempRSSI[i][0].toDouble();
                y1[i] = tempRSSI[i][1].toDouble();
                y2[i] = tempRSSI[i][2].toDouble();
                y3[i] = tempRSSI[i][3].toDouble();
                y4[i] = tempRSSI[i][4].toDouble();
            }

            ui->readplotRSSI_widget->xAxis->setRange(0, tempscanTimes-1);
            ui->readplotRSSI_widget->yAxis->setRange(-100, 0);
            ui->readplotRSSI_widget->yAxis->setLabel("RSSI (dBm)");

            ui->readplotRSSI_widget->addGraph()->setName("Loc0"); //plot RSSI[0], using red curve
            ui->readplotRSSI_widget->graph(0)->setPen(QPen(Qt::red));
            ui->readplotRSSI_widget->graph(0)->setData(x,y0);
            ui->readplotRSSI_widget->addGraph()->setName("Loc1"); //plot RSSI[1], using darkYellow curve
            ui->readplotRSSI_widget->graph(1)->setPen(QPen(Qt::darkYellow));
            ui->readplotRSSI_widget->graph(1)->setData(x,y1);
            ui->readplotRSSI_widget->addGraph()->setName("Loc2"); //plot RSSI[2], using blue curve
            ui->readplotRSSI_widget->graph(2)->setPen(QPen(Qt::blue));
            ui->readplotRSSI_widget->graph(2)->setData(x,y2);
            ui->readplotRSSI_widget->addGraph()->setName("Loc3"); //plot RSSI[3], using darkCyan curve
            ui->readplotRSSI_widget->graph(3)->setPen(QPen(Qt::darkCyan));
            ui->readplotRSSI_widget->graph(3)->setData(x,y3);
            ui->readplotRSSI_widget->addGraph()->setName("Loc4"); //plot RSSI[4], using black curve
            ui->readplotRSSI_widget->graph(4)->setPen(QPen(Qt::black));
            ui->readplotRSSI_widget->graph(4)->setData(x,y4);
            //        ui->readplotRSSI_widget->legend->setVisible(true);
            //        ui->readplotRSSI_widget->legend->setFont(QFont("Helvetica",9));
            ui->readplotRSSI_widget->replot();
        }


        if ( ui->readdataSelection_comboBox->currentIndex()==1 )
        {
            qreal scanQualityValue[ui->scanTimes_spinBox->maximum()][5];
            for ( int i=0; i<tempscanTimes; i++)   //display the RSSI Fingerprint
            {
                ui->readmeasurementInfo_plainTextEdit->appendPlainText("Scanning ID:"+temp.setNum(i+1)+'\n'+"Quality Fingerprint:  ");
                for ( int j=0; j<5; j++ )
                {
                    ui->readmeasurementInfo_plainTextEdit->insertPlainText(tempQuality[i][j]+"  ");
                    QString scanQualityNumerator = tempQuality[i][j].split("/").at(0);
                    QString scanQualityDenominator  = tempQuality[i][j].split("/").at(1);
                    scanQualityValue[i][j] = scanQualityNumerator.toDouble() / scanQualityDenominator.toDouble();
                }
            }

            QVector<double> x(tempscanTimes), y0(tempscanTimes), y1(tempscanTimes), y2(tempscanTimes), y3(tempscanTimes), y4(tempscanTimes); // initialize with entries 0..100
            for (int i=0; i<tempscanTimes; ++i)
            {
                x[i] = i;
                y0[i] = scanQualityValue[i][0];
                y1[i] = scanQualityValue[i][1];
                y2[i] = scanQualityValue[i][2];
                y3[i] = scanQualityValue[i][3];
                y4[i] = scanQualityValue[i][4];
            }

            ui->readplotRSSI_widget->xAxis->setRange(0, tempscanTimes-1);
            ui->readplotRSSI_widget->yAxis->setRange(0, 1.1);
            ui->readplotRSSI_widget->yAxis->setLabel("Quality Value");

            ui->readplotRSSI_widget->addGraph()->setName("Loc0"); //plot RSSI[0], using red curve
            ui->readplotRSSI_widget->graph(0)->setPen(QPen(Qt::red));
            ui->readplotRSSI_widget->graph(0)->setData(x,y0);
            ui->readplotRSSI_widget->addGraph()->setName("Loc1"); //plot RSSI[1], using darkYellow curve
            ui->readplotRSSI_widget->graph(1)->setPen(QPen(Qt::darkYellow));
            ui->readplotRSSI_widget->graph(1)->setData(x,y1);
            ui->readplotRSSI_widget->addGraph()->setName("Loc2"); //plot RSSI[2], using blue curve
            ui->readplotRSSI_widget->graph(2)->setPen(QPen(Qt::blue));
            ui->readplotRSSI_widget->graph(2)->setData(x,y2);
            ui->readplotRSSI_widget->addGraph()->setName("Loc3"); //plot RSSI[3], using darkCyan curve
            ui->readplotRSSI_widget->graph(3)->setPen(QPen(Qt::darkCyan));
            ui->readplotRSSI_widget->graph(3)->setData(x,y3);
            ui->readplotRSSI_widget->addGraph()->setName("Loc4"); //plot RSSI[4], using black curve
            ui->readplotRSSI_widget->graph(4)->setPen(QPen(Qt::black));
            ui->readplotRSSI_widget->graph(4)->setData(x,y4);
            //        ui->readplotRSSI_widget->legend->setVisible(true);
            //        ui->readplotRSSI_widget->legend->setFont(QFont("Helvetica",9));
            ui->readplotRSSI_widget->replot();
        }

    }
    else if ( readDatabaseSucessFlag == 1 ) //The data can not been found
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("The data can not been found in the database!");
    else if ( readDatabaseSucessFlag == 0 ) //The XML file doesn't exist
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("The XML Database file doesn't exist!");

}

void MainWindow::on_saveAs_pushButton_clicked()
{
    QString saveTextDataFileName = QFileDialog::getSaveFileName(this, tr("Save Text Data File"), "untitled.txt", tr("Text Documents (*.txt)"));

    ui->readmeasurementInfo_plainTextEdit->setPlainText("");  //reset the plain text dispaly

    int tempX, tempY, tempscanTimes;
    QString tempstartMeasurementTime, tempendMeasurementTime;
    QString tempAddress[ui->scanTimes_spinBox->maximum()][5];   //here the ui->scanTimes_spinBox->maximum() is 10000
    QString tempChannel[ui->scanTimes_spinBox->maximum()][5];
    QString tempFrequency[ui->scanTimes_spinBox->maximum()][5];
    QString tempQuality[ui->scanTimes_spinBox->maximum()][5];
    QString tempRSSI[ui->scanTimes_spinBox->maximum()][5];
    QString tempEncryption[ui->scanTimes_spinBox->maximum()][5];
    QString tempESSID[ui->scanTimes_spinBox->maximum()][5];

    CLocalization databaseReader;
    tempX = ui->readX_spinBox->value();
    tempY = ui->readY_spinBox->value();
    int readDatabaseSucessFlag = 0;

    readDatabaseSucessFlag = databaseReader.readDatabase( tempX, tempY, tempscanTimes,
                                                          tempstartMeasurementTime,tempendMeasurementTime,
                                                          tempAddress, tempChannel, tempFrequency, tempQuality, tempRSSI, tempEncryption, tempESSID );

    if ( readDatabaseSucessFlag == 2 )  //The data has been found successfully
    {
        QFile file(saveTextDataFileName);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QMessageBox::critical(NULL, "tip", "can not create file");
        }
        QTextStream out(&file);
        for ( int i=0; i<tempscanTimes; i++)
        {
            for ( int j=0; j<5; j++ )
            {
                out << tempRSSI[i][j]<<" ";
            }
            out << endl;
        }
        out.flush();
        file.close();
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("The database has been successfully saved as a text data file!");
    }
    else if ( readDatabaseSucessFlag == 1 ) //The data can not been found
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("The data can not been found in the database! \nSaving text data file failed!");
    else if ( readDatabaseSucessFlag == 0 ) //The XML file doesn't exist
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("The XML Database file doesn't exist! \nSaving text data file failed!");
}

void MainWindow::on_creatTrainingDataset_pushButton_clicked()
{
    ui->readmeasurementInfo_plainTextEdit->setPlainText("");  //reset the plain text dispaly
    ui->readmeasurementInfo_plainTextEdit->setPlainText("Generate training dataset from database...");
    ui->readmeasurementInfo_plainTextEdit->repaint();

    //trainingDatasetType_comboBox->currentIndex() = 0  raw datatype; trainingDatasetType_comboBox->currentIndex() = 1  mean datatype

    int trainingDatasetLength =0;
    trainingData *trainingDataset = new trainingData [40*23*ui->scanTimes_spinBox->maximum()];
    CLocalization trainingDatasetCreator;   //read database from 0<=X<=39 and 0<=Y<=22

    int creatTrainingDatasetSucessFlag = trainingDatasetCreator.creatTrainingDataset(ui->trainingDatasetType_comboBox->currentIndex(),trainingDatasetLength,trainingDataset);

    if ( creatTrainingDatasetSucessFlag==0 )
    {
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("Reading database failed, can't creat training dataset.");
        delete trainingDataset;
        return ;
    }
    else
    {
        QString saveTextDataFileName = QFileDialog::getSaveFileName(this, tr("Save Training Data File"), "untitled.txt", tr("Text Documents (*.txt)"));
        QFile file(saveTextDataFileName);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QMessageBox::critical(NULL, "tip", "can not create file");
            ui->readmeasurementInfo_plainTextEdit->appendPlainText("Can't find the text file to save training data!");
            delete trainingDataset;
            return ;
        }
        QTextStream out(&file);
        for ( int i=0; i<trainingDatasetLength; i++)
        {
            for ( int j=0; j<5; j++ )
            {
                out << trainingDataset[i].fingerprint[j]<<" ";
            }
            out << trainingDataset[i].x <<" ";
            out << trainingDataset[i].y <<" ";
            out << endl;
        }
        out.flush();
        file.close();
        ui->readmeasurementInfo_plainTextEdit->appendPlainText("The training dataset has been successfully saved as a text data file!");

        //        for ( int i=0; i<trainingDatasetLength; i++ )
        //            qDebug() << trainingDataset[i].fingerprint[0] << " " <<trainingDataset[i].fingerprint[1] << " " <<trainingDataset[i].fingerprint[2] << " "<< trainingDataset[i].fingerprint[3] << " " << trainingDataset[i].fingerprint[4] << " " << trainingDataset[i].x << " " << trainingDataset[i].y << " " ;

    }

    delete []trainingDataset;
    return;
}

void MainWindow::on_getSparseDatabase_pushButton_clicked()
{
    QMessageBox msgBox(QMessageBox::Warning, tr("Creat sparse database..."),
                       "This will make irretrievable loss to the database! ", 0, this);
    msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
    msgBox.addButton(tr("&Cancel"), QMessageBox::RejectRole);
    if (msgBox.exec() == QMessageBox::AcceptRole)
    {
        ui->readmeasurementInfo_plainTextEdit->setPlainText("");  //reset the plain text dispaly
        ui->readmeasurementInfo_plainTextEdit->setPlainText("Creat sparse database...");
        ui->readmeasurementInfo_plainTextEdit->repaint();
        CLocalization sparseDatabaseCreator;
        int creatSparseDatabaseFlag = 0;
        creatSparseDatabaseFlag = sparseDatabaseCreator.creatSparseDatabase();
        if ( creatSparseDatabaseFlag==0 )
        {
            ui->readmeasurementInfo_plainTextEdit->appendPlainText("Creat sparse database failed.");
            ui->readmeasurementInfo_plainTextEdit->repaint();
        }
        else
        {
            ui->readmeasurementInfo_plainTextEdit->appendPlainText("Creat sparse database successful!.");
            ui->readmeasurementInfo_plainTextEdit->repaint();
        }
        return;
    }
    else
        return;


}

/******************************Tab 3*******************************/

void MainWindow::on_zoom_verticalSlider_valueChanged(int value)
{
    QTransform mapTransform;
    ui->map_graphicsView->setTransform(mapTransform.scale(value/100.0,value/100.0),false);
    QString temp;
    ui->scale_label->setText(temp.setNum(30.0/(value/100.0),'f',1)+"cm");
}

void MainWindow::on_showGrids_radioButton_toggled(bool checked)
{
    if ( checked )
        crossItemGroup->setVisible(true);
    else
        crossItemGroup->setVisible(false);
}

void MainWindow::on_showAP_radioButton_toggled(bool checked)
{
    if ( checked )
        APEllipseItemGroup->setVisible(true);
    else
        APEllipseItemGroup->setVisible(false);
}

void MainWindow::on_showWCS_radioButton_toggled(bool checked)
{
    if ( checked )
        coordinateAxisItemGroup->setVisible(true);
    else
        coordinateAxisItemGroup->setVisible(false);
}

void MainWindow::on_showGroundTruth_radioButton_toggled(bool checked)
{
    if ( checked )
    {
        groundTruthRectItem->setVisible(true);
        groundTruthRectItem->setRect(15+30*ui->groundTruthX_lineEdit->text().toDouble()-25/2, 15+30*ui->groundTruthY_lineEdit->text().toDouble()-25/2, 25, 25);
    }
    else
        groundTruthRectItem->setVisible(false);
}


void MainWindow::on_locate_PushButton_clicked()
{
    ui->locate_PushButton->setEnabled(false);
    ui->methodSelection_comboBox->setEnabled(false);
    ui->groundTruthSelection_comboBox->setEnabled(false);
    ui->groundTruthX_lineEdit->setEnabled(false);
    ui->groundTruthY_lineEdit->setEnabled(false);

    //qDebug()<<ui->methodSelection_comboBox->currentIndex();
    if ( ui->methodSelection_comboBox->currentIndex()==0 )  //use KNN, the following code is exactly same as Improved KNN, just set the threhold to a very large number!!!
    {

        ui->localizationInfo_plainTextEdit->setPlainText("");   //reset the plainTextEdit
        ui->localizationInfo_plainTextEdit->appendPlainText("Start localization.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database...");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        int trainingDatasetLength =0, modeTrainingDatasetLength =0;
        trainingData *trainingDataset = new trainingData [40*23*ui->scanTimes_spinBox->maximum()];  //raw data
        trainingData *modeTrainingDataset = new trainingData [40*23];   //mode data
        CLocalization trainingDatasetCreator;   //read database from 0<=X<=39 and 0<=Y<=22

        int creatTrainingDatasetSucessFlag = trainingDatasetCreator.creatTrainingDataset(0,trainingDatasetLength,trainingDataset);
        if ( creatTrainingDatasetSucessFlag==0 )
        {
            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
            delete []trainingDataset;
            delete []modeTrainingDataset;
            ui->locateStop_PushButton->setChecked(false);
            ui->locate_PushButton->setEnabled(true);
            ui->methodSelection_comboBox->setEnabled(true);
            ui->groundTruthSelection_comboBox->setEnabled(true);
            ui->groundTruthX_lineEdit->setEnabled(true);
            ui->groundTruthY_lineEdit->setEnabled(true);
            return ;
        }

        creatTrainingDatasetSucessFlag = trainingDatasetCreator.creatTrainingDataset(2,modeTrainingDatasetLength,modeTrainingDataset);
        if ( creatTrainingDatasetSucessFlag==0 )
        {
            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
            delete []modeTrainingDataset;
            delete []trainingDataset;
            ui->locateStop_PushButton->setChecked(false);
            ui->locate_PushButton->setEnabled(true);
            ui->methodSelection_comboBox->setEnabled(true);
            ui->groundTruthSelection_comboBox->setEnabled(true);
            ui->groundTruthX_lineEdit->setEnabled(true);
            ui->groundTruthY_lineEdit->setEnabled(true);
            return ;
        }

        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database finished.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Start WiFi scanning...\n");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        QGraphicsEllipseItem *locationEllipseItem = new QGraphicsEllipseItem;
        locationEllipseItem->setPen(Qt::NoPen);
        locationEllipseItem->setBrush(QBrush(Qt::blue));
        QGraphicsEllipseItem *locationPossibleEllipseItem = new QGraphicsEllipseItem;
        locationPossibleEllipseItem->setPen(Qt::NoPen);
        locationPossibleEllipseItem->setBrush(QBrush(QColor(118,184,234,80)));
        mapScence->addItem(locationEllipseItem);
        mapScence->addItem(locationPossibleEllipseItem);
        QGraphicsEllipseItem *possiblePositions = new QGraphicsEllipseItem [40*23]; //use dot to represent the possible location
        for ( int i=0; i<40*23; i++ )
        {
            mapScence->addItem(possiblePositions+i);
        }

        CLocalization scanner;

        int numScanned = 0;
        QString scanAddress[5] = {"--","--","--","--","--"};   //These strings are for scanning
        QString scanChannel[5]= {"--","--","--","--","--"};
        QString scanFrequency[5]= {"--","--","--","--","--"};
        QString scanQuality[5]= {"--","--","--","--","--"};
        QString scanRSSI[5]= {"--","--","--","--","--"};
        QString scanEncryption[5]= {"--","--","--","--","--"};
        QString scanESSID[5]= {"--","--","--","--","--"};
        bool stopFlag = false;
        qreal currentFingerprint[5] = {0.0,0.0,0.0,0.0,0.0};
        int count = 0;  //count the following while loop

        //The following code is to check if there is a results record file, if not, build one.
        QFile resultsFile("knn_results_evaluation.txt");
        if (!resultsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            resultsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream out(&resultsFile);
            out << "";
            out.flush();
            resultsFile.close();
        }
        resultsFile.close();
        resultsFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
        resultsFile.resize(0);  //clear the text

        // start localization
        while ( !stopFlag )
        {
            if ( ui->locateStop_PushButton->isChecked() ) //break the loop, end the scan
            {
                stopFlag = true;
                ui->locateStop_PushButton->setChecked(false);
                break ;
            }

            MainWindow::sleep();
            scanner.scanWifi( linuxPassword, numScanned, scanAddress, scanChannel,
                              scanFrequency, scanQuality, scanRSSI, scanEncryption, scanESSID);

            for ( int i=0; i<40*23; i++ )   //reset the possible positions dot
            {
                possiblePositions[i].setRect(QRectF()); //QRectF() construct a null rectangle
            }

            if ( numScanned==5 )
            {
                ui->localizationInfo_plainTextEdit->appendPlainText("Scan Times: "+QString::number(count+1));
                ui->localizationInfo_plainTextEdit->appendPlainText("The current RSSI fingerprint: ");
                ui->localizationInfo_plainTextEdit->insertPlainText(scanRSSI[0]+"  "+scanRSSI[1]+"  "+scanRSSI[2]+"  "+scanRSSI[3]+"  "+scanRSSI[4]+"  ");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                currentFingerprint[0] = scanRSSI[0].toDouble();
                currentFingerprint[1] = scanRSSI[1].toDouble();
                currentFingerprint[2] = scanRSSI[2].toDouble();
                currentFingerprint[3] = scanRSSI[3].toDouble();
                currentFingerprint[4] = scanRSSI[4].toDouble();

                int k = k_knn;
                int variationThreshold = 10000; //when set a large variationThreshold, such as 10000, the improved KNN becomes KNN

                location estimatedLocation;
                location locationSet[23*40];
                CLocalization localizationAlgorithm;
                ui->localizationInfo_plainTextEdit->appendPlainText("Estimating the location with KNN algorithm(k=" + QString::number(k) + ").");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
                localizationAlgorithm.eknnClassifier( trainingDatasetLength, trainingDataset, modeTrainingDatasetLength, modeTrainingDataset, k, variationThreshold, currentFingerprint, estimatedLocation, locationSet );
                ui->localizationInfo_plainTextEdit->appendPlainText("The estimated location is ( " + QString::number(estimatedLocation.x) + ", " + QString::number(estimatedLocation.y) + " ).");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                qreal locationError = sqrt( pow((estimatedLocation.x-ui->groundTruthX_lineEdit->text().toDouble()),2) + pow((estimatedLocation.y-ui->groundTruthY_lineEdit->text().toDouble()),2) );  //error*30 is the coordinate in px

                //display information in the lineEdits
                ui->locationX_lineEdit->setText(QString::number(estimatedLocation.x,'f',1));
                ui->locationY_lineEdit->setText(QString::number(estimatedLocation.y,'f',1));
                ui->locationError_lineEdit->setText(QString::number(locationError,'f',2));
                ui->locationPossible_lineEdit->setText("NA");

                ui->localizationInfo_plainTextEdit->appendPlainText("The localization error is " + QString::number(locationError,'f',2) +".\n");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time


                //dispaly location and possible location ellipse
                locationEllipseItem->setRect(15+30*estimatedLocation.x-10, 15+30*estimatedLocation.y-10, 20, 20);
                // locationPossibleEllipseItem->setRect(15+30*estimatedLocation.x-locationError*30, 15+30*estimatedLocation.y-locationError*30, 2*locationError*30,2*locationError*30);

                //display possible position
                int maxVote = locationSet[0].vote;
                for ( int i=0; i<40*23; i++ )
                {
                    if ( maxVote<locationSet[i].vote )
                        maxVote = locationSet[i].vote;
                }
                int minVote = maxVote;
                for ( int i=0; i<40*23; i++ )
                {
                    if ( (minVote>locationSet[i].vote)&&(locationSet[i].vote!=0) )
                        minVote = locationSet[i].vote;
                }

                for ( int i=0; i<40*23; i++ )   //radius formula: r=((x-min)/(max-min))*(rmax-rmin)+rmin, here we set the rmax=5, rmin=0
                {
                    if ( locationSet[i].vote!=0 )
                    {
                        if ( !((locationSet[i].x==estimatedLocation.x)&&(locationSet[i].y==estimatedLocation.y)) )
                        {
                            qreal radius = 5+(20-5)*(locationSet[i].vote-minVote)/(maxVote-minVote);
                            possiblePositions[i].setRect(15+30*locationSet[i].x-radius/2, 15+30*locationSet[i].y-radius/2, radius, radius);
                            possiblePositions[i].setPen(QPen(Qt::NoPen));
                            possiblePositions[i].setBrush(QBrush(Qt::black));
                        }
                    }
                }

                //The following code is for results recording.
                if ( maxVote!=0 )
                {
                    QTextStream out(&resultsFile);
                    out << ui->groundTruthX_lineEdit->text().toDouble() << " " << ui->groundTruthY_lineEdit->text().toDouble() << " " << estimatedLocation.x << " " << estimatedLocation.y << " " << QString::number(locationError,'f',2) << endl;
                    out.flush();
                }
                //End recording code

                if (maxVote==0)
                    count--;

                count++;

                if (count==localizationTimes)
                    break;
            }
            numScanned = 0; //reset numScanned
        }

        resultsFile.close();

        ui->localizationInfo_plainTextEdit->appendPlainText("Stop localization.");

        delete []trainingDataset;
        delete []modeTrainingDataset;
        delete locationEllipseItem;
        delete locationPossibleEllipseItem;
        delete []possiblePositions;

    }

    if ( ui->methodSelection_comboBox->currentIndex()==1 )  //use Improved KNN
    {

        ui->localizationInfo_plainTextEdit->setPlainText("");   //reset the plainTextEdit
        ui->localizationInfo_plainTextEdit->appendPlainText("Start localization.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database...");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        int trainingDatasetLength =0, modeTrainingDatasetLength =0;
        trainingData *trainingDataset = new trainingData [40*23*ui->scanTimes_spinBox->maximum()];  //raw data
        trainingData *modeTrainingDataset = new trainingData [40*23];   //mode data
        CLocalization trainingDatasetCreator;   //read database from 0<=X<=39 and 0<=Y<=22

        int creatTrainingDatasetSucessFlag = trainingDatasetCreator.creatTrainingDataset(0,trainingDatasetLength,trainingDataset);
        if ( creatTrainingDatasetSucessFlag==0 )
        {
            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
            delete []trainingDataset;
            delete []modeTrainingDataset;
            ui->locateStop_PushButton->setChecked(false);
            ui->locate_PushButton->setEnabled(true);
            ui->methodSelection_comboBox->setEnabled(true);
            ui->groundTruthSelection_comboBox->setEnabled(true);
            ui->groundTruthX_lineEdit->setEnabled(true);
            ui->groundTruthY_lineEdit->setEnabled(true);
            return ;
        }

        creatTrainingDatasetSucessFlag = trainingDatasetCreator.creatTrainingDataset(2,modeTrainingDatasetLength,modeTrainingDataset);
        if ( creatTrainingDatasetSucessFlag==0 )
        {
            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
            delete []modeTrainingDataset;
            delete []trainingDataset;
            ui->locateStop_PushButton->setChecked(false);
            ui->locate_PushButton->setEnabled(true);
            ui->methodSelection_comboBox->setEnabled(true);
            ui->groundTruthSelection_comboBox->setEnabled(true);
            ui->groundTruthX_lineEdit->setEnabled(true);
            ui->groundTruthY_lineEdit->setEnabled(true);
            return ;
        }

        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database finished.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Start WiFi scanning...\n");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        QGraphicsEllipseItem *locationEllipseItem = new QGraphicsEllipseItem;
        locationEllipseItem->setPen(Qt::NoPen);
        locationEllipseItem->setBrush(QBrush(Qt::blue));
        QGraphicsEllipseItem *locationPossibleEllipseItem = new QGraphicsEllipseItem;
        locationPossibleEllipseItem->setPen(Qt::NoPen);
        locationPossibleEllipseItem->setBrush(QBrush(QColor(118,184,234,80)));
        mapScence->addItem(locationEllipseItem);
        mapScence->addItem(locationPossibleEllipseItem);
        QGraphicsEllipseItem *possiblePositions = new QGraphicsEllipseItem [40*23]; //use dot to represent the possible location
        for ( int i=0; i<40*23; i++ )
        {
            mapScence->addItem(possiblePositions+i);
        }

        CLocalization scanner;

        int numScanned = 0;
        QString scanAddress[5] = {"--","--","--","--","--"};   //These strings are for scanning
        QString scanChannel[5]= {"--","--","--","--","--"};
        QString scanFrequency[5]= {"--","--","--","--","--"};
        QString scanQuality[5]= {"--","--","--","--","--"};
        QString scanRSSI[5]= {"--","--","--","--","--"};
        QString scanEncryption[5]= {"--","--","--","--","--"};
        QString scanESSID[5]= {"--","--","--","--","--"};
        bool stopFlag = false;
        qreal currentFingerprint[5] = {0.0,0.0,0.0,0.0,0.0};
        int count = 0;  //count the following while loop

        //The following code is to check if there is a results record file, if not, build one.
        QFile resultsFile("improvedKnn_results_evaluation.txt");
        if (!resultsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            resultsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream out(&resultsFile);
            out << "";
            out.flush();
            resultsFile.close();
        }
        resultsFile.close();
        resultsFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
        resultsFile.resize(0);  //clear the text

        // start localization
        while ( !stopFlag )
        {
            if ( ui->locateStop_PushButton->isChecked() ) //break the loop, end the scan
            {
                stopFlag = true;
                ui->locateStop_PushButton->setChecked(false);
                break ;
            }

            MainWindow::sleep();
            scanner.scanWifi( linuxPassword, numScanned, scanAddress, scanChannel,
                              scanFrequency, scanQuality, scanRSSI, scanEncryption, scanESSID);

            for ( int i=0; i<40*23; i++ )   //reset the possible positions dot
            {
                possiblePositions[i].setRect(QRectF()); //QRectF() construct a null rectangle
            }

            if ( numScanned==5 )
            {
                ui->localizationInfo_plainTextEdit->appendPlainText("Scan Times: "+QString::number(count+1));
                ui->localizationInfo_plainTextEdit->appendPlainText("The current RSSI fingerprint: ");
                ui->localizationInfo_plainTextEdit->insertPlainText(scanRSSI[0]+"  "+scanRSSI[1]+"  "+scanRSSI[2]+"  "+scanRSSI[3]+"  "+scanRSSI[4]+"  ");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                currentFingerprint[0] = scanRSSI[0].toDouble();
                currentFingerprint[1] = scanRSSI[1].toDouble();
                currentFingerprint[2] = scanRSSI[2].toDouble();
                currentFingerprint[3] = scanRSSI[3].toDouble();
                currentFingerprint[4] = scanRSSI[4].toDouble();

                int k = k_knn;
                int variationThreshold = variationThreshold_eknn; //when set a large variationThreshold, such as 100, the E-KNN becomes KNN

                location estimatedLocation;
                location locationSet[23*40];
                CLocalization localizationAlgorithm;
                ui->localizationInfo_plainTextEdit->appendPlainText("Estimating the location with Improved KNN algorithm(k=" + QString::number(k) + ", Variation Threshold= " + QString::number(variationThreshold) +  ").");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
                localizationAlgorithm.eknnClassifier( trainingDatasetLength, trainingDataset, modeTrainingDatasetLength, modeTrainingDataset, k, variationThreshold, currentFingerprint, estimatedLocation, locationSet );
                ui->localizationInfo_plainTextEdit->appendPlainText("The estimated location is ( " + QString::number(estimatedLocation.x) + ", " + QString::number(estimatedLocation.y) + " ).");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                qreal locationError = sqrt( pow((estimatedLocation.x-ui->groundTruthX_lineEdit->text().toDouble()),2) + pow((estimatedLocation.y-ui->groundTruthY_lineEdit->text().toDouble()),2) );  //error*30 is the coordinate in px

                //display information in the lineEdits
                ui->locationX_lineEdit->setText(QString::number(estimatedLocation.x,'f',1));
                ui->locationY_lineEdit->setText(QString::number(estimatedLocation.y,'f',1));
                ui->locationError_lineEdit->setText(QString::number(locationError,'f',2));
                ui->locationPossible_lineEdit->setText("NA");

                ui->localizationInfo_plainTextEdit->appendPlainText("The localization error is " + QString::number(locationError,'f',2) +".\n");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time


                //dispaly location and possible location ellipse
                locationEllipseItem->setRect(15+30*estimatedLocation.x-10, 15+30*estimatedLocation.y-10, 20, 20);
                // locationPossibleEllipseItem->setRect(15+30*estimatedLocation.x-locationError*30, 15+30*estimatedLocation.y-locationError*30, 2*locationError*30,2*locationError*30);

                //display possible position
                int maxVote = locationSet[0].vote;
                for ( int i=0; i<40*23; i++ )
                {
                    if ( maxVote<locationSet[i].vote )
                        maxVote = locationSet[i].vote;
                }
                int minVote = maxVote;
                for ( int i=0; i<40*23; i++ )
                {
                    if ( (minVote>locationSet[i].vote)&&(locationSet[i].vote!=0) )
                        minVote = locationSet[i].vote;
                }

                for ( int i=0; i<40*23; i++ )   //radius formula: r=((x-min)/(max-min))*(rmax-rmin)+rmin, here we set the rmax=5, rmin=0
                {
                    if ( locationSet[i].vote!=0 )
                    {
                        if ( !((locationSet[i].x==estimatedLocation.x)&&(locationSet[i].y==estimatedLocation.y)) )
                        {
                            qreal radius = 5+(20-5)*(locationSet[i].vote-minVote)/(maxVote-minVote);
                            possiblePositions[i].setRect(15+30*locationSet[i].x-radius/2, 15+30*locationSet[i].y-radius/2, radius, radius);
                            possiblePositions[i].setPen(QPen(Qt::NoPen));
                            possiblePositions[i].setBrush(QBrush(Qt::black));
                        }
                    }
                }

                //The following code is for results recording.
                if ( maxVote!=0 )
                {
                    QTextStream out(&resultsFile);
                    out << ui->groundTruthX_lineEdit->text().toDouble() << " " << ui->groundTruthY_lineEdit->text().toDouble() << " " << estimatedLocation.x << " " << estimatedLocation.y << " " << QString::number(locationError,'f',2) << endl;
                    out.flush();
                }
                //End recording code

                if (maxVote==0)
                    count--;

                count++;

                if (count==localizationTimes)
                    break;
            }
            numScanned = 0; //reset numScanned
        }

        resultsFile.close();

        ui->localizationInfo_plainTextEdit->appendPlainText("Stop localization.");

        delete []trainingDataset;
        delete []modeTrainingDataset;
        delete locationEllipseItem;
        delete locationPossibleEllipseItem;
        delete []possiblePositions;

    }

    if ( ui->methodSelection_comboBox->currentIndex()==2 )  //use GPR with Bayesian Filter (simulation)
    {
        QDir *simulationDir = new QDir;
        if(simulationDir->exists("./simulation"));
        else
            simulationDir->mkdir("./simulation");

          /////Generate Scan Simulation File//////////////
//        /////////////////////////////////////////////////////////////////////////////////////////////
//        ui->localizationInfo_plainTextEdit->setPlainText("");   //reset the plainTextEdit
//        ui->localizationInfo_plainTextEdit->setPlainText("Start creat scan data from database.");   //start creat scan data from database
//        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
//        for ( int tempY = 0; tempY<=22; tempY++ ) //read database from 0<=X<=39 and 0<=Y<=22
//        {
//            for ( int tempX = 0; tempX<=39; tempX++ )
//            {
//                QString saveTextDataFileName = QString::number(tempX)+ "_" + QString::number(tempY) + ".txt";

//                int tempscanTimes;
//                QString tempstartMeasurementTime, tempendMeasurementTime;
//                QString tempAddress[ui->scanTimes_spinBox->maximum()][5];   //here the ui->scanTimes_spinBox->maximum() is 10000
//                QString tempChannel[ui->scanTimes_spinBox->maximum()][5];
//                QString tempFrequency[ui->scanTimes_spinBox->maximum()][5];
//                QString tempQuality[ui->scanTimes_spinBox->maximum()][5];
//                QString tempRSSI[ui->scanTimes_spinBox->maximum()][5];
//                QString tempEncryption[ui->scanTimes_spinBox->maximum()][5];
//                QString tempESSID[ui->scanTimes_spinBox->maximum()][5];

//                CLocalization databaseReader;
//                int readDatabaseSucessFlag = 0;

//                readDatabaseSucessFlag = databaseReader.readDatabase( tempX, tempY, tempscanTimes,
//                                                                      tempstartMeasurementTime,tempendMeasurementTime,
//                                                                      tempAddress, tempChannel, tempFrequency, tempQuality, tempRSSI, tempEncryption, tempESSID );

//                qDebug() << "I am here! " + QString::number(tempX)+ "_" + QString::number(tempY);
//                if ( readDatabaseSucessFlag == 2 )  //The data has been found successfully
//                {
//                    QFile file("./simulation/" + saveTextDataFileName);
//                    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
//                    {
//                        QMessageBox::critical(NULL, "tip", "can not create file");
//                    }
//                    QTextStream out(&file);
//                    for ( int i=0; i<tempscanTimes; i++)
//                    {
//                        for ( int j=0; j<5; j++ )
//                        {
//                            out << tempRSSI[i][j]<<" ";
//                        }
//                        out << endl;
//                    }
//                    out.flush();
//                    file.close();
//                    ui->localizationInfo_plainTextEdit->appendPlainText(saveTextDataFileName + " created.");
//                    ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
//                }
//                else if ( readDatabaseSucessFlag == 1 ) //The data can not been found
//                {
//                    ui->readmeasurementInfo_plainTextEdit->appendPlainText(saveTextDataFileName + " can not been found.");
//                    ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
//                }
//                else if ( readDatabaseSucessFlag == 0 ) //The XML file doesn't exist
//                {
//                    ui->readmeasurementInfo_plainTextEdit->appendPlainText("The XML Database doesn't exist! Failed!");
//                    ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
//                }
//            }
//        }



        //////////////////////////////////////////////////////////////////////////////////////////////////
        //The following code is to check if there is a results record file, if not, build one.
        QFile resultsFile("./simulation/gprBayesian_simulation_results.txt");
        if (!resultsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            resultsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream out(&resultsFile);
            out << "";
            out.flush();
            resultsFile.close();
        }
        resultsFile.close();
        resultsFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
        resultsFile.resize(0);

        //////////////////////////////////////////////////////////////////////////////////////////////////
        for ( int tempY = 0; tempY<=22; tempY++ ) //read database from 0<=X<=39 and 0<=Y<=22
        {
            for ( int tempX = 0; tempX<=39; tempX++ )
            {
                QString saveTextDataFileName = QString::number(tempX)+ "_" + QString::number(tempY) + ".txt";
                //////////////////////////////
                QFile scanfile("./simulation/" + saveTextDataFileName);
                if(!scanfile.open(QIODevice::ReadOnly | QIODevice::Text))
                            continue;
                int scanFileLinecnt = 0;
                //////////////////////////////

                ui->groundTruthX_lineEdit->setText(QString::number(tempX));
                ui->groundTruthY_lineEdit->setText(QString::number(tempY));
                ui->locationX_lineEdit->setText(QString::number(tempX));
                ui->locationY_lineEdit->setText(QString::number(tempY));

                // ui->localizationInfo_plainTextEdit->setPlainText("");   //reset the plainTextEdit
                ui->localizationInfo_plainTextEdit->appendPlainText("Start localization " + QString::number(tempX)+ "_" + QString::number(tempY) +".");
                ui->localizationInfo_plainTextEdit->appendPlainText("Reading database...");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                trainingData *trainingDataset = new trainingData [40*23];
                QFile file("gprDatabase.txt"); //Keep in mind that the relative path is the current working directory!!!
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
                    ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
                    delete []trainingDataset;
                    ui->locate_PushButton->setEnabled(true);
                    ui->methodSelection_comboBox->setEnabled(true);
                    ui->groundTruthSelection_comboBox->setEnabled(true);
                    ui->groundTruthX_lineEdit->setEnabled(true);
                    ui->groundTruthY_lineEdit->setEnabled(true);
                    return ;
                }
                int linecnt = 0;
                while (!file.atEnd())
                {
                    file.readLine();
                    linecnt++;
                }
                file.seek(0);
                QString line;
                QStringList list;
                location *locationSet = new location [linecnt];
                linecnt = 0;
                while (!file.atEnd())
                {
                    line = file.readLine();
                    list = line.split(" ");
                    for ( int i=0; i<=4; i++ )
                    {
                        locationSet[linecnt].priorProbability[i] = 1.0; //initialize prior probability
                        locationSet[linecnt].posteriorProbability[i] = 1.0; //set the belief for state(0)
                        locationSet[linecnt].mean[i] = list.at(2*i).toDouble();
                        locationSet[linecnt].standardDeviation[i] = list.at(2*i+1).toDouble();
                    }
                    locationSet[linecnt].x = list.at(10).toDouble();
                    locationSet[linecnt].y = list.at(11).toDouble();
                    linecnt++;
                }
                file.close();

                location estimatedLocation = locationSet[0];

                //unreachable area. set corresponding posterior probability = 0
                for ( int i=0; i<linecnt; i++ )
                {
                    if ( (locationSet[i].x>=11)&&(locationSet[i].x<=39)&&(locationSet[i].y>=0)&&(locationSet[i].y<=2.8) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=0)&&(locationSet[i].x<=4)&&(locationSet[i].y>=5)&&(locationSet[i].y<=23) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=9)&&(locationSet[i].x<=11.5)&&(locationSet[i].y>=6)&&(locationSet[i].y<=23) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=11.5)&&(locationSet[i].x<=17.5)&&(locationSet[i].y>=5)&&(locationSet[i].y<=7.3) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=11.5)&&(locationSet[i].x<=14)&&(locationSet[i].y>=7)&&(locationSet[i].y<=23) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=14)&&(locationSet[i].x<=21)&&(locationSet[i].y>=19)&&(locationSet[i].y<=23) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=9)&&(locationSet[i].x<=11.5)&&(locationSet[i].y>=6)&&(locationSet[i].y<=23) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=21)&&(locationSet[i].x<=24)&&(locationSet[i].y>=12)&&(locationSet[i].y<=23) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=24)&&(locationSet[i].x<=27)&&(locationSet[i].y>=5.5)&&(locationSet[i].y<=7.5) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=31)&&(locationSet[i].x<=39)&&(locationSet[i].y>=8)&&(locationSet[i].y<=11) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=24)&&(locationSet[i].x<=36)&&(locationSet[i].y>=15)&&(locationSet[i].y<=23) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                    if ( (locationSet[i].x>=22)&&(locationSet[i].x<=24)&&(locationSet[i].y>=5.5)&&(locationSet[i].y<=7.8) )
                    {
                        locationSet[i].posteriorProbability[0] = 0.0;
                        locationSet[i].posteriorProbability[1] = 0.0;
                        locationSet[i].posteriorProbability[2] = 0.0;
                        locationSet[i].posteriorProbability[3] = 0.0;
                        locationSet[i].posteriorProbability[4] = 0.0;
                    }
                }

                ui->localizationInfo_plainTextEdit->appendPlainText("Reading database finished.");
                ui->localizationInfo_plainTextEdit->appendPlainText("Start WiFi scanning...\n");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                // for display
                QGraphicsEllipseItem *locationEllipseItem = new QGraphicsEllipseItem;
                locationEllipseItem->setPen(Qt::NoPen);
                locationEllipseItem->setBrush(QBrush(Qt::blue));
                QGraphicsEllipseItem *locationPossibleEllipseItem = new QGraphicsEllipseItem;
                locationPossibleEllipseItem->setPen(Qt::NoPen);
                locationPossibleEllipseItem->setBrush(QBrush(QColor(118,184,234,80)));
                mapScence->addItem(locationEllipseItem);
                mapScence->addItem(locationPossibleEllipseItem);
                QGraphicsEllipseItem *possiblePositions = new QGraphicsEllipseItem [linecnt]; //use dot to represent the possible location
                for ( int i=0; i<linecnt; i++ )
                {
                    mapScence->addItem(possiblePositions+i);
                }

                // for WiFi RSSI scan
                int numScanned = 0;
                QString scanRSSI[5]= {"--","--","--","--","--"};
                bool stopFlag = false;
                qreal currentFingerprint[5] = {0.0,0.0,0.0,0.0,0.0};
                int count = 0;  //count the following while loop

                // start localization
                while ( (!stopFlag)&&(scanFileLinecnt<100) )
                {
                    if ( ui->locateStop_PushButton->isChecked() ) //break the loop, end the scan
                    {
                        stopFlag = true;
                        ui->locateStop_PushButton->setChecked(false);
                        break ;
                    }
                    MainWindow::sleep();

                    ////////////////////////////////////////////////////
                    line = scanfile.readLine();
                    list = line.split(" ");
                    for ( int i=0; i<=4; i++ )
                    {
                        scanRSSI[i] = list.at(i);
                    }
                    scanFileLinecnt++;
                    numScanned = 5;
                    ////////////////////////////////////////////////////

                    //reset the possible positions dot for display
                    for ( int i=0; i<linecnt; i++ )
                    {
                        possiblePositions[i].setRect(QRectF()); //QRectF() construct a null rectangle
                    }

                    if ( numScanned==5 )
                    {
                        ui->localizationInfo_plainTextEdit->appendPlainText("Scan Times: "+QString::number(count+1));
                        ui->localizationInfo_plainTextEdit->appendPlainText("The current RSSI fingerprint: ");
                        ui->localizationInfo_plainTextEdit->insertPlainText(scanRSSI[0]+"  "+scanRSSI[1]+"  "+scanRSSI[2]+"  "+scanRSSI[3]+"  "+scanRSSI[4]+"  ");
                        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                        currentFingerprint[0] = scanRSSI[0].toDouble();
                        currentFingerprint[1] = scanRSSI[1].toDouble();
                        currentFingerprint[2] = scanRSSI[2].toDouble();
                        currentFingerprint[3] = scanRSSI[3].toDouble();
                        currentFingerprint[4] = scanRSSI[4].toDouble();

                        ui->localizationInfo_plainTextEdit->appendPlainText("Estimating the location with Gaussian Process Regression and Bayesian Filter Method.");
                        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                        //calculate the posterior probability, this is the heart of the bayesian filter!!!
                        for ( int i=0; i<linecnt; i++ )
                        {
                            // time update for prediction
                            locationSet[i].priorProbability[0] = locationSet[i].posteriorProbability[0];
                            locationSet[i].priorProbability[1] = locationSet[i].posteriorProbability[1];
                            locationSet[i].priorProbability[2] = locationSet[i].posteriorProbability[2];
                            locationSet[i].priorProbability[3] = locationSet[i].posteriorProbability[3];
                            locationSet[i].priorProbability[4] = locationSet[i].posteriorProbability[4];
                            // measurement update for correct
                            locationSet[i].posteriorProbability[0] = ((exp(-pow((currentFingerprint[0]-locationSet[i].mean[0]),2)/(2*pow(locationSet[i].standardDeviation[0],2))))/(locationSet[i].standardDeviation[0]*sqrt(2*3.1416)))*locationSet[i].priorProbability[0];
                            locationSet[i].posteriorProbability[1] = ((exp(-pow((currentFingerprint[1]-locationSet[i].mean[1]),2)/(2*pow(locationSet[i].standardDeviation[1],2))))/(locationSet[i].standardDeviation[1]*sqrt(2*3.1416)))*locationSet[i].priorProbability[1];
                            locationSet[i].posteriorProbability[2] = ((exp(-pow((currentFingerprint[2]-locationSet[i].mean[2]),2)/(2*pow(locationSet[i].standardDeviation[2],2))))/(locationSet[i].standardDeviation[2]*sqrt(2*3.1416)))*locationSet[i].priorProbability[2];
                            locationSet[i].posteriorProbability[3] = ((exp(-pow((currentFingerprint[3]-locationSet[i].mean[3]),2)/(2*pow(locationSet[i].standardDeviation[3],2))))/(locationSet[i].standardDeviation[3]*sqrt(2*3.1416)))*locationSet[i].priorProbability[3];
                            locationSet[i].posteriorProbability[4] = ((exp(-pow((currentFingerprint[4]-locationSet[i].mean[4]),2)/(2*pow(locationSet[i].standardDeviation[4],2))))/(locationSet[i].standardDeviation[4]*sqrt(2*3.1416)))*locationSet[i].priorProbability[4];
                            locationSet[i].jointPosteriorProbability = locationSet[i].posteriorProbability[0]*locationSet[i].posteriorProbability[1]*locationSet[i].posteriorProbability[2]*locationSet[i].posteriorProbability[3]*locationSet[i].posteriorProbability[4];
                        }
                        //the jointPosteriorProbability should be normalized!!
                        qreal sumJointPosteriorProbability = 0;
                        for ( int i=0; i<linecnt; i++ )
                        {
                            sumJointPosteriorProbability = sumJointPosteriorProbability + locationSet[i].jointPosteriorProbability;
                        }
                        qreal eta = 1/sumJointPosteriorProbability;  //normalization factor
                        for ( int i=0; i<linecnt; i++ )
                        {
                            locationSet[i].jointPosteriorProbability = eta*(locationSet[i].jointPosteriorProbability);
                        }
                        qDebug()<<eta;

                        //find the Maximum A Posterior location
                        qreal maxJointPosteriorProbability = 0;
                        location maxJointPosteriorProbabilityLocation = locationSet[0];
                        for ( int i=0; i<linecnt; i++ )
                        {
                            if (maxJointPosteriorProbability<locationSet[i].jointPosteriorProbability)
                            {
                                maxJointPosteriorProbability = locationSet[i].jointPosteriorProbability;
                                maxJointPosteriorProbabilityLocation = locationSet[i];
                            }
                        }
                        qreal minJointPosteriorProbability = maxJointPosteriorProbability;
                        for ( int i=0; i<linecnt; i++ )
                        {
                            if ((minJointPosteriorProbability>locationSet[i].jointPosteriorProbability)&&(locationSet[i].jointPosteriorProbability!=0))
                                minJointPosteriorProbability = locationSet[i].jointPosteriorProbability;
                        }

                        qDebug()<<"maxJointPosteriorProbability: "<< maxJointPosteriorProbability;
                        qDebug()<<"minJointPosteriorProbability: "<< minJointPosteriorProbability;

                        //                if ( maxJointPosteriorProbability>minJointPosteriorProbability  )
                        //                    estimatedLocation = maxJointPosteriorProbabilityLocation;


                        if ( maxJointPosteriorProbability>minJointPosteriorProbability  )
                        {
                            estimatedLocation.x = 0;
                            estimatedLocation.y = 0;
                            for ( int i=0; i<linecnt; i++ )
                            {
                                estimatedLocation.x = estimatedLocation.x + (locationSet[i].x)*(locationSet[i].jointPosteriorProbability);
                                estimatedLocation.y = estimatedLocation.y + (locationSet[i].y)*(locationSet[i].jointPosteriorProbability);
                            }
                        }

                        ui->localizationInfo_plainTextEdit->appendPlainText("The estimated location is ( " + QString::number(estimatedLocation.x) + ", " + QString::number(estimatedLocation.y) + " ).");
                        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
                        locationEllipseItem->setRect(15+30*estimatedLocation.x-10, 15+30*estimatedLocation.y-10, 20, 20);

                        //find the 0.8 possible maxJointPosterior location
                        location possibleEstimatedLocation = locationSet[0];
                        qreal tempDistance = 1;
                        for ( int i=0; i<linecnt; i++ )
                        {
                            if ((fabs(0.8*maxJointPosteriorProbability-locationSet[i].jointPosteriorProbability)<tempDistance)&&(locationSet[i].jointPosteriorProbability!=0))
                            {
                                tempDistance = fabs(0.8*maxJointPosteriorProbability-locationSet[i].jointPosteriorProbability);
                                possibleEstimatedLocation = locationSet[i];
                            }
                        }
                        if ( !(maxJointPosteriorProbability>minJointPosteriorProbability)  )
                            possibleEstimatedLocation = estimatedLocation;
                        qreal locationPossibleRadius = sqrt(pow(estimatedLocation.x-possibleEstimatedLocation.x,2)+pow(estimatedLocation.y-possibleEstimatedLocation.y,2));  //error*30 is the coordinate in px

                        locationPossibleEllipseItem->setRect(15+30*estimatedLocation.x-locationPossibleRadius*30, 15+30*estimatedLocation.y-locationPossibleRadius*30, 2*locationPossibleRadius*30,2*locationPossibleRadius*30);

                        //display possible position using random dots
                        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));    //this is to generate the following random number
                        int rand = 0;
                        for ( int i=0; i<linecnt; i=i+rand )   //radius formula: r=((x-min)/(max-min))*(rmax-rmin)+rmin, here we set the rmax=5, rmin=0
                        {
                            rand = qrand()%40;
                            qreal radius = ((log(locationSet[i].jointPosteriorProbability)-log(minJointPosteriorProbability))/(log(maxJointPosteriorProbability)-log(minJointPosteriorProbability)))*(8-0)+0;
                            if (locationSet[i].jointPosteriorProbability!=0)
                            {
                                possiblePositions[i].setRect(15+30*locationSet[i].x-radius/2, 15+30*locationSet[i].y-radius/2, radius, radius);
                                possiblePositions[i].setPen(QPen(Qt::NoPen));
                                possiblePositions[i].setBrush(QBrush(Qt::black));
                            }
                        }

                        //                double time_Start = (double)clock();
                        //                double time_End = (double)clock();
                        //                qDebug()<<(time_End - time_Start)/1000.0<<"s";

                        qreal locationError = sqrt( pow((estimatedLocation.x-ui->groundTruthX_lineEdit->text().toDouble()),2) + pow((estimatedLocation.y-ui->groundTruthY_lineEdit->text().toDouble()),2) );  //error*30 is the coordinate in px
                        ui->localizationInfo_plainTextEdit->appendPlainText("The location error is " + QString::number(locationError,'f',2) + ".\n");
                        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                        // dispaly information in lineEdits
                        ui->locationX_lineEdit->setText(QString::number(estimatedLocation.x,'f',1));
                        ui->locationY_lineEdit->setText(QString::number(estimatedLocation.y,'f',1));
                        ui->locationError_lineEdit->setText(QString::number(locationError,'f',2));
                        ui->locationPossible_lineEdit->setText(QString::number(locationPossibleRadius,'f',1));

                        //The following code is for results recording.
                        if (count == 99) // only record the convergent value
                        {
                        QTextStream out(&resultsFile);
                        out << ui->groundTruthX_lineEdit->text().toDouble() << " " << ui->groundTruthY_lineEdit->text().toDouble() << " " << estimatedLocation.x << " " << estimatedLocation.y << " " << QString::number(locationError,'f',2) << endl;
                        out.flush();
                        }

                        count++;

                        if (count==localizationTimes)
                            break;
                    }
                    numScanned = 0; //reset numScanned
                }
                scanfile.close();

                ui->localizationInfo_plainTextEdit->appendPlainText("Stop localization.\n");

                delete []trainingDataset;
                delete locationEllipseItem;
                delete locationPossibleEllipseItem;
                delete []possiblePositions;
                delete []locationSet;
            }
         }
    }

    if ( ui->methodSelection_comboBox->currentIndex()==3 )  //use GPR with Bayesian Filter
    {

        ui->localizationInfo_plainTextEdit->setPlainText("");   //reset the plainTextEdit
        ui->localizationInfo_plainTextEdit->appendPlainText("Start localization.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database...");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        int trainingDatasetLength =0;
        trainingData *trainingDataset = new trainingData [40*23];
        //        CLocalization trainingDatasetCreator;   //read database from 0<=X<=39 and 0<=Y<=22
        //        int creatTrainingDatasetSucessFlag = trainingDatasetCreator.creatTrainingDataset(2,trainingDatasetLength,trainingDataset);
        //        if ( creatTrainingDatasetSucessFlag==0 )
        //        {
        //            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
        //            delete []trainingDataset;
        //            return;
        //        }

        //read gprDatabase
        QFile file("gprDatabase.txt"); //Keep in mind that the relative path is the current working directory!!!
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
            ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
            delete []trainingDataset;
            ui->locate_PushButton->setEnabled(true);
            ui->methodSelection_comboBox->setEnabled(true);
            ui->groundTruthSelection_comboBox->setEnabled(true);
            ui->groundTruthX_lineEdit->setEnabled(true);
            ui->groundTruthY_lineEdit->setEnabled(true);
            return ;
        }
        int linecnt = 0;
        while (!file.atEnd())
        {
            file.readLine();
            linecnt++;
        }
        file.seek(0);
        QString line;
        QStringList list;
        location *locationSet = new location [linecnt];
        linecnt = 0;
        while (!file.atEnd())
        {
            line = file.readLine();
            list = line.split(" ");
            for ( int i=0; i<=4; i++ )
            {
                locationSet[linecnt].priorProbability[i] = 1.0; //initialize prior probability
                locationSet[linecnt].posteriorProbability[i] = 1.0; //set the belief for state(0)
                locationSet[linecnt].mean[i] = list.at(2*i).toDouble();
                locationSet[linecnt].standardDeviation[i] = list.at(2*i+1).toDouble();
            }
            locationSet[linecnt].x = list.at(10).toDouble();
            locationSet[linecnt].y = list.at(11).toDouble();
            linecnt++;
        }
        file.close();

        location estimatedLocation = locationSet[0];

        //unreachable area. set corresponding posterior probability = 0
        for ( int i=0; i<linecnt; i++ )
        {
            if ( (locationSet[i].x>=11)&&(locationSet[i].x<=39)&&(locationSet[i].y>=0)&&(locationSet[i].y<=2.8) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=0)&&(locationSet[i].x<=4)&&(locationSet[i].y>=5)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=9)&&(locationSet[i].x<=11.5)&&(locationSet[i].y>=6)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=11.5)&&(locationSet[i].x<=17.5)&&(locationSet[i].y>=5)&&(locationSet[i].y<=7.3) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=11.5)&&(locationSet[i].x<=14)&&(locationSet[i].y>=7)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=14)&&(locationSet[i].x<=21)&&(locationSet[i].y>=19)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=9)&&(locationSet[i].x<=11.5)&&(locationSet[i].y>=6)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=21)&&(locationSet[i].x<=24)&&(locationSet[i].y>=12)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=24)&&(locationSet[i].x<=27)&&(locationSet[i].y>=5.5)&&(locationSet[i].y<=7.5) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=31)&&(locationSet[i].x<=39)&&(locationSet[i].y>=8)&&(locationSet[i].y<=11) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=24)&&(locationSet[i].x<=36)&&(locationSet[i].y>=15)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=22)&&(locationSet[i].x<=24)&&(locationSet[i].y>=5.5)&&(locationSet[i].y<=7.8) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
        }

        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database finished.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Start WiFi scanning...\n");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        // for display
        QGraphicsEllipseItem *locationEllipseItem = new QGraphicsEllipseItem;
        locationEllipseItem->setPen(Qt::NoPen);
        locationEllipseItem->setBrush(QBrush(Qt::blue));
        QGraphicsEllipseItem *locationPossibleEllipseItem = new QGraphicsEllipseItem;
        locationPossibleEllipseItem->setPen(Qt::NoPen);
        locationPossibleEllipseItem->setBrush(QBrush(QColor(118,184,234,80)));
        mapScence->addItem(locationEllipseItem);
        mapScence->addItem(locationPossibleEllipseItem);
        QGraphicsEllipseItem *possiblePositions = new QGraphicsEllipseItem [linecnt]; //use dot to represent the possible location
        for ( int i=0; i<linecnt; i++ )
        {
            mapScence->addItem(possiblePositions+i);
        }

        // for WiFi RSSI scan
        CLocalization scanner;
        int numScanned = 0;
        QString scanAddress[5] = {"--","--","--","--","--"};   //These strings are for scanning
        QString scanChannel[5]= {"--","--","--","--","--"};
        QString scanFrequency[5]= {"--","--","--","--","--"};
        QString scanQuality[5]= {"--","--","--","--","--"};
        QString scanRSSI[5]= {"--","--","--","--","--"};
        QString scanEncryption[5]= {"--","--","--","--","--"};
        QString scanESSID[5]= {"--","--","--","--","--"};
        bool stopFlag = false;
        qreal currentFingerprint[5] = {0.0,0.0,0.0,0.0,0.0};
        int count = 0;  //count the following while loop

        //The following code is to check if there is a results record file, if not, build one.
        QFile resultsFile("gprBayesian_results_evaluation.txt");
        if (!resultsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            resultsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream out(&resultsFile);
            out << "";
            out.flush();
            resultsFile.close();
        }
        resultsFile.close();
        resultsFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
        resultsFile.resize(0);

        // start localization
        while ( !stopFlag )
        {
            if ( ui->locateStop_PushButton->isChecked() ) //break the loop, end the scan
            {
                stopFlag = true;
                ui->locateStop_PushButton->setChecked(false);
                break ;
            }
            MainWindow::sleep();
            scanner.scanWifi( linuxPassword, numScanned, scanAddress, scanChannel,
                              scanFrequency, scanQuality, scanRSSI, scanEncryption, scanESSID);

            //reset the possible positions dot for display
            for ( int i=0; i<linecnt; i++ )
            {
                possiblePositions[i].setRect(QRectF()); //QRectF() construct a null rectangle
            }

            if ( numScanned==5 )
            {
                ui->localizationInfo_plainTextEdit->appendPlainText("Scan Times: "+QString::number(count+1));
                ui->localizationInfo_plainTextEdit->appendPlainText("The current RSSI fingerprint: ");
                ui->localizationInfo_plainTextEdit->insertPlainText(scanRSSI[0]+"  "+scanRSSI[1]+"  "+scanRSSI[2]+"  "+scanRSSI[3]+"  "+scanRSSI[4]+"  ");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                currentFingerprint[0] = scanRSSI[0].toDouble();
                currentFingerprint[1] = scanRSSI[1].toDouble();
                currentFingerprint[2] = scanRSSI[2].toDouble();
                currentFingerprint[3] = scanRSSI[3].toDouble();
                currentFingerprint[4] = scanRSSI[4].toDouble();

                ui->localizationInfo_plainTextEdit->appendPlainText("Estimating the location with Gaussian Process Regression and Bayesian Filter Method.");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                //calculate the posterior probability, this is the heart of the bayesian filter!!!
                for ( int i=0; i<linecnt; i++ )
                {
                    // time update for prediction
                    locationSet[i].priorProbability[0] = locationSet[i].posteriorProbability[0];
                    locationSet[i].priorProbability[1] = locationSet[i].posteriorProbability[1];
                    locationSet[i].priorProbability[2] = locationSet[i].posteriorProbability[2];
                    locationSet[i].priorProbability[3] = locationSet[i].posteriorProbability[3];
                    locationSet[i].priorProbability[4] = locationSet[i].posteriorProbability[4];
                    // measurement update for correct
                    locationSet[i].posteriorProbability[0] = ((exp(-pow((currentFingerprint[0]-locationSet[i].mean[0]),2)/(2*pow(locationSet[i].standardDeviation[0],2))))/(locationSet[i].standardDeviation[0]*sqrt(2*3.1416)))*locationSet[i].priorProbability[0];
                    locationSet[i].posteriorProbability[1] = ((exp(-pow((currentFingerprint[1]-locationSet[i].mean[1]),2)/(2*pow(locationSet[i].standardDeviation[1],2))))/(locationSet[i].standardDeviation[1]*sqrt(2*3.1416)))*locationSet[i].priorProbability[1];
                    locationSet[i].posteriorProbability[2] = ((exp(-pow((currentFingerprint[2]-locationSet[i].mean[2]),2)/(2*pow(locationSet[i].standardDeviation[2],2))))/(locationSet[i].standardDeviation[2]*sqrt(2*3.1416)))*locationSet[i].priorProbability[2];
                    locationSet[i].posteriorProbability[3] = ((exp(-pow((currentFingerprint[3]-locationSet[i].mean[3]),2)/(2*pow(locationSet[i].standardDeviation[3],2))))/(locationSet[i].standardDeviation[3]*sqrt(2*3.1416)))*locationSet[i].priorProbability[3];
                    locationSet[i].posteriorProbability[4] = ((exp(-pow((currentFingerprint[4]-locationSet[i].mean[4]),2)/(2*pow(locationSet[i].standardDeviation[4],2))))/(locationSet[i].standardDeviation[4]*sqrt(2*3.1416)))*locationSet[i].priorProbability[4];
                    locationSet[i].jointPosteriorProbability = locationSet[i].posteriorProbability[0]*locationSet[i].posteriorProbability[1]*locationSet[i].posteriorProbability[2]*locationSet[i].posteriorProbability[3]*locationSet[i].posteriorProbability[4];
                }
                //the jointPosteriorProbability should be normalized!!
                qreal sumJointPosteriorProbability = 0;
                for ( int i=0; i<linecnt; i++ )
                {
                    sumJointPosteriorProbability = sumJointPosteriorProbability + locationSet[i].jointPosteriorProbability;
                }
                qreal eta = 1/sumJointPosteriorProbability;  //normalization factor
                for ( int i=0; i<linecnt; i++ )
                {
                    locationSet[i].jointPosteriorProbability = eta*(locationSet[i].jointPosteriorProbability);
                }
                qDebug()<<eta;


                // for simulation. this can be deleted
                if ( count==60 )
                {
                    QFile filelocationSetAnother("locationSetAnother.txt");
                    if (!filelocationSetAnother.open(QIODevice::ReadOnly | QIODevice::Text))
                    {
                        filelocationSetAnother.open(QIODevice::WriteOnly | QIODevice::Truncate);
                        QTextStream outAnother(&filelocationSetAnother);
                        outAnother << "";
                        outAnother.flush();
                        filelocationSetAnother.close();
                    }
                    filelocationSetAnother.close();
                    filelocationSetAnother.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
                    filelocationSetAnother.resize(0);
                    QTextStream outAnother(&filelocationSetAnother);
                    for ( int i=0; i<linecnt; i++ )
                    {
                        outAnother << locationSet[i].x << " " << locationSet[i].y << " " << locationSet[i].jointPosteriorProbability << endl;
                        outAnother.flush();
                    }
                    filelocationSetAnother.close();
                }
                //end temp code


                //find the Maximum A Posterior location
                qreal maxJointPosteriorProbability = 0;
                location maxJointPosteriorProbabilityLocation = locationSet[0];
                for ( int i=0; i<linecnt; i++ )
                {
                    if (maxJointPosteriorProbability<locationSet[i].jointPosteriorProbability)
                    {
                        maxJointPosteriorProbability = locationSet[i].jointPosteriorProbability;
                        maxJointPosteriorProbabilityLocation = locationSet[i];
                    }
                }
                qreal minJointPosteriorProbability = maxJointPosteriorProbability;
                for ( int i=0; i<linecnt; i++ )
                {
                    if ((minJointPosteriorProbability>locationSet[i].jointPosteriorProbability)&&(locationSet[i].jointPosteriorProbability!=0))
                        minJointPosteriorProbability = locationSet[i].jointPosteriorProbability;
                }

                qDebug()<<"maxJointPosteriorProbability: "<< maxJointPosteriorProbability;
                qDebug()<<"minJointPosteriorProbability: "<< minJointPosteriorProbability;

                //                if ( maxJointPosteriorProbability>minJointPosteriorProbability  )
                //                    estimatedLocation = maxJointPosteriorProbabilityLocation;


                if ( maxJointPosteriorProbability>minJointPosteriorProbability  )
                {
                    estimatedLocation.x = 0;
                    estimatedLocation.y = 0;
                    for ( int i=0; i<linecnt; i++ )
                    {
                        estimatedLocation.x = estimatedLocation.x + (locationSet[i].x)*(locationSet[i].jointPosteriorProbability);
                        estimatedLocation.y = estimatedLocation.y + (locationSet[i].y)*(locationSet[i].jointPosteriorProbability);
                    }
                }

                ui->localizationInfo_plainTextEdit->appendPlainText("The estimated location is ( " + QString::number(estimatedLocation.x) + ", " + QString::number(estimatedLocation.y) + " ).");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
                locationEllipseItem->setRect(15+30*estimatedLocation.x-10, 15+30*estimatedLocation.y-10, 20, 20);

                //find the 0.8 possible maxJointPosterior location
                location possibleEstimatedLocation = locationSet[0];
                qreal tempDistance = 1;
                for ( int i=0; i<linecnt; i++ )
                {
                    if ((fabs(0.8*maxJointPosteriorProbability-locationSet[i].jointPosteriorProbability)<tempDistance)&&(locationSet[i].jointPosteriorProbability!=0))
                    {
                        tempDistance = fabs(0.8*maxJointPosteriorProbability-locationSet[i].jointPosteriorProbability);
                        possibleEstimatedLocation = locationSet[i];
                    }
                }
                if ( !(maxJointPosteriorProbability>minJointPosteriorProbability)  )
                    possibleEstimatedLocation = estimatedLocation;
                qreal locationPossibleRadius = sqrt(pow(estimatedLocation.x-possibleEstimatedLocation.x,2)+pow(estimatedLocation.y-possibleEstimatedLocation.y,2));  //error*30 is the coordinate in px

                locationPossibleEllipseItem->setRect(15+30*estimatedLocation.x-locationPossibleRadius*30, 15+30*estimatedLocation.y-locationPossibleRadius*30, 2*locationPossibleRadius*30,2*locationPossibleRadius*30);

                //display possible position using random dots
                qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));    //this is to generate the following random number
                int rand = 0;
                for ( int i=0; i<linecnt; i=i+rand )   //radius formula: r=((x-min)/(max-min))*(rmax-rmin)+rmin, here we set the rmax=5, rmin=0
                {
                    rand = qrand()%40;
                    qreal radius = ((log(locationSet[i].jointPosteriorProbability)-log(minJointPosteriorProbability))/(log(maxJointPosteriorProbability)-log(minJointPosteriorProbability)))*(8-0)+0;
                    if (locationSet[i].jointPosteriorProbability!=0)
                    {
                        possiblePositions[i].setRect(15+30*locationSet[i].x-radius/2, 15+30*locationSet[i].y-radius/2, radius, radius);
                        possiblePositions[i].setPen(QPen(Qt::NoPen));
                        possiblePositions[i].setBrush(QBrush(Qt::black));
                    }
                }

                //                double time_Start = (double)clock();
                //                double time_End = (double)clock();
                //                qDebug()<<(time_End - time_Start)/1000.0<<"s";

                qreal locationError = sqrt( pow((estimatedLocation.x-ui->groundTruthX_lineEdit->text().toDouble()),2) + pow((estimatedLocation.y-ui->groundTruthY_lineEdit->text().toDouble()),2) );  //error*30 is the coordinate in px
                ui->localizationInfo_plainTextEdit->appendPlainText("The location error is " + QString::number(locationError,'f',2) + ".\n");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                // dispaly information in lineEdits
                ui->locationX_lineEdit->setText(QString::number(estimatedLocation.x,'f',1));
                ui->locationY_lineEdit->setText(QString::number(estimatedLocation.y,'f',1));
                ui->locationError_lineEdit->setText(QString::number(locationError,'f',2));
                ui->locationPossible_lineEdit->setText(QString::number(locationPossibleRadius,'f',1));

                //The following code is for results recording.
                QTextStream out(&resultsFile);
                out << ui->groundTruthX_lineEdit->text().toDouble() << " " << ui->groundTruthY_lineEdit->text().toDouble() << " " << estimatedLocation.x << " " << estimatedLocation.y << " " << QString::number(locationError,'f',2) << endl;
                out.flush();

                count++;

                if (count==localizationTimes)
                    break;
            }
            numScanned = 0; //reset numScanned
        }

        resultsFile.close();

        ui->localizationInfo_plainTextEdit->appendPlainText("Stop localization.");

        delete []trainingDataset;
        delete locationEllipseItem;
        delete locationPossibleEllipseItem;
        delete []possiblePositions;
        delete []locationSet;
    }

    if ( ui->methodSelection_comboBox->currentIndex()==4 )  //collaborative simulation
    {

        ui->localizationInfo_plainTextEdit->setPlainText("");   //reset the plainTextEdit
        ui->localizationInfo_plainTextEdit->appendPlainText("Start localization.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database...");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        int trainingDatasetLength =0;
        trainingData *trainingDataset = new trainingData [40*23];
        //        CLocalization trainingDatasetCreator;   //read database from 0<=X<=39 and 0<=Y<=22
        //        int creatTrainingDatasetSucessFlag = trainingDatasetCreator.creatTrainingDataset(2,trainingDatasetLength,trainingDataset);
        //        if ( creatTrainingDatasetSucessFlag==0 )
        //        {
        //            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
        //            delete []trainingDataset;
        //            return;
        //        }

        //read gprDatabase
        QFile file("gprDatabase.txt"); //Keep in mind that the relative path is the current working directory!!!
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui->localizationInfo_plainTextEdit->appendPlainText("Reading database failed, stop localization.");
            ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
            delete []trainingDataset;
            ui->locate_PushButton->setEnabled(true);
            ui->methodSelection_comboBox->setEnabled(true);
            ui->groundTruthSelection_comboBox->setEnabled(true);
            ui->groundTruthX_lineEdit->setEnabled(true);
            ui->groundTruthY_lineEdit->setEnabled(true);
            return ;
        }
        int linecnt = 0;
        while (!file.atEnd())
        {
            file.readLine();
            linecnt++;
        }
        file.seek(0);
        QString line;
        QStringList list;
        location *locationSet = new location [linecnt];
        linecnt = 0;
        while (!file.atEnd())
        {
            line = file.readLine();
            list = line.split(" ");
            for ( int i=0; i<=4; i++ )
            {
                locationSet[linecnt].priorProbability[i] = 1.0; //initialize prior probability
                locationSet[linecnt].posteriorProbability[i] = 1.0; //set the belief for state(0)
                locationSet[linecnt].mean[i] = list.at(2*i).toDouble();
                locationSet[linecnt].standardDeviation[i] = list.at(2*i+1).toDouble();
            }
            locationSet[linecnt].x = list.at(10).toDouble();
            locationSet[linecnt].y = list.at(11).toDouble();
            linecnt++;
        }
        file.close();

        location estimatedLocation = locationSet[0];

        //unreachable area. set corresponding posterior probability = 0
        for ( int i=0; i<linecnt; i++ )
        {
            if ( (locationSet[i].x>=11)&&(locationSet[i].x<=39)&&(locationSet[i].y>=0)&&(locationSet[i].y<=2.8) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=0)&&(locationSet[i].x<=4)&&(locationSet[i].y>=5)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=9)&&(locationSet[i].x<=11.5)&&(locationSet[i].y>=6)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=11.5)&&(locationSet[i].x<=17.5)&&(locationSet[i].y>=5)&&(locationSet[i].y<=7.3) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=11.5)&&(locationSet[i].x<=14)&&(locationSet[i].y>=7)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=14)&&(locationSet[i].x<=21)&&(locationSet[i].y>=19)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=9)&&(locationSet[i].x<=11.5)&&(locationSet[i].y>=6)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=21)&&(locationSet[i].x<=24)&&(locationSet[i].y>=12)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=24)&&(locationSet[i].x<=27)&&(locationSet[i].y>=5.5)&&(locationSet[i].y<=7.5) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=31)&&(locationSet[i].x<=39)&&(locationSet[i].y>=8)&&(locationSet[i].y<=11) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=24)&&(locationSet[i].x<=36)&&(locationSet[i].y>=15)&&(locationSet[i].y<=23) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
            if ( (locationSet[i].x>=22)&&(locationSet[i].x<=24)&&(locationSet[i].y>=5.5)&&(locationSet[i].y<=7.8) )
            {
                locationSet[i].posteriorProbability[0] = 0.0;
                locationSet[i].posteriorProbability[1] = 0.0;
                locationSet[i].posteriorProbability[2] = 0.0;
                locationSet[i].posteriorProbability[3] = 0.0;
                locationSet[i].posteriorProbability[4] = 0.0;
            }
        }


        // for display
        QGraphicsEllipseItem *locationEllipseItem = new QGraphicsEllipseItem;
        locationEllipseItem->setPen(Qt::NoPen);
        locationEllipseItem->setBrush(QBrush(Qt::blue));
        QGraphicsEllipseItem *locationPossibleEllipseItem = new QGraphicsEllipseItem;
        locationPossibleEllipseItem->setPen(Qt::NoPen);
        locationPossibleEllipseItem->setBrush(QBrush(QColor(118,184,234,80)));
        mapScence->addItem(locationEllipseItem);
        mapScence->addItem(locationPossibleEllipseItem);
        QGraphicsEllipseItem *possiblePositions = new QGraphicsEllipseItem [linecnt]; //use dot to represent the possible location
        for ( int i=0; i<linecnt; i++ )
        {
            mapScence->addItem(possiblePositions+i);
        }

        // for simulation.
        location *locationSetAnother = new location [linecnt];
        QFile filelocationSetAnother("locationSetAnother.txt");
        if (!filelocationSetAnother.open(QIODevice::ReadOnly | QIODevice::Text))
            return ;
        QString lineAnother;
        QStringList listAnother;
        for ( int i=0; i<linecnt; i++ )
        {
            lineAnother = filelocationSetAnother.readLine();
            listAnother = lineAnother.split(" ");
            locationSetAnother[i].x = listAnother.at(0).toDouble(); //initialize prior probability
            locationSetAnother[i].y = listAnother.at(1).toDouble(); //set the belief for state(0)
            locationSetAnother[i].jointPosteriorProbability = listAnother.at(2).toDouble();
        }
        filelocationSetAnother.close();


        qreal maxJointPosteriorProbabilityAnother = 0;
        location maxJointPosteriorProbabilityLocationAnother = locationSetAnother[0];
        for ( int i=0; i<linecnt; i++ )
        {
            if (maxJointPosteriorProbabilityAnother<locationSetAnother[i].jointPosteriorProbability)
            {
                maxJointPosteriorProbabilityAnother = locationSetAnother[i].jointPosteriorProbability;
                maxJointPosteriorProbabilityLocationAnother = locationSetAnother[i];
            }
        }
        qreal minJointPosteriorProbabilityAnother = maxJointPosteriorProbabilityAnother;
        for ( int i=0; i<linecnt; i++ )
        {
            if ((minJointPosteriorProbabilityAnother>locationSetAnother[i].jointPosteriorProbability)&&(locationSetAnother[i].jointPosteriorProbability!=0))
                minJointPosteriorProbabilityAnother = locationSetAnother[i].jointPosteriorProbability;
        }
        QGraphicsEllipseItem *possiblePositionsAnother = new QGraphicsEllipseItem [linecnt]; //use dot to represent the possible location
        for ( int i=0; i<linecnt; i++ )
        {
            mapScence->addItem(possiblePositionsAnother+i);
        }
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));    //this is to generate the following random number
        int randAnother = 0;
        for ( int i=0; i<linecnt; i=i+randAnother )   //radius formula: r=((x-min)/(max-min))*(rmax-rmin)+rmin, here we set the rmax=5, rmin=0
        {
            randAnother = rand()%40;
            qreal radius = ((log(locationSetAnother[i].jointPosteriorProbability)-log(minJointPosteriorProbabilityAnother))/(log(maxJointPosteriorProbabilityAnother)-log(minJointPosteriorProbabilityAnother)))*(8-0)+0;
            if (locationSetAnother[i].jointPosteriorProbability!=0)
            {
                possiblePositionsAnother[i].setRect(15+30*locationSetAnother[i].x-radius/2, 15+30*locationSetAnother[i].y-radius/2, radius, radius);
                possiblePositionsAnother[i].setPen(QPen(Qt::NoPen));
                possiblePositionsAnother[i].setBrush(QBrush(Qt::red));
            }
        }
        QGraphicsRectItem  *groundTruthRectItemAnother = new QGraphicsRectItem;
        groundTruthRectItemAnother->setPen(Qt::NoPen);
        groundTruthRectItemAnother->setBrush(QBrush(Qt::red));  //red
        groundTruthRectItemAnother->setRect(15+30*20-25/2, 15+30*10-25/2, 25, 25);  //x=20,y=10
        mapScence->addItem(groundTruthRectItemAnother);
        qreal startX = 32.0;
        while ( startX > 0 )     //model the collaborative
        {
            for ( int i=0; i<87193; i++ )
            {
                if ( locationSetAnother[i].x ==startX )
                {
                    locationSetAnother[i+70].jointPosteriorProbability = locationSetAnother[i].jointPosteriorProbability;
                }
            }
            startX=startX-0.50;
        }
        startX = 31.9;
        while ( startX > 0 )     //model the collaborative
        {
            for ( int i=0; i<87193; i++ )
            {
                if ( locationSetAnother[i].x ==startX )
                {
                    //qDebug()<<startX;
                    locationSetAnother[i+70].jointPosteriorProbability = locationSetAnother[i].jointPosteriorProbability;
                    //qDebug()<<"yes";
                }
            }
            startX=startX-0.50;
        }
        startX = 31.8;
        while ( startX > 0 )     //model the collaborative
        {
            for ( int i=0; i<87193; i++ )
            {
                if ( locationSetAnother[i].x ==startX )
                {
                    locationSetAnother[i+70].jointPosteriorProbability = locationSetAnother[i].jointPosteriorProbability;
                }
            }
            startX=startX-0.50;
        }
        startX = 31.7;
        while ( startX > 0 )     //model the collaborative
        {
            for ( int i=0; i<87193; i++ )
            {
                if ( locationSetAnother[i].x ==startX )
                {
                    locationSetAnother[i+70].jointPosteriorProbability = locationSetAnother[i].jointPosteriorProbability;
                }
            }
            startX=startX-0.50;
        }
        startX = 31.6;
        while ( startX > 0 )     //model the collaborative
        {
            for ( int i=0; i<87193; i++ )
            {
                if ( locationSetAnother[i].x ==startX )
                {
                    locationSetAnother[i+70].jointPosteriorProbability = locationSetAnother[i].jointPosteriorProbability;
                }
            }
            startX=startX-0.50;
        }
        for ( int i=0; i<linecnt; i++ )
        {
            if ( (locationSetAnother[i].x>=14)&&(locationSetAnother[i].x<=24.5))
                locationSetAnother[i].jointPosteriorProbability = 0;
        }
        for ( int i=0; i<linecnt; i++ )
        {
            if ( (locationSetAnother[i].y>=12)&&(locationSetAnother[i].y<=22))
                locationSetAnother[i].jointPosteriorProbability = 0;
        }
        for ( int i=0; i<linecnt; i++ )
        {
            if ( (locationSetAnother[i].y>=0)&&(locationSetAnother[i].y<=8))
                locationSetAnother[i].jointPosteriorProbability = 0;
        }
        for ( int i=0; i<linecnt; i=i+randAnother )   //radius formula: r=((x-min)/(max-min))*(rmax-rmin)+rmin, here we set the rmax=5, rmin=0
        {
            randAnother = rand()%40;
            qreal radius = ((log(locationSetAnother[i].jointPosteriorProbability)-log(minJointPosteriorProbabilityAnother))/(log(maxJointPosteriorProbabilityAnother)-log(minJointPosteriorProbabilityAnother)))*(8-0)+0;
            if (locationSetAnother[i].jointPosteriorProbability!=0)
            {
                possiblePositionsAnother[i].setRect(15+30*locationSetAnother[i].x-radius/2, 15+30*locationSetAnother[i].y-radius/2, radius, radius);
                possiblePositionsAnother[i].setPen(QPen(Qt::NoPen));
                possiblePositionsAnother[i].setBrush(QBrush(QColor(0,153,68,255)));
            }
        }


        // for WiFi RSSI scan
        CLocalization scanner;
        int numScanned = 0;
        QString scanAddress[5] = {"--","--","--","--","--"};   //These strings are for scanning
        QString scanChannel[5]= {"--","--","--","--","--"};
        QString scanFrequency[5]= {"--","--","--","--","--"};
        QString scanQuality[5]= {"--","--","--","--","--"};
        QString scanRSSI[5]= {"--","--","--","--","--"};
        QString scanEncryption[5]= {"--","--","--","--","--"};
        QString scanESSID[5]= {"--","--","--","--","--"};
        bool stopFlag = false;
        qreal currentFingerprint[5] = {0.0,0.0,0.0,0.0,0.0};
        int count = 0;  //count the following while loop

        //The following code is to check if there is a results record file, if not, build one.
        QFile resultsFile("gprBayesian_results_evaluation.txt");
        if (!resultsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            resultsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream out(&resultsFile);
            out << "";
            out.flush();
            resultsFile.close();
        }
        resultsFile.close();
        resultsFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
        resultsFile.resize(0);

        ui->localizationInfo_plainTextEdit->appendPlainText("Reading database finished.");
        ui->localizationInfo_plainTextEdit->appendPlainText("Start WiFi scanning...\n");
        ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

        // start localization
        while ( !stopFlag )
        {
            if ( ui->locateStop_PushButton->isChecked() ) //break the loop, end the scan
            {
                stopFlag = true;
                ui->locateStop_PushButton->setChecked(false);
                break ;
            }
            MainWindow::sleep();
            scanner.scanWifi( linuxPassword, numScanned, scanAddress, scanChannel,
                              scanFrequency, scanQuality, scanRSSI, scanEncryption, scanESSID);

            //reset the possible positions dot for display
            for ( int i=0; i<linecnt; i++ )
            {
                possiblePositions[i].setRect(QRectF()); //QRectF() construct a null rectangle
            }

            if ( numScanned==5 )
            {
                ui->localizationInfo_plainTextEdit->appendPlainText("Scan Times: "+QString::number(count+1));
                ui->localizationInfo_plainTextEdit->appendPlainText("The current RSSI fingerprint: ");
                ui->localizationInfo_plainTextEdit->insertPlainText(scanRSSI[0]+"  "+scanRSSI[1]+"  "+scanRSSI[2]+"  "+scanRSSI[3]+"  "+scanRSSI[4]+"  ");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                currentFingerprint[0] = scanRSSI[0].toDouble();
                currentFingerprint[1] = scanRSSI[1].toDouble();
                currentFingerprint[2] = scanRSSI[2].toDouble();
                currentFingerprint[3] = scanRSSI[3].toDouble();
                currentFingerprint[4] = scanRSSI[4].toDouble();

                ui->localizationInfo_plainTextEdit->appendPlainText("Estimating the location with Gaussian Process Regression and Bayesian Filter Method.");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                //calculate the posterior probability, this is the heart of the bayesian filter!!!
                for ( int i=0; i<linecnt; i++ )
                {
                    // time update for prediction
                    locationSet[i].priorProbability[0] = locationSet[i].posteriorProbability[0];
                    locationSet[i].priorProbability[1] = locationSet[i].posteriorProbability[1];
                    locationSet[i].priorProbability[2] = locationSet[i].posteriorProbability[2];
                    locationSet[i].priorProbability[3] = locationSet[i].posteriorProbability[3];
                    locationSet[i].priorProbability[4] = locationSet[i].posteriorProbability[4];
                    // measurement update for correct
                    locationSet[i].posteriorProbability[0] = ((exp(-pow((currentFingerprint[0]-locationSet[i].mean[0]),2)/(2*pow(locationSet[i].standardDeviation[0],2))))/(locationSet[i].standardDeviation[0]*sqrt(2*3.1416)))*locationSet[i].priorProbability[0];
                    locationSet[i].posteriorProbability[1] = ((exp(-pow((currentFingerprint[1]-locationSet[i].mean[1]),2)/(2*pow(locationSet[i].standardDeviation[1],2))))/(locationSet[i].standardDeviation[1]*sqrt(2*3.1416)))*locationSet[i].priorProbability[1];
                    locationSet[i].posteriorProbability[2] = ((exp(-pow((currentFingerprint[2]-locationSet[i].mean[2]),2)/(2*pow(locationSet[i].standardDeviation[2],2))))/(locationSet[i].standardDeviation[2]*sqrt(2*3.1416)))*locationSet[i].priorProbability[2];
                    locationSet[i].posteriorProbability[3] = ((exp(-pow((currentFingerprint[3]-locationSet[i].mean[3]),2)/(2*pow(locationSet[i].standardDeviation[3],2))))/(locationSet[i].standardDeviation[3]*sqrt(2*3.1416)))*locationSet[i].priorProbability[3];
                    locationSet[i].posteriorProbability[4] = ((exp(-pow((currentFingerprint[4]-locationSet[i].mean[4]),2)/(2*pow(locationSet[i].standardDeviation[4],2))))/(locationSet[i].standardDeviation[4]*sqrt(2*3.1416)))*locationSet[i].priorProbability[4];
                    locationSet[i].jointPosteriorProbability = locationSet[i].posteriorProbability[0]*locationSet[i].posteriorProbability[1]*locationSet[i].posteriorProbability[2]*locationSet[i].posteriorProbability[3]*locationSet[i].posteriorProbability[4]*locationSetAnother[i].jointPosteriorProbability;
                }
                //the jointPosteriorProbability should be normalized!!
                qreal sumJointPosteriorProbability = 0;
                for ( int i=0; i<linecnt; i++ )
                {
                    sumJointPosteriorProbability = sumJointPosteriorProbability + locationSet[i].jointPosteriorProbability;
                }
                qreal eta = 1/sumJointPosteriorProbability;  //normalization factor
                for ( int i=0; i<linecnt; i++ )
                {
                    locationSet[i].jointPosteriorProbability = eta*(locationSet[i].jointPosteriorProbability);
                }
                qDebug()<<eta;


                //find the Maximum A Posterior location
                qreal maxJointPosteriorProbability = 0;
                location maxJointPosteriorProbabilityLocation = locationSet[0];
                for ( int i=0; i<linecnt; i++ )
                {
                    if (maxJointPosteriorProbability<locationSet[i].jointPosteriorProbability)
                    {
                        maxJointPosteriorProbability = locationSet[i].jointPosteriorProbability;
                        maxJointPosteriorProbabilityLocation = locationSet[i];
                    }
                }
                qreal minJointPosteriorProbability = maxJointPosteriorProbability;
                for ( int i=0; i<linecnt; i++ )
                {
                    if ((minJointPosteriorProbability>locationSet[i].jointPosteriorProbability)&&(locationSet[i].jointPosteriorProbability!=0))
                        minJointPosteriorProbability = locationSet[i].jointPosteriorProbability;
                }

                qDebug()<<"maxJointPosteriorProbability: "<< maxJointPosteriorProbability;
                qDebug()<<"minJointPosteriorProbability: "<< minJointPosteriorProbability;

                if ( maxJointPosteriorProbability>minJointPosteriorProbability  )
                    estimatedLocation = maxJointPosteriorProbabilityLocation;
                ui->localizationInfo_plainTextEdit->appendPlainText("The estimated location is ( " + QString::number(estimatedLocation.x) + ", " + QString::number(estimatedLocation.y) + " ).");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time
                locationEllipseItem->setRect(15+30*estimatedLocation.x-10, 15+30*estimatedLocation.y-10, 20, 20);

                //find the 0.8 possible maxJointPosterior location
                location possibleEstimatedLocation = locationSet[0];
                qreal tempDistance = 1;
                for ( int i=0; i<linecnt; i++ )
                {
                    if ((fabs(0.8*maxJointPosteriorProbability-locationSet[i].jointPosteriorProbability)<tempDistance)&&(locationSet[i].jointPosteriorProbability!=0))
                    {
                        tempDistance = fabs(0.8*maxJointPosteriorProbability-locationSet[i].jointPosteriorProbability);
                        possibleEstimatedLocation = locationSet[i];
                    }
                }
                if ( !(maxJointPosteriorProbability>minJointPosteriorProbability)  )
                    possibleEstimatedLocation = estimatedLocation;
                qreal locationPossibleRadius = sqrt(pow(estimatedLocation.x-possibleEstimatedLocation.x,2)+pow(estimatedLocation.y-possibleEstimatedLocation.y,2));  //error*30 is the coordinate in px

                locationPossibleEllipseItem->setRect(15+30*estimatedLocation.x-locationPossibleRadius*30, 15+30*estimatedLocation.y-locationPossibleRadius*30, 2*locationPossibleRadius*30,2*locationPossibleRadius*30);

                //display possible position using random dots
                qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));    //this is to generate the following random number
                int rand = 0;
                for ( int i=0; i<linecnt; i=i+rand )   //radius formula: r=((x-min)/(max-min))*(rmax-rmin)+rmin, here we set the rmax=5, rmin=0
                {
                    rand = qrand()%40;
                    qreal radius = ((log(locationSet[i].jointPosteriorProbability)-log(minJointPosteriorProbability))/(log(maxJointPosteriorProbability)-log(minJointPosteriorProbability)))*(8-0)+0;
                    if (locationSet[i].jointPosteriorProbability!=0)
                    {
                        possiblePositions[i].setRect(15+30*locationSet[i].x-radius/2, 15+30*locationSet[i].y-radius/2, radius, radius);
                        possiblePositions[i].setPen(QPen(Qt::NoPen));
                        possiblePositions[i].setBrush(QBrush(Qt::black));
                    }
                }

                double time_Start = (double)clock();
                double time_End = (double)clock();
                qDebug()<<(time_End - time_Start)/1000.0<<"s";

                qreal locationError = sqrt( pow((estimatedLocation.x-ui->groundTruthX_lineEdit->text().toDouble()),2) + pow((estimatedLocation.y-ui->groundTruthY_lineEdit->text().toDouble()),2) );  //error*30 is the coordinate in px
                ui->localizationInfo_plainTextEdit->appendPlainText("The location error is " + QString::number(locationError,'f',2) + ".\n");
                ui->localizationInfo_plainTextEdit->repaint();  //make the plainTextEdit display text real time

                // dispaly information in lineEdits
                ui->locationX_lineEdit->setText(QString::number(estimatedLocation.x,'f',1));
                ui->locationY_lineEdit->setText(QString::number(estimatedLocation.y,'f',1));
                ui->locationError_lineEdit->setText(QString::number(locationError,'f',2));
                ui->locationPossible_lineEdit->setText(QString::number(locationPossibleRadius,'f',1));

                //The following code is for results recording.
                QTextStream out(&resultsFile);
                out << ui->groundTruthX_lineEdit->text().toDouble() << " " << ui->groundTruthY_lineEdit->text().toDouble() << " " << estimatedLocation.x << " " << estimatedLocation.y << " " << QString::number(locationError,'f',2) << endl;
                out.flush();

                count++;

                if (count==localizationTimes)
                    break;
            }
            numScanned = 0; //reset numScanned
        }

        resultsFile.close();

        ui->localizationInfo_plainTextEdit->appendPlainText("Stop localization.");

        delete []trainingDataset;
        delete locationEllipseItem;
        delete locationPossibleEllipseItem;
        delete []possiblePositions;
        delete []locationSet;
    }

    ui->locateStop_PushButton->setChecked(false);
    ui->locate_PushButton->setEnabled(true);
    ui->methodSelection_comboBox->setEnabled(true);
    ui->groundTruthSelection_comboBox->setEnabled(true);
    ui->groundTruthX_lineEdit->setEnabled(true);
    ui->groundTruthY_lineEdit->setEnabled(true);
    return;
}


void MainWindow::on_methodSettings_pushButton_clicked()
{
    QDialog *methodSettingsDlg = new QDialog(this);
    QLabel *methodSettingsDlgLabel = new QLabel(methodSettingsDlg);
    QSpinBox *methodSettingsDlgKSpinBox = new QSpinBox(methodSettingsDlg);
    QSpinBox *methodSettingsDlgVariationThresholdSpinBox = new QSpinBox(methodSettingsDlg);
    QSpinBox *methodSettingsDlglocalizationTimesSpinBox = new QSpinBox(methodSettingsDlg);
    QPushButton *methodSettingsDlgPushButton = new QPushButton(methodSettingsDlg);

    methodSettingsDlg->resize(260,130);
    methodSettingsDlg->setWindowTitle("Parameters Settings...");
    methodSettingsDlgLabel->setText("Please input the value of \"k\",\"Variation \nThreshold\", \"localization times\".");
    methodSettingsDlgLabel->move(12,15);

    methodSettingsDlgKSpinBox->move(25,50);
    methodSettingsDlgKSpinBox->resize(60,22);
    methodSettingsDlgKSpinBox->setRange(1,1000);
    methodSettingsDlgKSpinBox->setValue(k_knn);

    methodSettingsDlgVariationThresholdSpinBox->move(25,90);
    methodSettingsDlgVariationThresholdSpinBox->resize(60,22);
    methodSettingsDlgVariationThresholdSpinBox->setRange(0,10000);
    methodSettingsDlgVariationThresholdSpinBox->setValue(variationThreshold_eknn);

    methodSettingsDlglocalizationTimesSpinBox->move(97,50);
    methodSettingsDlglocalizationTimesSpinBox->resize(60,22);
    methodSettingsDlglocalizationTimesSpinBox->setRange(1,10000);
    methodSettingsDlglocalizationTimesSpinBox->setValue(localizationTimes);

    methodSettingsDlgPushButton->move(97,90);
    methodSettingsDlgPushButton->resize(60,22);
    methodSettingsDlgPushButton->setText("OK");

    connect(methodSettingsDlgPushButton,SIGNAL(clicked()),methodSettingsDlg,SLOT(accept()));
    if (methodSettingsDlg->exec() == QDialog::Accepted)   //use modal dialog to relize the data transfer
    {
        k_knn = methodSettingsDlgKSpinBox->value();
        variationThreshold_eknn = methodSettingsDlgVariationThresholdSpinBox->value();
        localizationTimes = methodSettingsDlglocalizationTimesSpinBox->value();
    }
    delete methodSettingsDlg;
}
