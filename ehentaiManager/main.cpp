#include "GamieHentaiObject.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiGlobalSettings.h"
#include <QCoreApplication>
#include <QTimer>
#include <QElapsedTimer>
#include <QObject>

#ifdef USE_UI_SHOW
#include "GamieHentaiListDownloadInfo.h"
#include "GamieHentaiItemDownloadInfo.h"
#include <QListWidget>
#include <QApplication>

GamieHentaiListDownloadInfo *GeteHentaiListInstance(){
    static GamieHentaiListDownloadInfo  instance;
    return &instance;
}
#endif


GamieHentaiObject *GetObjectInstance(){
    static GamieHentaiObject w;
    return &w;
}


void OnDownloaderManagerRelease(GamieHentaiImageDownloaderManager *m){
#ifdef USE_UI_SHOW
    GeteHentaiListInstance()->delItemList(m);
#endif
}
void OnExec(){
    GamieHentaiGlobalSettings::global().execDownloaderManager([](QString d, GamieHentaiImageDownloaderManager *m){
#ifdef USE_UI_SHOW
        GeteHentaiListInstance()->addItemList(m);
        GeteHentaiListInstance()->udtItemList();
#endif
    });
}
int main(int argc, char *argv[]){
#ifdef USE_UI_SHOW
    QApplication a(argc, argv);
#else
    QCoreApplication a(argc, argv);
#endif

    GamieHentaiGlobalSettings::global();

#ifdef USE_UI_SHOW
    GeteHentaiListInstance()->resize(800,600);
    GeteHentaiListInstance()->show();
#endif
    QTimer default_timer;
    default_timer.start(16);
    QObject::connect(&default_timer, &QTimer::timeout, OnExec);

    GamieHentaiGlobalSettings::global().setUseNetProxy(true);
    GamieHentaiGlobalSettings::global().setSocks5Proxy("10.255.169.18", 65533);
    GetObjectInstance()->request(QString("https://e-hentai.org/?f_search=WaterRing"));

    return a.exec();
}

