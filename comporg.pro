# Component Organizer
# Copyright (C) Mário Ribeiro (mario.ribas@gmail.com)

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

QT       += core gui

win32:TARGET = comporg_win
unix:TARGET = comporg_unix
TEMPLATE = app

#DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_DEBUG

win32:RC_FILE = resources/app.rc

INCLUDEPATH += core/
INCLUDEPATH += gui/

SOURCES += main.cpp \
    core/manufacturer.cpp \
    core/datasheet.cpp \
    core/container.cpp \
    core/component.cpp \
    core/applicationnote.cpp \
    gui/ptablewidget.cpp \
    gui/mainwindow.cpp \
    gui/applicationnotetable.cpp \
    gui/componenttable.cpp \
    gui/componentdialog.cpp \
    gui/ptoolbutton.cpp \
    core/package.cpp \
    gui/componentdetails.cpp \
    gui/pminitablewidget.cpp \
    gui/datasheettable.cpp \
    gui/stocktable.cpp \
    core/stock.cpp \
    gui/pspinbox.cpp \
    core/label.cpp \
    gui/optionsdialog.cpp \
    gui/applicationnotedialog.cpp \
    core/co.cpp

HEADERS  += core/manufacturer.h \
    core/datasheet.h \
    core/container.h \
    core/component.h \
    core/applicationnote.h \
    gui/ptablewidget.h \
    gui/mainwindow.h \
    gui/applicationnotetable.h \
    gui/componenttable.h \
    gui/componentdialog.h \
    gui/ptoolbutton.h \
    core/package.h \
    gui/componentdetails.h \
    gui/pminitablewidget.h \
    gui/datasheettable.h \
    gui/stocktable.h \
    core/stock.h \
    core/co_defs.h \
    gui/pspinbox.h \
    core/label.h \
    gui/optionsdialog.h \
    gui/applicationnotedialog.h \
    core/co.h

FORMS    += gui/mainwindow.ui \
    gui/componentdialog.ui \
    gui/componentdetails.ui \
    gui/optionsdialog.ui \
    gui/applicationnotedialog.ui

OBJECTS_DIR =   _build/tmp/obj
MOC_DIR =       _build/tmp/moc
RCC_DIR =       _build/tmp/rcc
UI_DIR =        _build/tmp/ui

CONFIG(debug, debug|release) {
    DESTDIR = _build/debug/bin
} else {
    DESTDIR = _build/release/bin
}


RESOURCES += \
    resources/img.qrc

OTHER_FILES += \
    resources/app.rc
