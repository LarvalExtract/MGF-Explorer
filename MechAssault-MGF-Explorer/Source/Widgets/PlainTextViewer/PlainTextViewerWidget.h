#pragma once

#include "Widgets/IAssetViewerWidget.h"

namespace Ui {
	class MGFPlainTextViewerWidget;
}

class MGFPlainTextViewerWidget : public IAssetViewerWidget
{
	Q_OBJECT

public:
	explicit MGFPlainTextViewerWidget(QWidget* parent = nullptr);
	~MGFPlainTextViewerWidget();

	void OnAssetLoaded(MGFAsset& asset) override;

private:
	Ui::MGFPlainTextViewerWidget* ui;
};


