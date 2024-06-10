#ifndef GAMIMAINWIDGET_H
#define GAMIMAINWIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class GamiMainWidget; }
QT_END_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QImage;
class QPaintEvent;
class QResizeEvent;
class GamiMainWidget : public QWidget
{
    Q_OBJECT

public:
     GamiMainWidget(QWidget *parent = nullptr, QString socks5_ip="10.255.169.18", unsigned short socks5_port=65533);
    ~GamiMainWidget();
private:
     void ResizeCanvas();

protected:
     virtual void paintEvent(QPaintEvent *event) override;
     virtual void resizeEvent(QResizeEvent *event) override;
protected slots:
     virtual void OnFinished(QNetworkReply *reply);
     virtual void OnResponse(QByteArray &msg);
private:
    Ui::GamiMainWidget *ui;
private:
    QNetworkAccessManager           *_net_manager;
private:
    QImage                          *_canvas;
};
#endif // GAMIMAINWIDGET_H
