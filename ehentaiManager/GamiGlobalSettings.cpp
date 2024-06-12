#include "GamiGlobalSettings.h"
#include <QNetworkProxy>

GamiGlobalSettings::GamiGlobalSettings(){
    _net_proxy = new QNetworkProxy;
}

GamiGlobalSettings::~GamiGlobalSettings(){
    delete _net_proxy;
}

GamiGlobalSettings &GamiGlobalSettings::global(){
    static GamiGlobalSettings instance;
    return instance;
}

void GamiGlobalSettings::setSocks5Proxy(QString address, unsigned short port)
{
    _net_proxy->setType(QNetworkProxy::Socks5Proxy);
    _net_proxy->setHostName(address);
    _net_proxy->setPort(port);

}

QNetworkProxy &GamiGlobalSettings::getProxy(){
    return  *_net_proxy;
}
