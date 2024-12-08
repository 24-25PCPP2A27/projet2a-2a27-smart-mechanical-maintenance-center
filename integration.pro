QT       += sql printsupport charts network serialport quick qml multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -lQt5Qml

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_QML_DEBUG

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../vehiculeee/VehiMain.cpp \
    ../vehiculeee/qrcodedialog.cpp \
    ../vehiculeee/statistique.cpp \
    ../vehiculeee/vehicule.cpp \
    ../vehiculeee/vehiculeui.cpp \
    EmployeMain.cpp \
    EquipMain.cpp \
    MainRDV.cpp \
    RDV.cpp \
    ServiceMain.cpp \
    arduino.cpp \
    chatbot.cpp \
    chatbotdialog.cpp \
    connection.cpp \
    employe.cpp \
    employeeprofiledialog.cpp \
    equipements.cpp \
    forgotpassworddialog.cpp \
    login.cpp \
    logviewer.cpp \
    main.cpp \
    qcustomplot.cpp \
    qrcodedialog.cpp \
    qrcodegen.cpp \
    service.cpp  # Remove the extra backslash

HEADERS += \
    ../vehiculeee/VehiMain.h \
    ../vehiculeee/qrcodedialog.h \
    ../vehiculeee/statistique.h \
    ../vehiculeee/vehicule.h \
    ../vehiculeee/vehiculeui.h \
    EmployeMain.h \
    EquipMain.h \
    ImageDelegate.h \
    MainRDV.h \
    RDV.h \
    ServiceMain.h \
    arduino.h \
    chatbot.h \
    chatbotdialog.h \
    connection.h \
    employe.h \
    employeeprofiledialog.h \
    equipements.h \
    forgotpassworddialog.h \
    login.h \
    logviewer.h \
    qcustomplot.h \
    qrcodedialog.h \
    qrcodegen.h \
    service.h  # Remove the extra backslash
FORMS += \
    ../vehiculeee/VehiMain.ui \
    ../vehiculeee/statistique.ui \
    ../vehiculeee/vehiculeui.ui \
    EmployeMain.ui \
    EquipMain.ui \
    MainRDV.ui \
    ServiceMain.ui \
    chatbotdialog.ui \
    employeeprofiledialog.ui \
    forgotpassworddialog.ui \
    login.ui \
    qrcodedialog.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imagebg.qrc
