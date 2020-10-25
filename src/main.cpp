#include "windows/mainwindow.h"
#include <QApplication>

#include <filesystem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  

    MainWindow w;
    w.show();

    /*
    for (int i = 1; i < argc; i++)
    {
        std::filesystem::path mgfPath(argv[i]);

        if (mgfPath.extension() == "mgf")
            w.OpenMGFWorkspace(mgfPath.u8string().c_str());
    }
    */

    return a.exec();
}
