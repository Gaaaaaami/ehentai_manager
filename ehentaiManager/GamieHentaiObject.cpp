#include "GamieHentaiObject.h"
#include "GamieHentaiImageManager.h"
#include <QCoreApplication>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkProxy>
#include <QDebug>
#include <QDateTime>
#include <QDir>
GamieHentaiObject::GamieHentaiObject(QObject *parent)
    : QObject(parent),_retry_count(0),_progress(0.0),_net_reply(nullptr),
    _net_manager(new QNetworkAccessManager(this)){
    if(GamieHentaiGlobalSettings::global().useProxy())
        _net_manager->setProxy(GamieHentaiGlobalSettings::global().getProxy());
    connect(_net_manager, &QNetworkAccessManager::finished, this, &GamieHentaiObject::OnFinished);

    _net_cookie = new QNetworkCookieJar;
    _net_manager->setCookieJar(_net_cookie);
}
GamieHentaiObject::~GamieHentaiObject(){
    _net_manager->clearAccessCache();
    _net_manager->clearConnectionCache();

    delete _net_manager;
}

void GamieHentaiObject::setSaveTo(QString path){
    _save_to = path;
}
void GamieHentaiObject::request(QString url){
    _request_url = url;
    qDebug() <<  __FUNCTION__<< url;
    OnRequest();

    QList<QNetworkCookie> cookies;
    cookies.append(QNetworkCookie("nw",  "1"));
    _net_cookie->setCookiesFromUrl(cookies, url);

    QNetworkRequest request;
    request.setRawHeader("Connection", "keep-alive");
    request.setUrl(QUrl(url));
    QNetworkReply *rep = _net_manager->get(request);
    _net_reply = rep;
    connect(rep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(OnError(QNetworkReply::NetworkError)));
    connect(rep, SIGNAL(downloadProgress(qint64 , qint64 )), this, SLOT(OnProgressChange(qint64 , qint64 )));

}

void GamieHentaiObject::request(QString url, unsigned int already_download_byte){
    _request_url = url;
    qDebug() <<  __FUNCTION__<< url << ", already download byte->" << already_download_byte;
    OnRequest();

    QList<QNetworkCookie> cookies;
    cookies.append(QNetworkCookie("nw",  "1"));
    _net_cookie->setCookiesFromUrl(cookies, url);
    QString rangeHeader = QString("bytes=%1-").arg(QString::number(already_download_byte));
    QNetworkRequest request;
    request.setRawHeader("Range", rangeHeader.toUtf8());
    request.setUrl(QUrl(url));
    QNetworkReply *rep = _net_manager->get(request);
    _net_reply = rep;
    connect(rep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(OnError(QNetworkReply::NetworkError)));
    connect(rep, SIGNAL(readyRead()), this, SLOT(OnNewData()));
}
void GamieHentaiObject::printHex(char *data, unsigned int le){
    for(int i = 0; i< le;i++){
        int d = data[i];
        printf("0x%02x, ", d & 0xff);
        if(((i+1) % 5) == 0)
            printf("\n");
    }
}
double GamieHentaiObject::getProgress(){
    return _progress;
}

qint64 GamieHentaiObject::getCurrent(){
    return _current;
}
qint64 GamieHentaiObject::getTotal(){
    return _total;
}

QString GamieHentaiObject::getRequestUrl()
{
    return _request_url;
}

void GamieHentaiObject::OnFinished(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            OnResponse(response);
    }
    reply->deleteLater();
}
void GamieHentaiObject::OnResponse(QByteArray &msg){
    GamieHentaiParser parser(msg);
   _ehentai_main_index_list = parser.GetMainPageIndexList();
   Controller();
}
void GamieHentaiObject::OnError(QNetworkReply::NetworkError err){
    //超时重连机制
    if(err != QNetworkReply::NoError){

        qDebug() << __FUNCTION__ << err;
        _retry_count++;
        OnRetry(_retry_count);
    }
}

void GamieHentaiObject::OnRetry(int count){
    QNetworkRequest request;
    request.setUrl(QUrl(_request_url));
    QNetworkReply *rep = _net_manager->get(request);
    qDebug() << __FUNCTION__ << _request_url;
    connect(rep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(OnError(QNetworkReply::NetworkError)));
}

void GamieHentaiObject::OnRequest(){}
void GamieHentaiObject::OnProgressChange(qint64 current, qint64 total){
    _current = current;
    _total = total;
    _progress = double(current) / double(total);
}

void GamieHentaiObject::OnNewData(){
    auto data = _net_reply->readAll();
    //printHex(data.data(), data.length());
    qDebug() << (data.length()/1024) << "kb";

}
void GamieHentaiObject::Controller(){
#ifdef USE_CONSOLE_CONTROLLER
    for(auto item: _ehentai_main_index_list)
        qDebug() <<item.index << item.title ;

    int index = -1;
    printf("# ");
    bool scanf_ret = true;
    fflush(stdout);
    fflush(stdin);
    while( (scanf_ret = !scanf("%d", &index)) || (index < 1 || index > _ehentai_main_index_list.size())){
        fflush(stdout);
        fflush(stdin);
        if(scanf_ret || (index < 1 || index >= _ehentai_main_index_list.size()))
            printf("# ");
    }
    QString url = GamieHentaiParser::ToNormalURL(_ehentai_main_index_list.at(index-1).herf);

    QString utf8_name = _ehentai_main_index_list.at(index-1).title.toUtf8();
    QString save = QCoreApplication::applicationDirPath()+"/"+utf8_name;

    QDir dir;

    if(!dir.exists(save)){
        bool mkdir = false;
        mkdir = dir.mkdir(save);
        qDebug() << (mkdir ? "create dir success!" : "create dir error!");
        if(!mkdir){
            utf8_name = QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss_zzz");
            ///utf8_name = "16_06_2024_14_02_00_089";
            save = QCoreApplication::applicationDirPath()+"/"+utf8_name;
            dir.mkdir(save);
        }

    }

    qDebug() << "save to->" << save;
    qDebug() << "request url->" << url;
    GamieHentaiImagePageIndexManager *im = new GamieHentaiImagePageIndexManager;
    im->setSaveTo(save);
    im->request(url);
#endif
}

