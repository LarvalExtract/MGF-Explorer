#-------------------------------------------------
#
# Project created by QtCreator 2020-05-09T13:37:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MGF-Explorer_Qt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        src/configfile.cpp \
        src/fileextractor.cpp \
		src/mgfresourcemanager.cpp \  
        src/main.cpp \
        src/mgf/mgfarchive.cpp \
        src/mgf/mgftreeitem.cpp \
        src/mgf/mgmaterialfactory.cpp \
        src/mgf/mgmeshfactory.cpp \
        src/mgf/mgnode.cpp \
        src/mgf/mgtexture.cpp \
        src/models/MGFAnimationTableModel.cpp \
        src/models/mgffiletablemodel.cpp \
        src/models/mgffiletreemodel.cpp \
        src/models/MGFMaterialTableModel.cpp \
        src/models/MGFMeshTableModel.cpp \
        src/models/MGFNodeTreeModel.cpp \
        src/models/FileExtractListModel.cpp \
        src/widgets/mgmodelviewerwidget.cpp \
        src/widgets/mgftextureviewerwidget.cpp \
        src/widgets/mgftextviewwidget.cpp \
        src/widgets/mgfworkspace.cpp \
		src/windows/mainwindow.cpp \
        src/windows/ogrewindow.cpp \
		src/windows/fileextractdialog.cpp \
		src/windows/listviewtexture.cpp \
        src/vendor/pugixml-1.10/src/pugixml.cpp

HEADERS += \
        src/configfile.h \       
        src/fileextractor.h \
		src/mgfresourcemanager.h \
        src/mgfresourceviewer.h \
        src/mgf/mgfarchive.h \
        src/mgf/mgftreeitem.h \
        src/mgf/mgmaterialfactory.h \
        src/mgf/mgmeshfactory.h \
        src/mgf/mgnode.h \
        src/mgf/mgtexture.h \
        src/models/MGFAnimationTableModel.h \
        src/models/mgffiletablemodel.h \
        src/models/mgffiletreemodel.h \
        src/models/MGFMaterialTableModel.h \
        src/models/MGFMeshTableModel.h \
        src/models/MGFNodeTreeModel.h \
        src/models/FileExtractListModel.h \
        src/widgets/mgmodelviewerwidget.h \
        src/widgets/mgftextureviewerwidget.h \
        src/widgets/mgftextviewwidget.h \
		src/widgets/mgfworkspace.h \
		src/windows/listviewtexture.h \
        src/windows/mainwindow.h \
		src/windows/ogrewindow.h \
        src/windows/fileextractdialog.h \
        src/vendor/pugixml-1.10/src/pugixml.hpp

FORMS += \
        src/windows/fileextractdialog.ui \
        src/windows/listviewtexture.ui \
        src/windows/mainwindow.ui \
        src/widgets/mgftextureviewerwidget.ui \
        src/widgets/mgftextviewwidget.ui \
        src/widgets/mgfworkspace.ui \
        src/widgets/mgmodelviewerwidget.ui \
        src/windowfiletable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$(OGRE3D_DIR)/Binaries/lib/Release/ -lOgreMain \
                                              -L$$(OGRE3D_DIR)/Binaries/lib/Release/ -lRenderSystem_Direct3D9

else:win32:CONFIG(debug, debug|release): LIBS += -L$$(OGRE3D_DIR)/Binaries/lib/Debug/ -lOgreMain_d \
                                                 -L$$(OGRE3D_DIR)/Binaries/lib/Debug/ -lRenderSystem_Direct3D9_d


INCLUDEPATH += $$(OGRE3D_DIR)/Binaries/include \
               $$(OGRE3D_DIR)/Source/ogre-1.12.6/OgreMain/include \
               $$(OGRE3D_DIR)/Source/ogre-1.12.6/RenderSystems/Direct3D9/include \
               $$PWD/src/vendor/pugixml-1.10/src \
               $$PWD/src

DEPENDPATH += $$(OGRE3D_DIR)/Source/ogre-1.12.6/OgreMain/include \
              $$(OGRE3D_DIR)/Source/ogre-1.12.6/RenderSystems/Direct3D9/include \
