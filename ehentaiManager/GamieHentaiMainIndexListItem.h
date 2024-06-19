#ifndef GAMIEHENTAIMAININDEXLISTITEM_H
#define GAMIEHENTAIMAININDEXLISTITEM_H

#include <QWidget>

namespace Ui {
class GamieHentaiMainIndexListItem;
}

class GamieHentaiMainIndexListItem : public QWidget
{
    Q_OBJECT

public:
    explicit GamieHentaiMainIndexListItem(QWidget *parent = nullptr);
    ~GamieHentaiMainIndexListItem();
public:
    QString getTitle();
    QString getHref();
public:
    void setTitle(QString val);
    void setHref(QString href);
private:
    Ui::GamieHentaiMainIndexListItem *ui;
private:
    QString             _href;
};

#endif // GAMIEHENTAIMAININDEXLISTITEM_H
