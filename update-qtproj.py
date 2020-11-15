import xml.etree.ElementTree as xmlt
import argparse

def to_lines(strings):
    lines = ""
    for string in strings:
        lines += f"\t\t{string} \\\n"
    return lines

def create_qt_project_file(target, sources, headers, forms, libs):
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
{to_lines(sources)}

HEADERS += \\
{to_lines(headers)}

FORMS += \\
{to_lines(forms)}

# Default rules for deployment.
qnx: target.path = /tmp/$${{TARGET}}/bin
else: unix:!android: target.path = /opt/$${{TARGET}}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += 

else:win32:CONFIG(debug, debug|release): LIBS += 
"""
    return qt_project_template

def main():
    args = argparse.ArgumentParser()
    args.add_argument('-v', '--vcproject', help="vcproject file to convert", action="store")
    args = args.parse_args()
    vcproj = xmlt.parse(args.vcproject).getroot()
    # strip namespaces from tags
    for n in vcproj.iter():
        n.tag = n.tag.split('}')[1]

    headers = []
    sources = []
    forms = []
    libs = {}
    include_dirs = {}

    for child in vcproj:
        if child.tag == 'ItemDefinitionGroup':
            configuration = child.attrib['Condition'].split('==')[1].strip('\'')
            additional_dependencies = child.find('Link').find('AdditionalDependencies').text.split(';')
            additional_includes = child.find('ClCompile').find('AdditionalIncludeDirectories').text.split(';')
            libs[configuration] = additional_dependencies
            include_dirs[configuration] = additional_includes
        elif child.tag == 'ItemGroup':
            for item in child:
                if item.tag == 'ClInclude':
                    headers.append(item.attrib['Include'])
                elif item.tag == 'ClCompile':
                    sources.append(item.attrib['Include'])
                elif item.tag == 'None':
                    forms.append(item.attrib['Include'])

    contents = create_qt_project_file('MechAssault MGF Explorer', sources, headers, forms, libs)
    print(1)


if __name__ == "__main__":
    main()