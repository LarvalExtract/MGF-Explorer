import xml.etree.ElementTree as xmlt
import argparse
import os


def create_qt_project_file(
        target,
        source_files,
        header_files,
        form_files,
        debug_libs,
        release_libs,
        include_paths,
        depend_paths,
        uic_dir,
        moc_dir):
    qt_project_template = f""" 
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
    
TARGET = {target}
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \\
\t{f' {chr(92)}{chr(10)}{chr(9)}'.join(source_files)}

HEADERS += \\
\t{f' {chr(92)}{chr(10)}{chr(9)}'.join(header_files)}

FORMS += \\
\t{f' {chr(92)}{chr(10)}{chr(9)}'.join(form_files)}

# Default rules for deployment.
qnx: target.path = /tmp/$${{TARGET}}/bin
else: unix:!android: target.path = /opt/$${{TARGET}}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += \\
\t{f' {chr(92)}{chr(10)}{chr(9)}'.join(release_libs)}

else:win32:CONFIG(debug, debug|release): LIBS += \\
\t{f' {chr(92)}{chr(10)}{chr(9)}'.join(debug_libs)}

INCLUDEPATH += \\
\t{f' {chr(92)}{chr(10)}{chr(9)}'.join(include_paths)}

DEPENDPATH += \\
\t{f' {chr(92)}{chr(10)}{chr(9)}'.join(depend_paths)}

win32:UI_DIR = $$PWD/{uic_dir}
win32:MOC_DIR = $$PWD/{moc_dir}

win32:CONFIG(debug, debug|release):OBJECTS_DIR = $$PWD/obj/x64/Debug
win32:CONFIG(release, debug|release):OBJECTS_DIR = $$PWD/obj/x64/Release

"""
    return qt_project_template


if __name__ == "__main__":
    args = argparse.ArgumentParser()
    args.add_argument('-v', '--vcproject', help="vcproject file to convert", action="store")
    #args.add_argument('-u', '--uic-dir', help="folder for Qt generated uic files, relative to vcproj", action="store")
    #args.add_argument('-m', '--moc-dir', help="folder for Qt generated moc files, relative to vcproj", action="store")
    args = args.parse_args()
    vcproj = xmlt.parse(args.vcproject).getroot()
    # strip namespaces from tags
    for n in vcproj.iter():
        n.tag = n.tag.split('}')[1]

    configurations = [c.attrib['Include'] for c in vcproj.findall('*/ProjectConfiguration')]
    headers = [h.attrib['Include'].replace('\\', '/') for h in vcproj.findall('./ItemGroup/ClInclude')]
    sources = [s.attrib['Include'].replace('\\', '/') for s in vcproj.findall('./ItemGroup/ClCompile')]
    forms = [f.attrib['Include'].replace('\\', '/') for f in vcproj.findall('./ItemGroup/None')]

    additional_include_directories = {}
    additional_library_directories = {}
    libs = {}

    for node in vcproj.findall('./ItemDefinitionGroup'):
        config = node.attrib['Condition'].split('==')[1].strip('\'')
        additional_include_directories[config] = node.find('ClCompile').find('AdditionalIncludeDirectories').text.replace('\\', '/').split(';')[:-1]
        additional_library_directories[config] = node.find('Link').find('AdditionalLibraryDirectories').text.replace('\\', '/').split(';')[:-1]
        libs[config] = node.find('Link').find('AdditionalDependencies').text.replace('\\', '/').split(';')[:-1]

    # replace VS project directory macros with Qt project macros
    for paths in additional_include_directories.values():
        for i in range(len(paths)):
            paths[i] = paths[i].replace('$', '$$')
            paths[i] = paths[i].replace('(ProjectDir)', 'PWD/')

    # vcproj libs separate lib directories from lib files
    # however, qt projects tie them together
    # for each lib, for each folder,
    for cfg in configurations:
        for i in range(len(libs[cfg])):
            lib = libs[cfg][i]
            for folder in additional_library_directories[cfg]:
                path = folder + '/' + lib
                path2 = os.path.expandvars(path.replace('$(', '%').replace(')', '%'))
                if os.path.exists(path2):
                    lib = lib.split('.')[0] #remove .lib extension
                    libs[cfg][i] = f"-L${folder}/ -l{lib}"
                    break

    file_name = args.vcproject.split('.')[0]
    contents = create_qt_project_file(
        target=file_name,
        source_files=sources,
        header_files=headers,
        form_files=forms,
        debug_libs=libs['Debug|x64'],
        release_libs=libs['Release|x64'],
        include_paths=additional_include_directories['Debug|x64'],
        depend_paths=additional_library_directories['Debug|x64'],
        uic_dir="src/uic",
        moc_dir="src/moc"
    )

    with open(f"{file_name}.pro", 'w') as qt_file:
        qt_file.write(contents)
