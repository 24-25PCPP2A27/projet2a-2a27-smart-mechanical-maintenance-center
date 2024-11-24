QT       += sql printsupport charts network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    EmployeMain.cpp \
    connection.cpp \
    employe.cpp \
    forgotpassworddialog.cpp \
    login.cpp \
    logviewer.cpp \
    main.cpp \
    qrcodedialog.cpp \
    qrcodegen.cpp \
    sendsmsdialog.cpp \
    smshandler.cpp

HEADERS += \
    EmployeMain.h \
    connection.h \
    employe.h \
    forgotpassworddialog.h \
    login.h \
    logviewer.h \
    qrcodedialog.h \
    qrcodegen.h \
    sendsmsdialog.h \
    smshandler.h

FORMS += \
    EmployeMain.ui \
    forgotpassworddialog.ui \
    login.ui \
    qrcodedialog.ui \
    sendsmsdialog.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
