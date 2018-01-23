#-------------------------------------------------
#
# Project created by QtCreator 2018-01-20T17:28:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += ordered

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include(cpu/cpu.pro)
#include(common/common.pro)
#include(executables/executables.pro)
#include(archives/archives.pro)
#include(bindings/bindings.pro)
#include(gui/gui.pro)

TARGET = revtoolkit

#SUBDIRS = architectures \
#        common \
#        executables \
#        archives \
#        bindings \
#        gui

COMMON_DIR = common
COMMON_HEADERS = $$COMMON_DIR/register.h \
    $$COMMON_DIR/types.h \
    $$COMMON_DIR/instruction.h

CPU_DIR = cpu
#CPU_SOURCES =
CPU_HEADERS = $$CPU_DIR/arm/instructions.h \
    $$CPU_DIR/c64/instructions.h \
    $$CPU_DIR/power/instructions.h \
    $$CPU_DIR/sparc/instructions.h \
    $$CPU_DIR/x86/instructions.h \
    $$CPU_DIR/z80/instructions.h

ARCHIVES_DIR = archives
ARCHIVES_HEADERS = $$ARCHIVES_DIR/gameboy/gb.h
ARCHIVES_SOURCES = $$ARCHIVES_DIR/gameboy/gb.cpp

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        $$ARCHIVES_SOURCES

HEADERS += \
    mainwindow.h \
    $$CPU_HEADERS \
    $$ARCHIVES_HEADERS \
    $$COMMON_HEADERS
