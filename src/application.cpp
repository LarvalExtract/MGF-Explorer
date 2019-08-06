#include "application.h"

IMPLEMENT_APP(Application)


Application::Application()
{
    
}

Application::~Application()
{

}

bool Application::OnInit()
{    
	mainWindow = new MainWindow();
	mainWindow->Show(true);

    return true;
}

int Application::OnExit()
{    
	return wxApp::OnExit();
}

