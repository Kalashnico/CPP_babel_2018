#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T12:30:30
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = babel_client
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

CONFIG += c++11

SOURCES += \
        src/main.cpp \
    src/CallWindow.cpp \
    src/MainWindow.cpp \
    src/LoginWindow.cpp \
    src/TcpClient.cpp \
    src/Protocol.cpp \
    src/CallReceiveWindow.cpp

HEADERS += \
    include/CallWindow.hpp \
    include/MainWindow.hpp \
    include/LoginWindow.hpp \
    include/TcpClient.hpp \
    include/Protocol.hpp \
    include/CallReceiveWindow.hpp

FORMS += \
    form/CallWindow.ui \
    form/MainWindow.ui \
    form/LoginWindow.ui \
    form/CallReceiveWindow.ui

INCLUDEPATH += include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
