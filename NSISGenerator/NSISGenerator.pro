
QT += quick widgets sql

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        models/db.cpp \
        viewmodels/generator.cpp \
        main.cpp \
        mainwindow.cpp \
        models/sharedinfo.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#=================導入Log===================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lTBLog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lTBLog

INCLUDEPATH += $$PWD/../TBLog
DEPENDPATH += $$PWD/../TBLog
#==========================================================


#=================將需要的檔案放在執行檔旁邊===================
#isEmpty(TARGET_EXT) {
#    win32 {
#        TARGET_CUSTBoard_EXT = .exe
#    }
#} else {
#    TARGET_CUSTBoard_EXT = $${TARGET_EXT}
#}

#win32 {
#    DEPLOY_COMMAND = windeployqt
#}

#win32:CONFIG(debug, debug|release): DEPLOY_TARGET = $$shell_quote($$shell_path($$OUT_PWD/debug/$${TARGET}$${TARGET_CUSTBoard_EXT}))
#win32:CONFIG(release, debug|release): DEPLOY_TARGET = $$shell_quote($$shell_path($$OUT_PWD/release/$${TARGET}$${TARGET_CUSTBoard_EXT}))

#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

#contains(QMAKE_TARGET.arch, x86_64) {
#    win32:CONFIG(debug, debug|release): QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}  --qmldir C:\Qt\Qt5.13.1\5.13.1\msvc2015_64\qml --debug
#    win32:CONFIG(release, debug|release): QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}  --qmldir C:\Qt\Qt5.13.1\5.13.1\msvc2015_64\qml --release
#}
#==========================================================

HEADERS += \
    models/SystemParamItem.h \
    models/db.h \
    viewmodels/generator.h \
    mainwindow.h \
    models/sharedinfo.h


