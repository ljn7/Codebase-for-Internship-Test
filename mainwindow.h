#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmediaplayer.h"
#include "qvideowidget.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int counter;

private slots:

    void on_lineEdit_returnPressed();

    void on_actionOpen_triggered();

    void on_sendBtn_clicked();

//    void on_playSlider_valueChanged(int value);

    void on_volumeSilder_valueChanged(int value);

    void on_muteBtn_clicked();

    void on_stopBtn_clicked();

    void on_playPauseBtn_clicked();

//    void on_playSlider_valueChanged(int value);


    void on_playSlider_sliderMoved(int position);

    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;

    void updateText();

    void onDurationChanged(qint64 duration);

    void onPositionChanged(qint64 position);

    void onPositionChanged1(qint64 position);

    void onDurationChanged1(qint64 duration);

    QMediaPlayer *Player;
    QVideoWidget *Video;
    QAudioOutput *Audio;

    qint64 totalDuration;
    bool isPaused = true;
    bool isMuted = false;
    qreal linearVolume;
    qint64 currentVolumeSlider;


protected:
    virtual void resizeEvent(QResizeEvent* event) override;

};
#endif // MAINWINDOW_H
