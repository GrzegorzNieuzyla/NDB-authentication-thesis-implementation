#-------------------------------------------------
#
# Project created by QtCreator 2020-12-08T22:13:26
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AuthenticationSystem
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

CONFIG += c++17

SOURCES += \
        AuthenticationSystem/AuthenticationManager.cpp \
        AuthenticationSystem/DatabaseAccess.cpp \
        AuthenticationSystem/LoginScreen.cpp \
        AuthenticationSystem/MainWindow.cpp \
        AuthenticationSystem/RegisterScreen.cpp \
        AuthenticationSystem/UserScreen.cpp \
        AuthenticationSystem/main.cpp \
        DB.cpp \
        NDB.cpp \
        PasswordStrengthValidator.cpp \
        Streams/NDBFileStream.cpp \
        Generators/NDB_KHiddenGenerator.cpp \
        Generators/NDB_Generator.cpp \
        Utils/RandomValuesGenerator.cpp \
        Validator/NDBValidator.cpp

HEADERS += \
        AuthenticationSystem/AuthenticationManager.h \
        AuthenticationSystem/Config.h \
        AuthenticationSystem/DatabaseAccess.h \
        AuthenticationSystem/LoginScreen.h \
        AuthenticationSystem/MainWindow.h \
        AuthenticationSystem/RegisterScreen.h \
        AuthenticationSystem/UserScreen.h \
        DB.h \
        NDB.h \
        PasswordStrengthValidator.h \
        Streams/NDBFileStream.h \
        Generators/NDB_KHiddenGenerator.h \
        Generators/NDB_Generator.h \
        Streams/Stream.h \
        Utils/FileUtils.h \
        Utils/RandomValuesGenerator.h \
        Validator/NDBValidator.h

FORMS += \
        AuthenticationSystem/LoginScreen.ui \
        AuthenticationSystem/MainWindow.ui \
        AuthenticationSystem/RegisterScreen.ui \
        AuthenticationSystem/UserScreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
