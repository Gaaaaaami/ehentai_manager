#ifndef GAMIEHENTAIPARSER_H
#define GAMIEHENTAIPARSER_H

#include <QString>
#include <QStringList>
#include <QVector>
class GamieHentaiParser
{
public:
    typedef struct {
        unsigned int page_index;
        unsigned int index;
        QString herf;
        QString title;
    }steHentaiItemInfo;
public:
    GamieHentaiParser(QString msg);
    virtual ~GamieHentaiParser();
public:
    QVector<steHentaiItemInfo>                                   GetMainPageIndexList();
    QVector<steHentaiItemInfo>                                   GetImageList();
    QVector<steHentaiItemInfo>                                   GetImageList_PageHref();
    QVector<steHentaiItemInfo>                                   GetPageIndexToArray();
public:
    QString                                                      GetImageDownloadHref();
    QString                                                      GetErrorNL();

    unsigned int                                                 GetImageTotol();
public:
    static QString                                                      ToNormalURL(QString addr);
protected:
    QString                    GetTag(QString tag);
    QString                    GetTag(QString tag, QString classname);
    QString                    GetTagById(QString tag, QString id);

    QString                    GetContent(QString tag);
    QString                    GetAttribute(QString tag, QString attname);
private:
    QStringList                GetAllTag(QString tag);
    QStringList                GetAllTrTag();
    QStringList                GetAlltdTagInfo();
private:
    QString                    GetTagName(QString tag);
    QString                    GetTagEnd(QString tag, int start = -1);
    QString                    GetEnd(int start);
    QString                    GetClass(QString obj);
    QString                    GetID(QString obj);

private:
    QString                    _msg;
};

#endif // GAMIEHENTAIPARSER_H
