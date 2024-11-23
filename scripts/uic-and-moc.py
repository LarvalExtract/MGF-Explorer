import xml.etree.ElementTree as xml
import argparse
import subprocess
import glob
import os.path


def clean_files(glob_query):
    moc_files = glob.glob(glob_query, recursive=True)
    for file in moc_files:
        os.remove(file)
        print('Removed ' + file)


def remove_items_from_vcxproj(node, filename_query):
    files = [file_path for file_path in node if file_path.attrib['Include'].find(filename_query) != -1]
    for file in files:
        node.remove(file)


def run_moc_step(project_folder, vcxproj_root, qt_version):
    # clean all moc files from project source folder
    print('Cleaning moc source files...')
    clean_files(f"{project_folder}/src/**/moc_*.cpp")

    sources_node = vcxproj_root.find('./ItemGroup/ClCompile/..')
    headers_node = vcxproj_root.find('./ItemGroup/ClInclude/..')

    remove_items_from_vcxproj(sources_node, 'moc_')

    moc_binary_path = os.path.join(os.path.expandvars('%QT_DIR%'), qt_version, 'msvc2019_64', 'bin', 'moc.exe')

    # run moc on all headers in project and add new moc files to project
    for node in headers_node:
        header_path = node.attrib['Include']
        header_folder = os.path.dirname(header_path)
        header_filename = os.path.basename(header_path)
        header_path_full = os.path.join(project_folder, header_path)

        # execute moc.exe
        if os.path.exists(header_path_full):
            moc_args = [moc_binary_path, header_path_full, '-f', header_filename]
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
        else:
            print(f"moc: Header file '{header_path}' referenced in project does not exist on disk, removing from project file and skipping")
            headers_node.remove(node)


def run_uic_step(project_folder, vcxproj_root, qt_version):
    # clean all ui headers from project folder
    print('Cleaning ui header files...')
    clean_files(f"{project_folder}/src/**/ui_*.h")

    # remove all ui_ headers from project file
    includes_node = vcxproj_root.find('./ItemGroup/ClInclude/..')
    remove_items_from_vcxproj(includes_node, 'ui_')

    uic_binary_path = os.path.join(os.path.expandvars('%QT_DIR%'), qt_version, 'msvc2019_64', 'bin', 'uic.exe')

    ui_files = [n.attrib['Include'] for n in vcxproj_root.findall('./ItemGroup/None') if n.attrib['Include'].endswith('.ui')]
    for ui in ui_files:
        folder = os.path.dirname(ui)
        filename = os.path.basename(ui)

        # construct filename for result header (ui_Filename.h)
        ui_header_filename = f"ui_{filename.split('.')[0]}.h"
        ui_header_path = os.path.join(folder, ui_header_filename)
        uic_args = [uic_binary_path, os.path.join(project_folder, ui), '-o', os.path.join(project_folder, ui_header_path)]

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
    args.add_argument('--qtversion', help='Qt version')
    args = args.parse_args()

    vcxproject_path = args.vcxproject
    vcxproject_folder = os.path.dirname(vcxproject_path)
    vcxproject = xml.parse(vcxproject_path)

    qt_version = args.qtversion

    root = vcxproject.getroot()

    run_moc_step(vcxproject_folder, root, qt_version)
    run_uic_step(vcxproject_folder, root, qt_version)

    # overwrite project file
    vcxproject.write(vcxproject_path)
    print('Saved changes to ' + vcxproject_path)
