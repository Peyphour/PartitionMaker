#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    ui->statusbar->setSizeGripEnabled(false);
    recorder = new QAudioRecorder(this);
    createTempDir();

    this->dataProcessor = new DataProcessor(this);

    this->canProcessData = false;

    this->state = WAITING;
    this->mode = NONE;

    connect(this->recorder, SIGNAL(durationChanged(qint64)),
            this, SLOT(updateDuration(qint64)));
    connect(this->recorder, SIGNAL(statusChanged(QMediaRecorder::Status)),
            this, SLOT(updateStatus(QMediaRecorder::Status)));
    connect(this->recorder, SIGNAL(stateChanged(QMediaRecorder::State)),
            this, SLOT(onStateChanged(QMediaRecorder::State)));
    connect(this->recorder, SIGNAL(error(QMediaRecorder::Error)),
            this, SLOT(showError()));

    // list settings
    foreach(const QString &device, recorder->audioInputs()) {
        ui->input_device->addItem(device, QVariant(device));
    }
    foreach(const QString &codec, recorder->supportedAudioCodecs()) {
        ui->codec->addItem(codec, QVariant(codec));
    }
    foreach(int sample_rate, recorder->supportedAudioSampleRates()) {
        ui->sample_rate->addItem(QString::number(sample_rate), QVariant(sample_rate));
    }
    this->ui->widget->addGraph();
    this->ui->widget->xAxis->setRange(0, 20000);
    this->ui->widget->yAxis->setRange(0, 20000);

    for(int i = 0; i < 20000; i ++) keys.append(i);

    this->log("Ready :)");
}

MainWindow::~MainWindow() {
    delete recorder;
    this->deleteTempDir();
    delete tmpDir;
    delete player;
    delete dataProcessor;
    delete ui;
    QApplication::quit();
}

void MainWindow::updateDuration(qint64 newDuration) {
    ui->duration_label->setText("Duration : "+QString::number(newDuration/1000)+" s");
    QFile *file = new QFile(recorder->outputLocation().toString());
    ui->size_label->setText("Size : "+QString::number(file->size()));
    delete file;
}
void MainWindow::updateStatus(QMediaRecorder::Status status) {

    QString statusMessage;

    switch (status) {
    case QMediaRecorder::RecordingStatus:
        statusMessage = tr("Recording");
        break;
    case QMediaRecorder::PausedStatus:
        statusMessage = tr("Recorder Paused");
        break;
    case QMediaRecorder::UnloadedStatus:
    case QMediaRecorder::LoadedStatus:
        statusMessage = tr("Recorder Stopped");
    default:
        break;
    }

    ui->statusbar->showMessage(statusMessage);
}

void MainWindow::log(QString msg) {
    ui->logger->appendPlainText(msg);
    ui->logger->verticalScrollBar()->setValue(ui->logger->verticalScrollBar()->maximum());
}

void MainWindow::onStateChanged(QMediaRecorder::State state) {

    switch(state) {

    case QMediaRecorder::RecordingState:
        ui->pushButton_record->setEnabled(false);
        ui->pushButton_play->setEnabled(false);
        break;
    case QMediaRecorder::StoppedState:
        ui->pushButton_record->setEnabled(true);
        ui->pushButton_play->setEnabled(true);
        break;
    case QMediaRecorder::PausedState:
        break;

    }
}

void MainWindow::showError() {
    ui->statusbar->showMessage(recorder->errorString());
}

void MainWindow::setPlayingMode(bool isPlaying) {
    ui->pushButton_record->setEnabled(!isPlaying);
    ui->pushButton_play->setEnabled(!isPlaying);
}


void MainWindow::on_pushButton_play_clicked() {
    if(player == Q_NULLPTR) {
        this->log("Initializing player ...");
        player = new QSound(this->tmpDir->path()+"/main.wav");
    }
    if(this->canProcessData) {
        state = PLAYING;
        this->setPlayingMode(true);
        player->play();
    }
}

void MainWindow::on_pushButton_stop_clicked() {
    if(state == RECORDING) {
        recorder->stop();
        this->log("Recorder stopped.");
        if(QFile(this->tmpDir->path()+"/clip_0001.wav").rename(this->tmpDir->path()+"/main.wav"))
            this->log("Record saved !");
        this->log("Ready to process data.");
        this->canProcessData = true;
        this->mode = RECORD;
    } else if(state == PLAYING) {
        player->stop();
        this->setPlayingMode(false);
        if(this->mode == FILE) {
            this->setRecordEnable(false);
        }
        this->log("Player stopped !");
    }
    state = WAITING;
}

