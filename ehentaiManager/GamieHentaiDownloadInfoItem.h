#pragma once

#include <QWidget>
#include "GamieHentaiImageManager.h"
namespace Ui {
class GamieHentaiItemDownloadInfo;
}

class GamieHentaiDownloadInfoItem : public QWidget
{
    Q_OBJECT

public:
    explicit GamieHentaiDownloadInfoItem(QWidget *parent = nullptr);
    ~GamieHentaiDownloadInfoItem();
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

