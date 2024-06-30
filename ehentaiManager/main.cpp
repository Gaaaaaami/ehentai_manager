#include "GamieHentaiObject.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiGlobalSettings.h"
#include "GamieHentaiMainWindow.h"
#include "GamieHentaiList.h"
#include "GamieHentaiDownloadInfoItem.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QObject>
#include <QApplication>
#include <QCoreApplication>

GamieHentaiList             *GetDownloadingList(){
    static GamieHentaiList w;
    return &w;
}
GamieHentaiMainWindow       *GetMainWindowInstance(){
    static GamieHentaiMainWindow w;
    return &w;
}
GamieHentaiObject           *GetObjectInstance(){
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
    GetDownloadingList();
    GetMainWindowInstance()->BindehentaiObject(GetObjectInstance());
    GetMainWindowInstance()->show();
    GetObjectInstance()->request(QString("https://e-hentai.org/"));
    return a.exec();
}

