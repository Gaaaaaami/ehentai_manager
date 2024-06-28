#include "GamieHentaiList.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiDownloadInfoItem.h"
GamieHentaiList::GamieHentaiList(QWidget *parent) :
    QListWidget(parent){}
GamieHentaiList::~GamieHentaiList(){}
void GamieHentaiList::udtItemList(){
    for(int i = 0; i < this->count();i++){
        auto item = this->item(i);
        GamieHentaiDownloadInfoItem *widget = dynamic_cast<GamieHentaiDownloadInfoItem*>(this->itemWidget(item));
        if(widget)
           widget->updateInfo();
    }
}
void GamieHentaiList::addItemList(GamieHentaiDownloadInfoItem *it){
    QListWidgetItem *list_it = new QListWidgetItem;
    list_it->setSizeHint(QSize(0, it->height()));
    this->addItem(list_it);
    this->setItemWidget(list_it, it);
}
void GamieHentaiList::delItemList(GamieHentaiDownloadInfoItem *it){
    for(int i = 0; i < this->count(); i++){
        QListWidgetItem *item= this->item(i);
        QWidget *widget = this->itemWidget(item);
        if(widget == it)
        {
            this->takeItem(i);
            return;
        }
    }
}
