 
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
    
TARGET = MechAssault-MGF-Explorer
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
        src/main.cpp \
        src/ArchiveViewer/ArchiveViewerWidget.cpp \
        src/ArchiveViewer/moc_ArchiveViewerWidget.cpp \
        src/ArchiveViewer/ContextMenus/FileMenu.cpp \
        src/ArchiveViewer/ContextMenus/TextureFileMenu.cpp \
        src/ArchiveViewer/Models/ArchiveFileTreeModel.cpp \
        src/AssetExtractors/TextureExtractor/TextureExtractor.cpp \
        src/AssetViewers/EntityViewer/EntityViewerWidget.cpp \
        src/AssetViewers/EntityViewer/moc_EntityViewerWidget.cpp \
        src/AssetViewers/EntityViewer/Models/AttributeTableModel.cpp \
        src/AssetViewers/EntityViewer/Models/EntityTreeModel.cpp \
        src/AssetViewers/ModelViewer/moc_ModelViewerWidget.cpp \
        src/AssetViewers/ModelViewer/ModelViewerWidget.cpp \
        src/AssetViewers/ModelViewer/Models/AnimationTableModel.cpp \
        src/AssetViewers/ModelViewer/Models/MaterialTableModel.cpp \
        src/AssetViewers/ModelViewer/Models/MeshTableModel.cpp \
        src/AssetViewers/ModelViewer/Models/NodeTreeModel.cpp \
        src/AssetViewers/PlainTextViewer/moc_PlainTextViewerWidget.cpp \
        src/AssetViewers/PlainTextViewer/PlainTextViewerWidget.cpp \
        src/AssetViewers/StringTableViewer/moc_StringTableViewerWidget.cpp \
        src/AssetViewers/StringTableViewer/StringTableViewerWidget.cpp \
        src/AssetViewers/StringTableViewer/Models/StringTableModel.cpp \
        src/AssetViewers/TextureViewer/moc_TextureViewerWidget.cpp \
        src/AssetViewers/TextureViewer/TextureViewerWidget.cpp \
        src/AssetViewers/TextureViewer/Models/TextureDetailsTableModel.cpp \
        src/FileExtractorWindow/Extractor.cpp \
        src/FileExtractorWindow/FileExtractorDialog.cpp \
        src/FileExtractorWindow/moc_FileExtractorDialog.cpp \
        src/FileExtractorWindow/Models/FileExtractListModel.cpp \
        src/FileExtractorWindow/States/FileExtractorStateGroup.cpp \
        src/FileExtractorWindow/States/FinishedState.cpp \
        src/FileExtractorWindow/States/IdleState.cpp \
        src/FileExtractorWindow/States/ProgressState.cpp \
        src/MainWindow/MainWindow.cpp \
        src/MainWindow/moc_MainWindow.cpp \
        src/MGF/Archive.cpp \
        src/MGF/File.cpp \
        src/MGF/Assets/EntityAsset.cpp \
        src/MGF/Assets/ModelAsset.cpp \
        src/MGF/Assets/PlainText.cpp \
        src/MGF/Assets/StringTable.cpp \
        src/MGF/Assets/Texture.cpp \
        src/MGF/Assets/Factories/MaterialFactory.cpp \
        src/MGF/Assets/Factories/MeshFactory.cpp \
        src/MGF/Factories/ImageFactory.cpp \
        src/OgreWindow/moc_ogrewindow.cpp \
        src/OgreWindow/ogrewindow.cpp \
        src/ResourceManager/AssetFactory.cpp \
        src/ResourceManager/ResourceManager.cpp \
        src/Utilities/configfile.cpp \
        src/Utilities/ContextProvider/Context.cpp \
        src/Utilities/ContextProvider/ServiceProvider.cpp \
        src/Utilities/StateMachine/State.cpp \
        src/Utilities/StateMachine/StateGroup.cpp

