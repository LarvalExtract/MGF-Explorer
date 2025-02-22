#pragma once

#include "Widgets/IAssetViewerWidget.h"
#include "Widgets/3DSceneWidget.h"

#include "Qt3DForwardDeclarations.h"

namespace Ui {
    class MGFModelViewerWidget;
}

class MGFModelViewerWidget : public IAssetViewerWidget
{
	Q_OBJECT

public:
	explicit MGFModelViewerWidget(QWidget* parent = nullptr);
	~MGFModelViewerWidget();

	void OnAssetLoaded(MGFAsset& asset) override;

private:
	Ui::MGFModelViewerWidget* ui;
	TestWidget* SceneWidget = nullptr;

private slots:
	void on_lightPositionXInput_changed(int value);
	void on_lightPositionYInput_changed(int value);
	void on_lightPositionZInput_changed(int value);
	void on_enableSceneLightsCheckBox_stateChanged(int state);
};

