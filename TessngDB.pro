QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console
CONFIG += precompiled_header
PRECOMPILED_HEADER = pch

#FORMS += \
#        TESS_API_EXAMPLE.ui

SOURCES += \
    DBPluginGui.cpp \
    GraphicsBaseItem.cpp \
    InterestElliseItem.cpp \
    InterestPolylineItem.cpp \
    InterestRectItem.cpp \
    TessngDBCopy.cpp \
    TessngDBNet.cpp \
    TessngDBToolsRemove.cpp \
    TessngDBToolsUpdate.cpp \
    exception.cpp \
    #TESS_API_EXAMPLE.cpp \
    TessngDBToolsRemoveBase.cpp \
    main.cpp

HEADERS += \
    DBClassDefine.h \
    DBPluginGui.h \
    GraphicsBaseItem.h \
    InterestElliseItem.h \
    InterestPolylineItem.h \
    InterestRectItem.h \
    TessngDBCopy.h \
    TessngDBNet.h \
    TessngDBToolsRemove.h \
    TessngDBToolsRemoveBase.h \
    TessngDBToolsUpdate.h \
    exception.h \
    #TESS_API_EXAMPLE.h \
    pch \
    singleholder.h

msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8


MOC_DIR += $$PWD/OutPut/temp/moc_dir
OBJECTS_DIR += $$PWD/OutPut/temp/obj_dir
UI_DIR += $$PWD/OutPut/temp/ui_dir
RCC_DIR += $$PWD/OutPut/temp/rcc_dir

TARGET=TessngDB
TEMPLATE=app


CONFIG  += warn_on           #告诉qmake要把编译器设置为输出警告信息的

win32:CONFIG(release, debug|release): DESTDIR=$$PWD/OutPut/release/Bin
else:win32:CONFIG(debug, debug|release):DESTDIR=$$PWD/OutPut/debug/Bin

#disable C4819 warning
win32:QMAKE_CXXFLAGS_WARN_ON += -wd4819
win32:QMAKE_CXXFLAGS += /FS
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-msvc* {
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}

win32{
contains(QT_ARCH, i386) {
    message("32-bit")
  }else {
    message("64-bit")

    INCLUDEPATH += $$PWD/TESSNG_Plugin/include
    DEPENDPATH += $$PWD/TESSNG_Plugin/include
    INCLUDEPATH += $$PWD/TESSNG_Plugin/include/Plugin
    DEPENDPATH += $$PWD/TESSNG_Plugin/include/Plugin
    INCLUDEPATH += $$PWD/TESSNG_Plugin/include/TESS_NET
    DEPENDPATH += $$PWD/TESSNG_Plugin/include/TESS_NET
    INCLUDEPATH += $$PWD/TESSNG_Plugin/include/TESS_DB
    DEPENDPATH += $$PWD/TESSNG_Plugin/include/TESS_DB

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/release/ -lTESS_DB
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/debug/ -lTESS_DB

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/release/ -lTESS_NET
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/debug/ -lTESS_NET

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/release/ -lTESS_SIMU
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/debug/ -lTESS_SIMU

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/release/ -lTESS_WIN
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/debug/ -lTESS_WIN

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/release/ -lTessInterfaces
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/debug/ -lTessInterfaces

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/release/ -lTessSupport
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/TESSNG_Plugin/lib/debug/ -lTessSupport
  }
}


