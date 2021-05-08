#ifndef FILEEXTRACTORDIALOG_H
#define FILEEXTRACTORDIALOG_H

#include "FileExtractorWindow/Models/FileExtractListModel.h"
#include "FileExtractorWindow/States/FileExtractorStateGroup.h"

#include <QDialog>
#include <filesystem>

namespace Ui {
	class FileExtractorDialog;
}

namespace FileExtractor {

	class FileExtractorDialog : public QDialog
	{
		Q_OBJECT

	public:
		explicit FileExtractorDialog(QWidget* parent = nullptr);
		~FileExtractorDialog();

		void QueueFiles(const std::vector<Models::FileExtractItem>& fileList);

		inline const std::filesystem::path& GetDestination() const { return Destination; }

	private slots:
		void on_browseButton_clicked();
		void OnStarted();
		void OnFinished();

	public:
		Ui::FileExtractorDialog* ui;

	private:
		std::vector<Models::FileExtractItem> FileExtractList;
		Models::FileExtractListModel Model;
		States::FileExtractorStateGroup StateMachine;
		std::filesystem::path Destination;
	};

}

#endif // !FILEEXTRACTORDIALOG_H