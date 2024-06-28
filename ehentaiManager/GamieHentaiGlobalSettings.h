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
class GamieHentaiImagePageIndexManager;

class GamieHentaiGlobalSettings{
public:
    enum enuImageDownloadStatus{
        SUCCESS,
        FAILED,
        DOWNLOADING,
        NO_START_DOWNLOADING,
    };
    typedef struct{
        enuImageDownloadStatus status;
        QString title;
        QString href;
    }stDownloadFileStatus;
    typedef struct{
        QString href;
        QString save;
        enuImageDownloadStatus    state;
    }stDownloadListItem;
public:
    GamieHentaiGlobalSettings();
    virtual ~GamieHentaiGlobalSettings();

public:
    static GamieHentaiGlobalSettings &global();
    void setSocks5Proxy(QString url, unsigned short port);
    void setUseNetProxy(bool val);
public:
    void addDownloaderManager(QString dir, GamieHentaiGlobalSettings::stDownloadFileStatus  p);
    void addImageTotal(QString dir ,unsigned int total);
    void modDownloaderManager(QString dir, QString request_url, GamieHentaiGlobalSettings::enuImageDownloadStatus status);
    void delDownloaderManager(QString dir, GamieHentaiGlobalSettings::stDownloadFileStatus  p);
    void execDownloaderManager(std::function<void(QString , GamieHentaiGlobalSettings::stDownloadFileStatus)> fun);
    void execDownloaderManager(std::function<void(QString )> fun);
public:
    bool         checkImageDownloadIsDone(QString dir, unsigned int &success, unsigned int &failed, unsigned int &downloading);
    unsigned int getImageTotal(QString dir);
    QVector<GamieHentaiGlobalSettings::stDownloadFileStatus>    &getDownloadManager(QString dir);
public:
    void addDownloadListItem(stDownloadListItem &item);
    void modDownloadListItemState(QString save , GamieHentaiGlobalSettings::enuImageDownloadStatus );
    void downloadNextNoDownloadStateListItem();
    bool hasDownloadListItemDownloading();
    stDownloadListItem getDownloadListItem(QString dir);
public:
    void update();
public:
    qint64          runtime();
    unsigned int    getDownloadSize(QString dir);
public:
    bool             useProxy();
    QNetworkProxy   &getProxy();
private:
    QNetworkProxy                                                           *_net_proxy;
    bool                                                                     _use_net_proxy;
private:
    QMap<QString,QVector<GamieHentaiGlobalSettings::stDownloadFileStatus>>   _download_manager;
    QMap<QString,unsigned int>                                               _image_total;
    QElapsedTimer                                                            _runtime;
private:
    QVector<stDownloadListItem>                                              _download_list_item;
private:
    unsigned int                                                             _max_download_number;
private:
    GamieHentaiImagePageIndexManager                                        *_page_index_manager;

};

