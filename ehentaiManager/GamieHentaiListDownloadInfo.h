#ifndef GAMIEHENTAILISTDOWNLOADINFO_H
#define GAMIEHENTAILISTDOWNLOADINFO_H

#include <QWidget>
#include <QListWidget>


class GamieHentaiImageDownloaderManager;
class GamieHentaiListDownloadInfo : public QListWidget
{
    Q_OBJECT

public:
    explicit GamieHentaiListDownloadInfo(QWidget *parent = nullptr);
    ~GamieHentaiListDownloadInfo();
public:
    bool hasItem(GamieHentaiImageDownloaderManager *manger);
    void udtItemList();
    void addItemList(GamieHentaiImageDownloaderManager *manager);
    void delItemList(GamieHentaiImageDownloaderManager *manager);
};

#endif // GAMIEHENTAILISTDOWNLOADINFO_H
