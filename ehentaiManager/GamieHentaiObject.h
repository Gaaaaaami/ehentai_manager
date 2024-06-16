#pragma once
#include <QObject>
#include <QNetworkReply>
#include "GamieHentaiGlobalSettings.h"
#include "GamieHentaiParser.h"
QT_BEGIN_NAMESPACE
namespace Ui { class GamiMainWidget; }
QT_END_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QImage;
class QPaintEvent;
class QResizeEvent;
class QNetworkCookie;
class QNetworkCookieJar;
class GamieHentaiObject : public QObject
{
    Q_OBJECT
public:
     GamieHentaiObject(QObject *parent = nullptr);
     virtual ~GamieHentaiObject();
public:
     void setSaveTo(QString path);
     void request(QString url);
     void request(QString url,unsigned int already_download_byte);
     void printHex(char *data, unsigned int le);
public:
     double             getProgress();
     qint64             getCurrent();
     qint64             getTotal();
     QString            getRequestUrl();
protected slots:
     virtual void OnFinished(QNetworkReply *reply);
     virtual void OnResponse(QByteArray &msg);
     virtual void OnError(QNetworkReply::NetworkError err);
     virtual void OnRetry(int count);
     virtual void OnRequest();
     virtual void OnProgressChange(qint64 current, qint64 total);
     virtual void OnNewData();
protected:
     virtual void Controller();
protected:
    QString                          _save_to;
    QString                          _request_url;
    QNetworkAccessManager           *_net_manager;
    QNetworkCookieJar               *_net_cookie;
    QNetworkReply                   *_net_reply;
protected:
    unsigned int                     _retry_count;
private:
    QVector<GamieHentaiParser::steHentaiItemInfo>           _ehentai_main_index_list;
protected:
    double                                                  _progress;
protected:
    qint64                                                  _total;
    qint64                                                  _current;
};
