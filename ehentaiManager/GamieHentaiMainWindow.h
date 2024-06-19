#ifndef GAMIEHENTAIMAINWINDOW_H
#define GAMIEHENTAIMAINWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class GamieHentaiMainWindow;
}
class QListWidgetItem;
class GamieHentaiObject;
class GamieHentaiMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GamieHentaiMainWindow(QWidget *parent = nullptr);
    ~GamieHentaiMainWindow();
public:
    void BindehentaiObject(GamieHentaiObject *object);
protected slots:
    virtual void slot_timeout();
private slots:
    void on_listWidget_main_index_list_itemPressed(QListWidgetItem *item);
    void on_pushButton_search_clicked();
    void on_pushButton_next_clicked();

    void on_pushButton_prev_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_skip_page_textChanged(const QString &arg1);

private:
    Ui::GamieHentaiMainWindow *ui;
private:
    GamieHentaiObject       *_ehentai_object;
    QTimer                   _default_timer;
    QString                  _url;
private:
    unsigned int             _range;
};

#endif // GAMIEHENTAIMAINWINDOW_H
