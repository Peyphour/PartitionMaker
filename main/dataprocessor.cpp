#include "dataprocessor.h"

DataProcessor::DataProcessor(QObject *parent) : QThread(parent) {

}

DataProcessor::~DataProcessor() {

}

void DataProcessor::setWorkingDir(QString dir) {
    this->workingDir = dir;
}

template<typename T> T dAbs(T a) { return a < 0 ? -a : a;}

void DataProcessor::performFFT(QList<double> *data) {
    int size = data->size();
    kiss_fft_cpx *fft_in = (kiss_fft_cpx*) malloc(size * sizeof(kiss_fft_cpx));
    kiss_fft_cpx *fft_out =(kiss_fft_cpx*) malloc(size * sizeof(kiss_fft_cpx));
    kiss_fftr_cfg fft = kiss_fftr_alloc(size * 2, 0, 0, 0);
    kiss_fft_scalar zero;
    memset(&zero, 0, sizeof(zero));
    // Init FFT data
    for(int i = 0; i < size; i++) {
        fft_in[i].r = data->at(i);
        fft_in[i].i = zero;
        fft_out[i].r = zero;
        fft_out[i].i = zero;
    }
    kiss_fftr(fft, (kiss_fft_scalar*) fft_in, fft_out);
    // now copy the output
    // Remember that the output of the FFT is an array of complex values
    // We must take the modulus of the complex number
    for(int i = 0; i < size; i++) {
        //if((fft_out[i].r == 0.0 && fft_out[i].i < 0.0)|| (fft_out[i].i == 0.0 && fft_out[i].r < 0.0)) continue;
        data->replace(i, sqrt(dAbs(fft_out[i].r*fft_out[i].r + fft_out[i].i*fft_out[i].i)));
    }
    kiss_fftr_free(fft);
    free(fft_in);
    free(fft_out);
    emit dataChanged(*data);
}

void DataProcessor::findPeakFrequencies(QList<double> *freqTab) {
    // We must analyze the frequency array only to the half of himself
    // (The FFT spectrum is symmetric arround the Nyquist frequency : freqTab.size() / 2
    double threshold = 5000; // This is a test value that will most likely change when I find a better way.
    double freq = 0.0;
    QList<double> peakFrequencies;
    for(int i = 0; i < freqTab->size() / 2; i++) {
        if(freqTab->at(i) > threshold) {
            freq = (i / 2) * this->ACCURACY;
            peakFrequencies.append(freq);
        }
    }
    freqTab->clear();
    freqTab->append(peakFrequencies);
}

void DataProcessor::setTempo(int tempo) {
    this->tempo = tempo;
}

void DataProcessor::builderProgressChanged(int newProgress) {
    emit progressChanged(newProgress);
}

void DataProcessor::run() {

    GuitarFilter *filter = new GuitarFilter;

    QList<GuitarNote*> *noteList = filter->getNoteList();

    GuitarTab *tab;

    emit progressChanged(0);
    log("Processing data ...");
    tab = this->processFile();

    tab->writeToFile(this->workingDir.append("/tab.gp4"));

    delete filter;
    delete noteList;
    delete tab;
    emit finish();
}

/**
 * This function will process each files by pulsation.
 */
