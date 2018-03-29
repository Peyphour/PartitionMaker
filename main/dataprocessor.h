#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <stdlib.h>
#include <QPlainTextEdit>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QStack>
#include <QThread>
#include "guitarfilter.h"
#include "guitarnote.h"
#include "tab/guitartab.h"
#include "tab/partitionbar.h"
#include "tab/partitionnote.h"
#include "tab/partitionpulsation.h"
#include "tab/guitartabbuilder.h"
#include "util/wavfile.h"
#include "util/utils.h"
#include "fft/kiss_fft.h"
#include "fft/kiss_fftr.h"

class DataProcessor : public QThread
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = 0);
    ~DataProcessor();
    void setWorkingDir(QString dir);
    void run();
    void setTempo(int tempo = 100);
private:
    void performFFT(QList<double>*);
    void findPeakFrequencies(QList<double> *);
    GuitarTab* processFile();
signals:
    void finish();
    void progressChanged(int);
    void log(QString);
    void dataChanged(QList<double>);
public slots:
private slots:
    void builderProgressChanged(int);
private:
    QString workingDir;
    int tempo;

    const static double ACCURACY = 0.5; // Frequency detection accuracy in Hertz
};

#endif // DATAPROCESSOR_H
