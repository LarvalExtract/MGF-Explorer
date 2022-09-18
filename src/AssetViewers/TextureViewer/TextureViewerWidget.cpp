#include "TextureViewerWidget.h"
#include "ui_TextureViewerWidget.h"

#include "MGF/Assets/TextureAsset.h"

#include "MGFExplorerApplication.h"

using namespace TextureViewer;

TextureViewerWidget::TextureViewerWidget(QWidget *parent) :
    ui(new Ui::TextureViewerWidget)
{
    ui->setupUi(this);
    
    connect(
        ui->ToggleAlphaCheckBox,
        &QCheckBox::clicked,
        this,
        &TextureViewerWidget::on_ToggleAlphaCheckBox_toggled
    );
}

TextureViewerWidget::~TextureViewerWidget()
{
    delete ui;
}

void TextureViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    const auto textureAsset = static_cast<MGF::Asset::TextureAsset*>(asset.get());

    ui->TextureDetailsTable->setModel(&textureAsset->TextureDetails);

	qApp->GetRenderWindowContainer()->resize(textureAsset->OgreTexture->getWidth(), textureAsset->OgreTexture->getHeight());
	m_TextureUnit->setTexture(textureAsset->OgreTexture);
	qApp->GetRenderWindow()->render();
	qApp->GetRenderWindowContainer()->update();
}

void TextureViewerWidget::InitialiseScene()
{
    m_SceneManager = Ogre::Root::getSingleton().createSceneManager();

    m_OrthoCamera = m_SceneManager->createCamera("TextureViewerCamera");
    m_OrthoCamera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
    m_OrthoCamera->setNearClipDistance(1.0f);
    m_OrthoCamera->setFarClipDistance(2.0f);
    m_OrthoCamera->setOrthoWindow(200.0f, 200.0f);

    m_TextureViewerViewport = qApp->GetRenderWindow()->GetWindow().addViewport(m_OrthoCamera);
    constexpr float grey = 160.0f / 255.0f;
	const auto bgColour = Ogre::ColourValue(grey, grey, grey, 1.0f);
    m_TextureViewerViewport->setBackgroundColour(bgColour);

    m_OrthoCameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    m_OrthoCameraNode->attachObject(m_OrthoCamera);
    m_OrthoCameraNode->setPosition(0.0f, 0.0f, 1.0f);
    
    auto result = Ogre::MaterialManager::getSingleton().createOrRetrieve("MatUnlit", "General");
    m_MatUnlitTextured = result.first;
    auto technique = m_MatUnlitTextured->getTechnique(0);
    auto pass = technique->getPass(0);
    pass->setLightingEnabled(false);
    m_TextureUnit = pass->createTextureUnitState();
    m_TextureUnit->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);

    Ogre::SceneNode* node = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    m_TexturePlane = m_SceneManager->createEntity(Ogre::SceneManager::PrefabType::PT_PLANE);
    m_TexturePlane->setMaterial(m_MatUnlitTextured);
    node->attachObject(m_TexturePlane);
    node->setPosition(0.5f, -0.5f, 0.0f);
}

void TextureViewerWidget::on_ToggleAlphaCheckBox_toggled(bool checked)
{
    auto pass = m_MatUnlitTextured->getTechnique(0)->getPass(0);
    pass->setSceneBlending(checked
        ? Ogre::SBT_TRANSPARENT_ALPHA
        : Ogre::SBT_REPLACE
    );

    qApp->GetRenderWindow()->render();
}

void TextureViewerWidget::showEvent(QShowEvent* event)
{
    qApp->GetRenderWindowContainer()->show();
    ui->scrollArea->setWidget(qApp->GetRenderWindowContainer());
}

void TextureViewerWidget::hideEvent(QHideEvent* event)
{
    qApp->GetRenderWindowContainer()->hide();
    ui->scrollArea->setWidget(nullptr);
}
