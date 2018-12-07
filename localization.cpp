// Copyright (C) 2018 Yuxiang Sun
// Email: yxsun@link.cuhk.edu.hk
// MIT License

#include "localization.h"
#include <QApplication>
#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QtXml>

CLocalization::CLocalization()
{  }

int CLocalization::scanWifi( QString linuxPassword, int &numScanned,
                             QString *scanAddress, QString *scanChannel, QString *scanFrequency,
                             QString *scanQuality, QString *scanRSSI, QString *scanEncryption, QString *scanESSID )
{
    numScanned = 0; // reset numScanned
    int scannedFlag[5] = {0,0,0,0,0};   // reset the flags

    QString linuxCommand = "echo " + linuxPassword + " | sudo -S iwlist scanning | grep -B5 Loc > scanning";    // note that system() function can only support constant *char, so you need to transfer QString to constant *char
    // system( linuxCommand.toLatin1().data() );   // generate a scanning file in the current working directory

    QFile file("scanning"); // open the scanning file.
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;   // the scanning file does not exist
    QTextStream in(&file);
    QString scanString;
    int numLine = 0;
    while(!in.atEnd())
    {
        scanString = in.readLine();
        numLine++;
    }   // count the number of the lines
    in.seek(0); // make the cursor back to the start, otherwise, readAll() will return an empty string
    scanString = in.readAll();  // read scanning file to string "scanstrall"
    //qDebug() << scanString;
    //qDebug() << "numLine:" << numLine;
    if ( numLine == 0 ) // if numLine == 34, all the 5 APs are sanned
        numScanned = 0;
    else if ( numLine == 6 )
        numScanned = 1;
    else if ( numLine == 13 )
        numScanned = 2;
    else if ( numLine == 20 )
        numScanned = 3;
    else if ( numLine == 27 )
        numScanned = 4;
    else if ( numLine == 34 )
        numScanned = 5;
    QStringList strlist = scanString.split("--");   //  split the string scanstrall
    for( int i=0; i<strlist.count(); i++ )
    {
        QRegExp Address_re("\\bAddress\\b");
        QRegExp Channel_re("\\bChannel\\b");
        QRegExp Frequency_re("\\bFrequency\\b");
        QRegExp Quality_re("\\bQuality\\b");
        QRegExp RSSI_re("\\bSignal\\slevel\\b");
        QRegExp Encryption_re("\\bEncryption\\b");
        QRegExp ESSID_re("\\bESSID\\b");

        int Address_pos = 0, Channel_pos = 0, Frequency_pos = 0, Quality_pos = 0, RSSI_pos = 0, Encryption_pos = 0, ESSID_pos = 0;

        Address_pos = Address_re.indexIn(strlist.at(i));
        Channel_pos = Channel_re.indexIn(strlist.at(i));
        Frequency_pos = Frequency_re.indexIn(strlist.at(i));
        Quality_pos = Quality_re.indexIn(strlist.at(i));
        RSSI_pos = RSSI_re.indexIn(strlist.at(i));
        Encryption_pos = Encryption_re.indexIn(strlist.at(i));
        ESSID_pos = ESSID_re.indexIn(strlist.at(i));

        if(strlist.at(i).mid(Address_pos+9, 17) == "64:66:B3:BC:51:8E") // AP0
        {   scanAddress[0] = strlist.at(i).mid(Address_pos+9, 17).trimmed();
            scanChannel[0] = strlist.at(i).mid(Channel_pos+8, 2).trimmed();
            scanFrequency[0] = strlist.at(i).mid(Frequency_pos+10, 5).trimmed();
            scanQuality[0] = strlist.at(i).mid(Quality_pos+8, 5).trimmed();
            scanRSSI[0] = strlist.at(i).mid(RSSI_pos+13, 3).trimmed();
            scanEncryption[0] = strlist.at(i).mid(Encryption_pos+15, 3).trimmed();
            scanESSID[0] = strlist.at(i).mid(ESSID_pos+7, 4).trimmed();
            scannedFlag[0] = 1;
        }
        else if(strlist.at(i).mid(Address_pos+9, 17) == "64:66:B3:BC:4E:F2")    // AP1
        {   scanAddress[1] = strlist.at(i).mid(Address_pos+9, 17).trimmed();
            scanChannel[1] = strlist.at(i).mid(Channel_pos+8, 2).trimmed();
            scanFrequency[1] = strlist.at(i).mid(Frequency_pos+10, 5).trimmed();
            scanQuality[1] = strlist.at(i).mid(Quality_pos+8, 5).trimmed();
            scanRSSI[1] = strlist.at(i).mid(RSSI_pos+13, 3).trimmed();
            scanEncryption[1] = strlist.at(i).mid(Encryption_pos+15, 3).trimmed();
            scanESSID[1] = strlist.at(i).mid(ESSID_pos+7, 4).trimmed();
            scannedFlag[1] = 1;
        }
        else if (strlist.at(i).mid(Address_pos+9, 17) == "64:66:B3:8F:01:74")   // AP2
        {   scanAddress[2] = strlist.at(i).mid(Address_pos+9, 17).trimmed();
            scanChannel[2] = strlist.at(i).mid(Channel_pos+8, 2).trimmed();
            scanFrequency[2] = strlist.at(i).mid(Frequency_pos+10, 5).trimmed();
            scanQuality[2] = strlist.at(i).mid(Quality_pos+8, 5).trimmed();
            scanRSSI[2] = strlist.at(i).mid(RSSI_pos+13, 3).trimmed();
            scanEncryption[2] = strlist.at(i).mid(Encryption_pos+15, 3).trimmed();
            scanESSID[2] = strlist.at(i).mid(ESSID_pos+7, 4).trimmed();
            scannedFlag[2] = 1;
        }
        else if (strlist.at(i).mid(Address_pos+9, 17) == "64:66:B3:8F:02:4A")   // AP3
        {   scanAddress[3] = strlist.at(i).mid(Address_pos+9, 17).trimmed();
            scanChannel[3] = strlist.at(i).mid(Channel_pos+8, 2).trimmed();
            scanFrequency[3] = strlist.at(i).mid(Frequency_pos+10, 5).trimmed();
            scanQuality[3] = strlist.at(i).mid(Quality_pos+8, 5).trimmed();
            scanRSSI[3] = strlist.at(i).mid(RSSI_pos+13, 3).trimmed();
            scanEncryption[3] = strlist.at(i).mid(Encryption_pos+15, 3).trimmed();
            scanESSID[3] = strlist.at(i).mid(ESSID_pos+7, 4).trimmed();
            scannedFlag[3] = 1;
        }
        else if (strlist.at(i).mid(Address_pos+9, 17) == "64:66:B3:8F:18:2A")   // AP4
        {   scanAddress[4] = strlist.at(i).mid(Address_pos+9, 17).trimmed();
            scanChannel[4] = strlist.at(i).mid(Channel_pos+8, 2).trimmed();
            scanFrequency[4] = strlist.at(i).mid(Frequency_pos+10, 5).trimmed();
            scanQuality[4] = strlist.at(i).mid(Quality_pos+8, 5).trimmed();
            scanRSSI[4] = strlist.at(i).mid(RSSI_pos+13, 3).trimmed();
            scanEncryption[4] = strlist.at(i).mid(Encryption_pos+15, 3).trimmed();
            scanESSID[4] = strlist.at(i).mid(ESSID_pos+7, 4).trimmed();
            scannedFlag[4] = 1;
        }
    }
    for ( int i=0; i<=4; i++ )
    {
        if (scannedFlag[i]==0)
        {
            scanAddress[i] = "--";
            scanChannel[i] = "--";
            scanFrequency[i] = "--";
            scanQuality[i] = "--";
            scanRSSI[i] = "--";
            scanEncryption[i] = "--";
            scanESSID[i] = "--";
        }
        scannedFlag[i]=0; // reset the flags
    }
    //qDebug() << "numScanned is " << numScanned;
    file.close();
    return 1;
}

