import xml.etree.ElementTree as xml
import argparse
import subprocess
import glob
import os.path
import datetime

MOC_PATH = os.path.join(os.path.expandvars('%QT_DIR%'), '5.14.2', 'msvc2017_64', 'bin', 'moc.exe')
UIC_PATH = os.path.join(os.path.expandvars('%QT_DIR%'), '5.14.2', 'msvc2017_64', 'bin', 'uic.exe')


def compile_uic(vcxproj):
    # list UI files
    ui_forms = [node.attrib['Include'] for node in vcxproj.findall('./ItemGroup/CustomBuild')]

    ui_headers = {}
    for node in vcxproj.findall('./ItemGroup/ClInclude'):
        if node.attrib['Include'].find('ui_') != -1:
            ui_headers[node.attrib['Include']] = node.attrib['ModifiedDate']

    for form in ui_forms:
        form_folder = os.path.dirname(form)
        output_path = str(os.path.normpath(form_folder, f"ui_{form.split('.')[0]}.h"))
        if output_path not in ui_headers:
            continue
        modified_time_disk = datetime.datetime(os.path.getmtime(output_path))
        modified_time_xml = datetime.datetime(ui_headers[output_path])


def main(vcxproject_path):
    project_folder = os.path.dirname(vcxproject_path)
    vcxproject = xml.parse(vcxproject_path)
    root = vcxproject.getroot()

    # clean all moc files from project source folder
    print('Cleaning moc source files...')
    moc_file_query = f"{project_folder}/src/**/moc_*.cpp"
    moc_files = glob.glob(moc_file_query, recursive=True)
    for file in moc_files:
        os.remove(file)
        print('Removed ' + file)

    # remove namespaces from xml nodes
    #for n in root.iter():
    #    n.tag = n.tag.split('}')[1]

    # remove all moc_ sources from project file
    moc_files = []
    sources_node = root.find('./ItemGroup/ClCompile/..')
    for source_file in sources_node:
        if source_file.attrib['Include'].find('moc_') != -1:
            moc_files.append(source_file)
    for moc_source_file in moc_files:
        sources_node.remove(moc_source_file)

    # run moc on all headers in project and add new moc files to project
    headers = [n.attrib['Include'] for n in root.findall('./ItemGroup/ClInclude')]
    for h in headers:
        header_folder = os.path.dirname(h)
        header_filename = os.path.basename(h)
        moc_args = [MOC_PATH, os.path.join(project_folder, h), '-f', header_filename]
        output = subprocess.run(
            args=moc_args,
            capture_output=True
        )
        if output.returncode != 0:
            print(output.stderr.decode('utf-8'))
            exit(1)
        if len(output.stdout) == 0:
            continue
        moc_filename = f"moc_{header_filename.split('.')[0]}.cpp"
        moc_filepath = os.path.join(header_folder, moc_filename)
        with open(os.path.join(project_folder, moc_filepath), 'wb') as moc:
            moc.write(output.stdout)
        sources_node.append(xml.Element('ClCompile', attrib={'Include': moc_filepath}))
        print(f"moc: successfully compiled {moc_filepath} ({' '.join(moc_args)})")

    print('Cleaning ui header files...')
    # clean all ui headers from project folder
    ui_file_query = f"{project_folder}/src/**/ui_*.h"
    ui_headers = glob.glob(ui_file_query, recursive=True)
    for file in ui_headers:
        os.remove(file)
        print('Removed ' + file)

    # remove all ui_ headers from project file
    ui_headers = []
    headers_node = root.find('./ItemGroup/ClInclude/..')
    for ui_file in headers_node:
        if ui_file.attrib['Include'].find('ui_') != -1:
            ui_headers.append(ui_file)
    for header in ui_headers:
        headers_node.remove(header)

    # run uic on all .ui files and add resulting headers to project
    ui_files = [n.attrib['Include'] for n in root.findall('./ItemGroup/None')]
    for ui in ui_files:
        folder = os.path.dirname(ui)
        filename = os.path.basename(ui)
        ui_header_filename = f"ui_{filename.split('.')[0]}.h"
        ui_header_path = os.path.join(folder, ui_header_filename)
        uic_args = [UIC_PATH, os.path.join(project_folder, ui), '-o', os.path.join(project_folder, ui_header_path)]
        result = subprocess.run(
            args=uic_args,
            capture_output=True
        )
        if result.returncode != 0:
            print(result.stderr.decode('utf-8'))
            exit(1)
        headers_node.append(xml.Element('ClInclude', attrib={'Include': os.path.join(folder, ui_header_filename)}))
        print(f"uic: successfully compiled {ui_header_path} ({' '.join(uic_args)})")

    # overwrite project file
    vcxproject.write(vcxproject_path)
    print('Saved changes to ' + vcxproject_path)


if __name__ == "__main__":
    args = argparse.ArgumentParser()
    args.add_argument('-v', '--vcxproject', help='vcxproject file')
    args = args.parse_args()
    main(args.vcxproject)
