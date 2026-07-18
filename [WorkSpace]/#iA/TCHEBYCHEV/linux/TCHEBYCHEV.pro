# Linux-specific build configuration
# Use GCC with long double support

QT       += core gui widgets charts
CONFIG   += c++17
TARGET    = TCHEBYCHEV
TEMPLATE  = app

SOURCES += \
    ../main.cpp \
    ../mainwindow.cpp

HEADERS += \
    ../Types.h \
    ../TCHBYCHV.h \
    ../mainwindow.h

# Enable long double math and optimizations
QMAKE_CXXFLAGS += -mfpmath=387 -ffloat-store -O2
LIBS += -lm
