#include "GamieHentaiMainWindow.h"
#include "GamieHentaiObject.h"
#include "GamieHentaiMainIndexListItem.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiList.h"
#include "GamieHentaiDownloadInfoItem.h"
#include "ui_GamieHentaiMainWindow.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>

extern GamieHentaiList             *GetDownloadingList();

GamieHentaiMainWindow::GamieHentaiMainWindow(QWidget *parent) :
    QWidget(parent),_ehentai_object(nullptr),_range(0),
    ui(new Ui::GamieHentaiMainWindow){
    ui->setupUi(this);
    connect(&_default_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    _default_timer.start(16);
    _finished_mode = enuFinishedEditLine::SEARCH_IMAGE_NAME;
}
GamieHentaiMainWindow::~GamieHentaiMainWindow(){
    delete ui;
}
void GamieHentaiMainWindow::BindehentaiObject(GamieHentaiObject *object){
    _ehentai_object = object;
}

void GamieHentaiMainWindow::slot_timeout(){
    if(!_ehentai_object)
        return;

    double progress= _ehentai_object->getProgress();
    ui->progressBar_load_progress->setValue(progress * 100);
    if(_ehentai_object->getRequestUrl() != ui->lineEdit_url->text())
        ui->lineEdit_url->setText(_ehentai_object->getRequestUrl());

    for(auto ind : _ehentai_object->getMainIndexList()){
        QListWidgetItem *item = new QListWidgetItem;
        GamieHentaiMainIndexListItem *list_item = new GamieHentaiMainIndexListItem;
        ui->listWidget_main_index_list->addItem(item);
        ui->listWidget_main_index_list->setItemWidget(item  ,list_item);
        item->setSizeHint(QSize(0,list_item->height() ));
        list_item->setTitle(ind.title);
        list_item->setHref(ind.herf);
    }
    if(!_ehentai_object->getMainIndexList().empty())
        _ehentai_object->getMainIndexList().clear();
}

void GamieHentaiMainWindow::on_listWidget_main_index_list_itemPressed(QListWidgetItem *item)
{
    auto it = dynamic_cast<GamieHentaiMainIndexListItem *>(ui->listWidget_main_index_list->itemWidget(item));
    if(it){


        QString url = GamieHentaiParser::ToNormalURL(it->getHref());

        QString utf8_name = it->getTitle(); //_ehentai_main_index_list.at(index-1).title.toUtf8();
        QString save = QCoreApplication::applicationDirPath()+"/"+utf8_name;
        QString file_name;
        QDir dir;



        if(!dir.exists(save)){
            bool mkdir = false;
            mkdir = dir.mkdir(save);
            qDebug() << (mkdir ? "create dir success!" : "create dir error!");
            if(!mkdir){

                auto lst = it->getHref().split("/");
                utf8_name = lst.at(lst.size() - 2);
                save = QCoreApplication::applicationDirPath()+"/"+utf8_name;
                dir.mkdir(save);

#if 0
                utf8_name = QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss_zzz");
                save = QCoreApplication::applicationDirPath()+"/"+utf8_name;
                dir.mkdir(save);
#endif
            }

        }


        QFile file(save+"/"+"name.txt");
        if(file.open(QFile::WriteOnly)){
            QString s = it->getTitle() + "\n"+it->getHref();
            file.write(s.toUtf8());
            file.close();
        }

        qDebug() << it->getTitle() << it->getHref();

        GamieHentaiGlobalSettings::stDownloadListItem download_list_item;
        download_list_item.save =save;
        download_list_item.href =url;
        download_list_item.state =GamieHentaiGlobalSettings::NO_START_DOWNLOADING;
        GamieHentaiGlobalSettings::global().addDownloadListItem(download_list_item);

        GamieHentaiDownloadInfoItem *item = new GamieHentaiDownloadInfoItem;
        item->setKey(save);
        item->setImageName(it->getTitle());
        GetDownloadingList()->addItemList( item);

#if 0
        GamieHentaiImagePageIndexManager *im = new GamieHentaiImagePageIndexManager;
        im->setSaveTo(save);
        im->request(url);
#endif
    }
}

void GamieHentaiMainWindow::on_pushButton_search_clicked()
{
    if( enuFinishedEditLine::SEARCH_IMAGE_NAME == _finished_mode){
        QString search_text = ui->lineEdit_search->text();
        for(auto &it : search_text)
            if(it == " ")
               it = '+';
        ui->listWidget_main_index_list->clear();
        ui->progressBar_load_progress->setValue(0);
        QString url = "https://e-hentai.org/";
        url += "?f_search="+search_text;
        _url  =url;
        _ehentai_object->request(url);
        _range = 0;
        ui->lineEdit_skip_page->setText(QString::number(_range));
    }else if( enuFinishedEditLine::SEARCH_IAMGE_UPLOADER == _finished_mode){
        QString search_text = ui->lineEdit_uploader->text();
        for(auto &it : search_text)
            if(it == " ")
               it = '+';
        ui->listWidget_main_index_list->clear();
        ui->progressBar_load_progress->setValue(0);
        QString url = "https://e-hentai.org/uploader/" + search_text;
        _url  =url;
       _ehentai_object->request(url);
        _range = 0;
        ui->lineEdit_skip_page->setText(QString::number(_range));
    }

}

void GamieHentaiMainWindow::on_pushButton_next_clicked()
{
    ui->progressBar_load_progress->setValue(0);
    _range++;
    ui->listWidget_main_index_list->clear();

    QString url = _url + "&range=" + QString::number(_range*2);

    if(_ehentai_object->getRequestUrl().indexOf("f_search") == -1 &&
       _ehentai_object->getRequestUrl().indexOf("e-hentai.org/uploader") == -1)
        url = "https://e-hentai.org/?range="+QString::number(_range*2);
    else  if(_ehentai_object->getRequestUrl().indexOf("e-hentai.org/uploader") != -1){
        url = _url +"?range="+QString::number(_range*2);
    }

    _ehentai_object->request(url);

    ui->lineEdit_skip_page->setText(QString::number(_range));

}

void GamieHentaiMainWindow::on_pushButton_prev_clicked()
{
    ui->progressBar_load_progress->setValue(0);
    if(_range > 0)
       _range--;
    else
        return;
    ui->listWidget_main_index_list->clear();

    QString url = _url + "&range=" + QString::number(_range*2);
    if(_ehentai_object->getRequestUrl().indexOf("f_search") == -1 &&
       _ehentai_object->getRequestUrl().indexOf("e-hentai.org/uploader") == -1)
        url = "https://e-hentai.org/?range="+QString::number(_range*2);
    else  if(_ehentai_object->getRequestUrl().indexOf("e-hentai.org/uploader") != -1){
        url = _url +"?range="+QString::number(_range*2);
    }
    _ehentai_object->request(url);

    ui->lineEdit_skip_page->setText(QString::number(_range));

}
void GamieHentaiMainWindow::on_lineEdit_skip_page_textChanged(const QString &arg1)
{
    bool ok = false;
    QString c = ui->lineEdit_skip_page->text();
    int range = c.toUInt(&ok);
    if( ok){
        _range = range;
        ui->progressBar_load_progress->setValue(0);
        ui->listWidget_main_index_list->clear();
        QString url = _url + "&range=" + QString::number(_range*2);
        if(_ehentai_object->getRequestUrl().indexOf("f_search") == -1 &&
           _ehentai_object->getRequestUrl().indexOf("e-hentai.org/uploader") == -1)
            url = "https://e-hentai.org/?range="+QString::number(_range*2);
        else  if(_ehentai_object->getRequestUrl().indexOf("e-hentai.org/uploader") != -1){
            url = _url +"?range="+QString::number(_range*2);
        }
        _ehentai_object->request(url);
    }else{
        QString n =arg1;
        QString final = "";
        for(auto &it : n ){
            if(it.isNumber())
                final += it;
        }
        ui->lineEdit_skip_page->setText(final);
    }
}

void GamieHentaiMainWindow::on_lineEdit_uploader_editingFinished() {

    _finished_mode = enuFinishedEditLine::SEARCH_IAMGE_UPLOADER;

}
void GamieHentaiMainWindow::on_lineEdit_search_editingFinished()
{

    _finished_mode = enuFinishedEditLine::SEARCH_IMAGE_NAME;
}

void GamieHentaiMainWindow::on_pushButton_download_list_clicked(){
    extern GamieHentaiList             *GetDownloadingList();
    GetDownloadingList()->show();
    GetDownloadingList()->resize(800,600);
}
