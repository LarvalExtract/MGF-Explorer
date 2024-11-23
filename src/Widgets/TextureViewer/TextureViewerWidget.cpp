#include "TextureViewerWidget.h"
#include "ui_TextureViewerWidget.h"

#include "MGF/Assets/MGFTexture.h"
#include "Widgets/3DSceneWidget.h"

#include <QTextureMaterial>
#include <QEntity>
#include <Qt3DCore/QTransform>
#include <QPlaneMesh>

MGFTextureViewerWidget::MGFTextureViewerWidget(QWidget *parent) :
    ui(new Ui::MGFTextureViewerWidget)
{
    ui->setupUi(this);

    SceneWidget = new TestWidget(ui->scrollArea, this);

    TextureViewerSceneRoot = new Qt3DCore::QEntity;

    TextureMaterial = new Qt3DExtras::QTextureMaterial;
    TextureViewerSceneRoot->addComponent(TextureMaterial);

    TextureSurface = new Qt3DExtras::QPlaneMesh;
    TextureViewerSceneRoot->addComponent(TextureSurface);

    Qt3DCore::QTransform* planeTransform = new Qt3DCore::QTransform;
    planeTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    planeTransform->setRotation(QQuaternion::fromEulerAngles(QVector3D(90.0f, 0.0f, 0.0f)));
    planeTransform->setScale(1.0f);

    TextureViewerSceneRoot->addComponent(planeTransform);
    
    connect(
        ui->ToggleAlphaCheckBox,
        &QCheckBox::clicked,
        this,
        &MGFTextureViewerWidget::on_ToggleAlphaCheckBox_toggled
    );
}

MGFTextureViewerWidget::~MGFTextureViewerWidget()
{
    delete ui;
}

void MGFTextureViewerWidget::OnAssetLoaded(MGFAsset& asset)
{
    MGFTexture& textureAsset = static_cast<MGFTexture&>(asset);

    TextureMaterial->setTexture(textureAsset.mTexture);
    
    ui->TextureDetailsTable->setModel(&textureAsset.TextureDetails);

    QSize size(textureAsset.mTexture->width(), textureAsset.mTexture->height());
    SceneWidget->resize(size);
	SceneWidget->update();
}

void MGFTextureViewerWidget::on_ToggleAlphaCheckBox_toggled(bool checked)
{
    MGFTextureViewerWidget::TextureMaterial->setAlphaBlendingEnabled(checked);
}
