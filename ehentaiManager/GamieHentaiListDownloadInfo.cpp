#include "GamieHentaiListDownloadInfo.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiItemDownloadInfo.h"
GamieHentaiListDownloadInfo::GamieHentaiListDownloadInfo(QWidget *parent) :
    QListWidget(parent)
{
}

GamieHentaiListDownloadInfo::~GamieHentaiListDownloadInfo()
{
}
bool GamieHentaiListDownloadInfo::hasItem(GamieHentaiImageDownloaderManager *manger){
    for(int i = 0; i < this->count();i++){
        auto item = this->item(i);
        GamieHentaiItemDownloadInfo *widget = (GamieHentaiItemDownloadInfo*)this->itemWidget(item);
        if(widget->getManager() == manger){
            return true;
        }
    }
    return false;
}
void GamieHentaiListDownloadInfo::udtItemList(){
    for(int i = 0; i < this->count();i++){
        auto item = this->item(i);
        GamieHentaiItemDownloadInfo *widget = dynamic_cast<GamieHentaiItemDownloadInfo*>(this->itemWidget(item));
        if(widget)
           widget->updateInfo();
    }
}
void GamieHentaiListDownloadInfo::addItemList(GamieHentaiImageDownloaderManager *manager){
    if(!hasItem(manager)){
        QListWidgetItem *i = new QListWidgetItem;
        GamieHentaiItemDownloadInfo *item_info = new GamieHentaiItemDownloadInfo;
        item_info->setManager(manager);
        i->setSizeHint(QSize(0,80));
        this->addItem(i);
        this->setItemWidget(i, item_info);
    }
}
void GamieHentaiListDownloadInfo::delItemList(GamieHentaiImageDownloaderManager *manager){
    for(int i = 0; i < this->count();i++){
        auto item = this->item(i);
        GamieHentaiItemDownloadInfo *widget = (GamieHentaiItemDownloadInfo*)this->itemWidget(item);
        if(widget->getManager() == manager){
            qDebug() << __FUNCTION__ << ", index -> " << i;
            this->takeItem(i);
            return;
        }
    }
}
