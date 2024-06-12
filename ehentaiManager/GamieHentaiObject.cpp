#include "GamieHentaiObject.h"
#include "GamieHentaiImageManager.h"
#include "ui_GamiMainWidget.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QDateTime>
GamieHentaiObject::GamieHentaiObject(QObject *parent)
    : QObject(parent),
    _net_manager(new QNetworkAccessManager(this)){
    //qDebug()<<QSslSocket::sslLibraryBuildVersionString();
    //qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();//若输出：OpenSSL支持情况: false，那说明当前 Qt 不支持 OpenSSL
    //for(auto supported : _net_manager->supportedSchemes())
    //    qDebug() << supported;
    _net_manager->setProxy(GamiGlobalSettings::global().getProxy());
    connect(_net_manager, &QNetworkAccessManager::finished, this, &GamieHentaiObject::OnFinished);
}
GamieHentaiObject::~GamieHentaiObject(){
    _net_manager->clearAccessCache();
    _net_manager->clearConnectionCache();
    delete _net_manager;
}
void GamieHentaiObject::request(QString url){
    QNetworkRequest request;
    request.setUrl(QUrl(url)/*QUrl("https://e-hentai.org/g/2947169/980c019daf/")*/);
    _net_manager->get(request);
}

void GamieHentaiObject::OnFinished(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            OnResponse(response);
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
}
void GamieHentaiObject::OnResponse(QByteArray &msg){
    GamieHentaiParser parser(msg);
   _ehentai_main_index_list = parser.GetMainPageIndexList();
    Controller();
}

void GamieHentaiObject::Controller()
{
#ifdef USE_CONSOLE_CONTROLLER
    for(auto item: _ehentai_main_index_list)
        qDebug() <<item.index << item.title ;

    printf("\n");
    int index = -1;
    printf("input index and use key of enter load -> ");
    bool scanf_ret = true;
    while( (scanf_ret = !scanf("%d", &index)) || (index < 1 || index > _ehentai_main_index_list.size())){
        fflush(stdout);
        fflush(stdin);
        if(scanf_ret || (index < 1 || index >= _ehentai_main_index_list.size()))
            printf("input index and use key of enter load -> ");
    }
    QString url = GamieHentaiParser::ToNormalURL(_ehentai_main_index_list.at(index-1).herf);
    GamieHentaiImageManager *im = new GamieHentaiImageManager;
    im->request(url);
#endif


}

