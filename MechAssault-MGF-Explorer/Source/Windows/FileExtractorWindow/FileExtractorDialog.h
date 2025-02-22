#ifndef FILEEXTRACTORDIALOG_H
#define FILEEXTRACTORDIALOG_H

#include "Models/FileExtractListModel.h"
#include "States/FileExtractorStateGroup.h"

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
		~FileExtractorDialog() override;

		void QueueFiles(const Models::FileItemList& fileList);

		const auto& GetDestination() const { return Destination; }

	private slots:
		void on_browseButton_clicked();
		void OnStarted();
		void OnFinished();

	public:
		Ui::FileExtractorDialog* ui;

	private:
		std::unique_ptr<Models::FileExtractListModel> Model;
		States::FileExtractorStateGroup StateMachine;
		std::filesystem::path Destination;
	};

}

#endif // !FILEEXTRACTORDIALOG_H