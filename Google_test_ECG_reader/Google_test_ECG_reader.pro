include(gtest_dependency.pri)

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

SOURCES += \
        ../ECG_reader/ecg_data.cpp \
        ../ECG_reader/mainwindow.cpp \
        main.cpp         tst_ecg_reader_test_case.cpp \
        qcustomplot.cpp

HEADERS += \
    ../ECG_reader/Google_test_main_cpp_header.h \
    ../ECG_reader/ecg_data.h \
    ../ECG_reader/mainwindow.h \
    qcustomplot.h
