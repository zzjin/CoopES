QT += core gui xml network sql

TARGET      = CoopES
DESTDIR     = ./bin
TEMPLATE    = app
CONFIG      += qt\
            warn_on\
            debug\
            console
#syntax highlightter include PATH
INCLUDEPATH +=  ./include/ssyntax
#log4qt include PATH
INCLUDEPATH += ./include/log4qt
#different plaform with different libs
win32{
    LIBS += ./lib/windows/libqtsourceview.a
    LIBS +=./lib/windows/liblog4qt1.a
}
unix{
    LIBS += ./lib/linux/libqtsourceview.a
    LIBS +=./lib/linux/liblog4qt1.a
}
SOURCES     += main.cpp\
            dialog.cpp \
    fileinfo.cpp \
    coopnetwork.cpp
HEADERS     += dialog.h \
    config.h \
    fileinfo.h \
    coopnetwork.h
FORMS       += dialog.ui
#choose where to put the ui_*.h files
UI_DIR += ./tmp
#use the 'rcc' cofig to choose where to put the qrc_*.h files
RCC_DIR += ./tmp
#use the 'moc' cofig to choose where to put the standad *.h files
MOC_DIR += ./tmp
#choose the target directory
OBJECTS_DIR += ./tmp

RESOURCES +=

TRANSLATIONS = CoopES_zh_CN.ts \
               CoopES_en.ts

OTHER_FILES += README \
    generalBackup.txt
