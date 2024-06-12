#pragma once
#include <QObject>
#include "GamiGlobalSettings.h"
#include "GamieHentaiParser.h"
QT_BEGIN_NAMESPACE
namespace Ui { class GamiMainWidget; }
QT_END_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QImage;
class QPaintEvent;
class QResizeEvent;

class GamieHentaiObject : public QObject
{
    Q_OBJECT
public:
     GamieHentaiObject(QObject *parent = nullptr);
    ~GamieHentaiObject();
public:
     void request(QString url);
protected slots:
     virtual void OnFinished(QNetworkReply *reply);
     virtual void OnResponse(QByteArray &msg);
protected:
     virtual void Controller();
protected:
    QNetworkAccessManager           *_net_manager;
protected:
    QVector<GamieHentaiParser::steHentaiItemInfo>           _ehentai_main_index_list;
};
