import xml.etree.ElementTree as xml
import argparse
import subprocess
import glob
import os.path

MOC_PATH = os.path.join(os.path.expandvars('%QT_DIR%'), '6.7.1', 'msvc2019_64', 'bin', 'moc.exe')
UIC_PATH = os.path.join(os.path.expandvars('%QT_DIR%'), '6.7.1', 'msvc2019_64', 'bin', 'uic.exe')


def clean_files(glob_query):
    moc_files = glob.glob(glob_query, recursive=True)
    for file in moc_files:
        os.remove(file)
        print('Removed ' + file)


def remove_items_from_vcxproj(node, filename_query):
    files = [file_path for file_path in node if file_path.attrib['Include'].find(filename_query) != -1]
    for file in files:
        node.remove(file)


def run_moc_step(project_folder, vcxproj_root):
    # clean all moc files from project source folder
    print('Cleaning moc source files...')
    clean_files(f"{project_folder}/src/**/moc_*.cpp")

    # remove all moc_ sources from project file
    sources_node = vcxproj_root.find('./ItemGroup/ClCompile/..')

    remove_items_from_vcxproj(sources_node, 'moc_')

    # run moc on all headers in project and add new moc files to project
    headers = [n.attrib['Include'] for n in vcxproj_root.findall('./ItemGroup/ClInclude')]
    for h in headers:
        header_folder = os.path.dirname(h)
        header_filename = os.path.basename(h)

        # execute moc.exe
        moc_args = [MOC_PATH, os.path.join(project_folder, h), '-f', header_filename]
        output = subprocess.run(args=moc_args, capture_output=True)

        if output.returncode:
            print(output.stderr.decode('utf-8'))
            exit(1)

        # moc.exe did not generate any code, which means the source file had no Qt macros in it. skip
        if len(output.stdout) == 0:
            continue

        moc_filename = f"moc_{header_filename.split('.')[0]}.cpp"
        moc_filepath = os.path.join(header_folder, moc_filename)

        # create the new source file using the output from moc.exe as the file's content
        with open(os.path.join(project_folder, moc_filepath), "wb") as moc:
            moc.write(output.stdout)
        sources_node.append(xml.Element('ClCompile', attrib={'Include': moc_filepath}))
        print(f"moc: successfully compiled {moc_filepath} ({' '.join(moc_args)})")


def run_uic_step(project_folder, vcxproj_root):
    # clean all ui headers from project folder
    print('Cleaning ui header files...')
    clean_files(f"{project_folder}/src/**/ui_*.h")

    # remove all ui_ headers from project file
    includes_node = vcxproj_root.find('./ItemGroup/ClInclude/..')
    remove_items_from_vcxproj(includes_node, 'ui_')

    ui_files = [n.attrib['Include'] for n in vcxproj_root.findall('./ItemGroup/None')]
    for ui in ui_files:
        folder = os.path.dirname(ui)
        filename = os.path.basename(ui)

        # construct filename for result header (ui_Filename.h)
        ui_header_filename = f"ui_{filename.split('.')[0]}.h"
        ui_header_path = os.path.join(folder, ui_header_filename)
        uic_args = [UIC_PATH, os.path.join(project_folder, ui), '-o', os.path.join(project_folder, ui_header_path)]

        # execute uic.exe
        result = subprocess.run(args=uic_args, capture_output=True)
        if result.returncode:
            print(result.stderr.decode('utf-8'))
            exit(1)

        # successfully created ui_Filename.h files, now add it to the vcxproj file
        includes_node.append(xml.Element('ClInclude', attrib={'Include': os.path.join(folder, ui_header_filename)}))
        print(f"uic: successfully compiled {ui_header_path} ({' '.join(uic_args)})")


if __name__ == "__main__":
    args = argparse.ArgumentParser()
    args.add_argument('-v', '--vcxproject', help='vcxproject file')
    args = args.parse_args()

    vcxproject_path = args.vcxproject
    vcxproject_folder = os.path.dirname(vcxproject_path)
    vcxproject = xml.parse(vcxproject_path)

    root = vcxproject.getroot()

    run_moc_step(vcxproject_folder, root)
    run_uic_step(vcxproject_folder, root)

    # overwrite project file
    vcxproject.write(vcxproject_path)
    print('Saved changes to ' + vcxproject_path)
