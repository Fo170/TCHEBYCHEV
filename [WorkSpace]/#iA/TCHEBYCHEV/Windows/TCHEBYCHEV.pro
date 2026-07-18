# Windows-specific build configuration
# Use MSVC or MinGW

QT       += core gui widgets charts
CONFIG   += c++17
TARGET    = TCHEBYCHEV
TEMPLATE  = app

# Source files are in parent directory
SOURCES += \
    ../main.cpp \
    ../mainwindow.cpp

HEADERS += \
    ../Types.h \
    ../TCHBYCHV.h \
    ../mainwindow.h

RC_ICONS = ../icon.ico

win32-msvc* {
    QMAKE_CXXFLAGS += /fp:precise
    QMAKE_LFLAGS   += /LARGEADDRESSAWARE
}

win32-g++* {
    QMAKE_CXXFLAGS += -mfpmath=387 -ffloat-store
}
