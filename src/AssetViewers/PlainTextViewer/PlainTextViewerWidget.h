#ifndef MGFTEXTVIEWWIDGET_H
#define MGFTEXTVIEWWIDGET_H

#include "AssetViewers/AssetViewerWidgetBase.h"

#include <QWidget>

namespace Ui {
class PlainTextViewerWidget;
}

namespace PlainTextViewer {

	class PlainTextViewerWidget : public AssetViewerWidgetBase
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


#endif // MGFTEXTVIEWWIDGET_H
