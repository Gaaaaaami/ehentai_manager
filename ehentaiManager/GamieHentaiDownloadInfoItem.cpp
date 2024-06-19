#include "GamieHentaiDownloadInfoItem.h"
#include "ui_GamieHentaiDownloadInfoItem.h"

GamieHentaiDownloadInfoItem::GamieHentaiDownloadInfoItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamieHentaiItemDownloadInfo)
{
    ui->setupUi(this);
}

GamieHentaiDownloadInfoItem::~GamieHentaiDownloadInfoItem(){
    delete ui;
}
void GamieHentaiDownloadInfoItem::setImageName(QString name){
    ui->label_image_name->setText(name);
}

void GamieHentaiDownloadInfoItem::setValue(double p){
    ui->progressBar->setValue(p);
}

void GamieHentaiDownloadInfoItem::setManager(GamieHentaiImageDownloaderManager *manager){
    _manager = manager;
}

void GamieHentaiDownloadInfoItem::updateInfo()
{
    double p = _manager->getProgress() * 100.0;
    setValue(p);
    setImageName(_manager->getImageName());
}
GamieHentaiImageDownloaderManager *GamieHentaiDownloadInfoItem::getManager(){
    return _manager;
}
void GamieHentaiDownloadInfoItem::on_pushButton_redownload_clicked(){
    _manager->OnRetry(0);
}
