#ifndef GAMIEHENTAIIMAGEMANAGER_H
#define GAMIEHENTAIIMAGEMANAGER_H
#include "GamieHentaiObject.h"
#include "GamieHentaiGlobalSettings.h"
#include <QTimer>

class QNetworkAccessManager;
class QNetworkReply;
class QImage;
class GamieHentaiImagePageIndexManager;
class GamieHentaiImageHrefManager;

class GamieHentaiImagePageIndexManager:public GamieHentaiObject
{
    Q_OBJECT
public:
    GamieHentaiImagePageIndexManager();
    virtual ~GamieHentaiImagePageIndexManager();
public:
     GamieHentaiImageManager *getValidImageManager();
public:
    virtual void OnResponse(QByteArray &msg) override;
protected slots:
    virtual void slot_timeout();
    virtual void OnHasNewImageLink(QVector<GamieHentaiParser::steHentaiItemInfo> &v);
protected:
    QVector<GamieHentaiImageManager *>                          _image_manager_pool;
    QVector<GamieHentaiParser::steHentaiItemInfo>               _page_href;
    QQueue<GamieHentaiParser::steHentaiItemInfo>                _image_href_list;
    GamieHentaiImageHrefManager                                *_m;
    int                                                         _ind;
    bool                                                        _wait;
protected:
    QTimer                                                      _default_timer;
};

class GamieHentaiImageHrefManager:public GamieHentaiObject
{
    Q_OBJECT
public:
    GamieHentaiImageHrefManager();
    virtual ~GamieHentaiImageHrefManager();
public:
    virtual void         OnResponse(QByteArray &msg) override;
signals:
    void                 signal_has_image_link(QVector<GamieHentaiParser::steHentaiItemInfo> &);
protected:

};






class GamieHentaiImageDownloaderManager;
class GamieHentaiImageManager:public GamieHentaiObject
{
    Q_OBJECT
    friend class GamieHentaiImageDownloaderManager;
public:
    GamieHentaiImageManager();
    virtual ~GamieHentaiImageManager();
public:
    virtual void OnRequest() override;
    virtual void OnResponse(QByteArray &msg) override;
public:
    bool hasResponse();
    bool hasFree();
public slots:
    virtual void slot_reconnect(QString url);
    virtual void slot_delete();
    virtual void slot_timeout();

public:
    void setRequestURL(QString url);
private:
    bool                        _has_delete;
    bool                        _has_res;
    bool                        _has_req;
    bool                        _has_reconnect;
    bool                        _has_free;
private:
    GamieHentaiImageDownloaderManager       *_m;
private:
    QTimer                                   _default_timeout;
};




class GamieHentaiImageDownloaderManager:public GamieHentaiObject
{
    Q_OBJECT
public:
    GamieHentaiImageDownloaderManager();
    virtual ~GamieHentaiImageDownloaderManager();
public:
    virtual void OnResponse(QByteArray &msg) override;
    virtual void OnRetry(int count) override;
    virtual void OnRequest() override;
    virtual void OnProgressChange(qint64 current, qint64 total) override;

public:
    static bool hasFile(QString path);
    void setRequestURL(QString url);
    void setImageHref(QString href);
    void setImageManager(GamieHentaiImageManager *manager);
public:
    QString getImageHref();
    QString getImageName();
    QString getFixedImageName();
signals:
    void signal_delete();
    void signal_reconnect(QString url);
protected:
    QString                     _image_href;
    QString                     _image_name;
    GamieHentaiImageManager    *_manager;
};

#endif // GAMIEHENTAIIMAGEMANAGER_H
