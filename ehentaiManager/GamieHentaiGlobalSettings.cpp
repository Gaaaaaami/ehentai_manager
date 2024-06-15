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

void GamieHentaiGlobalSettings::addImageManager(QString dir, GamieHentaiImageManager *p)
{
    auto it = _image_manager.find(dir);
    if( it == _image_manager.end()){
        QQueue<GamieHentaiImageManager *> f;
        f.clear();
        it = _image_manager.insert(dir , f);
    }
    auto &vector = it.value();
    vector.push_back(p);
}
void GamieHentaiGlobalSettings::delImageManager(QString dir, GamieHentaiImageManager *p)
{
    auto it = _image_manager.find(dir);
    if( it == _image_manager.end())
        return;

    auto &vector = it.value();
    for(int i = 0; i < vector.size();i++){
        if(vector.at(i) == p){
           vector.removeAt(i);
           return;
        }
    }
}
void GamieHentaiGlobalSettings::execImageManager(std::function<void (QString, GamieHentaiImageManager *)> fun)
{
    for(auto it = _image_manager.begin();it != _image_manager.end();it++){
        auto &vector = it.value();
        for(auto *item : vector)
            fun(it.key(), item);
    }
}

void GamieHentaiGlobalSettings::execImageManager(std::function<bool (QString, GamieHentaiImageManager *)> fun, int n){
    for(auto it = _image_manager.begin();it != _image_manager.end();it++){
        auto &queue = it.value();
        int ind = 0;
        while(!queue.empty() && ind < n){
            if(fun(it.key(), queue.head()))
                return;
            queue.pop_front();
            ind++;
        }
    }
}
void GamieHentaiGlobalSettings::addImageDownloadingManager( GamieHentaiImageManager *p){

    QVector<int > rel;
    int index = 0;
    for(auto *item : _image_downloading_manager){
        if(item->getRequestUrl() == p->getRequestUrl()){
           rel.push_back(index);
        }
        index++;
    }
    for(auto ind : rel)
        _image_downloading_manager.remove(ind);
    _image_downloading_manager.push_back(p);

}
void GamieHentaiGlobalSettings::delImageDownloadingManager( GamieHentaiImageManager *p){
    for(int i = 0; i < _image_downloading_manager.size();i++){
        if(_image_downloading_manager.at(i) == p){
           _image_downloading_manager.remove(i);
           return;
        }
    }
}

qint64 GamieHentaiGlobalSettings::runtime()
{
    return _runtime.elapsed();
}
unsigned int GamieHentaiGlobalSettings::imageDownloadingSize(){
    return _image_downloading_manager.size();
}
unsigned int GamieHentaiGlobalSettings::downloaderManagerSize(QString dir){
    auto it = _download_manager.find(dir);
    if( it == _download_manager.end()){
        return 0;
    }
    auto &vector = *it;
    return vector.size();
}

unsigned int GamieHentaiGlobalSettings::imageManagerSize(QString dir){
    auto it = _image_manager.find(dir);
    if(  it == _image_manager.end()){
        return 0;
    }
    auto &vector = *it;
    return vector.size();
}
unsigned int GamieHentaiGlobalSettings::dwonloaderMaxSize(){
    return _max_download_number;
}

QVector<GamieHentaiImageManager *> &GamieHentaiGlobalSettings::getImageDownloadingVector(){
    return _image_downloading_manager;
}
bool GamieHentaiGlobalSettings::useProxy(){
    return _use_net_proxy;
}
QNetworkProxy &GamieHentaiGlobalSettings::getProxy(){
    return  *_net_proxy;
}
