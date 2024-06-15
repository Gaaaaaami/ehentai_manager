#include "GamieHentaiImageManager.h"
#include <cmath>
#include <math.h>
#include <algorithm>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QDebug>
#include <QFile>

GamieHentaiImagePageIndexManager::GamieHentaiImagePageIndexManager(){}
GamieHentaiImagePageIndexManager::~GamieHentaiImagePageIndexManager(){}
void GamieHentaiImagePageIndexManager::OnResponse(QByteArray &msg){

    GamieHentaiParser parser(msg);
    if(_page_index_and_href.size() == 0){
       _page_index_and_href = parser.GetImageList_PageHref();
       _image_src_href.append(parser.GetImageList());

        GamieHentaiImageHrefManager *m = new GamieHentaiImageHrefManager;
        m->setSaveTo(_save_to);
        m->setPageIndexandHref(_page_index_and_href);
        m->request(_page_index_and_href.begin()->herf);
    }

    deleteLater();
}
GamieHentaiImageHrefManager::GamieHentaiImageHrefManager(){}
GamieHentaiImageHrefManager::~GamieHentaiImageHrefManager(){}
void GamieHentaiImageHrefManager::setPageIndexandHref(QVector<GamieHentaiParser::steHentaiItemInfo> &h){
    _page_index_and_href = h;
}
void GamieHentaiImageHrefManager::OnResponse(QByteArray &msg){
    GamieHentaiParser parser(msg);
    _page_index_and_href.erase(_page_index_and_href.begin());
    auto list = parser.GetImageList();
    int index = 0;
    for(auto item : list){
        GamieHentaiImageManager *m = new GamieHentaiImageManager;
        m->setSaveTo(_save_to);
        m->setRequestURL(item.herf);
        //m->request(item.herf);
        GamieHentaiGlobalSettings::global().addImageManager(_save_to, m);
        index++;
    }
    if(!_page_index_and_href.empty()){
        GamieHentaiImageHrefManager *m = new GamieHentaiImageHrefManager;
        m->setSaveTo(_save_to);
        m->setPageIndexandHref(_page_index_and_href);
        m->request(_page_index_and_href.begin()->herf);
    }
    deleteLater();
}
GamieHentaiImageManager::GamieHentaiImageManager():_has_delete(false),_has_res(false){}
GamieHentaiImageManager::~GamieHentaiImageManager(){
    GamieHentaiGlobalSettings::global().delImageManager(_save_to, this);
    GamieHentaiGlobalSettings::global().delImageDownloadingManager(this);
}
void GamieHentaiImageManager::OnResponse(QByteArray &msg){
    GamieHentaiParser parser(msg);
    QString href = parser.GetImageDownloadHref();
    GamieHentaiImageDownloaderManager *m = new GamieHentaiImageDownloaderManager;

    _has_res = true;

    connect(m, SIGNAL(signal_delete()), this ,SLOT(slot_delete()),Qt::QueuedConnection);
    m->setImageHref(_request_url);
    m->setSaveTo(_save_to);
    m->setImageManager(this);
    m->request(href);
}
void GamieHentaiImageManager::OnRetry(int count){
    GamieHentaiGlobalSettings::global().delImageDownloadingManager(this);
    GamieHentaiGlobalSettings::global().addImageManager(_save_to, this);
}

bool GamieHentaiImageManager::hasResponse()
{
    return _has_res;
}

void GamieHentaiImageManager::slot_delete(){
    if(_has_delete)
        return;
    deleteLater();
    _has_delete = true;
}
void GamieHentaiImageManager::setRequestURL(QString url){
    _request_url = url;
}

GamieHentaiImageDownloaderManager::GamieHentaiImageDownloaderManager(){}
GamieHentaiImageDownloaderManager::~GamieHentaiImageDownloaderManager(){
    GamieHentaiGlobalSettings::global().delDownloaderManager(_save_to,this);
    emit signal_delete();
}
void GamieHentaiImageDownloaderManager::OnResponse(QByteArray &msg){
    QString save_to = _save_to + "/";
    QString name = _image_name;
    save_to += name;
    int ret = -1;
    QFile file(save_to);
    if(file.open(QFile::WriteOnly)){
       ret = file.write(msg);
       file.close();
    }
    deleteLater();
}
void GamieHentaiImageDownloaderManager::OnRetry(int count){
    GamieHentaiImageManager *m = new GamieHentaiImageManager;
    m->setSaveTo(_save_to);

    if(std::isnan(getProgress()))
       _image_href+= "?nl=39299-477344";
    m->setRequestURL(_image_href);

    GamieHentaiGlobalSettings::global().addImageManager(_save_to, m);
    deleteLater();
}
void GamieHentaiImageDownloaderManager::OnRequest(){
    _image_name = getImageName();
    GamieHentaiGlobalSettings::global().addDownloaderManager(_save_to, this);
}
void GamieHentaiImageDownloaderManager::OnProgressChange(qint64 current, qint64 total){
    _current = current;
    _total = total;
    _progress = double(current) / double(total);
    if(std::isnan(getProgress())){
        OnRetry(3);
    }
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
    QString name = QDateTime::currentDateTime().toString("dd_MM_yyyy_h_m_s_");
    if(!list.isEmpty()){
        name +=  list.at(list.size() - 1);
    }
    return name;
}
QString GamieHentaiImageDownloaderManager::getFixedImageName(){
    return _image_name;
}
