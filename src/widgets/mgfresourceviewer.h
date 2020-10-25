#ifndef MGFRESOURCEVIEWER_H
#define MGFRESOURCEVIEWER_H

#include "mgf/mgftreeitem.h"
#include "mgfresourcemanager.h"

#include <set>

// all resource viewer widgets must inherit from this
class AMGFResourceViewer
{
public:
    virtual void LoadMGFItem(const MGFTreeItem* item)
    {
        if (m_SupportedTypes.count(item->FileType()) == 0)
        {
            QString msg = "Tried to open " + item->Name() + " in ";
            switch (item->FileType())
            {
            case MGFFileType::Text:     msg += "MGFTextViewerWidget"; break;
            case MGFFileType::Strings:  msg += "MGFStringsViewerWidget"; break;
            case MGFFileType::Texture:  msg += "MGFTextureViewerWidget"; break;
            case MGFFileType::Model:    msg += "MGFModelViewerWidget"; break;
            default:                    msg += "unimplemented widget"; break;
            }

            throw std::runtime_error(msg.toLatin1().data());
        }
        else
            m_ItemPtr = item;
    }

protected:
    AMGFResourceViewer();

    std::set<MGFFileType> m_SupportedTypes;
    const MGFTreeItem* m_ItemPtr = nullptr;
    MGFResourceManager& ResourceManager;
};

#endif // MGFRESOURCEVIEWER_H
