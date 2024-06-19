
QT       += core network widgets gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += USE_CONSOLE_CONTROLLER
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH+=$$PWD/include
INCLUDEPATH+=$$PWD/include/openssl

win32{
DEFINES += USE_UI_SHOW
LIBS+=-L$$PWD/lib -llibssl -llibcrypto -lcapi -ldasync -llibapps -lopenssl -lossltest -lpadlock  -luitest
}else{
#LIBS+= -llibssl -llibcrypto -lopenssl

}
SOURCES += \
    GamieHentaiDownloadInfoItem.cpp \
    GamieHentaiGlobalSettings.cpp \
    GamieHentaiImageManager.cpp \
    GamieHentaiList.cpp \
    GamieHentaiMainIndexListItem.cpp \
    GamieHentaiMainWindow.cpp \
    GamieHentaiObject.cpp \
    GamieHentaiParser.cpp \
    main.cpp

HEADERS += \
    GamieHentaiDownloadInfoItem.h \
    GamieHentaiGlobalSettings.h \
    GamieHentaiImageManager.h \
    GamieHentaiList.h \
    GamieHentaiMainIndexListItem.h \
    GamieHentaiMainWindow.h \
    GamieHentaiObject.h \
    GamieHentaiParser.h

FORMS += \
    GamieHentaiDownloadInfoItem.ui \
    GamieHentaiMainIndexListItem.ui \
    GamieHentaiMainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    htmlcxx/css/default.css
