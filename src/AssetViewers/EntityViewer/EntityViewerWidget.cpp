#include "EntityViewerWidget.h"
#include "ui_EntityViewerWidget.h"

#include "MGF/Assets/EntityAsset.h"

using namespace EntityViewer;

EntityViewerWidget::EntityViewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntityViewerWidget)
{
    ui->setupUi(this);

	connect(
		ui->EntityTreeView->selectionModel(),
		SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		this,
		SLOT(on_EntityTreeView_selectionChanged(const QModelIndex&, const QModelIndex&))
	);
}

EntityViewerWidget::~EntityViewerWidget()
{
    delete ui;
}

void EntityViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    const auto entityAsset = static_cast<MGF::Asset::EntityAsset*>(asset.get());

    ui->EntityTreeView->setModel(entityAsset->GetEntityTreeModel());
}

void EntityViewerWidget::on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel)
{
    
}
