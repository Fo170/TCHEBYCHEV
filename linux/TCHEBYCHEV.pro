# Linux-specific build configuration
# Use GCC with long double support

QT       += core gui widgets charts network
CONFIG   += c++17
TARGET    = TCHEBYCHEV
TEMPLATE  = app

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

DESTDIR     = .
OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .rcc
UI_DIR      = .ui

# Enable long double math and optimizations
QMAKE_CXXFLAGS += -mfpmath=387 -ffloat-store -O2
LIBS += -lm