#include "TextureViewerWidget.h"
#include "ui_TextureViewerWidget.h"

#include "MGFExplorerApplication.h"
#include "MGF/Assets/TextureAsset.h"

#include <QAbstractTexture>
#include <QTextureMaterial>
#include <QCamera>
#include <QForwardRenderer>

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

    QSize size(textureAsset->mTexture->width(), textureAsset->mTexture->height());
    
    const auto [pCamera, pMaterial] = qApp->GetTextureViewerData();

    pMaterial->setTexture(textureAsset->mTexture);
    const auto bgColor = ui->scrollArea->palette().color(ui->scrollArea->backgroundRole());
    qApp->GetRenderWindow()->defaultFrameGraph()->setClearColor(bgColor);
    qApp->GetRenderWindowContainer()->resize(size);
	qApp->GetRenderWindowContainer()->update();
}

void TextureViewerWidget::on_ToggleAlphaCheckBox_toggled(bool checked)
{
    const auto [_, pMaterial] = qApp->GetTextureViewerData();
    pMaterial->setAlphaBlendingEnabled(checked);
}

void TextureViewerWidget::showEvent(QShowEvent* event)
{
    ui->scrollArea->setWidget(qApp->GetRenderWindowContainer());
    qApp->GetRenderWindowContainer()->show();
}

void TextureViewerWidget::hideEvent(QHideEvent* event)
{
    qApp->GetRenderWindowContainer()->hide();
    ui->scrollArea->setWidget(nullptr);
}
