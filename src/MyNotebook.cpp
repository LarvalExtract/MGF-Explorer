#include "MyNotebook.h"

#include <wx/msgdlg.h>

MyNotebook::MyNotebook(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
	wxNotebook(parent, id, pos, size)
{

}

MyNotebook::~MyNotebook()
{
	CloseAllMgfFiles();
}

void MyNotebook::OpenMgfFile(const std::filesystem::path& archiveFilepath)
{
	MGFArchive* newArchive = new MGFArchive(archiveFilepath);
	mapArchives.insert(std::make_pair(newArchive->FileName().ToStdString(), newArchive));

	this->AddPage(new NotebookPage(this, newArchive), newArchive->FileName(), true);
}

void MyNotebook::CloseMgfFile(int index)
{
	const std::string& archiveName = static_cast<NotebookPage*>(this->GetPage(index))->GetMGFFile().FileName().ToStdString();

	this->DeletePage(index);

	delete mapArchives[archiveName];

	mapArchives.erase(archiveName);
}

void MyNotebook::CloseAllMgfFiles()
{
	this->DeleteAllPages();

	for (auto& archive : mapArchives)
		delete archive.second;

	mapArchives.clear();
}

bool MyNotebook::MgfFileIsOpen(const std::string& archiveName)
{
	return mapArchives.find(archiveName) != mapArchives.end();
}

void MyNotebook::SelectMgfFile(const std::string& archiveName)
{
	int index = 0;
	for (auto& mgfFile : mapArchives)
	{
		if (mgfFile.second->FileName().ToStdString() == archiveName)
		{
			this->SetSelection(index);
			break;
		}
		index++;
	}
}
