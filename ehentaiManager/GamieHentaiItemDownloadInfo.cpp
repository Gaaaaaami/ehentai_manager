#include "GamieHentaiItemDownloadInfo.h"
#include "ui_GamieHentaiItemDownloadInfo.h"

GamieHentaiItemDownloadInfo::GamieHentaiItemDownloadInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamieHentaiItemDownloadInfo)
{
    ui->setupUi(this);
}

GamieHentaiItemDownloadInfo::~GamieHentaiItemDownloadInfo(){
    delete ui;
}
void GamieHentaiItemDownloadInfo::setImageName(QString name){
    ui->label_image_name->setText(name);
}

void GamieHentaiItemDownloadInfo::setValue(double p){
    ui->progressBar->setValue(p);
}

void GamieHentaiItemDownloadInfo::setManager(GamieHentaiImageDownloaderManager *manager){
    _manager = manager;
}

void GamieHentaiItemDownloadInfo::updateInfo()
{
    double p = _manager->getProgress() * 100.0;
    setValue(p);
    setImageName(_manager->getImageName());
}
GamieHentaiImageDownloaderManager *GamieHentaiItemDownloadInfo::getManager(){
    return _manager;
}
void GamieHentaiItemDownloadInfo::on_pushButton_redownload_clicked(){
    _manager->OnRetry(0);
}
