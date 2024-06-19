#pragma once

#include <QWidget>
#include <QListWidget>


class GamieHentaiImageDownloaderManager;
class GamieHentaiList : public QListWidget
{
    Q_OBJECT

public:
    explicit GamieHentaiList(QWidget *parent = nullptr);
    ~GamieHentaiList();
public:
    bool hasItem(GamieHentaiImageDownloaderManager *manger);
    void udtItemList();
    void addItemList(GamieHentaiImageDownloaderManager *manager);
    void delItemList(GamieHentaiImageDownloaderManager *manager);
};
