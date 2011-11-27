#-------------------------------------------------
#
# Project created by QtCreator 2011-10-02T14:57:21
#
#-------------------------------------------------

QT       += core gui xml sql network

TARGET = Ziba_SQLite
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    podcastsdb.cpp \
    rssfeeditem.cpp \
    opmlimport.cpp \
    fileDownloader.cpp \
    downloadqueue.cpp \
    rssUpdater.cpp \
    zibazing.cpp \
    rssfeedsmodel.cpp \
    rssParser.cpp

HEADERS  += mainwindow.h \
    podcastsdb.h \
    rssfeeditem.h \
    opmlimport.h \
    fileDownloader.h \
    downloadqueue.h \
    rssUpdater.h \
    zibazing.h \
    rssfeedsmodel.h \
    rssParser.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ListOfRSSItem.qml \
    ListOfRSSFeed.qml \
    feeds01.opml \
    mordaLitza.qml \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

RESOURCES += \
    iconsPack.qrc




























