#ifndef FILEEXTRACTDIALOG_H
#define FILEEXTRACTDIALOG_H

#include "mgf/mgftreeitem.h"
#include "fileextractor.h"
#include "models/FileExtractListModel.h"

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>

#include <vector>

namespace Ui {
class FileExtractDialog;
}

class FileExtractDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileExtractDialog(const QModelIndexList& selection, const MGFArchive& archive, QWidget *parent = nullptr);
    ~FileExtractDialog();

public slots:
    void accept();

private slots:
    void on_browseButton_clicked();
    void on_checkBoxFilepaths_stateChanged(int arg1);
    void on_checkBoxOverwrite_stateChanged(int arg1);

private:
    Ui::FileExtractDialog *ui;
    FileExtractor m_Extractor;
    FileExtractListModel FileListModel;

    bool bPreserveFolders = true;
    bool bOverwriteFiles = false;
    bool bDone = false;

private:
    void Start();
    void SetOutputFolder(const char* path);
};

#endif // FILEEXTRACTDIALOG_H