int CLocalization::writeDatabase( int &dbX, int &dbY, int &dbscanTimes,
                                  QString &dbstartMeasurementTime, QString &dbendMeasurementTime,
                                  QString (*dbAddress)[5], QString (*dbChannel)[5], QString (*dbFrequency)[5],
                                  QString (*dbQuality)[5], QString (*dbRSSI)[5], QString (*dbEncryption)[5], QString (*dbESSID)[5] )
{
    int successFlag = 1;    // 0: fail, 1: success

    QFile file("database.xml"); // check if there is a XML file in the current working directory, if not, create one, the relative path is the current working directory
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  // make sure there is a XML file before writting
    {
        QDomDocument doc;
        QDomProcessingInstruction instruction;
        instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(instruction);
        QDomElement root = doc.createElement(QString("DATABASE"));
        doc.appendChild(root);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            successFlag = 0;
            return successFlag;
        }
        QTextStream out(&file);
        doc.save(out, 4);
        file.close();
    }
    file.close();

    QDomDocument doc;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        successFlag = 0;
        return successFlag;
    }
    if (!doc.setContent(&file)) // first read all into RAM, then modify it in RAM and write the RAM data to file
    {
        successFlag = 0;
        return successFlag;
    }
    file.close();
    QDomNode root = doc.documentElement();   // get the root element of this XML file, that is DATABASE
    QDomNodeList list = doc.elementsByTagName("MEASUREMENT");
    for( int i=0; i<list.count(); i++ ) // first check the old data for the given location and deleting the old data
    {
        if( (list.at(i).toElement().attribute("x").toInt()==dbX) && (list.at(i).toElement().attribute("y").toInt()==dbY) )
        {
            root.removeChild(list.at(i));   // list.at(i) is a child of the root element
        }
    }
    QDomElement measurementnode = doc.createElement("MEASUREMENT"); // after delete the old data, append the new data for the given location
    measurementnode.setAttribute("x",dbX);  // the followings are to construct the new MEASUREMENT node
    measurementnode.setAttribute("y",dbY);
    measurementnode.setAttribute("scantimes",dbscanTimes);
    measurementnode.setAttribute("starttime",dbstartMeasurementTime);
    measurementnode.setAttribute("endtime",dbendMeasurementTime);
    for ( int i=0; i<dbscanTimes; i++ ) // construct the SCAN node
    {
        QDomElement scannode = doc.createElement("SCAN");
        scannode.setAttribute("id",i);
        for ( int j=0; j<5; j++ )   // construct the AP node
        {
            QDomElement apnode = doc.createElement("AP");
            apnode.setAttribute("id",j);

            QDomText text;
            QDomElement essidnode = doc.createElement("ESSID");
            text = doc.createTextNode(dbESSID[i][j]);
            essidnode.appendChild(text);
            apnode.appendChild(essidnode);
            QDomElement channelnode = doc.createElement("CHANNEL");
            text = doc.createTextNode(dbChannel[i][j]);
            channelnode.appendChild(text);
            apnode.appendChild(channelnode);
            QDomElement frequencynode = doc.createElement("FREQUENCY");
            text = doc.createTextNode(dbFrequency[i][j]);
            frequencynode.appendChild(text);
            apnode.appendChild(frequencynode);
            QDomElement rssinode = doc.createElement("RSSI");
            text = doc.createTextNode(dbRSSI[i][j]);
            rssinode.appendChild(text);
            apnode.appendChild(rssinode);
            QDomElement qualitynode = doc.createElement("QUALITY");
            text = doc.createTextNode(dbQuality[i][j]);
            qualitynode.appendChild(text);
            apnode.appendChild(qualitynode);
            QDomElement encryptionnode = doc.createElement("ENCRYPTION");
            text = doc.createTextNode(dbEncryption[i][j]);
            encryptionnode.appendChild(text);
            apnode.appendChild(encryptionnode);
            QDomElement addressnode = doc.createElement("ADDRESS");
            text = doc.createTextNode(dbAddress[i][j]);
            addressnode.appendChild(text);
            apnode.appendChild(addressnode);

            scannode.appendChild(apnode);   // append each AP node to the current SCAN node
        }
        measurementnode.appendChild(scannode);
    }
    root.appendChild(measurementnode);  // we have constructed the new MEASUREMENT node in RAM, now append this MEASUREMENT node to the root node
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))  // save the MEASUREMENT node of RAM into file
    {
        successFlag = 0;
        return successFlag;
    }
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
    return successFlag;
}

