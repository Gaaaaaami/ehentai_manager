#include "GamieHentaiMainWindow.h"
#include "GamieHentaiObject.h"
#include "GamieHentaiMainIndexListItem.h"
#include "GamieHentaiImageManager.h"
#include "ui_GamieHentaiMainWindow.h"
#include <QListWidget>
#include <QListWidgetItem>
GamieHentaiMainWindow::GamieHentaiMainWindow(QWidget *parent) :
    QWidget(parent),_ehentai_object(nullptr),_range(0),
    ui(new Ui::GamieHentaiMainWindow){
    ui->setupUi(this);
    connect(&_default_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    _default_timer.start(16);
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
        qDebug() << it->getTitle() << it->getHref();
#if 0
        GamieHentaiImagePageIndexManager *im = new GamieHentaiImagePageIndexManager;
        im->setSaveTo(save);
        im->request(url);
#endif
    }
}

void GamieHentaiMainWindow::on_pushButton_search_clicked()
{
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
}

void GamieHentaiMainWindow::on_pushButton_next_clicked()
{
    ui->progressBar_load_progress->setValue(0);
    _range++;
    ui->listWidget_main_index_list->clear();

    QString url = _url + "&range=" + QString::number(_range*2);
    if(_ehentai_object->getRequestUrl().indexOf("f_search") == -1 )
        url = "https://e-hentai.org/?range="+QString::number(_range*2);
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
    if(_ehentai_object->getRequestUrl().indexOf("f_search") == -1 )
        url = "https://e-hentai.org/?range="+QString::number(_range*2);
    _ehentai_object->request(url);

    ui->lineEdit_skip_page->setText(QString::number(_range));

}
void GamieHentaiMainWindow::on_lineEdit_editingFinished(){}
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
        if(_ehentai_object->getRequestUrl().indexOf("f_search") == -1 )
            url = "https://e-hentai.org/?range="+QString::number(_range*2);
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
