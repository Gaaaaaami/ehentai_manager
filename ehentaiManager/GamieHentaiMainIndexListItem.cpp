#include "GamieHentaiMainIndexListItem.h"
#include "ui_GamieHentaiMainIndexListItem.h"

GamieHentaiMainIndexListItem::GamieHentaiMainIndexListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamieHentaiMainIndexListItem)
{
    ui->setupUi(this);
}

GamieHentaiMainIndexListItem::~GamieHentaiMainIndexListItem(){
    delete ui;
}

QString GamieHentaiMainIndexListItem::getTitle()
{
    return ui->label_name->text();
}

QString GamieHentaiMainIndexListItem::getHref()
{
    return _href;
}
void GamieHentaiMainIndexListItem::setTitle(QString val){
    ui->label_name->setText( val);
}
void GamieHentaiMainIndexListItem::setHref(QString href){
    _href = href;
}
