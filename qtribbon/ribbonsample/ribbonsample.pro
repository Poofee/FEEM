

QT += core gui
QT += widgets printsupport gui-private

TARGET = ribbonsample
TEMPLATE = app

#QTITANDIR = $$quote($$(QTITANDIR))

include(qtitanribbon.pri)

DESTDIR = $$PWD/../../bin
#DESTDIR = $$member(DESTDIR, 0)$$QTITAN_LIB_PREFIX

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
    }
}


HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp

RESOURCES     = ribbonsample.qrc
FORMS         = ribbonsample.ui

include($$PWD/shared/aboutdialog.pri)
include($$PWD/shared/ribbonwindow.pri)

macx {
    CONFIG-=app_bundle
}

