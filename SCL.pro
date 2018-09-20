#-------------------------------------------------
#
# Project created by QtCreator 2013-06-16T08:12:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = SCL
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++17 #-static -static-libgcc -save-temps
QMAKE_LFLAGS += -fopenmp

OBJECTS_DIR = "objects"
UI_DIR = "objects/ui"
MOC_DIR = "objects/moc"
RCC_DIR = "objects/rcc"

win32:CONFIG(release, debug|release) {
  DESTDIR = "../Library/win32/release"
  OBJECTS_DIR = "../Library/win32/release/object/SCL"
}
else:win32:CONFIG(debug, debug|release) {
  DESTDIR = "../Library/win32/debug"
  OBJECTS_DIR = "../Library/win32/debug/object/SCL"
}
else:unix:CONFIG(release, debug|release) {
  DESTDIR = "../Library/unix/release"
  OBJECTS_DIR = "../Library/unix/release/object/SCL"
}
else:unix:CONFIG(debug, debug|release) {
  DESTDIR = ""
  OBJECTS_DIR = "objects"
}

INCLUDEPATH += \
  "../boost 1.62" \
  "../GCL" \
  "../MCL"

SOURCES += \
    source/AVLTree.cpp \
    source/common.cpp \
    source/config.cpp \
    source/valarray2DP.cpp \
    source/array2DP.cpp \
    source/array1DP.cpp \
    source/error.cpp

HEADERS += \
    include/VectorSorted.hpp \
    include/Base.h \
    include/AVLTree.h \
    include/Array2DT.hpp \
    include/valarray_mt.hpp \
    include/any.h \
    include/common.h \
    include/config.h \
    include/valarray2DP.h \
    include/algorithm.hpp \
    include/array2DP.h \
    include/array1DP.h \
    include/binaryTree.h \
    include/circularbuffer.hpp \
    include/error.h \
    SCL
