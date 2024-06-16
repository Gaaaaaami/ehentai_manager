#include "GamieHentaiGlobalSettings.h"
#include "GamieHentaiImageManager.h"
#include <QNetworkProxy>

GamieHentaiGlobalSettings::GamieHentaiGlobalSettings():_max_download_number(7){
    _net_proxy = new QNetworkProxy;
    _runtime.start();
}
GamieHentaiGlobalSettings::~GamieHentaiGlobalSettings(){
    delete _net_proxy;
}
GamieHentaiGlobalSettings &GamieHentaiGlobalSettings::global(){
    static GamieHentaiGlobalSettings instance;
    return instance;
}

void GamieHentaiGlobalSettings::setSocks5Proxy(QString address, unsigned short port)
{
    _net_proxy->setType(QNetworkProxy::Socks5Proxy);
    _net_proxy->setHostName(address);
    _net_proxy->setPort(port);

}
void GamieHentaiGlobalSettings::setUseNetProxy(bool val){
    _use_net_proxy = val;
}

void GamieHentaiGlobalSettings::addDownloaderManager(QString dir, GamieHentaiImageDownloaderManager *p){
    auto it = _download_manager.find(dir);
    if( it == _download_manager.end()){
        QVector<GamieHentaiImageDownloaderManager *> f;
        f.clear();
        it = _download_manager.insert(dir , f);
    }
    auto &vector = it.value();
    vector.push_back(p);
}
void GamieHentaiGlobalSettings::delDownloaderManager(QString dir, GamieHentaiImageDownloaderManager *p){
    auto it = _download_manager.find(dir);
    if( it == _download_manager.end())
        return;

    auto &vector = it.value();
    for(int i = 0; i < vector.size();i++){
        if(vector.at(i) == p){
           vector.remove(i);
           return;
        }
    }
}
void GamieHentaiGlobalSettings::execDownloaderManager(std::function<void (QString, GamieHentaiImageDownloaderManager *)> fun){
    for(auto it = _download_manager.begin();it != _download_manager.end();it++){
        auto &vector = it.value();
        for(auto *item : vector)
            fun(it.key(), item);
    }
}


qint64 GamieHentaiGlobalSettings::runtime()
{
    return _runtime.elapsed();
}
bool GamieHentaiGlobalSettings::useProxy(){
    return _use_net_proxy;
}
QNetworkProxy &GamieHentaiGlobalSettings::getProxy(){
    return  *_net_proxy;
}
