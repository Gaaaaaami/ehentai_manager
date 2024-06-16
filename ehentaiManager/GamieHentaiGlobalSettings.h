#pragma once
#include <QString>
#include <QElapsedTimer>
#include <QMap>
#include <QQueue>
#include "GamieHentaiParser.h"
class QNetworkAccessManager;
class QNetworkReply;
class QImage;
class QNetworkProxy;
class GamieHentaiImageDownloaderManager;
class GamieHentaiImageManager;
class GamieHentaiGlobalSettings{
public:
    GamieHentaiGlobalSettings();
    virtual ~GamieHentaiGlobalSettings();

public:
    static GamieHentaiGlobalSettings &global();
    void setSocks5Proxy(QString url, unsigned short port);
    void setUseNetProxy(bool val);
public:
    void addDownloaderManager(QString dir, GamieHentaiImageDownloaderManager*  p);
    void delDownloaderManager(QString dir, GamieHentaiImageDownloaderManager*  p);
    void execDownloaderManager(std::function<void(QString , GamieHentaiImageDownloaderManager *)> fun);


public:
    qint64          runtime();
    unsigned int    imageDownloadingSize();
public:
    bool             useProxy();
    QNetworkProxy   &getProxy();
private:
    QNetworkProxy                                                           *_net_proxy;
    bool                                                                     _use_net_proxy;
private:
    QMap<QString,QVector<GamieHentaiImageDownloaderManager *>>               _download_manager;
    QElapsedTimer                                                            _runtime;
private:
    unsigned int                                                             _max_download_number;
};

