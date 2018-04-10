#-------------------------------------------------
#
# Project created by QtCreator 2018-03-03T05:54:28
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gallery-desktop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    albumlistwidget.cpp \
    gallerywidget.cpp \
    albumwidget.cpp \
    picturewidget.cpp \
    thumbnailproxymodel.cpp \
    picturedelegate.cpp

HEADERS += \
        mainwindow.h \
    albumlistwidget.h \
    gallerywidget.h \
    albumwidget.h \
    picturewidget.h \
    thumbnailproxymodel.h \
    picturedelegate.h

FORMS += \
        mainwindow.ui \
    albumlistwidget.ui \
    gallerywidget.ui \
    albumwidget.ui \
    picturewidget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gallery-core/release/ -lgallery-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gallery-core/debug/ -lgallery-core
else:unix: LIBS += -L$$OUT_PWD/../gallery-core/ -lgallery-core

INCLUDEPATH += $$PWD/../gallery-core
DEPENDPATH += $$PWD/../gallery-core

RESOURCES += \
    gallery-desktop.qrc
