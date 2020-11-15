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
		src\FileExtractor\States\FileExtractorStateGroup.cpp \
		src\FileExtractor\States\FinishedState.cpp \
		src\FileExtractor\States\IdleState.cpp \
		src\FileExtractor\States\ProgressState.cpp \
		src\utils\events\EventDispatcher.cpp \
		src\utils\Contexts.cpp \
		src\utils\Context.cpp \
		src\FileExtractor\Models\FileExtractListModel.cpp \
		src\models\MGFAnimationTableModel.cpp \
		src\models\MGFMeshTableModel.cpp \
		src\models\MGFMaterialTableModel.cpp \
		src\models\MGFNodeTreeModel.cpp \
		src\utils\configfile.cpp \
		src\utils\states\State.cpp \
		src\utils\states\StateGroup.cpp \
		src\widgets\mgfresourceviewer.cpp \
		src\FileExtractor\FileExtractorDialog.cpp \
		src\FileExtractor\Extractor.cpp \
		src\windows\listviewtexture.cpp \
		src\main.cpp \
		src\windows\mainwindow.cpp \
		src\mgfresourcemanager.cpp \
		src\widgets\mgftextureviewerwidget.cpp \
		src\widgets\mgftextviewwidget.cpp \
		src\widgets\mgfworkspace.cpp \
		src\mgf\mgfarchive.cpp \
		src\models\mgffiletablemodel.cpp \
		src\models\mgffiletreemodel.cpp \
		src\mgf\mgftreeitem.cpp \
		src\mgf\mgmaterialfactory.cpp \
		src\mgf\mgmeshfactory.cpp \
		src\mgf\mgnode.cpp \
		src\mgf\mgtexture.cpp \
		src\widgets\mgmodelviewerwidget.cpp \
		src\windows\ogrewindow.cpp \
		src\vendor\pugixml-1.10\src\pugixml.cpp \


HEADERS += \
		src\FileExtractor\Models\FileExtractItem.h \
		src\FileExtractor\States\FileExtractorStateBase.h \
		src\FileExtractor\States\FileExtractorStateGroup.h \
		src\FileExtractor\States\FinishedState.h \
		src\FileExtractor\States\IdleState.h \
		src\FileExtractor\States\ProgressState.h \
		src\utils\events\EventData.h \
		src\utils\events\EventDispatcher.h \
		src\utils\Contexts.h \
		src\utils\Context.h \
		src\FileExtractor\Models\FileExtractListModel.h \
		src\models\MGFAnimationTableModel.h \
		src\models\MGFMeshTableModel.h \
		src\models\MGFMaterialTableModel.h \
		src\models\MGFNodeTreeModel.h \
		src\utils\configfile.h \
		src\utils\events\EventSelectedItemChanged.h \
		src\utils\events\IEventListener.h \
		src\utils\states\State.h \
		src\utils\states\StateGroup.h \
		src\FileExtractor\FileExtractorDialog.h \
		src\FileExtractor\Extractor.h \
		src\windows\listviewtexture.h \
		src\windows\mainwindow.h \
		src\mgfresourcemanager.h \
		src\widgets\mgfresourceviewer.h \
		src\widgets\mgftextureviewerwidget.h \
		src\widgets\mgftextviewwidget.h \
		src\widgets\mgfworkspace.h \
		src\mgf\mgfarchive.h \
		src\models\mgffiletablemodel.h \
		src\models\mgffiletreemodel.h \
		src\mgf\mgftreeitem.h \
		src\mgf\mgmaterialfactory.h \
		src\mgf\mgmeshfactory.h \
		src\mgf\mgnode.h \
		src\mgf\mgtexture.h \
		src\widgets\mgmodelviewerwidget.h \
		src\windows\ogrewindow.h \
		src\vendor\pugixml-1.10\src\pugixml.hpp \

FORMS += \
		src\FileExtractor\FileExtractorDialog.ui \
		src\widgets\mgftextureviewerwidget.ui \
		src\widgets\mgftextviewwidget.ui \
		src\widgets\mgfworkspace.ui \
		src\widgets\mgmodelviewerwidget.ui \
		src\windows\listviewtexture.ui \
		src\windows\mainwindow.ui \

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
               $$PWD/src \
               $$PWD/src/uic \
               $$PWD/src/moc

DEPENDPATH += $$(OGRE3D_DIR)/Source/ogre-1.12.6/OgreMain/include \
              $$(OGRE3D_DIR)/Source/ogre-1.12.6/RenderSystems/Direct3D9/include \

win32:UI_DIR = $$PWD/src/uic
win32:MOC_DIR = $$PWD/src/moc

win32:CONFIG(debug, debug|release):OBJECTS_DIR = $$PWD/obj/x64/Debug
win32:CONFIG(release, debug|release):OBJECTS_DIR = $$PWD/obj/x64/Release
