#include "TextureViewerWidget.h"
#include "ui_TextureViewerWidget.h"

#include "MGFExplorerApplication.h"
#include "MGF/Assets/TextureAsset.h"
#include "../3DSceneWidget.h"

#include <QAbstractTexture>
#include <QTextureMaterial>
#include <QPlaneMesh>
#include <QCamera>
#include <QForwardRenderer>

using namespace TextureViewer;

TextureViewerWidget::TextureViewerWidget(QWidget *parent) :
    ui(new Ui::TextureViewerWidget)
{
    ui->setupUi(this);

    SceneWidget = new TestWidget(ui->scrollArea, this);

    TextureViewerSceneRoot = new Qt3DCore::QEntity;

    Camera = new Qt3DRender::QCamera;
    Camera->lens()->setOrthographicProjection(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 10.0f);
    Camera->setPosition(QVector3D(0.0f, 0.0f, 1.0f));
    Camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    Camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

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

    TextureMaterial->setTexture(textureAsset->mTexture);
    
    ui->TextureDetailsTable->setModel(&textureAsset->TextureDetails);

    QSize size(textureAsset->mTexture->width(), textureAsset->mTexture->height());
    SceneWidget->resize(size);
	SceneWidget->update();
}

void TextureViewerWidget::on_ToggleAlphaCheckBox_toggled(bool checked)
{
    TextureViewerWidget::TextureMaterial->setAlphaBlendingEnabled(checked);
}
