#include "GamieHentaiObject.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiGlobalSettings.h"
#include "GamieHentaiMainWindow.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QObject>
#include <QApplication>
#include <QCoreApplication>

GamieHentaiObject *GetObjectInstance(){
    static GamieHentaiObject w;
    return &w;
}
void OnDownloaderManagerRelease(GamieHentaiImageDownloaderManager *m){}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    GamieHentaiGlobalSettings::global().setUseNetProxy(true);
    GamieHentaiGlobalSettings::global().setSocks5Proxy("10.255.169.18", 65533);
    GamieHentaiMainWindow w;
    w.BindehentaiObject(GetObjectInstance());
    w.show();
    GetObjectInstance()->request(QString("https://e-hentai.org/"));
    return a.exec();
}