GuitarTab *DataProcessor::processFile() {

    GuitarTab *tab = new GuitarTab;

    qreal interval = tab->SIXTEENTH_NOTE * 2;
    qreal max;

    // A quarter note last 60000/tempo ms
    // We will try to process by eighth notes to start
    // Therefore, we will process sound files by intervals of 60000/(tempo*2) ms
    qreal interval_ms = 60000.0/(this->tempo * interval);
    // Getting audio format
    WavFile *curFile = new WavFile;
    curFile->open(QString(this->workingDir).append("/main.wav"));
    QAudioFormat format = curFile->fileFormat();

    // For unknown reason, it crash for a sample rate lower than 44100 Hz
    if(format.sampleRate() < 44100) {
        emit log("Fatal error : sample rate cannot be lower than 44100 Hz !");
        return tab;
    }

    // Now getting the number of bytes that represents the interval :
    qint64 bytesPerInterval = audioLength(format, interval_ms * 1000);
    // We will process files by intervals of bytesPerInterval

    // Initializing data containers that will be used to build the tab
    QList<QList<PartitionNote*> *> *notes = new QList<QList<PartitionNote*> *>();
    for(int i = 0; i < 6; i++) {
        notes->push_back(new QList<PartitionNote *>());
    }

    GuitarNote *note;
    PartitionNote *pNote;
    // Getting file data.
    // Since data.left = data.right, we are gonna process only one channel

    curFile->toRealData();
    QList<double> data = curFile->getRealData().at(0);

    curFile->close();
    delete curFile;

    // Getting data size and the number of pulses that the file contains.
    // Since we know that an interval last bytesPerInterval bytes and that
    // we have data.size() bytes of data, data.size() / bytesPerInterval gives us
    // how many pulse we will have to process.
    qint64 pulseNumber = data.size() / bytesPerInterval + 1;
    // The "+1" will be useful to purge the data stream (we will never have
    // a perfect number of data samples : without it, we would have data that will
    // never be processed)

    // Now we will iterate over the file.
    // We will perform a FFT over each block, by doing this, we can get with precision
    // Which frequency are in which block and therefore we can build the partition

    int samplingFrequency = format.sampleRate() / this->ACCURACY;

    emit log(QString("Detected sampling frequency : ")  + QString::number(format.sampleRate()) +" Hz");
    emit log(QString("Working with an accuracy of ") + QString::number(this->ACCURACY) + " Hz");
    emit log(QString("Processing audio sample, this may take a while."));

    for(int blockNumber = 0; blockNumber < pulseNumber; blockNumber++) {

        emit progressChanged((int)((100*blockNumber)/((float)pulseNumber)));

        QList<double> toProcess = data.mid(blockNumber * bytesPerInterval, bytesPerInterval);

        // For now on, toProcess will be refered as the "sample"
        // Now we know that the duration of one sample is "interval_ms" in milliseconds.
        // And we know that to have a precision of one Hertz, we must have at least have the number
        // of samples corresponding to a second of our audio file.
        // What we can do is repeat the sample by himself while the size of the sample
        // is less than the sampling frequency. If the size of the sample is greater than
        // the sampling frequency (which should not happen) then its good for us

        if(toProcess.size() == 0) continue; // Little security to prevent division by 0

        // Normalizing sample
        max = toProcess.at(0);
        for(int i = 1; i < toProcess.size(); i++) {
            if(toProcess.at(i) >= max)
                max = toProcess.at(i);
        }
        qreal coef = 1 / max;
        for(int i = 0; i < toProcess.size(); i++) toProcess.replace(i, toProcess.at(i) * coef);
        // End normalizing sample
        int numberOfRepeat = (samplingFrequency - toProcess.size()) / toProcess.size(); // Number of times the sample must repeat by himself
        int extraData = (samplingFrequency - toProcess.size()) % toProcess.size(); // Extra data to be added to have the exact amount of samples
        int toAppend = toProcess.size();
        for(int i = 0; i < numberOfRepeat; i++) {
            toProcess.append(toProcess.mid(0, toAppend));
        }
        toProcess.append(toProcess.mid(0, extraData));
        // Now the sample size should be equals or greater than samplingFrequency.
        // Let us perform an FFT on this sample now.
        performFFT(&toProcess);
        // Finding peaks frequencies
        findPeakFrequencies(&toProcess);

        qDebug() << toProcess;
        toProcess.clear();
    }

    emit log(QString("Building tab ..."));
    GuitarTabBuilder *tabBuilder = new GuitarTabBuilder;
    tabBuilder->setData(notes, interval);
    connect(tabBuilder, SIGNAL(progressChanged(int)), this, SLOT(builderProgressChanged(int)));
    tab = tabBuilder->build();
    delete tabBuilder;
    return tab;
}
