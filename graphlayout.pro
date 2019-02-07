TEMPLATE = app
CONFIG += c++11
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt
QT += gui widgets

SOURCES += \
        main.cpp \
    gvgraph.cpp \
    textitem.cpp \
    graphlayout.cpp \
    pathitem.cpp

LIBS += \
        -lgvc \
        -lcgraph

HEADERS += \
    gvgraph.h \
    gvwrappers.h \
    textitem.h \
    graphlayout.h \
    pathitem.h