int CLocalization::readDatabase( int &dbX, int &dbY, int &dbscanTimes,
                                 QString &dbstartMeasurementTime, QString &dbendMeasurementTime,
                                 QString (*dbAddress)[5], QString (*dbChannel)[5], QString (*dbFrequency)[5],
                                 QString (*dbQuality)[5], QString (*dbRSSI)[5], QString (*dbEncryption)[5], QString (*dbESSID)[5] )
{
    int successFlag = 1;    // successFlag=0 means open XML file failing, successFlag=1 means can not find the data, successFlag=2 means everything is OK
    QDomDocument doc;
    QFile file("database.xml"); // the relative path is the current working directory
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        successFlag = 0;
        return successFlag;
    }
    if (!doc.setContent(&file))
    {
        successFlag = 0;
        return successFlag;
    }
    file.close();

    QDomNode node = doc.documentElement();  //get the entrance for this XML, here the node is "DATABASE"
    node = node.firstChild();   //now the node is the first "MEASUREMENT"

    while (!node.isNull())
    {
        if ( (node.toElement().attribute("x").toInt() == dbX) && (node.toElement().attribute("y").toInt() == dbY) ) //find the MEASUREMENT corresponding to the given location
        {
            dbscanTimes = node.toElement().attribute("scantimes").toInt();
            dbstartMeasurementTime = node.toElement().attribute("starttime");
            dbendMeasurementTime = node.toElement().attribute("endtime");

            QDomNodeList scannodes = node.childNodes(); // node is the MEASUREMENT, its childnodes are the scannodes
            for( int i=0; i<scannodes.count(); i++ )    // scannodes.count() is the number of the scanTimes
            {
                QDomNodeList apnodes = scannodes.at(i).childNodes(); // scannodes.at(i) is the ith scan, and its childnodes are the 5 AP
                for ( int j=0; j<apnodes.count(); j++ ) // j can be 0,1,2,3,4 which stands for the 5 AP
                {
                    QDomNode parameternode = apnodes.at(j).firstChild();    // parameternode is the nodes such as "ESSID", "ADDRESS", "RSSI"...
                    while ( !parameternode.isNull() )
                    {
                        if ( parameternode.nodeName()=="ESSID" )
                            dbESSID[i][j] = parameternode.toElement().text();   // i means the ith scan, j means the jth AP
                        else if ( parameternode.nodeName()=="CHANNEL" )
                            dbChannel[i][j] = parameternode.toElement().text();
                        else if ( parameternode.nodeName()=="FREQUENCY" )
                            dbFrequency[i][j] = parameternode.toElement().text();
                        else if ( parameternode.nodeName()=="RSSI" )
                            dbRSSI[i][j] = parameternode.toElement().text();
                        else if ( parameternode.nodeName()=="QUALITY" )
                            dbQuality[i][j] = parameternode.toElement().text();
                        else if ( parameternode.nodeName()=="ENCRYPTION" )
                            dbEncryption[i][j] = parameternode.toElement().text();
                        else if ( parameternode.nodeName()=="ADDRESS" )
                            dbAddress[i][j] = parameternode.toElement().text();

                        parameternode = parameternode.nextSibling();
                    }
                }
            }
            successFlag = 2;
            break ;
        }
        node = node.nextSibling();  // step to the next MEASUREMENT node
    }
    return successFlag;
}

