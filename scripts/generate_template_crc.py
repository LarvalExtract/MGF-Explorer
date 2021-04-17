import csv
import sys

if __name__ == "__main__":
    crc_strings_path = sys.argv[1]
    bt_path = sys.argv[2]

    with open(crc_strings_path) as csv_file:
        case_statements = {row[0]: '\t\tcase {0:>5}:\tresult = "{1}"; break;'.format(row[0], row[1].replace('\"', '\'').replace('\\', '\\\\')) for row in csv.reader(csv_file)}

    template = """string getStringFromCrc(uint32 crc)
{{
    string result = "UNKNOWN";
    switch (crc)
    {{
{statements}
    }}
    return result;
}}
""".format(statements='\n'.join(case_statements.values()))

    with open(bt_path, 'w') as bt_script:
        bt_script.write(template)

