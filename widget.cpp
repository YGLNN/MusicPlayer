#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    Player =new QMediaPlayer(this);
    ui->btn_Play->setEnabled(false);
    ui->btn_Pause->setEnabled(false);
    connect(Player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(onStateChanged(QMediaPlayer::State)));

}

Widget::~Widget()
{
    delete Player;
    delete ui;
}

//******************************Search*****************************

void Widget::ReadyRead_Search()
{
    if(Reply->error()==QNetworkReply::NoError)
    {
        QByteArray Bytes=Reply->readAll();
        GetSongInfo(Bytes);
        ui->tEd_Info->clear();
        ui->tEd_Info->append("已找到歌曲，请点击播放按钮播放\n");
        ui->tEd_Info->append("切歌失败请尝试重新搜索并再次点击播放");
        Player->stop();
        ui->btn_Play->setEnabled(true);
    }
    else
    {
        ui->tEd_Info->append("错误，请重试");
        return;
    }
}

void Widget::Finished_Search()
{
    Reply->disconnect();
    Reply->deleteLater();
    Reply=Q_NULLPTR;

}

void Widget::GetSongInfo(QByteArray Json)
{
    QJsonParseError JsonError;
    QJsonDocument ParseDoc=QJsonDocument::fromJson(Json,&JsonError);
    if(JsonError.error == QJsonParseError::NoError)
    {
        if(ParseDoc.isObject())
        {
            QJsonObject Root=ParseDoc.object();
            if(Root.contains("result"))
            {
                QJsonValue DataValue=Root.value("result");
                if(DataValue.isObject())
                {
                    QJsonObject DataObj=DataValue.toObject();
                    if(DataObj.contains("songs"))
                    {
                        QJsonValue SongsArray=DataObj.value("songs");
                        if(SongsArray.isArray())
                        {
                            QJsonArray Array=SongsArray.toArray();
                            QJsonValue Array0=Array.at(0);
                            if(Array0.isObject())
                            {
                                QJsonObject FirstSong=Array0.toObject();
                                if(FirstSong.contains("id"))
                                {
                                    QJsonValue SongID=FirstSong.take("id");

                                    FirstSongInfo.SongID=SongID.toInt();
                                }
                                if(FirstSong.contains("artists"))
                                {
                                    QJsonValue Artists=FirstSong.value("artists");
                                    if(Artists.isArray())
                                    {
                                       QJsonArray SongArtists=Artists.toArray();
                                       QJsonValue Array0=SongArtists.at(0);
                                       if(Array0.isObject())
                                       {
                                           QJsonObject FirstSong=Array0.toObject();
                                           if(FirstSong.contains("name"))
                                           {
                                               QJsonValue SongSinger=FirstSong.take("name");
                                               FirstSongInfo.Singer=SongSinger.toString();
                                           }
                                       }
                                    }
                                }
                            }

                        }
                    }

                }
            }
        }
    }
}

void Widget::on_btn_Search_clicked()
{
    ui->tEd_Info->clear();
    QString MusicName=ui->lEd_Search->text();
    FirstSongInfo.SongName=MusicName;

    if(MusicName.isEmpty())
    {
        QMessageBox::information(this,"Error","Plase Enter Music Name");
        return;
    }

    QString Key_Value=QString("type=1&s=%1").arg(MusicName);
    QUrlQuery Query(Key_Value);
    QUrl SearchUrl("http://music.163.com/api/search/pc/");
    SearchUrl.setQuery(Query);

    Reply=NetworkManager.get(QNetworkRequest(SearchUrl));

    connect(Reply,SIGNAL(readyRead()),this,SLOT(ReadyRead_Search()));
    connect(Reply,SIGNAL(finished()),this,SLOT(Finished_Search()));
}

///////////////////////////////////////////////////////////////////


//*******************************Play******************************

void Widget::ReadyRead_Play()
{
    if(Reply->error()==QNetworkReply::NoError)
    {
        QByteArray Bytes=Reply->readAll();
        GetPlayUrl(Bytes);
    }
    else
    {
        ui->tEd_Info->append("错误，请重试");
        return;
    }
}

void Widget::Finished_Play()
{
    Reply->disconnect();
    Reply->deleteLater();
    Reply=Q_NULLPTR;
}

void Widget::GetPlayUrl(QByteArray Json)
{
    QJsonParseError JsonError;
    QJsonDocument ParseDoc=QJsonDocument::fromJson(Json,&JsonError);
    if(JsonError.error==QJsonParseError::NoError)
    {
        if(ParseDoc.isObject())
        {
            QJsonObject Root=ParseDoc.object();
            if(Root.contains("data"))
            {
                QJsonValue Data=Root.take("data");
                if(Data.isArray())
                {
                    QJsonArray Arr=Data.toArray();
                    QJsonValue Arr0=Arr.at(0);
                    if(Arr0.isObject())
                    {
                        QJsonObject Song = Arr0.toObject();
                        if(Song.contains("url"))
                        {
                            QJsonValue PlayUrl=Song.take("url");
                            FirstSongInfo.SongPlayUrl_Str=PlayUrl.toString();
                        }
                    }
                }
            }
        }
    }
}

void Widget::PlayRequest()
{
    QString ID=QString::number(FirstSongInfo.SongID);
    QString Key_Value=QString("type=song&id=%1&br=128000").arg(ID);
    QUrl PlayUrl("https://api.imjad.cn/cloudmusic/");
    QUrlQuery Query(Key_Value);
    PlayUrl.setQuery(Query);

    Reply=NetworkManager.get(QNetworkRequest(PlayUrl));

    connect(Reply,SIGNAL(readyRead()),this,SLOT(ReadyRead_Play()));
    connect(Reply,SIGNAL(finished()),this,SLOT(Finished_Play()));
}

void Widget::onStateChanged(QMediaPlayer::State state)
{
    ui->btn_Play->setEnabled(!(state==QMediaPlayer::PlayingState));
    ui->btn_Pause->setEnabled((state==QMediaPlayer::PlayingState));
}

void Widget::on_btn_Play_clicked()
{
    ui->tEd_Info->clear();

    PlayRequest();

    ui->tEd_Info->append(QString("歌曲名：%1").arg(FirstSongInfo.SongName));
    ui->tEd_Info->append(QString("歌手：%1").arg(FirstSongInfo.Singer));
    ui->tEd_Info->append("播放链接：");
    ui->tEd_Info->append(FirstSongInfo.SongPlayUrl_Str);
    Player->setMedia(QUrl(FirstSongInfo.SongPlayUrl_Str));
    FirstSongInfo.SongPlayUrl_Str.clear();
    Player->play();

}

void Widget::on_btn_Pause_clicked()
{
    Player->pause();
}

///////////////////////////////////////////////////////////////////






