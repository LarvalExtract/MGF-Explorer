import xml.etree.ElementTree as xml
import subprocess
import argparse
import os.path

UIC_PATH = os.path.join(os.path.expandvars('%QT_DIR%'), '5.14.2', 'msvc2017_64', 'bin', 'uic.exe')


def run_uic(project_dir, vcxproj):
    headers_node = vcxproj.find('./ItemGroup/ClInclude/..')

    ui_headers = set()
    for header in headers_node:
        if header.attrib['Include'].find('ui_') != -1:
            ui_headers.add(header.attrib['Include'])

    ui_forms = [node for node in vcxproj.findall('./ItemGroup/CustomBuild')]

    for form in ui_forms:
        form_path = os.path.normpath(form.attrib['Include'])
        form_path_disk = os.path.join(project_dir, form_path)
        header_path = os.path.join(os.path.dirname(form_path), f"ui_{os.path.basename(form_path).split('.')[0]}.h")
        header_path_disk = os.path.join(project_dir, header_path)

        if header_path in ui_headers:
            if 'ModifiedTime' in form.attrib:
                form_modify_time_xml = int(form.attrib['ModifiedTime'])
                form_modify_time_disk = int(os.path.getmtime(form_path_disk))
                if form_modify_time_disk == form_modify_time_xml:
                    print(f"uic: skipping unchanged form {form_path}")
                    continue

        result = build_form(form_path_disk, header_path_disk)
        if result.returncode != 0:
            print(f"uic: error: {result.stderr}")
            return
        print(f"uic: compiled {form_path_disk} to {header_path_disk}")
        form.attrib['ModifiedTime'] = str(int(os.path.getmtime(form_path_disk)))
        headers_node.append(xml.Element('ClInclude', attrib={'Include': header_path}))


def build_form(input, output):
    uic_args = [UIC_PATH, input, '-o', output]
    result = subprocess.run(uic_args, capture_output=True)
    return result


def main(vcxproj_file):
    vcxproject_doc = xml.parse(vcxproj_file)
    vcxproject = vcxproject_doc.getroot()

    run_uic(os.path.dirname(vcxproj_file), vcxproject)

    vcxproject_doc.write(vcxproj_file)


if __name__ == "__main__":
    args = argparse.ArgumentParser()
    args.add_argument('-v', '--vcxproject')
    args = args.parse_args()

    try:
        main(args.vcxproject)
    except Exception as err:
        print(err)

