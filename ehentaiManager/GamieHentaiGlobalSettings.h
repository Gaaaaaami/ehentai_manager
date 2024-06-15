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
    void addImageManager(QString dir, GamieHentaiImageManager*  p);
    void delImageManager(QString dir, GamieHentaiImageManager*  p);
    void execImageManager(std::function<void(QString , GamieHentaiImageManager *)> fun);
    void execImageManager(std::function<bool(QString , GamieHentaiImageManager *)> fun, int n);

public:
    void addImageDownloadingManager( GamieHentaiImageManager*  p);
    void delImageDownloadingManager( GamieHentaiImageManager*  p);

public:
    qint64          runtime();
    unsigned int    imageDownloadingSize();
    unsigned int    downloaderManagerSize(QString dir);
    unsigned int    imageManagerSize(QString dir);
    unsigned int    dwonloaderMaxSize();
    QVector<GamieHentaiImageManager *> &getImageDownloadingVector();
public:
    bool             useProxy();
    QNetworkProxy   &getProxy();
private:
    QNetworkProxy                                                           *_net_proxy;
    bool                                                                     _use_net_proxy;
private:
    QMap<QString,QVector<GamieHentaiImageDownloaderManager *>>               _download_manager;
    QMap<QString,QQueue<GamieHentaiImageManager *>>                          _image_manager;
    QVector<GamieHentaiImageManager *>                                       _image_downloading_manager;


    QElapsedTimer                                                            _runtime;
private:
    unsigned int                                                             _max_download_number;
};

