#include "GamieHentaiObject.h"
#include "GamieHentaiImageManager.h"
#include "GamieHentaiGlobalSettings.h"
#include <QCoreApplication>
#include <QTimer>
#include <QElapsedTimer>
#include <QObject>



GamieHentaiObject *GetObjectInstance(){
    static GamieHentaiObject w;
    return &w;
}
void OnExec(){
        int m = GamieHentaiGlobalSettings::global().dwonloaderMaxSize();
        bool output_once = false;
        static int start = GamieHentaiGlobalSettings::global().runtime();
        int end = GamieHentaiGlobalSettings::global().runtime();
        bool has_data= false;
        bool has_timeout = false;
        if((end - start) >= 1000){
           start = GamieHentaiGlobalSettings::global().runtime();
           has_timeout = true;
        }

        GamieHentaiGlobalSettings::global().execImageManager([&output_once,&has_timeout,&m](QString dir ,GamieHentaiImageManager*item){
            int n = GamieHentaiGlobalSettings::global().imageDownloadingSize();
            int k = GamieHentaiGlobalSettings::global().imageManagerSize(dir);


#if 0
            if(!output_once && has_timeout){
               auto vector = GamieHentaiGlobalSettings::global().getImageDownloadingVector();
               for(auto  item : vector){
                   qDebug() << item->getRequestUrl() << ", res->"<< item->hasResponse();
               }
               output_once = true;
            }
#endif

            if(n >= m)
               return true;
            GamieHentaiGlobalSettings::global().addImageDownloadingManager(item);
            QString url = item->getRequestUrl();
            item->request(url);
            //qDebug() << __FUNCTION__<< url;
            return false;
        },m);

        if(has_timeout){
            GamieHentaiGlobalSettings::global().execDownloaderManager([&has_data](QString dir ,GamieHentaiImageDownloaderManager *manager){
                qDebug() << dir ;
                qDebug() << manager->getFixedImageName() << "["<< int(manager->getProgress()*100.0)<<"%]";
                has_data = true;
            });
            if(has_data)
                qDebug() << "**************************************************************************";
        }
}
int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);




    GamieHentaiGlobalSettings::global();
    QTimer default_timer;
    default_timer.start(16);
    QObject::connect(&default_timer, &QTimer::timeout, OnExec);

    GamieHentaiGlobalSettings::global().setUseNetProxy(true);
    GamieHentaiGlobalSettings::global().setSocks5Proxy("10.255.169.18", 65533);
    GetObjectInstance()->request(QString("https://e-hentai.org/?f_search=gif"));


    return a.exec();
}

