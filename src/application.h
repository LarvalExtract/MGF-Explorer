#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <wx/wx.h>

#include "mainwindow.h"

class Application: public wxApp
{    
public:
    /// Constructor
    Application();
	~Application();

    /// Initialises the application
    bool OnInit() override;

    /// Called on exit
    int OnExit() override;

private:
	MainWindow* mainWindow;
};

#endif // _APPLICATION_H_
