#-------------------------------------------------
#
# Project created by QtCreator 2013-06-16T08:12:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = SCL
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++17 -static -static-libgcc #-save-temps=obj
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
    Source/Array.cpp \
    Source/AVLTree.cpp \
    Source/Vector.cpp \
    Source/common.cpp \
    Source/config.cpp \
    Source/valarray2DP.cpp \
    Source/array2DP.cpp \
    Source/array1DP.cpp \

HEADERS += \
    Include/VectorSorted.hpp \
    Include/Vector.h \
    Include/List.h \
    Include/Base.h \
    Include/AVLTree.h \
    Include/Array.h \
#    Include/valarray_mt.hpp \
    Include/any.h \
    SCL \
    Include/common.h \
    Include/config.h \
    Include/valarray2DP.h \
    Include/algorithm.hpp \
    Include/array1DP.h \
    Include/binaryTree.h \
    Include/circularbuffer.hpp \
    Include/valarray_mt.hpp \
    Include/vectorStatic.hpp \
    Include/Array2DT.hpp \
    Include/vector2Dvalarray.hpp \
    Include/valarray_r.hpp
