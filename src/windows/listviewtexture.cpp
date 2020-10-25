#include "listviewtexture.h"
#include "ui_listviewtexture.h"

ListViewTexture::ListViewTexture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListViewTexture)
{
    ui->setupUi(this);
}

ListViewTexture::~ListViewTexture()
{
    delete ui;
}

void ListViewTexture::SetArchive(const MGFArchive &archive)
{
    m_TextureListModel.BuildTextureList(archive);

    ui->tableView->setModel(&m_TextureListModel);
    ui->tableView->update();
}

void ListViewTexture::closeEvent(QCloseEvent *event)
{
    m_TextureListModel.m_TextureList.clear();

    QWidget::closeEvent(event);
}

void TextureListModel::BuildTextureList(const MGFArchive &archive)
{
    m_TextureList.reserve(archive.NumTextures());

    for (const auto& item : archive.m_TreeItems)
    {
        if (item.FileType() == MGFFileType::Texture)
        {
            m_TextureList.emplace_back(const_cast<MGFTreeItem&>(item), true);
        }
    }

}

int TextureListModel::rowCount(const QModelIndex &parent) const
{
    return m_TextureList.size();
}

int TextureListModel::columnCount(const QModelIndex &parent) const
{
    return 10;
}

QVariant TextureListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    auto& texture = m_TextureList[index.row()];
    auto& item = texture.TreeItem();

    // returns the bpp for a texture
    auto bpp = [&texture]() -> unsigned int
    {
        int w = texture.Width();
        int h = texture.Height();
        int s = texture.Size();

        // subtract additional space caused by mip maps
        for (int i = 0; i < texture.Mips() - 1; i++)
        {
            w /= 2;
            h /= 2;
            s -= w * h;
        }

        s /= texture.Frames();
        s /= texture.Depth();

        return (s / (texture.Width() * texture.Height())) * 8;
    };

    switch (index.column())
    {
    case 0: return item.Name();
    case 1: return texture.Width();
    case 2: return texture.Height();
    case 3: return QString::number(texture.Flags(), 16);
    case 4: return texture.Mips();
    case 5: return texture.Size();
    case 6: return texture.Frames();
    case 7: return texture.Depth();
    case 8: return texture.Bits();
    case 9: return bpp();

    default: return QVariant();
    }
}

QVariant TextureListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case 0: return "File name";
    case 1: return "Width";
    case 2: return "Height";
    case 3: return "Flags";
    case 4: return "Mips";
    case 5: return "Size";
    case 6: return "Frames";
    case 7: return "Depth";
    case 8: return "Bits";
    case 9: return "Bits per pixel";

    default: return QVariant();
    }
}