int CLocalization::creatTrainingDataset( int datasetType, int &trainingDatasetLength, trainingData *trainingDataset )
{
    // datasetType = 0 output raw dataset; datasetType = 1 output mean dataset; datasetType = 2 output mode dataset

    trainingDatasetLength = 0;
    int successFlag = 1;
    QDomDocument doc;

    QFile file("database.xml"); // the relative path is the current working directory
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        successFlag = 0;
        return successFlag;
    }
    if (!doc.setContent(&file))
    {
        successFlag = 0;
        return successFlag;
    }
    file.close();
    QDomNode node = doc.documentElement();  // get the entrance for this XML, here the node is "DATABASE"
    node = node.firstChild();   // now the node is the first "MEASUREMENT"

    int foundFlag=0; // foundFlag==0 means can not find the corresponding data
    int dbX, dbY, dbscanTimes;
    QString dbRSSI[10000][5];

    for ( dbY = 0; dbY<=22; dbY++ ) // read database from 0<=X<=39 and 0<=Y<=22
    {
        for ( dbX = 0; dbX<=39; dbX++ )
        {
            while ( !node.isNull() )
            {
                if ( (node.toElement().attribute("x").toInt() == dbX) && (node.toElement().attribute("y").toInt() == dbY) ) // find the MEASUREMENT corresponding to the given location
                {
                    dbscanTimes = node.toElement().attribute("scantimes").toInt();
                    QDomNodeList scannodes = node.childNodes(); // node is the MEASUREMENT, its childnodes are the scannodes
                    for( int i=0; i<scannodes.count(); i++ )    // scannodes.count() is the number of the scanTimes
                    {
                        QDomNodeList apnodes = scannodes.at(i).childNodes(); // scannodes.at(i) is the ith scan, and its childnodes are the 5 AP
                        for ( int j=0; j<apnodes.count(); j++ ) // j can be 0,1,2,3,4 which stands for the 5 AP
                        {
                            QDomNode parameternode = apnodes.at(j).firstChild();    // parameternode is the nodes such as "ESSID", "ADDRESS", "RSSI"...
                            while ( !parameternode.isNull() )
                            {
                                if ( parameternode.nodeName()=="RSSI" )
                                    dbRSSI[i][j] = parameternode.toElement().text();  // i means the ith scan, j means the jth AP
                                parameternode = parameternode.nextSibling();
                            }
                        }
                    }
                    foundFlag = 1;
                    break;
                }
                node = node.nextSibling();  // step to the next MEASUREMENT node
            }
            if ( foundFlag == 1 )
            {
                if ( datasetType==0 )   // output the raw data
                {
                    for ( int i=0; i<dbscanTimes; i++ )
                    {
                        for ( int j=0; j<=4; j++ )
                        {
                            trainingDataset[trainingDatasetLength+i].fingerprint[j] = dbRSSI[i][j].toDouble();
                        }
                        trainingDataset[trainingDatasetLength+i].x  = dbX;
                        trainingDataset[trainingDatasetLength+i].y  = dbY;
                    }
                    trainingDatasetLength = trainingDatasetLength + dbscanTimes;
                    foundFlag = 0;
                }
                if ( datasetType==1 )  //output the mean data
                {
                    qreal sumRSSI[5] = {0.0,0.0,0.0,0.0,0.0};
                    for ( int i=0; i<dbscanTimes; i++ )
                    {
                        for ( int j=0; j<=4; j++ )
                        {
                            sumRSSI[j] = sumRSSI[j] + dbRSSI[i][j].toDouble();
                        }
                    }
                    trainingDataset[trainingDatasetLength].fingerprint[0] = sumRSSI[0]/dbscanTimes;
                    trainingDataset[trainingDatasetLength].fingerprint[1] = sumRSSI[1]/dbscanTimes;
                    trainingDataset[trainingDatasetLength].fingerprint[2] = sumRSSI[2]/dbscanTimes;
                    trainingDataset[trainingDatasetLength].fingerprint[3] = sumRSSI[3]/dbscanTimes;
                    trainingDataset[trainingDatasetLength].fingerprint[4] = sumRSSI[4]/dbscanTimes;
                    trainingDataset[trainingDatasetLength].x  = dbX;
                    trainingDataset[trainingDatasetLength].y  = dbY;
                    trainingDatasetLength ++;
                    foundFlag = 0;
                }
                if ( datasetType==2 )  //output the mode data
                {
                    qreal *tempRSSI0 = new qreal [dbscanTimes];
                    qreal *tempRSSI1 = new qreal [dbscanTimes];
                    qreal *tempRSSI2 = new qreal [dbscanTimes];
                    qreal *tempRSSI3 = new qreal [dbscanTimes];
                    qreal *tempRSSI4 = new qreal [dbscanTimes];
                    for ( int i=0; i<dbscanTimes; i++ )
                    {
                         tempRSSI0[i] = dbRSSI[i][0].toDouble();
                         tempRSSI1[i] = dbRSSI[i][1].toDouble();
                         tempRSSI2[i] = dbRSSI[i][2].toDouble();
                         tempRSSI3[i] = dbRSSI[i][3].toDouble();
                         tempRSSI4[i] = dbRSSI[i][4].toDouble();
                    }
                    trainingDataset[trainingDatasetLength].fingerprint[0] = CLocalization::mode(tempRSSI0,dbscanTimes);
                    trainingDataset[trainingDatasetLength].fingerprint[1] = CLocalization::mode(tempRSSI1,dbscanTimes);
                    trainingDataset[trainingDatasetLength].fingerprint[2] = CLocalization::mode(tempRSSI2,dbscanTimes);
                    trainingDataset[trainingDatasetLength].fingerprint[3] = CLocalization::mode(tempRSSI3,dbscanTimes);
                    trainingDataset[trainingDatasetLength].fingerprint[4] = CLocalization::mode(tempRSSI4,dbscanTimes);
                    trainingDataset[trainingDatasetLength].x  = dbX;
                    trainingDataset[trainingDatasetLength].y  = dbY;
                    trainingDatasetLength ++;
                    delete []tempRSSI0;
                    delete []tempRSSI1;
                    delete []tempRSSI2;
                    delete []tempRSSI3;
                    delete []tempRSSI4;
                    foundFlag = 0;
                }
            }
            node = doc.documentElement().firstChild(); // set the node back to the first "MEASUREMENT"
        }
    }
    return successFlag;
}


