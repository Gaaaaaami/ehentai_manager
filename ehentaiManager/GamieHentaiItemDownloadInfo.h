#ifndef GAMIEHENTAIITEMDOWNLOADINFO_H
#define GAMIEHENTAIITEMDOWNLOADINFO_H

#include <QWidget>
#include "GamieHentaiImageManager.h"
namespace Ui {
class GamieHentaiItemDownloadInfo;
}

class GamieHentaiItemDownloadInfo : public QWidget
{
    Q_OBJECT

public:
    explicit GamieHentaiItemDownloadInfo(QWidget *parent = nullptr);
    ~GamieHentaiItemDownloadInfo();
public:
    void setImageName(QString name);
    void setValue(double p);
    void setManager(GamieHentaiImageDownloaderManager *manager);
    void updateInfo();
public:
    GamieHentaiImageDownloaderManager *getManager();
private slots:
    void on_pushButton_redownload_clicked();

private:
    Ui::GamieHentaiItemDownloadInfo *ui;
private:
    GamieHentaiImageDownloaderManager   *_manager;
};

#endif // GAMIEHENTAIITEMDOWNLOADINFO_H
