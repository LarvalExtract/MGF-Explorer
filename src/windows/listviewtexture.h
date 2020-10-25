#ifndef LISTVIEWTEXTURE_H
#define LISTVIEWTEXTURE_H

#include "mgf/mgfarchive.h"
#include "mgf/mgtexture.h"

#include <QWidget>
#include <QAbstractTableModel>

#include <vector>

class TextureListModel : public QAbstractTableModel
{
public:
    void BuildTextureList(const MGFArchive& archive);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    std::vector<MGTexture> m_TextureList;
};

namespace Ui {
class ListViewTexture;
}

class ListViewTexture : public QWidget
{
    Q_OBJECT

public:
    explicit ListViewTexture(QWidget *parent = nullptr);
    ~ListViewTexture();

    void SetArchive(const MGFArchive& archive);

private:
    Ui::ListViewTexture *ui;

    TextureListModel m_TextureListModel;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // LISTVIEWTEXTURE_H
