#-------------------------------------------------
#
# Project created by QtCreator 2011-03-15T15:50:03
#
#-------------------------------------------------

QT          += core gui xml network sql

TARGET      = CoopES
DESTDIR     = ./bin
TEMPLATE    = app
CONFIG      += qt\
            warn_on\
            debug\
            console
#语法高亮头文件
INCLUDEPATH =  ./include/ssyntax
#不同的平台对应的库不一样
win32{
    LIBS += ./lib/windows/libqtsourceview.a
}
unix{
    LIBS += ./lib/linux/libqtsourceview.a
}
SOURCES     += main.cpp\
            dialog.cpp \
    fileinfo.cpp
HEADERS     += dialog.h \
    config.h \
    fileinfo.h
FORMS       += dialog.ui
#指定uic命令将.ui文件转化成ui_*.h文件的存放的目录
UI_DIR += ./tmp
#指定rcc命令将.qrc文件转换成qrc_*.h文件的存放目录
RCC_DIR += ./tmp
#指定moc命令将含Q_OBJECT的头文件转换成标准.h文件的存放目录
MOC_DIR += ./tmp
#指定目标文件的存放目录
OBJECTS_DIR += ./tmp

RESOURCES +=

TRANSLATIONS = CoopES_zh_CN.ts \
               CoopES_en.ts

OTHER_FILES += \
    README
