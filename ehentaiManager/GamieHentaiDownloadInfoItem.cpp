#include "GamieHentaiDownloadInfoItem.h"
#include "ui_GamieHentaiDownloadInfoItem.h"
#include "GamieHentaiGlobalSettings.h"
#include "math.h"
GamieHentaiDownloadInfoItem::GamieHentaiDownloadInfoItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamieHentaiItemDownloadInfo){
    ui->setupUi(this);
}
GamieHentaiDownloadInfoItem::~GamieHentaiDownloadInfoItem(){
    delete ui;
}

void GamieHentaiDownloadInfoItem::setKey(QString key){
    _key = key;
}
void GamieHentaiDownloadInfoItem::setImageName(QString name){
    ui->label_image_name->setText(name);
}
void GamieHentaiDownloadInfoItem::setValue(double p){
    ui->progressBar->setValue(p);
}
void GamieHentaiDownloadInfoItem::updateInfo(){
    unsigned int success,failed,downloading;
    bool n = GamieHentaiGlobalSettings::global().checkImageDownloadIsDone(_key, success, failed , downloading);
    unsigned int image_total = GamieHentaiGlobalSettings::global().getImageTotal(_key);

    auto state = GamieHentaiGlobalSettings::global().getDownloadListItem(_key);
    if(GamieHentaiGlobalSettings::enuImageDownloadStatus::NO_START_DOWNLOADING==state.state){
        ui->label_state->setText("nsd");
    }else if(GamieHentaiGlobalSettings::enuImageDownloadStatus::DOWNLOADING==state.state){
        ui->label_state->setText("dod");

    }else if(GamieHentaiGlobalSettings::enuImageDownloadStatus::SUCCESS ==state.state){
        ui->label_state->setText("suc");

    }else if(GamieHentaiGlobalSettings::enuImageDownloadStatus::FAILED ==state.state){
        ui->label_state->setText("fad");

    }

    double  p = double(success+failed) /double(image_total);
    setValue(round(p*100.0));
    if(GamieHentaiGlobalSettings::enuImageDownloadStatus::NO_START_DOWNLOADING==state.state){
        ui->label_download_real_prograss->setText("");
    }else
        ui->label_download_real_prograss->setText(QString::number(success+failed) + "/" + QString::number(image_total));
}
void GamieHentaiDownloadInfoItem::on_pushButton_redownload_clicked(){}
