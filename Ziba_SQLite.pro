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
    zibazing.cpp

HEADERS  += mainwindow.h \
    podcastsdb.h \
    rssfeeditem.h \
    opmlimport.h \
    fileDownloader.h \
    downloadqueue.h \
    rssUpdater.h \
    zibazing.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ListOfRSSItem.qml \
    ListOfRSSFeed.qml \
    feeds01.opml \
    podcast_db.sqlite

RESOURCES += \
    iconsPack.qrc





















