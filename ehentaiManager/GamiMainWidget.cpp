#include "GamiMainWidget.h"
#include "GamieHentaiParser.h"
#include "ui_GamiMainWidget.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QDateTime>
GamiMainWidget::GamiMainWidget(QWidget *parent,QString socks5_ip, unsigned short socks5_port)
    : QWidget(parent)
    , ui(new Ui::GamiMainWidget),
    _net_manager(new QNetworkAccessManager(this)){
    qDebug()<<QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();//若输出：OpenSSL支持情况: false，那说明当前 Qt 不支持 OpenSSL
    for(auto supported : _net_manager->supportedSchemes())
        qDebug() << supported;
    ui->setupUi(this);
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName(socks5_ip);
    proxy.setPort(socks5_port);
    _net_manager->setProxy(proxy);
    connect(_net_manager, &QNetworkAccessManager::finished, this, &GamiMainWidget::OnFinished);
    QNetworkRequest request;
    request.setUrl(QUrl("https://e-hentai.org/g/2947169/980c019daf/"));
    _net_manager->get(request);
}
GamiMainWidget::~GamiMainWidget(){
    delete ui;
    delete _net_manager;
    delete _canvas;
}

void GamiMainWidget::ResizeCanvas(){
    if(!_canvas)
        delete _canvas;
    _canvas = new QImage(width(),height(),QImage::Format_RGB16);
    _canvas->fill(Qt::transparent);
}
void GamiMainWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawImage(0,0,*_canvas);
}
void GamiMainWidget::resizeEvent(QResizeEvent *event){
    ResizeCanvas();
}
void GamiMainWidget::OnFinished(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            OnResponse(response);
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
    reply->deleteLater();
}
void GamiMainWidget::OnResponse(QByteArray &msg){
#if 0

    QImage im;
    im.loadFromData(msg);

    QString name = QDateTime::currentDateTime().toString("hh_mm_ss_zzz");
    QString save_path = QApplication::applicationDirPath()+"/"+name+".jpg";//im.save(QApplication::applicationDirPath()+"/"+name+".jpg","jpg");
    im.save(save_path);

    QPainter painter(_canvas);
    painter.drawImage(0,0,im.scaled(_canvas->width(),_canvas->height()));
    update();

#else


#if 0
    GamieHentaiParser parser(msg);
    auto list = parser.GetMainPageIndexList();
    for(auto item: list)
        qDebug() << item.title << item.herf;

#else
    GamieHentaiParser parser(msg);
    auto list = parser.GetImageList_PageHref();
    for(auto item : list)
        qDebug() << item.herf;

#endif






#endif
}

