// Copyright (C) 2018 Yuxiang Sun
// Email: yxsun@link.cuhk.edu.hk
// MIT License

#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <QString>

struct trainingData
{
    qreal x;
    qreal y;
    qreal fingerprint[5];
    qreal euclideanDistance;
};

struct location
{
    qreal x;
    qreal y;
    int vote;   // for KNN
    qreal priorProbability[5];  // prior probability for Gaussian Process Regression
    qreal posteriorProbability[5];  // posterior probability for Gaussian Process Regression
    qreal jointPosteriorProbability;
    qreal mean[5];  //mean. this is for Gaussian Process Regression
    qreal standardDeviation[5]; //standard deviation.  this is for Gaussian Process Regression
};

struct state
{
    qreal priorProbability[5];  // prior probability for Gaussian Process Regression
    qreal posteriorProbability[5];  // posterior probability for Gaussian Process Regression
    qreal jointPosteriorProbability;
    qreal mean[5];  //mean for Gaussian Process Regression
    qreal standardDeviation[5]; //standard deviation for Gaussian Process Regression
};


class CLocalization
{

public:
    CLocalization();

    int scanWifi( QString linuxPassword, int &numScanned,
                  QString *scanAddress, QString *scanChannel, QString *scanFrequency,
                  QString *scanQuality, QString *scanRSSI, QString *scanEncryption, QString *scanESSID );

    int writeDatabase( int &dbX, int &dbY, int &dbscanTimes,
                       QString &dbstartMeasurementTime, QString &dbendMeasurementTime,
                       QString (*dbAddress)[5], QString (*dbChannel)[5], QString (*dbFrequency)[5],
                       QString (*dbQuality)[5], QString (*dbRSSI)[5], QString (*dbEncryption)[5], QString (*dbESSID)[5] );

    int readDatabase( int &dbX, int &dbY, int &dbscanTimes,
                      QString &dbstartMeasurementTime, QString &dbendMeasurementTime,
                      QString (*dbAddress)[5], QString (*dbChannel)[5], QString (*dbFrequency)[5],
                      QString (*dbQuality)[5], QString (*dbRSSI)[5], QString (*dbEncryption)[5], QString (*dbESSID)[5] );

    int creatTrainingDataset( int datasetType, int &trainingDatasetLength, trainingData *trainingDataset );

    int creatSparseDatabase();

    void sort(int trainingDatasetLength, trainingData *trainingDataset);

    void eknnClassifier( int trainingDatasetLength, trainingData *trainingDataset, int modeTrainingDatasetLength, trainingData *modeTrainingDataset, int k, int variationThreshold, qreal *currentFingerprint, location &estimatedLocation, location *locationSet  );

    qreal mode( qreal a[], int size );
};

#endif // LOCALIZATION_H
