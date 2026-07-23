# Windows-specific build configuration
# Use MSVC or MinGW

QT       += core gui widgets charts network
CONFIG   += c++17
TARGET    = TCHEBYCHEV
TEMPLATE  = app

# Source files are in parent directory
SOURCES += \
    ../main.cpp \
    ../mainwindow.cpp \
    ../UpdateChecker.cpp \
    ../LangueManager.cpp \
    ../Project.cpp

HEADERS += \
    ../Types.h \
    ../TCHBYCHV.h \
    ../mainwindow.h \
    ../AppConfig.hpp \
    ../UpdateChecker.hpp \
    ../LangueManager.hpp \
    ../Project.hpp

RESOURCES += ../resources.qrc

RC_FILE   = TCHEBYCHEV_resource.rc

DESTDIR     = .
OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .rcc
UI_DIR      = .ui

win32-msvc* {
    QMAKE_CXXFLAGS += /fp:precise /utf-8
    QMAKE_LFLAGS   += /LARGEADDRESSAWARE
}

win32-g++* {
    QMAKE_CXXFLAGS += -mfpmath=387 -ffloat-store
}