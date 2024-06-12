#include "GamieHentaiImageManager.h"
#include <QNetworkAccessManager>
#include <QDebug>
GamieHentaiImageManager::GamieHentaiImageManager(){}
GamieHentaiImageManager::~GamieHentaiImageManager(){}
void GamieHentaiImageManager::OnResponse(QByteArray &msg){

    GamieHentaiParser parser(msg);
    if(_page_index_and_href.size() == 0){
       _page_index_and_href = parser.GetImageList_PageHref();
       _image_src_href.append(parser.GetImageList());


        qDebug() << "page size -> "<< _page_index_and_href.size();
        GamieHentaiImageHrefManager *m = new GamieHentaiImageHrefManager;
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
    qDebug()<<"downloaded->" << _page_index_and_href.begin()->herf;
    _page_index_and_href.erase(_page_index_and_href.begin());
    auto list = parser.GetImageList();
    int index = 0;
    for(auto item : list){
        qDebug() << index << item.herf;
        index++;
    }
    if(!_page_index_and_href.empty()){
        GamieHentaiImageHrefManager *m = new GamieHentaiImageHrefManager;
        m->setPageIndexandHref(_page_index_and_href);
        m->request(_page_index_and_href.begin()->herf);
        qDebug() << "next -> " << _page_index_and_href.begin()->herf;
    }else{
        extern GamieHentaiObject *GetObjectInstance();
        GetObjectInstance()->request(QString("https://e-hentai.org/"));
    }
    deleteLater();
}
