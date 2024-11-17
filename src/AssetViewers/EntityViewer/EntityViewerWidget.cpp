#include "EntityViewerWidget.h"
#include "ui_EntityViewerWidget.h"

#include "MGF/Assets/MapAsset.h"

using namespace EntityViewer;

EntityViewerWidget::EntityViewerWidget(QWidget *parent) :
    ui(new Ui::EntityViewerWidget)
{
    ui->setupUi(this);

    SceneWidget = new TestWidget(ui->horizontalLayout, this);
}

EntityViewerWidget::~EntityViewerWidget()
{
    delete ui;
}

void EntityViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    const auto entityAsset = static_cast<MGF::Asset::WdfMap*>(asset.get());

    ui->entityTreeView->setModel(&entityAsset->GetEntityTreeModel());

	connect(
		ui->entityTreeView->selectionModel(),
		SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		this,
		SLOT(on_EntityTreeView_selectionChanged(const QModelIndex&, const QModelIndex&))
	);
}

void EntityViewerWidget::on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel)
{
    if (!(sel.isValid() && sel != desel))
    {
        return;
    }

    auto& selectedEntity = *static_cast<MGF::Asset::WdfEntity*>(sel.internalPointer());
    const auto attributes = &selectedEntity.Attributes;

   // ui->AttributeTableView->setModel(attributes);
}
