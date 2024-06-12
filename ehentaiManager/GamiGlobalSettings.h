#ifndef GAMIGLOBALSETTINGS_H
#define GAMIGLOBALSETTINGS_H
#include <QString>
#include "GamieHentaiParser.h"
class QNetworkAccessManager;
class QNetworkReply;
class QImage;
class QNetworkProxy;
class GamiGlobalSettings
{
public:
    GamiGlobalSettings();
    virtual ~GamiGlobalSettings();

public:
    static GamiGlobalSettings &global();
    void setSocks5Proxy(QString address, unsigned short port);
public:
    QNetworkProxy   &getProxy();
private:
    QNetworkProxy                   *_net_proxy;
};

#endif // GAMIGLOBALSETTINGS_H
