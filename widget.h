#ifndef WIDGET_H
#define WIDGET_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>
#include <QUrlQuery>


struct SongsInfo
{
    QString SongName;
    QString Singer;
    int SongID;
    QString SongPlayUrl_Str;
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    QNetworkAccessManager NetworkManager;
    QNetworkReply *Reply;

    QMediaPlayer *Player;
    //QMediaPlaylist *PlayList;

    SongsInfo FirstSongInfo;

    void GetSongInfo(QByteArray Json);
    void GetPlayUrl(QByteArray Json);
    void PlayRequest();


private slots:
    void ReadyRead_Search();
    void Finished_Search();
    void ReadyRead_Play();
    void Finished_Play();
    void onStateChanged(QMediaPlayer::State);
    void on_btn_Search_clicked();
    void on_btn_Play_clicked();
    void on_btn_Pause_clicked();
};

#endif // WIDGET_H
