#include "TextureViewerWidget.h"
#include "ui_TextureViewerWidget.h"

#include "MGFExplorerApplication.h"
#include "MGF/Assets/TextureAsset.h"

#include <QAbstractTexture>
#include <QTextureMaterial>
#include <QPlaneMesh>
#include <QCamera>
#include <QForwardRenderer>

using namespace TextureViewer;

Qt3DExtras::Qt3DWindow* TextureViewerWidget::RenderWindowPtr = nullptr;
Qt3DCore::QEntity* TextureViewerWidget::TextureViewerSceneRoot = nullptr;
Qt3DExtras::QTextureMaterial* TextureViewerWidget::TextureMaterial = nullptr;
Qt3DExtras::QPlaneMesh* TextureViewerWidget::TextureSurface = nullptr;

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

    TextureViewerWidget::TextureMaterial->setTexture(textureAsset->mTexture);
    
    ui->TextureDetailsTable->setModel(&textureAsset->TextureDetails);

    Qt3DRender::QCamera* camera = TextureViewerWidget::RenderWindowPtr->camera();
    camera->lens()->setOrthographicProjection(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 10.0f);
    camera->setPosition(QVector3D(0.0f, 0.0f, 1.0f));
    camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    QSize size(textureAsset->mTexture->width(), textureAsset->mTexture->height());
    qApp->GetRenderWindowContainer()->resize(size);
	qApp->GetRenderWindowContainer()->update();
}

bool TextureViewerWidget::InitialiseScene(Qt3DExtras::Qt3DWindow* renderWindow)
{
    TextureViewerWidget::RenderWindowPtr = renderWindow;

    TextureViewerWidget::TextureViewerSceneRoot = new Qt3DCore::QEntity;

    TextureViewerWidget::TextureMaterial = new Qt3DExtras::QTextureMaterial;
    TextureViewerWidget::TextureViewerSceneRoot->addComponent(TextureMaterial);

    TextureViewerWidget::TextureSurface = new Qt3DExtras::QPlaneMesh;
    TextureViewerWidget::TextureViewerSceneRoot->addComponent(TextureSurface);

    Qt3DCore::QTransform* planeTransform = new Qt3DCore::QTransform;
    planeTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    planeTransform->setRotation(QQuaternion::fromEulerAngles(QVector3D(90.0f, 0.0f, 0.0f)));
    planeTransform->setScale(1.0f);
    TextureViewerWidget::TextureViewerSceneRoot->addComponent(planeTransform);

    return true;
}

void TextureViewerWidget::on_ToggleAlphaCheckBox_toggled(bool checked)
{
    TextureViewerWidget::TextureMaterial->setAlphaBlendingEnabled(checked);
}

void TextureViewerWidget::showEvent(QShowEvent* event)
{
    TextureViewerWidget::RenderWindowPtr->setRootEntity(TextureViewerWidget::TextureViewerSceneRoot);

    

    const QColor bgColor = ui->scrollArea->palette().color(ui->scrollArea->backgroundRole());

    ui->scrollArea->setWidget(qApp->GetRenderWindowContainer());
    qApp->GetRenderWindowContainer()->show();
}

void TextureViewerWidget::hideEvent(QHideEvent* event)
{
    TextureViewerWidget::RenderWindowPtr->setRootEntity(nullptr);

    qApp->GetRenderWindowContainer()->hide();
    ui->scrollArea->setWidget(nullptr);
}
