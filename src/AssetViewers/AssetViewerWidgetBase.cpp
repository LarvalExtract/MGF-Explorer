#include "AssetViewerWidgetBase.h"
#include "ui_AssetViewerWidgetBase.h"

AssetViewerWidgetBase::AssetViewerWidgetBase(QWidget *parent) :
    QWidget(parent),
    baseUi(new Ui::AssetViewerWidgetBase)
{
    baseUi->setupUi(this);
}

AssetViewerWidgetBase::~AssetViewerWidgetBase()
{
    delete baseUi;
}

void AssetViewerWidgetBase::LoadAsset(MGF::Asset::AssetPtr asset)
{
	Asset = asset;

	const auto& file = Asset->GetSourceFile();

	QString str = QString("%1 | %2 | %3 | %4 bytes").arg(
		file.FilePath().c_str(),
		QString::number(file.GUID()),
		QString::number(file.FileOffset()),
		QString::number(file.FileLength())
	);

	baseUi->assetFileDetails->setText(str);
}
