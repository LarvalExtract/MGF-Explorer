#include <wx/menu.h>
#include <stdint.h>

const wxString extractRawText("Extract file");
const wxString extractFolderText("Extract folder...");

const wxString extractTextureText("Extract texture as BMP");

const std::uint32_t idExtractRaw = 2000;
const std::uint32_t idExtractFolder = 2001;
const std::uint32_t idExtractTexture = 2002;

// Pop-up menus that appear when right clicking an item in the tree view

wxMenu popupMenuFile;
wxMenu popupMenuFolder;
wxMenu popupMenuTexture;
wxMenu popupMenuModel;

void InitPopupMenus()
{
	popupMenuFile.Append(idExtractRaw, extractRawText, nullptr);

	popupMenuFolder.Append(idExtractFolder, extractFolderText, nullptr);

	popupMenuTexture.Append(idExtractRaw, extractRawText, nullptr);
	popupMenuTexture.AppendSeparator();
	popupMenuTexture.Append(idExtractTexture, extractTextureText, nullptr);
}
