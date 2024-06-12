#ifndef GAMIEHENTAIIMAGEMANAGER_H
#define GAMIEHENTAIIMAGEMANAGER_H
#include "GamieHentaiObject.h"
#include "GamiGlobalSettings.h"

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


class GamieHentaiImageManager:public GamieHentaiObject
{
    Q_OBJECT
public:
    GamieHentaiImageManager();
    virtual ~GamieHentaiImageManager();
public:
    virtual void OnResponse(QByteArray &msg) override;
protected:
    QVector<GamieHentaiParser::steHentaiItemInfo>               _page_index_and_href;
    QVector<GamieHentaiParser::steHentaiItemInfo>               _image_src_href;
};

#endif // GAMIEHENTAIIMAGEMANAGER_H
