#include "TextureViewerWidget.h"
#include "ui_TextureViewerWidget.h"

#include "MGF/Assets/TextureAsset.h"

static int s_Count = 0;

using namespace TextureViewer;

TextureViewerWidget::TextureViewerWidget(QWidget *parent) :
    ui(new Ui::TextureViewerWidget)
{
    ui->setupUi(this);

    s_Count++;

    try
    {
        QString title("TextureViewerWindow");
        title += QString::number(s_Count);
        m_OgreWindow.initialize(title);

        m_Container = QWidget::createWindowContainer(&m_OgreWindow, this);
        ui->scrollArea->setWidget(m_Container);
        ui->scrollArea->setBackgroundRole(QPalette::Dark);

        InitialiseScene();
    }
    catch (const Ogre::Exception& e)
    {
        throw std::runtime_error(e.what());
    }
}

TextureViewerWidget::~TextureViewerWidget()
{
    delete ui;
}

void TextureViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    const auto textureAsset = static_cast<MGF::Asset::TextureAsset*>(asset.get());

    ui->TextureDetailsTable->setModel(&textureAsset->TextureDetails);

	m_Container->resize(textureAsset->OgreTexture->getWidth(), textureAsset->OgreTexture->getHeight());
	m_TextureUnit->setTexture(textureAsset->OgreTexture);
	m_OgreWindow.render();
	m_Container->update();
}

void TextureViewerWidget::InitialiseScene()
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
