#pragma once

#include "AssetViewers/IAssetViewerWidget.h"

namespace Ui {
	class PlainTextViewerWidget;
}

namespace PlainTextViewer {

	class PlainTextViewerWidget : public IAssetViewerWidget
	{
		Q_OBJECT

	public:
		explicit PlainTextViewerWidget(QWidget* parent = nullptr);
		~PlainTextViewerWidget();

		void LoadAsset(MGF::Asset::AssetPtr asset) override;

	private:
		Ui::PlainTextViewerWidget* ui;
	};

}

