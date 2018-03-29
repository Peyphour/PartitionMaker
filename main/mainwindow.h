#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QAudioRecorder>
#include <QFile>
#include <QFileDialog>
#include <QScrollBar>
#include <QDir>
#include <QSound>
#include <QThread>
#include <QMessageBox>
#include "dataprocessor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void log(const QString);
    QString getFilename() const;
private :
    void setRecordEnable(bool);
    void setInputFile(const QString);
    void createTempDir();
    void setPlayingMode(bool);
private slots:
    void updateDuration(qint64 newDuration);
    void updateStatus(QMediaRecorder::Status status);
    void onStateChanged(QMediaRecorder::State state);
    void showError();
    void deleteTempDir();

    void finishProcessing();

    void on_pushButton_play_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_record_clicked();

    void on_file_chooser_clicked();

    void on_cancel_file_input_clicked();

    void on_pushButton_process_clicked();

    void on_pushButton_clear_clicked();

    void on_tempo_textEdited(const QString &arg1);
    void dataChanged(QList<double>);
private:
    Ui::MainWindow *ui;
    QAudioRecorder *recorder;
    QString inputFile;
    QDir *tmpDir;
    QSound *player = Q_NULLPTR;
    bool canProcessData;
    DataProcessor *dataProcessor;
    QVector<double> keys;
    enum {
        RECORDING,
        PLAYING,
        WAITING
    } state;

    enum {
        RECORD,
        FILE,
        NONE
    } mode;
};

#endif // MAINWINDOW_H
