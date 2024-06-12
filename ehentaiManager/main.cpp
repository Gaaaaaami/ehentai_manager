
#include "GamieHentaiObject.h"
#include "GamieHentaiImageManager.h"
#include "GamiGlobalSettings.h"
#include <QCoreApplication>


GamieHentaiObject *GetObjectInstance(){
    static GamieHentaiObject w;
    return &w;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GamiGlobalSettings::global().setSocks5Proxy("10.255.169.18", 65533);
    GetObjectInstance()->request(QString("https://e-hentai.org/"));

    return a.exec();
}

