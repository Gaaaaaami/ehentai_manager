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
    GamieHentaiGlobalSettings::global().execDownloaderManager([](QString dir){

        unsigned int success,failed,downloading;
        bool n = GamieHentaiGlobalSettings::global().checkImageDownloadIsDone(dir, success, failed , downloading);
        unsigned int image_total = GamieHentaiGlobalSettings::global().getImageTotal(dir);
        qDebug() << dir << ", " << (success+failed) << "/" << image_total <<  n;

    });
}
int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QTimer exec_timer;
    QObject::connect(&exec_timer, &QTimer::timeout, OnExec);
    exec_timer.start(800);
    GamieHentaiGlobalSettings::global().setUseNetProxy(true);
    GamieHentaiGlobalSettings::global().setSocks5Proxy("10.255.169.18", 65533);
    GamieHentaiMainWindow w;
    w.BindehentaiObject(GetObjectInstance());
    w.show();
    GetObjectInstance()->request(QString("https://e-hentai.org/"));
    return a.exec();
}

