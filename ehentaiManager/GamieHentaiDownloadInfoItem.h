#pragma once

#include <QWidget>
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
    void setKey(QString key);
    void setImageName(QString name);
    void setValue(double p);
    void updateInfo();
private slots:
    void on_pushButton_redownload_clicked();

    void on_pushButton_open_dir_clicked();

private:
    Ui::GamieHentaiItemDownloadInfo *ui;
private:
    QString         _key;
};

