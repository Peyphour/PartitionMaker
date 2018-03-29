#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T21:39:38
#
#-------------------------------------------------

QT       += core gui multimedia printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PartitionMaker
TEMPLATE = app

SOURCES += main/dataprocessor.cpp \
    main/guitarfilter.cpp \
    main/guitarnote.cpp \
    main/main.cpp \
    main/mainwindow.cpp \
    tab/guitartab.cpp \
    tab/partitionbar.cpp \
    tab/partitionnote.cpp \
    util/utils.cpp \
    util/wavfile.cpp \
    tab/partitionpulsation.cpp \
    tab/guitartabbuilder.cpp \
    fft/fftutil.c \
    fft/kfc.c \
    fft/kiss_fastfir.c \
    fft/kiss_fft.c \
    fft/kiss_fftnd.c \
    fft/kiss_fftndr.c \
    fft/kiss_fftr.c \
    qcustomplot.cpp
HEADERS  += main/dataprocessor.h \
    main/guitarfilter.h \
    main/guitarnote.h \
    main/mainwindow.h \
    tab/guitartab.h \
    tab/partitionbar.h \
    tab/partitionnote.h \
    util/utils.h \
    util/wavfile.h \
    tab/partitionpulsation.h \
    tab/guitartabbuilder.h \
    fft/_kiss_fft_guts.h \
    fft/kfc.h \
    fft/kiss_fft.h \
    fft/kiss_fftnd.h \
    fft/kiss_fftndr.h \
    fft/kiss_fftr.h \
    fft/kissfft.hh \
    qcustomplot.h
FORMS    += \
    mainwindow.ui
