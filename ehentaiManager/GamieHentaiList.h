#pragma once

#include <QWidget>
#include <QListWidget>


class GamieHentaiDownloadInfoItem;
class GamieHentaiList : public QListWidget
{
    Q_OBJECT

public:
    explicit GamieHentaiList(QWidget *parent = nullptr);
    ~GamieHentaiList();
public:
    void udtItemList();
    void addItemList(GamieHentaiDownloadInfoItem *it);
    void delItemList(GamieHentaiDownloadInfoItem *it);
};
