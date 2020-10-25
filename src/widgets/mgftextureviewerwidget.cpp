#include "mgftextureviewerwidget.h"
#include "ui_mgftextureviewerwidget.h"

#include "windows/ogrewindow.h"

enum TextureTableColumns
{
    COLUMN_WIDTH,
    COLUMN_HEIGHT,
    COLUMN_FLAGS,
    COLUMN_MIPS,
    COLUMN_TYPE,
    COLUMN_FORMAT,
    COLUMN_FRAMES,
    COLUMN_DEPTH,
    COLUMN_SIZE
};

static const QStringList headerLabels = {
    "Width",
    "Height",
    "Flags",
    "Mips",
    "Type",
    "Format",
    "Frames",
    "Depth",
    "Size"
};

static int s_Count = 0;

MGFTextureViewerWidget::MGFTextureViewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MGFTextureViewerWidget)
{
    m_SupportedTypes.insert(MGFFileType::Texture);

    ui->setupUi(this);
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    s_Count++;

    try
    {
        QString title("TextureViewerWindow");
        title += QString::number(s_Count);
        m_OgreWindow.initialize(title);

        m_Container = QWidget::createWindowContainer(&m_OgreWindow, this);
        m_Container->setGeometry(ui->tableWidget->pos().x(), ui->tableWidget->height() + 9, 1, 1);

        InitialiseScene();
    }
    catch (const Ogre::Exception& e)
    {
        std::string s = e.what();
    }

    for (size_t i = 0; i < m_TableItems.size(); i++)
        ui->tableWidget->setItem(0, i, &m_TableItems[i]);

}

MGFTextureViewerWidget::~MGFTextureViewerWidget()
{
    //m_SceneManager->clearScene();
    //Ogre::Root::getSingleton().destroySceneManager(m_SceneManager);

    delete ui;
}

void MGFTextureViewerWidget::LoadMGFItem(const MGFTreeItem *item)
{
    AMGFResourceViewer::LoadMGFItem(item);

    const auto& mgTexture = ResourceManager.GetTexture(*item);

    m_TableItems[COLUMN_WIDTH   ].setData(Qt::DisplayRole, mgTexture.Width());
    m_TableItems[COLUMN_HEIGHT  ].setData(Qt::DisplayRole, mgTexture.Height());
    m_TableItems[COLUMN_FLAGS   ].setData(Qt::DisplayRole, QString::number(mgTexture.Flags(), 16));
    m_TableItems[COLUMN_MIPS    ].setData(Qt::DisplayRole, mgTexture.Mips());
    m_TableItems[COLUMN_TYPE    ].setData(Qt::DisplayRole, "UNF");
    m_TableItems[COLUMN_FORMAT  ].setData(Qt::DisplayRole, Ogre::PixelUtil::getFormatName(mgTexture.m_Texture->getFormat()).data());
    m_TableItems[COLUMN_FRAMES  ].setData(Qt::DisplayRole, mgTexture.Frames());
    m_TableItems[COLUMN_DEPTH   ].setData(Qt::DisplayRole, mgTexture.Depth());
    m_TableItems[COLUMN_SIZE    ].setData(Qt::DisplayRole, mgTexture.Size());   

    m_Container->resize(mgTexture.Width(), mgTexture.Height());
    m_TextureUnit->setTexture(mgTexture.m_Texture);
    m_OgreWindow.render();
    m_Container->update();
}

void MGFTextureViewerWidget::InitialiseScene()
{
    m_SceneManager = Ogre::Root::getSingleton().createSceneManager();

    m_OrthoCamera = m_SceneManager->createCamera("TextureViewerCamera");
    m_OrthoCamera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
    m_OrthoCamera->setNearClipDistance(1.0f);
    m_OrthoCamera->setFarClipDistance(2.0f);
    m_OrthoCamera->setOrthoWindow(200.0f, 200.0f);

    m_TextureViewerViewport = m_OgreWindow.GetWindow().addViewport(m_OrthoCamera);

    m_OrthoCameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    m_OrthoCameraNode->attachObject(m_OrthoCamera);
    m_OrthoCameraNode->setPosition(0.0f, 0.0f, 1.0f);

    m_MatUnlitTextured = Ogre::MaterialManager::getSingleton().create(m_OgreWindow.GetWindow().getName(), "General");
    auto technique = m_MatUnlitTextured->getTechnique(0);
    auto pass = technique->getPass(0);
    pass->setLightingEnabled(false);
    m_TextureUnit = pass->createTextureUnitState();
    m_TextureUnit->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);

    Ogre::SceneNode* node = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    m_TexturePlane = m_SceneManager->createEntity(Ogre::SceneManager::PrefabType::PT_PLANE);
    m_TexturePlane->setMaterial(m_MatUnlitTextured);
    node->attachObject(m_TexturePlane);
}
