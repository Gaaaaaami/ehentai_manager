/********************************************************************************
** Form generated from reading UI file 'GamiMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMIMAINWIDGET_H
#define UI_GAMIMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GamiMainWidget
{
public:

    void setupUi(QWidget *GamiMainWidget)
    {
        if (GamiMainWidget->objectName().isEmpty())
            GamiMainWidget->setObjectName(QString::fromUtf8("GamiMainWidget"));
        GamiMainWidget->resize(800, 600);

        retranslateUi(GamiMainWidget);

        QMetaObject::connectSlotsByName(GamiMainWidget);
    } // setupUi

    void retranslateUi(QWidget *GamiMainWidget)
    {
        GamiMainWidget->setWindowTitle(QCoreApplication::translate("GamiMainWidget", "GamiMainWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GamiMainWidget: public Ui_GamiMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMIMAINWIDGET_H
