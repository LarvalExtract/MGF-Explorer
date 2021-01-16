#include "mgfworkspace.h"
#include "ui_mgfworkspace.h"

#include "FileExtractor/FileExtractorDialog.h"

#include "mgf/Assets/StringTable.h"

MGFWorkspace::MGFWorkspace(const QString& mgfFilePath, QWidget *parent) :
    QWidget(parent),
    m_MgfArchive(mgfFilePath),
    ui(new Ui::MGFWorkspace)
{
    ui->setupUi(this); 
    ui->treeView->setModel(m_MgfArchive.FileTreeModel());
    ui->treeView->setColumnWidth(0, 300);

    ui->fileDetails->hide();

    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(on_treeView_selectionChanged(const QModelIndex&, const QModelIndex&)));


    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView,
            SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(on_treeView_customContextMenuRequested));

    auto action = m_ContextMenu.addAction("Extract...");
    connect(action, &QAction::triggered, this, &MGFWorkspace::on_customContextMenu_action_extract);

    m_FileTableWindowUi.setupUi(&m_FileTableWidget);

    m_FileTableWidget.setWindowTitle(m_MgfArchive.GetFileName() + " file table");
    m_FileTableWindowUi.tableView->setModel(m_MgfArchive.FileTableModel());
    m_FileTableWindowUi.tableView->setColumnWidth(0, 70);
    m_FileTableWindowUi.tableView->setColumnWidth(1, 140);
    m_FileTableWindowUi.tableView->setColumnWidth(2, 520);
    m_FileTableWidget.hide();
}

MGFWorkspace::~MGFWorkspace()
{
    delete ui;
}

QModelIndexList MGFWorkspace::GetSelection() const
{
    return ui->treeView->selectionModel()->selectedRows();
}

void MGFWorkspace::ShowFileTableWindow()
{
    if (m_FileTableWidget.isHidden())
        m_FileTableWidget.show();
}

void MGFWorkspace::on_treeView_selectionChanged(const QModelIndex &sel, const QModelIndex &desel)
{
    if (!sel.isValid())
        return;

    if (sel == desel)
        return;

    m_SelectedItem = static_cast<MGF::File*>(sel.internalPointer());

    switch (m_SelectedItem->FileType())
    {
    case MGFFileType::Text:
        m_CurrentWidget = DisplayWidget(&m_TextWidget);
        m_TextWidget.LoadMGFItem(m_SelectedItem);
        break;

    case MGFFileType::Texture:
        m_CurrentWidget = DisplayWidget(&m_TextureWidget);
        m_TextureWidget.LoadMGFItem(m_SelectedItem);
        break;

    case MGFFileType::Model:
        m_CurrentWidget = DisplayWidget(&m_ModelWidget);
        m_ModelWidget.LoadMGFItem(m_SelectedItem);
        break;

    case MGFFileType::Node:
        m_CurrentWidget = DisplayWidget(&m_ModelWidget);
        m_ModelWidget.LoadMGFItem(m_SelectedItem);
        break;

    case MGFFileType::Strings:
        m_CurrentWidget = DisplayWidget(&m_StringTableWidget);
        {
            MGF::Asset::StringTable strings(*m_SelectedItem);
        }
        break;

    default:
        m_CurrentWidget = DisplayWidget(nullptr);
        break;
    }

    PrintSelectedFileDetails(*m_SelectedItem);
}

QWidget* MGFWorkspace::DisplayWidget(QWidget *widget)
{
    // do nothing
    if (widget == m_CurrentWidget)
        return widget;

    auto frame = ui->assetViewFrame->layout();

    if (m_CurrentWidget != nullptr)
        m_CurrentWidget->hide();

    if (frame->isEmpty())
    {
        // add/remove widget
        if (widget == nullptr)
        {
            return widget;
        }
        else
        {
            frame->addWidget(widget);
            widget->show();
        }
    }
    else
    {
        // change widget
        if (widget == nullptr)
        {
            frame->removeWidget(m_CurrentWidget);
        }
        else
        {
            frame->replaceWidget(m_CurrentWidget, widget);
            widget->show();
        }
    }

    return widget;
}

void MGFWorkspace::PrintSelectedFileDetails(const MGF::File& selectedFile)
{
    if (!selectedFile.IsFile())
    {
        ui->fileDetails->hide();
        return;
    }

    QString str = QString("%1 | %2 | %3 | %4 bytes").arg(
        selectedFile.FilePath().c_str(),
        QString::number(selectedFile.GUID()),
        QString::number(selectedFile.FileOffset()),
        QString::number(selectedFile.FileLength())
    );

    ui->fileDetails->setText(str);
    ui->fileDetails->show();
}

void MGFWorkspace::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index(ui->treeView->indexAt(pos));
    if (index.isValid())
    {
        m_ContextMenu.popup(ui->treeView->viewport()->mapToGlobal(pos));
    }
}

void MGFWorkspace::on_customContextMenu_action_extract()
{
	const auto& selection = ui->treeView->selectionModel()->selection().indexes();

    FileExtractor::FileExtractorDialog dialog(this);
    dialog.SetSelection(selection, m_MgfArchive);
    dialog.exec();
}
