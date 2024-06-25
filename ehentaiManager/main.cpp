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
void OnExec(){
   GamieHentaiGlobalSettings::global().update();
}
int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QTimer exec_timer;
    QObject::connect(&exec_timer, &QTimer::timeout, OnExec);
    exec_timer.start(800);

    GamieHentaiGlobalSettings::global().setUseNetProxy(true);
    GamieHentaiGlobalSettings::global().setSocks5Proxy("10.255.169.18", 65533);

#if 1
    GamieHentaiMainWindow w;
    w.BindehentaiObject(GetObjectInstance());
    w.show();
    GetObjectInstance()->request(QString("https://e-hentai.org/"));
#else
    GamieHentaiImageManager *im =new GamieHentaiImageManager;
    im->setSaveTo("./");
    im->request("https://e-hentai.org/s/87d5dfb51a/2812298-10");
#endif
    return a.exec();
}

