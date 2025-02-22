import configparser
import requests
from requests.auth import HTTPBasicAuth
import xml.etree.ElementTree as xml
import os
import json


def to_bulk_request_entry(index, params):
    return f'{json.dumps({"index": {"_index": index}})}\n{json.dumps(params)}\n'


def collect_material_data(folder):
    mat_files = [os.path.join(r, fn)
                 for r, ds, fs in os.walk(folder)
                 for fn in fs if fn.endswith(".mat")]

    bulk_request = ""
    for mat_file in mat_files:
        config = configparser.ConfigParser()
        try:
            config.read(mat_file)
        except configparser.DuplicateOptionError as inst:
            print(inst.args)
        except configparser.DuplicateSectionError as inst:
            print(inst.args)

        material_params = config._sections["material"]
        material_params["file"] = os.path.relpath(mat_file, folder)
        for key, value in material_params.items():
            if type(value) == str:
                material_params[key] = str(value).lower()

        bulk_request += to_bulk_request_entry("material", material_params)

    return bulk_request


def collect_material_data_xml(folder):
    mgmodel_files = [os.path.join(r, fn)
                     for r, ds, fs in os.walk(folder)
                     for fn in fs if fn.endswith(".mgmodel")]

    bulk_request = ""
    for mgmodel_file in mgmodel_files:
        doc = xml.parse(mgmodel_file)
        for material_node in doc.getroot():
            if material_node.tag.startswith("material"):
                material_params = material_node.attrib
                material_params["type"] = material_node.tag.split('material_')[1]
                material_params["file"] = os.path.relpath(mgmodel_file, folder)
                for texture_node in material_node:
                    material_params[texture_node.tag] = texture_node.attrib

                bulk_request += to_bulk_request_entry("material", material_params)

    return bulk_request


if __name__ == "__main__":
    ma2_data_root = "D:\\Emulators\\Xbox1\\ISO\\MechAssault2LoneWolf_World_En-Ja-Fr-De-Es-It_redump_9236\\Extracted"
    ma1_data_root = "D:\\Emulators\\Xbox1\\ISO\\MechAssault (USA) (En,Fr,De) (v1.08)\\extracted"

    #payload = collect_material_data_xml(ma2_data_root)

    payload = collect_material_data(ma1_data_root)
    payload += collect_material_data(ma2_data_root)

    response = requests.post("https://localhost:9200/_bulk",
                             headers={"Content-Type": "application/x-ndjson"},
                             data=payload,
                             verify=False,
                             auth=HTTPBasicAuth('elastic', '7w3dJ+cuCfLye3_LlXeQ'))

    print(str(response.content))