HEADERS += \
        src/ArchiveViewer/ArchiveViewerWidget.h \
        src/ArchiveViewer/ui_ArchiveViewerWidget.h \
        src/ArchiveViewer/ContextMenus/FileMenu.h \
        src/ArchiveViewer/ContextMenus/TextureFileMenu.h \
        src/ArchiveViewer/Models/ArchiveFileTreeModel.h \
        src/AssetExtractors/IAssetExtractor.h \
        src/AssetExtractors/TextureExtractor/TextureExtractor.h \
        src/AssetViewers/IAssetViewerWidget.h \
        src/AssetViewers/EntityViewer/EntityViewerWidget.h \
        src/AssetViewers/EntityViewer/ui_EntityViewerWidget.h \
        src/AssetViewers/EntityViewer/Models\AttributeTableModel.h \
        src/AssetViewers/EntityViewer/Models\EntityTreeModel.h \
        src/AssetViewers/ModelViewer/ModelViewerWidget.h \
        src/AssetViewers/ModelViewer/ui_ModelViewerWidget.h \
        src/AssetViewers/ModelViewer/Models/AnimationTableModel.h \
        src/AssetViewers/ModelViewer/Models/MaterialTableModel.h \
        src/AssetViewers/ModelViewer/Models/MeshTableModel.h \
        src/AssetViewers/ModelViewer/Models/NodeTreeModel.h \
        src/AssetViewers/PlainTextViewer/PlainTextViewerWidget.h \
        src/AssetViewers/PlainTextViewer/ui_PlainTextViewerWidget.h \
        src/AssetViewers/StringTableViewer/StringTableViewerWidget.h \
        src/AssetViewers/StringTableViewer/ui_StringTableViewerWidget.h \
        src/AssetViewers/StringTableViewer/Models/StringTableModel.h \
        src/AssetViewers/TextureViewer/TextureViewerWidget.h \
        src/AssetViewers/TextureViewer/ui_TextureViewerWidget.h \
        src/AssetViewers/TextureViewer/Models/TextureDetailsTableModel.h \
        src/FileExtractorWindow/Extractor.h \
        src/FileExtractorWindow/FileExtractorDialog.h \
        src/FileExtractorWindow/ui_FileExtractorDialog.h \
        src/FileExtractorWindow/Models/FileExtractItem.h \
        src/FileExtractorWindow/Models/FileExtractListModel.h \
        src/FileExtractorWindow/States/FileExtractorStateBase.h \
        src/FileExtractorWindow/States/FileExtractorStateGroup.h \
        src/FileExtractorWindow/States/FinishedState.h \
        src/FileExtractorWindow/States/IdleState.h \
        src/FileExtractorWindow/States/ProgressState.h \
        src/MainWindow/MainWindow.h \
        src/MainWindow/ui_MainWindow.h \
        src/MGF/Archive.h \
        src/MGF/EFileTypes.h \
        src/MGF/File.h \
        src/MGF/VersionConstants.h \
        src/MGF/Assets/AssetBase.h \
        src/MGF/Assets/AssetMappings.h \
        src/MGF/Assets/EAssetTypes.h \
        src/MGF/Assets/EntityAsset.h \
        src/MGF/Assets/ModelAsset.h \
        src/MGF/Assets/PlainText.h \
        src/MGF/Assets/StringTable.h \
        src/MGF/Assets/Texture.h \
        src/MGF/Assets/Factories/AttributeNames.h \
        src/MGF/Assets/Factories/EntityNames.h \
        src/MGF/Assets/Factories/MaterialFactory.h \
        src/MGF/Assets/Factories/MeshFactory.h \
        src/MGF/Assets/Model/Animation.h \
        src/MGF/Assets/Model/Material.h \
        src/MGF/Assets/Model/Mesh.h \
        src/MGF/Assets/Model/Node.h \
        src/MGF/Factories/ImageFactory.h \
        src/MGF/Structures/Common.h \
        src/MGF/Structures/mgf.h \
        src/MGF/Structures/MgText.h \
        src/MGF/Structures/Tif.h \
        src/MGF/Structures/wdf.h \
        src/OgreWindow/ogrewindow.h \
        src/ResourceManager/AssetFactory.h \
        src/ResourceManager/ResourceManager.h \
        src/Utilities/configfile.h \
        src/Utilities/ContextProvider/Context.h \
        src/Utilities/ContextProvider/ServiceProvider.h \
        src/Utilities/StateMachine/State.h \
        src/Utilities/StateMachine/StateGroup.h

FORMS += \
        src/ArchiveViewer/ArchiveViewerWidget.ui \
        src/AssetViewers/EntityViewer/EntityViewerWidget.ui \
        src/AssetViewers/ModelViewer/ModelViewerWidget.ui \
        src/AssetViewers/PlainTextViewer/PlainTextViewerWidget.ui \
        src/AssetViewers/StringTableViewer/StringTableViewerWidget.ui \
        src/AssetViewers/TextureViewer/TextureViewerWidget.ui \
        src/FileExtractorWindow/FileExtractorDialog.ui \
        src/MainWindow/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += \
	-L$$(QT_DIR)/5.14.2/msvc2017_64/lib/ -lQt5Core \
	-L$$(QT_DIR)/5.14.2/msvc2017_64/lib/ -lQt5Gui \
	-L$$(QT_DIR)/5.14.2/msvc2017_64/lib/ -lQt5Widgets \
	-L$$(OGRE3D_DIR)/Binaries/lib/Release/ -lOgreMain \
	-L$$(OGRE3D_DIR)/Binaries/lib/Release/ -lRenderSystem_Direct3D9

else:win32:CONFIG(debug, debug|release): LIBS += \
	-L$$(QT_DIR)/5.14.2/msvc2017_64/lib/ -lQt5Cored \
	-L$$(QT_DIR)/5.14.2/msvc2017_64/lib/ -lQt5Guid \
	-L$$(QT_DIR)/5.14.2/msvc2017_64/lib/ -lQt5Widgetsd \
	-L$$(OGRE3D_DIR)/Binaries/lib/Debug/ -lOgreMain_d \
	-L$$(OGRE3D_DIR)/Binaries/lib/Debug/ -lRenderSystem_Direct3D9_d

INCLUDEPATH += \
        $$PWD/src \
	$$PWD/vendor \
	$$(QT_DIR)/5.14.2/msvc2017_64/include \
	$$(QT_DIR)/5.14.2/msvc2017_64/include/QtCore \
	$$(QT_DIR)/5.14.2/msvc2017_64/include/QtGui \
	$$(QT_DIR)/5.14.2/msvc2017_64/include/QtWidgets \
	$$(OGRE3D_DIR)/Binaries/include \
	$$(OGRE3D_DIR)/Source/ogre-1.12.6/OgreMain/include \
	$$(OGRE3D_DIR)/Source/ogre-1.12.6/RenderSystems/Direct3D9/include

DEPENDPATH += \
	$(QT_DIR)/5.14.2/msvc2017_64/lib \
	$(OGRE3D_DIR)/Binaries/lib/Debug

win32:CONFIG(debug, debug|release):OBJECTS_DIR = $$PWD/obj/x64/Debug
win32:CONFIG(release, debug|release):OBJECTS_DIR = $$PWD/obj/x64/Release

win32:RC_ICONS += madcat.ico
