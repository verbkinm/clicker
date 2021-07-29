QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    main_tab.cpp \
    mainwindow.cpp \
    one_window_coordinates.cpp \
    widget.cpp

HEADERS += \
    main_tab.h \
    mainwindow.h \
    one_window_coordinates.h \
    widget.h

FORMS += \
    main_tab.ui \
    mainwindow.ui \
    one_window_coordinates.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
