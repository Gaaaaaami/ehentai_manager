#include "GamieHentaiList.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiDownloadInfoItem.h"
GamieHentaiList::GamieHentaiList(QWidget *parent) :
    QListWidget(parent)
{
}

GamieHentaiList::~GamieHentaiList()
{
}
bool GamieHentaiList::hasItem(GamieHentaiImageDownloaderManager *manger){
    for(int i = 0; i < this->count();i++){
        auto item = this->item(i);
        GamieHentaiDownloadInfoItem *widget = (GamieHentaiDownloadInfoItem*)this->itemWidget(item);
        if(widget->getManager() == manger){
            return true;
        }
    }
    return false;
}
void GamieHentaiList::udtItemList(){
    for(int i = 0; i < this->count();i++){
        auto item = this->item(i);
        GamieHentaiDownloadInfoItem *widget = dynamic_cast<GamieHentaiDownloadInfoItem*>(this->itemWidget(item));
        if(widget)
           widget->updateInfo();
    }
}
void GamieHentaiList::addItemList(GamieHentaiImageDownloaderManager *manager){
    if(!hasItem(manager)){
        QListWidgetItem *i = new QListWidgetItem;
        GamieHentaiDownloadInfoItem *item_info = new GamieHentaiDownloadInfoItem;
        item_info->setManager(manager);
        i->setSizeHint(QSize(0,80));
        this->addItem(i);
        this->setItemWidget(i, item_info);
    }
}
void GamieHentaiList::delItemList(GamieHentaiImageDownloaderManager *manager){
    for(int i = 0; i < this->count();i++){
        auto item = this->item(i);
        GamieHentaiDownloadInfoItem *widget = (GamieHentaiDownloadInfoItem*)this->itemWidget(item);
        if(widget->getManager() == manager){
            //qDebug() << __FUNCTION__ << ", index -> " << i;
            this->takeItem(i);
            return;
        }
    }
}
