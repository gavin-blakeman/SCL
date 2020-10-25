#-----------------------------------------------------------------------------------------------------------------------------------
#
# PROJECT:            Storage Class Library
# FILE:								SCL.pro
# SUBSYSTEM:          Project File
# LANGUAGE:						C++
# TARGET OS:          All
# LIBRARY DEPENDANCE:	GCL, MCL.
# NAMESPACE:          N/A
# AUTHOR:							Gavin Blakeman.
# LICENSE:            GPLv2
#
#                     Copyright 2010, 2013-2020 Gavin Blakeman.
#                     This file is part of the Storage Class Library (SCL)
#
#                     SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
#                     License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
#                     any later version.
#
#                     SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#                     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#                     more details.
#
#                     You should have received a copy of the GNU General Public License along with SCL.  If not,
#                     see <http://www.gnu.org/licenses/>.
#
# OVERVIEW:
#
# HISTORY:            2015-09-22 GGB - astroManager 2015.09 release
#                     2013-06-16 GGB - Project file created.
#
#-----------------------------------------------------------------------------------------------------------------------------------

TARGET          = SCL
TEMPLATE        = lib
CONFIG          += staticlib

QMAKE_CXXFLAGS  += -std=c++17

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
  "../GCL" \
  "../MCL" \

SOURCES += \
    source/common.cpp \
    source/config.cpp \
    source/valarray2DP.cpp \
    source/array2DP.cpp \
    source/array1DP.cpp

HEADERS += \
    include/Base.h \
    include/any.hpp \
    include/valarray_mt.hpp \
    include/common.h \
    include/config.h \
    include/valarray2DP.h \
    include/algorithm.hpp \
    include/array2DP.h \
    include/array1DP.h \
    include/circularbuffer.hpp \
    SCL \
    include/hierarchyBuilder.hpp \
    include/array2D.hpp \
    include/vector2D.hpp \
    include/index2D.hpp \
    include/vectorSorted.hpp \
    include/mapAsVector.hpp

DISTFILES += \
    license.txt \
    Doxyfile \
    changelog.txt
