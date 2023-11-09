#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit->setReadOnly(true);
    counter = 0;

    Player = new QMediaPlayer();
    Audio = new QAudioOutput();

    Player->setAudioOutput(Audio);
    Video = new QVideoWidget();

    currentVolumeSlider = 30;

    ui->playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stopBtn->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->muteBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    this->resize(this->geometry().width()+1, this->geometry().height()+1);
    ui->volumeSilder->setMinimum(0);
    ui->volumeSilder->setMaximum(100);
    ui->volumeSilder->setValue(30);

    linearVolume = QAudio::convertVolume((30 / qreal(100.0)),
                          QAudio::LogarithmicVolumeScale,
                          QAudio::LinearVolumeScale);

    Player->audioOutput()->setVolume(qRound(linearVolume * 100));

    Player->audioOutput()->setVolume(ui->volumeSilder->value());
    ui->volumeLbl->setText(QString::number(30) + "%");


    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::onDurationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::onPositionChanged);

    ui->playSlider->setMinimum(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    updateText();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Video File"), QDir::homePath(), tr("Media Files (*.mp4 *.avi *.mkv *.mov *.wmv);;All Files (*)"));

    Video->setGeometry(0, 19, ui->frame->width(), ui->frame->height());
    Video->setParent(ui->frame);
    Player->setVideoOutput(Video);

    Player->setSource(QUrl(filename));
    Video->setVisible(true);

    Player->play();

    if (Player->isPlaying()) {
        ui->playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        isPaused = false;
    }
}


void MainWindow::on_sendBtn_clicked()
{
    updateText();
}

void MainWindow::on_volumeSilder_valueChanged(int value)
{
    linearVolume = QAudio::convertVolume((value / qreal(100.0)),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);

    Player->audioOutput()->setVolume(qRound(linearVolume * 100));
    ui->volumeLbl->setText(QString::number(value) + "%");
    currentVolumeSlider = value;
}

void MainWindow::on_muteBtn_clicked()
{
    if (isMuted) {
        isMuted = false;
        ui->muteBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        Player->audioOutput()->setMuted(false);
        Player->audioOutput()->setVolume(linearVolume);
        ui->volumeLbl->setText(QString::number(currentVolumeSlider) + "%");
    } else {
        isMuted = true;
        ui->muteBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        linearVolume = QAudio::convertVolume((currentVolumeSlider / qreal(100.0)),
                                             QAudio::LogarithmicVolumeScale,
                                             QAudio::LinearVolumeScale);
        Player->audioOutput()->setMuted(true);
        ui->volumeLbl->setText("0%");
    }
}

void MainWindow::updateText() {

    QString text = ui->lineEdit->text();

    if (text.isEmpty())
        return;

    ui->plainTextEdit->appendPlainText(QString::number(++counter) + " 🗣️ : " + text);
    ui->lineEdit->clear();
}

void MainWindow::on_stopBtn_clicked()
{
    Player->stop();
    ui->playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void MainWindow::on_playPauseBtn_clicked()
{
    if (isPaused) {
        Player->play();
        ui->playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        isPaused = false;
        return;
    }
    Player->pause();
    ui->playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    isPaused = true;
}

void MainWindow::onPositionChanged(qint64 position) {

    ui->playSlider->setValue(static_cast<int>(position));

    QTime currentTime(0, 0);
    currentTime = currentTime.addMSecs(position);

    QString formattedCurrentTime = currentTime.toString("mm:ss");

    QTime totalTime(0, 0);
    totalTime = totalTime.addMSecs(totalDuration);

    QString totalFormattedTime = totalTime.toString("mm:ss");

    ui->timeLbl->setText(formattedCurrentTime + " / " + totalFormattedTime);
}

void MainWindow::onDurationChanged(qint64 duration) {

    ui->playSlider->setMaximum(static_cast<int>(duration));
    totalDuration = duration;
}

void MainWindow::on_playSlider_sliderMoved(int position)
{
    qint64 duration = Player->duration();

    if (!Player->hasVideo() || !Player->hasAudio()) {
        return;
    }

    qint64 newPosition = duration * position / duration;

    Player->setPosition(newPosition);
}

void MainWindow::resizeEvent(QResizeEvent* event) {

    if (event->oldSize() == event->size()) {
        return;
    }
    QMainWindow::resizeEvent(event);

    Video->setGeometry(0, 19, ui->frame->width(), ui->frame->height());

}

void MainWindow::on_comboBox_activated(int index)
{
    Player->setPlaybackRate(index+1);
}



