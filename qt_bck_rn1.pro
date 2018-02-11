QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_bck_rn1
TEMPLATE = app

INCLUDEPATH += $$PWD/../../philslib/include
DEPENDPATH += $$PWD/../../philslib/include

CONFIG += c++14

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    news.cpp \
    text_edit_stream.cpp \
    rng.cpp

HEADERS += \
    MainWindow.hpp \
    news.hpp \
    text_edit_stream.hpp \
    rng.hpp

FORMS += \
    MainWindow.ui
