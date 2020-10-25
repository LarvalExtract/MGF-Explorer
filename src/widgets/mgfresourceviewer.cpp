#include "mgfresourceviewer.h"
#include "utils/Contexts.h"

AMGFResourceViewer::AMGFResourceViewer() :
	ResourceManager(*Contexts::Get<MGFResourceManager>())
{

}