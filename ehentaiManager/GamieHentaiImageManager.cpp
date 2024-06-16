#include "GamieHentaiImageManager.h"
#include <cmath>
#include <math.h>
#include <algorithm>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QDebug>
#include <QFile>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief GamieHentaiImagePageIndexManager::GamieHentaiImagePageIndexManager
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GamieHentaiImagePageIndexManager::GamieHentaiImagePageIndexManager():_ind(0),_wait(false){
    _m = new GamieHentaiImageHrefManager;
    connect(_m, SIGNAL(signal_has_image_link(QVector<GamieHentaiParser::steHentaiItemInfo> &)),
            this , SLOT(OnHasNewImageLink(QVector<GamieHentaiParser::steHentaiItemInfo> &)));
    connect(&_default_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    _default_timer.start(16);
    for(int i = 0;i < 3;i++)
        _image_manager_pool.push_back(new GamieHentaiImageManager);
}
GamieHentaiImagePageIndexManager::~GamieHentaiImagePageIndexManager(){}

GamieHentaiImageManager *GamieHentaiImagePageIndexManager::getValidImageManager(){
    for(auto *item : _image_manager_pool)
        if(item->hasFree())
            return item;
    return nullptr;
}
void GamieHentaiImagePageIndexManager::OnResponse(QByteArray &msg){
    GamieHentaiParser parser(msg);
    if(_page_href.size() == 0){
       _page_href = parser.GetImageList_PageHref();
       _m->setSaveTo(_save_to);
    }
}
void GamieHentaiImagePageIndexManager::slot_timeout(){
    for(int i =0;i < _image_manager_pool.size();i++){
        GamieHentaiImageManager *m = getValidImageManager();

        if(m)
            if(!_image_href_list.isEmpty()){
                auto item = _image_href_list.head();
                m->setSaveTo(_save_to);
                m->setRequestURL(item.herf);
                m->request(item.herf);
                _image_href_list.pop_front();
            }
    }

    if(_image_href_list.empty()){
        if(_ind < _page_href.size() && !_wait){
            _wait = true;
            qDebug() << "req->" << _page_href.at(_ind).herf;
           _m->request(_page_href.at(_ind).herf);
           _ind++;
        }
    }

}
void GamieHentaiImagePageIndexManager::OnHasNewImageLink(QVector<GamieHentaiParser::steHentaiItemInfo> &v){
    for(auto item : v)
        _image_href_list.push_back(item);
    _wait = false;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief GamieHentaiImageHrefManager::GamieHentaiImageHrefManager
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GamieHentaiImageHrefManager::GamieHentaiImageHrefManager(){}
GamieHentaiImageHrefManager::~GamieHentaiImageHrefManager(){}
void GamieHentaiImageHrefManager::OnResponse(QByteArray &msg){
    GamieHentaiParser parser(msg);
    auto list = parser.GetImageList();
    emit signal_has_image_link(list);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief GamieHentaiImageManager::GamieHentaiImageManager
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


GamieHentaiImageManager::GamieHentaiImageManager():
    _has_delete(false),_has_res(false),_has_req(false),_has_reconnect(false),_has_free(true),
    _m(nullptr){
    connect(&_default_timeout, SIGNAL(timeout()), this , SLOT(slot_timeout()));
    _default_timeout.start(16);
}
GamieHentaiImageManager::~GamieHentaiImageManager(){
    _default_timeout.stop();
}
void GamieHentaiImageManager::OnRequest(){
    _has_free = false;
}
void GamieHentaiImageManager::OnResponse(QByteArray &msg){
    GamieHentaiParser parser(msg);
    QString href = parser.GetImageDownloadHref();
    if(_m == nullptr){
       _m = new GamieHentaiImageDownloaderManager;
       connect(_m, SIGNAL(signal_delete()), this ,SLOT(slot_delete()),Qt::QueuedConnection);
       connect(_m, SIGNAL(signal_reconnect(QString)), this ,SLOT(slot_reconnect(QString)),Qt::QueuedConnection);
    }
    _m->setImageHref(_request_url);
    _m->setSaveTo(_save_to);
    _m->setImageManager(this);
    _m->setRequestURL(href);

#if 0
    qDebug()<<"parse url ->"<<href;
    qDebug()<<"save ->"<<_save_to;
#endif

    QString path = _save_to + "/" +_m->getImageName();
    if(GamieHentaiImageDownloaderManager::hasFile(path)){
        _has_free = true;
        return;
    }else
        qDebug() << path << "-> download";
    _m->request(href);

    _has_res = true;
    _has_req = false;
}
bool GamieHentaiImageManager::hasResponse(){
    return _has_res;
}
bool GamieHentaiImageManager::hasFree(){
    return _has_free;
}
void GamieHentaiImageManager::slot_reconnect(QString url){
    _has_res =false;
    _has_reconnect =true;
    request(url);
}
void GamieHentaiImageManager::slot_delete(){
    if(_has_delete)
        return;
    deleteLater();
    _has_delete = true;
}
void GamieHentaiImageManager::slot_timeout(){}
void GamieHentaiImageManager::setRequestURL(QString url){
    _request_url = url;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief GamieHentaiImageDownloaderManager::GamieHentaiImageDownloaderManager
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



GamieHentaiImageDownloaderManager::GamieHentaiImageDownloaderManager(){}
GamieHentaiImageDownloaderManager::~GamieHentaiImageDownloaderManager(){
    GamieHentaiGlobalSettings::global().delDownloaderManager(_save_to,this);
    extern void OnDownloaderManagerRelease(GamieHentaiImageDownloaderManager *m);
    OnDownloaderManagerRelease(this);
}
void GamieHentaiImageDownloaderManager::OnResponse(QByteArray &msg){
    qDebug() <<getRequestUrl()<< "downloaded";
    QString save_to = _save_to + "/";
    QString name = _image_name;
    save_to += name;
    int ret = -1;
    QFile file(save_to);
    if(file.open(QFile::WriteOnly)){
       qDebug() << getFixedImageName() << ", √";
       ret = file.write(msg);
       file.close();
       _manager->_has_free = true;
    }else
        OnRetry(0);
}
void GamieHentaiImageDownloaderManager::OnRetry(int count){
    qDebug() << __FUNCTION__ << getImageHref() << getImageName();


    GamieHentaiGlobalSettings::global().delDownloaderManager(_save_to,this);
    extern void OnDownloaderManagerRelease(GamieHentaiImageDownloaderManager *m);
    OnDownloaderManagerRelease(this);

    _net_manager->clearAccessCache();
    _net_manager->clearConnectionCache();

    QString image_href = _image_href;
    if(std::isnan(getProgress()) || _total == 0)
       image_href+= "?nl=39299-477344";
    emit signal_reconnect(image_href);
}
void GamieHentaiImageDownloaderManager::OnRequest(){
    _image_name = getImageName();
    GamieHentaiGlobalSettings::global().addDownloaderManager(_save_to, this);
}
void GamieHentaiImageDownloaderManager::OnProgressChange(qint64 current, qint64 total){
    _current = current;
    _total = total;
    _progress = double(current) / double(total);
    qDebug() << getFixedImageName() << ", progress->"<<(_progress * 100.0) << "%";
    if(std::isnan(getProgress()) || _total == 0){
        OnRetry(3);
    }
}
bool GamieHentaiImageDownloaderManager::hasFile(QString path){
    return QFile::exists(path);
}

void GamieHentaiImageDownloaderManager::setRequestURL(QString url){
    _request_url = url;
}
void GamieHentaiImageDownloaderManager::setImageHref(QString href){
    _image_href = href;
}

void GamieHentaiImageDownloaderManager::setImageManager(GamieHentaiImageManager *manager)   {
    _manager = manager;
}
QString GamieHentaiImageDownloaderManager::getImageHref(){
    return _image_href;
}
QString GamieHentaiImageDownloaderManager::getImageName(){
    QStringList list = _request_url.split("/");
    QString name = QDateTime::currentDateTime().toString("dd_MM_yyyy_h_m_s_")+".png";
    if(!list.isEmpty()){
        name =  list.at(list.size() - 1);
    }
    return name;
}
QString GamieHentaiImageDownloaderManager::getFixedImageName(){
    return _image_name;
}