int CLocalization::creatSparseDatabase()
{
    int successFlag = 1;    // successFlag=0 means fail, successFlag=1 means success
    QFile file("database.xml"); // check if there is the XML file in the current working directory, if not, create one, the relative path is the current working directory
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        successFlag = 0;
        return successFlag;
    }
    if (!doc.setContent(&file)) // first read all the content into RAM, then modify it in RAM, then write the RAM data to file
    {
        successFlag = 0;
        return successFlag;
    }
    file.resize(0);
    file.close();
    QDomNode root = doc.documentElement();   // get the root element of this XML file, that is DATABASE
    QDomNodeList list = doc.elementsByTagName("MEASUREMENT");
    for( int i=0; i<list.count(); i=i+2 )
    {
        root.removeChild(list.at(i));   // list.at(i) is a child of the root element
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))  // save the MEASUREMENT node of RAM into file
    {
        successFlag = 0;
        return successFlag;
    }
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
    return successFlag;
}

qreal CLocalization::mode( qreal a[],int size )
{
   int *frequency = new int [size];
   for(int i=0;i<size;i++)
   {
      frequency[i] = 0;
   }
   for(int i=0;i<size;i++)
   {
      for(int j=0;j<size;j++)
      {
          if(a[i]==a[j])
          frequency[i]++;
      }
   }
   qreal temp = a[0];
   qreal max = frequency[0];
   for(int i=0;i<size;i++)
   {
      if(frequency[i]>max)
      {
          max = frequency[i];
          temp = a[i];
      }
   }
   delete []frequency;
   return temp;
}