static QVariant boxValue(QComboBox *box) {
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void MainWindow::on_pushButton_record_clicked() {

    if(recorder->state() == QMediaRecorder::StoppedState) {

        // First, delete previous file
        if(QFile(this->tmpDir->path().append("/main.wav")).remove())
            this->log("Previous file deleted");

        recorder->setAudioInput(boxValue(ui->input_device).toString());

        QAudioEncoderSettings settings;
        settings.setCodec(boxValue(ui->codec).toString());
        settings.setSampleRate(boxValue(ui->sample_rate).toInt());
        settings.setBitRate(96000);
        settings.setChannelCount(2);
        settings.setQuality(QMultimedia::VeryHighQuality);
        settings.setEncodingMode(QMultimedia::ConstantBitRateEncoding);

        recorder->setEncodingSettings(settings, QVideoEncoderSettings(), "x/wav");
        recorder->record();
        state = RECORDING;
        this->log("Recording ...");
    }
}

void MainWindow::setRecordEnable(bool enabled) {
    ui->pushButton_record->setEnabled(enabled);
}

void MainWindow::setInputFile(QString filePath) {
    this->inputFile = filePath;
}

void MainWindow::on_file_chooser_clicked() {
    const QString fileName = QFileDialog::getOpenFileName(this,"Choose a WAV File","./","*.wav");
    QString tmp = fileName.split("/").last();
    if(fileName != "") {
        ui->file_chooser->setText(tmp);
        this->setRecordEnable(false);
        this->log(QString("Copying file %1 to temp dir ...").arg(fileName));
        QFile(fileName).copy(this->tmpDir->path().append("/main.wav"));
        this->log(QString("Ready to process file %1.").arg(fileName));
        this->canProcessData = true;
        this->mode = FILE;
    } else {
        this->setInputFile(Q_NULLPTR);
    }
}

void MainWindow::on_cancel_file_input_clicked() {

    if(this->state == RECORDING || this->state == PLAYING) return; // Cannot do this while recording or playing

    ui->file_chooser->setText("Choose a file");
    this->setRecordEnable(true);
    this->setInputFile(Q_NULLPTR);
}

void MainWindow::createTempDir() {
    this->tmpDir = new QDir;
    this->tmpDir->setPath(QDir::currentPath());
    if(this->tmpDir->mkdir("./tmp")) {
        this->tmpDir->setPath(QDir::currentPath().append("/tmp"));
        QString loc = this->tmpDir->path();
        this->recorder->setOutputLocation(QUrl::fromLocalFile(loc));
    } else {
        this->deleteTempDir();
        this->createTempDir();
    }
}

void MainWindow::deleteTempDir() {
    this->tmpDir->removeRecursively();
}

QString MainWindow::getFilename() const {
    return this->tmpDir->path().append("/main.wav");
}

void MainWindow::dataChanged(QList<double> data) {
    this->ui->widget->graph(0)->setData(keys, data.toVector());
    this->ui->widget->replot();
}

void MainWindow::on_pushButton_process_clicked() {

    if(!this->canProcessData) {
        this->log("No data found :/ Please start a recording or choose a file.");
        return;
    }

    this->log("Initializing data processor ...");

    this->dataProcessor->setTempo();
    this->dataProcessor->setWorkingDir(this->tmpDir->path());
    ui->pushButton_process->setEnabled(false);
    this->setRecordEnable(false);
    ui->pushButton_play->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    ui->file_chooser->setEnabled(false);
    ui->cancel_file_input->setEnabled(false);
    ui->progressBar->setValue(0);
    qRegisterMetaType<QList<double> >("<QList<double> >");
    connect(dataProcessor, SIGNAL(log(QString)), ui->logger, SLOT(appendHtml(QString)));
    connect(dataProcessor, SIGNAL(progressChanged(int)), ui->progressBar, SLOT(setValue(int)));
    connect(dataProcessor, SIGNAL(finish()), this, SLOT(finishProcessing()));
    connect(dataProcessor, SIGNAL(finish()), dataProcessor, SLOT(quit()));
    connect(dataProcessor, SIGNAL(dataChanged(QList<double>)), this, SLOT(dataChanged(QList<double>)));
    this->log("Starting data processing ...");
    this->dataProcessor->start();
}

void MainWindow::on_pushButton_clear_clicked() {
    ui->logger->clear();
}

void MainWindow::finishProcessing() {
    ui->pushButton_process->setEnabled(true);
    this->setRecordEnable(true);
    ui->pushButton_play->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
    ui->file_chooser->setEnabled(true);
    ui->cancel_file_input->setEnabled(true);
    ui->progressBar->setValue(0);
    this->log("Finished processing !");
}

void MainWindow::on_tempo_textEdited(const QString &arg1) {
    this->dataProcessor->setTempo(arg1.toInt());
}
