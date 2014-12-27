QT += widgets

SOURCES += \
    src/main.cpp \
    src/layer.cpp \
    src/mousestate.cpp \
    widgets/canvas.cpp \
    widgets/layerstack.cpp \
    widgets/mainwindow.cpp \
    widgets/workspace.cpp \
    tools/tool.cpp \
    dialogs/brushselector.cpp \
    widgets/patternmaker.cpp \
    widgets/colorpicker.cpp \
    tools/ellipsetool.cpp \
    tools/recttool.cpp \
    tools/linetool.cpp \
    tools/marqueetool.cpp \
    tools/selectiontool.cpp \
    tools/ellipseselectiontool.cpp \
    tools/movetool.cpp \
    dialogs/modifyselection.cpp \
    dialogs/smoothselection.cpp \
    tools/buckettool.cpp \
    tools/wandtool.cpp \
    dialogs/fillselector.cpp \
    widgets/magiclabel.cpp \
    tools/gradienttool.cpp \
    dialogs/importdialog.cpp \
    widgets/imagecropper.cpp \
    widgets/imageimporter.cpp

HEADERS += \
    src/layer.h \
    src/mousestate.h \
    widgets/canvas.h \
    widgets/layerstack.h \
    widgets/mainwindow.h \
    widgets/workspace.h \
    tools/tool.h \
    dialogs/brushselector.h \
    widgets/patternmaker.h \
    widgets/colorpicker.h \
    tools/tools.h \
    tools/recttool.h \
    tools/ellipsetool.h \
    tools/linetool.h \
    tools/marqueetool.h \
    tools/selectiontool.h \
    tools/ellipseselectiontool.h \
    tools/movetool.h \
    dialogs/modifyselection.h \
    dialogs/smoothselection.h \
    tools/buckettool.h \
    tools/wandtool.h \
    dialogs/fillselector.h \
    widgets/magiclabel.h \
    tools/gradienttool.h \
    dialogs/importdialog.h \
    widgets/imagecropper.h \
    widgets/imageimporter.h \
    src/const.h

RESOURCES += \
    icons.qrc

FORMS += \
    dialogs/brushselector.ui \
    dialogs/modifyselection.ui \
    dialogs/smoothselection.ui \
    dialogs/fillselector.ui \
    dialogs/importdialog.ui

QMAKE_CFLAGS = -Wno-unused-parameter