void CLocalization::sort( int trainingDatasetLength, trainingData *trainingDataset )
{
    trainingData temp;
    for( int i=0; i<=trainingDatasetLength-2; i++ )
    {
        for( int j=i+1; j<=trainingDatasetLength-1; j++)
        {
            if( trainingDataset[j].euclideanDistance<trainingDataset[i].euclideanDistance )
            {
                temp = trainingDataset[i];
                trainingDataset[i] = trainingDataset[j];
                trainingDataset[j] = temp;
            }
        }
    }

}

void CLocalization::eknnClassifier( int trainingDatasetLength, trainingData *trainingDataset, int modeTrainingDatasetLength, trainingData *modeTrainingDataset, int k, int variationThreshold, qreal *currentFingerprint, location &estimatedLocation, location *locationSet )
{
//    qDebug()<<trainingDatasetLength;
//    for ( int i=0; i<trainingDatasetLength; i++ )
//    {
//        qDebug()<<trainingDataset[i].fingerprint[0]<<" "<<trainingDataset[i].fingerprint[1]<<" "<<trainingDataset[i].fingerprint[2]<<" "<<trainingDataset[i].fingerprint[3]<<" "<<trainingDataset[i].fingerprint[4]<<" "<<trainingDataset[i].x<<" "<<trainingDataset[i].y<<" "<<trainingDataset[i].euclideanDistance;
//    }

    // First step: caculate euclidean distance for each training data

    for ( int i=0; i<trainingDatasetLength; i++ )  //reset the euclideanDistance
    {
        trainingDataset[i].euclideanDistance = 0;
    }

    for ( int i=0; i<trainingDatasetLength; i++ )
    {
        for ( int j=0; j<=4; j++ )
        {
            trainingDataset[i].euclideanDistance = trainingDataset[i].euclideanDistance + pow((currentFingerprint[j]-trainingDataset[i].fingerprint[j]), 2);
        }
        trainingDataset[i].euclideanDistance = sqrt(trainingDataset[i].euclideanDistance);
    }

    // Second step: sort the trainning dataset by the euclidean distance in ascending order

    trainingData temp;
    for( int i=0; i<=trainingDatasetLength-2; i++ )
    {
        for( int j=i+1; j<=trainingDatasetLength-1; j++)
        {
            if( trainingDataset[j].euclideanDistance<trainingDataset[i].euclideanDistance )
            {
                temp = trainingDataset[i];
                trainingDataset[i] = trainingDataset[j];
                trainingDataset[j] = temp;
            }
        }
    }

    // Third step: find the corresponding vote for the first k training data

    for ( int j = 0; j<=22; j++ )   //initialize the locationSet
    {
        for ( int i = 0; i<=39; i++ )
        {
            locationSet[40*j+i].x = i;
            locationSet[40*j+i].y = j;
            locationSet[40*j+i].vote = 0;
        }
    }
    for ( int j=0; j<23*40; j++ )
    {
        for ( int i=0; i<k; i++ )
        {
            if ( (locationSet[j].x==trainingDataset[i].x) && (locationSet[j].y==trainingDataset[i].y ) )
            {
                for ( int h=0; h<modeTrainingDatasetLength; h++ )   // check if the variation of the trainingDataset[i] is too large with the modeTrainingDataset[h]
                {
                    if ( (trainingDataset[i].x==modeTrainingDataset[h].x)&&(trainingDataset[i].y==modeTrainingDataset[h].y) )
                    {
                        int sumVariation=0;
                        for ( int t=0; t<=4; t++ )
                        {
                            sumVariation = sumVariation + abs(trainingDataset[i].fingerprint[t]-modeTrainingDataset[h].fingerprint[t]);
                        }
                        if ( sumVariation<=variationThreshold )
                            locationSet[j].vote++;
                    }
                }
            }
        }
    }

    // Fourth step: find the maximum vote location

    location maxVoteLocation = locationSet[0];
    for ( int i=0; i<23*40; i++ )
    {
        if ( locationSet[i].vote>maxVoteLocation.vote )
            maxVoteLocation = locationSet[i];
    }
    estimatedLocation = maxVoteLocation;

}
