#ifndef GAMIEHENTAIIMAGEMANAGER_H
#define GAMIEHENTAIIMAGEMANAGER_H
#include "GamieHentaiObject.h"
#include "GamieHentaiGlobalSettings.h"

class QNetworkAccessManager;
class QNetworkReply;
class QImage;


class GamieHentaiImageHrefManager:public GamieHentaiObject
{
    Q_OBJECT
public:
    GamieHentaiImageHrefManager();
    virtual ~GamieHentaiImageHrefManager();
public:
    void            setPageIndexandHref(QVector<GamieHentaiParser::steHentaiItemInfo> &h);
public:
    virtual void OnResponse(QByteArray &msg) override;
protected:
    QVector<GamieHentaiParser::steHentaiItemInfo>               _page_index_and_href;
    QVector<GamieHentaiParser::steHentaiItemInfo>               _image_src_href;
};

class GamieHentaiImagePageIndexManager:public GamieHentaiObject
{
    Q_OBJECT
public:
    GamieHentaiImagePageIndexManager();
    virtual ~GamieHentaiImagePageIndexManager();
public:
    virtual void OnResponse(QByteArray &msg) override;
protected:
    QVector<GamieHentaiParser::steHentaiItemInfo>               _page_index_and_href;
    QVector<GamieHentaiParser::steHentaiItemInfo>               _image_src_href;
};

class GamieHentaiImageManager:public GamieHentaiObject
{
    Q_OBJECT
public:
    GamieHentaiImageManager();
    virtual ~GamieHentaiImageManager();
public:
    virtual void OnResponse(QByteArray &msg) override;
    virtual void OnRetry(int count) override;
public:
    bool hasResponse();
public slots:
    virtual void slot_delete();
public:
    void setRequestURL(QString url);
private:
    bool                        _has_delete;
    bool                        _has_res;

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
    void setImageHref(QString href);
    void setImageManager(GamieHentaiImageManager *manager);
public:
    QString getImageHref();
    QString getImageName();
    QString getFixedImageName();
signals:
    void signal_delete();
protected:
    QString                     _image_href;
    QString                     _image_name;
    GamieHentaiImageManager    *_manager;
private:
};

#endif // GAMIEHENTAIIMAGEMANAGER_H
