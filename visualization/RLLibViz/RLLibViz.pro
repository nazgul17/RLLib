#-------------------------------------------------
#
# Project created by QtCreator 2013-10-08T00:39:26
#
#-------------------------------------------------

QT       += core gui

TARGET = RLLibViz
TEMPLATE = app

INCLUDEPATH += ../../src \
               ../../simulation

SOURCES += Main.cpp\
        ModelBase.cpp \
	ModelThread.cpp \
	ContinuousGridworldModel.cpp \
	ViewBase.cpp \
	Framebuffer.cpp \
	ContinuousGridworldView.cpp \
    	Window.cpp \
	PlotView.cpp

HEADERS  += Mat.h \
    	Vec.h \
    	ViewBase.h \
	Window.h \
	ModelBase.h \
	ModelThread.h \
	Framebuffer.h \
	ContinuousGridworldView.h \
	ContinuousGridworldModel.h \
	PlotView.h

	