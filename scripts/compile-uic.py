import xml.etree.ElementTree as xml
import subprocess
import argparse

def main(vcxproj_file):
    vcxproject_doc = xml.parse(vcxproj_file)
    vcxproject = vcxproject_doc.getroot()

    for n in vcxproject.iter():
        n.tag = n.tag.split('}')[1]

    ui_files = [node.attrib['Include'].replace('\\', '/') for node in vcxproject.findall('./ItemGroup/CustomBuild')]
    header_files = {node.attrib['Include'].replace('\\', '/') for node in vcxproject.findall('./ItemGroup/ClInclude')}

    for path in ui_files:
        separated = path.split('/')
        separated[-1] = f"ui_{separated[-1].split('.')[0]}.h"
        ui_path = '/'.join(separated)
        subprocess.run(
            args=['C:/Qt/5.14.2/msvc2017_64/bin/uic.exe', f"../{path}", '-o', f"../{ui_path}"],
            check=True)

        print(f'Succesfully compiled {ui_path}')
        if ui_path not in header_files:
            header = xml.Element('ClInclude', attrib={'Include': ui_path})
            vcxproject.find('./ItemGroup/ClInclude').append(header)

    vcxproject_doc.write(vcxproj_file)


if __name__ == "__main__":
    args = argparse.ArgumentParser()
    args.add_argument('-v', '--vcxproject')
    args = args.parse_args()

    try:
        main(args.vcxproject)
    except Exception as err:
        print(err)