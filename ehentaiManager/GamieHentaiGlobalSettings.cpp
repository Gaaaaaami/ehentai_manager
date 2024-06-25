#include "GamieHentaiGlobalSettings.h"
#include "GamieHentaiImageManager.h"
#include <QNetworkProxy>

GamieHentaiGlobalSettings::GamieHentaiGlobalSettings():_max_download_number(7){
    _page_index_manager = nullptr;
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
void GamieHentaiGlobalSettings::addDownloaderManager(QString dir, GamieHentaiGlobalSettings::stDownloadFileStatus p){
    auto it = _download_manager.find(dir);
    if( it == _download_manager.end()){
        QVector<stDownloadFileStatus> f;
        f.clear();
        it = _download_manager.insert(dir , f);
    }
    auto &vector = it.value();
    bool n = true;
    for(auto &item : vector)
        if(item.href == p.href){
            n = false;
            break;
        }
    if(n){
        vector.push_back(p);
    }
    return;
}

void GamieHentaiGlobalSettings::addImageTotal(QString dir, unsigned int total){
    _image_total.insert(dir , total);
}
void GamieHentaiGlobalSettings::modDownloaderManager(QString dir, QString request_url, GamieHentaiGlobalSettings::enuImageDownloadStatus status){
    auto it = _download_manager.find(dir);
    if( it == _download_manager.end())
        return;
    auto &vector = it.value();
    for(int i = 0; i < vector.size();i++){
        if(vector[i].href == request_url){
           vector[i].status = status;
        }
    }
}
void GamieHentaiGlobalSettings::delDownloaderManager(QString dir, GamieHentaiGlobalSettings::stDownloadFileStatus p){
    auto it = _download_manager.find(dir);
    if( it == _download_manager.end())
        return;

    auto &vector = it.value();
    for(int i = 0; i < vector.size();i++){
        if(vector.at(i).href == p.href){
           vector.remove(i);
        }
    }
}
void GamieHentaiGlobalSettings::execDownloaderManager(std::function<void (QString, GamieHentaiGlobalSettings::stDownloadFileStatus)> fun){
    for(auto it = _download_manager.begin();it != _download_manager.end();it++){
        auto &vector = it.value();
        for(auto item : vector)
            fun(it.key(), item);
    }
}

void GamieHentaiGlobalSettings::execDownloaderManager(std::function<void (QString)> fun){
    for(auto it = _download_manager.begin();it != _download_manager.end();it++){
        fun(it.key());
    }
}

bool GamieHentaiGlobalSettings::checkImageDownloadIsDone(QString dir,unsigned int &success, unsigned int &failed, unsigned int &downloading){
    unsigned int image_total = GamieHentaiGlobalSettings::global().getImageTotal(dir);
    if(image_total == 0)
        return false;

    downloading = 0;
    failed = 0;
    success = 0;


    auto it = _download_manager.find(dir);
    if(it ==  _download_manager.end())
       return false;
    auto &vector= it.value();
    for(auto &item :vector){
        if(GamieHentaiGlobalSettings::DOWNLOADING == item.status){
            downloading++;
        }else if(GamieHentaiGlobalSettings::FAILED == item.status){
            failed++;
        }else if(GamieHentaiGlobalSettings::SUCCESS == item.status){
            success++;
        }
    }

    if(image_total != (failed+success)){
        return false;
    }else {
        return true;
    }

}
unsigned int GamieHentaiGlobalSettings::getImageTotal(QString dir){
    auto it = _image_total.find(dir);
    if(it == _image_total.end())
        return 0;
    return it.value();
}
QVector<GamieHentaiGlobalSettings::stDownloadFileStatus> &GamieHentaiGlobalSettings::getDownloadManager(QString dir){
    auto it = _download_manager.find(dir);
    return it.value();
}

void GamieHentaiGlobalSettings::addDownloadListItem(GamieHentaiGlobalSettings::stDownloadListItem &item){
    _download_list_item.push_back(item);
}
void GamieHentaiGlobalSettings::modDownloadListItemState(QString save, GamieHentaiGlobalSettings::enuImageDownloadStatus state){
    for(auto &it : _download_list_item)
        if(it.save == save){
           it.state = state;
        }
}

void GamieHentaiGlobalSettings::downloadNextNoDownloadStateListItem(){
    if(hasDownloadListItemDownloading())
        return;
    for(auto & it : _download_list_item){
        if(it.state == GamieHentaiGlobalSettings::NO_START_DOWNLOADING)
        {
           qDebug() << it.href << "download now...";
           it.state = GamieHentaiGlobalSettings::DOWNLOADING;
           if(_page_index_manager == nullptr)
              _page_index_manager = new GamieHentaiImagePageIndexManager;

           _page_index_manager->setSaveTo(it.save);
           _page_index_manager->request(it.href);
           return;
        }
    }

}

bool GamieHentaiGlobalSettings::hasDownloadListItemDownloading(){
    for(auto & it : _download_list_item){
        if(it.state == GamieHentaiGlobalSettings::DOWNLOADING)
            return true;
    }
    return false;
}

void GamieHentaiGlobalSettings::update(){
    execDownloaderManager([&](QString dir){
        unsigned int success,failed,downloading;
        bool n = GamieHentaiGlobalSettings::global().checkImageDownloadIsDone(dir, success, failed , downloading);
        unsigned int image_total = GamieHentaiGlobalSettings::global().getImageTotal(dir);
        if((success+failed) == image_total){
            modDownloadListItemState(dir, GamieHentaiGlobalSettings::SUCCESS);
        }
        qDebug() << dir <<  (success+failed) << "/"<<image_total;
    });
    downloadNextNoDownloadStateListItem();


    for(auto &it : _download_list_item)
        if(it.state == GamieHentaiGlobalSettings::NO_START_DOWNLOADING)
            qDebug() << it.save << it.href << " -"<< "waiting";
}



qint64 GamieHentaiGlobalSettings::runtime()
{
    return _runtime.elapsed();
}

unsigned int GamieHentaiGlobalSettings::getDownloadSize(QString dir){
    auto it = _download_manager.find(dir);
    if(it ==  _download_manager.end())
        return 0;
    auto  &vector = it.value();
    return vector.size();
}
bool GamieHentaiGlobalSettings::useProxy(){
    return _use_net_proxy;
}
QNetworkProxy &GamieHentaiGlobalSettings::getProxy(){
    return  *_net_proxy;
}
