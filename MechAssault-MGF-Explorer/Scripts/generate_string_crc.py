import zlib
import csv
import sys


def calc_crc32_jamcrc(s):
    return 0xFFFFFFFF - zlib.crc32(s.encode())


if __name__ == "__main__":
    strings_file_path = sys.argv[1]
    csv_file_path = sys.argv[2]

    strings = {}
    result = [['CRC', 'String']]
    with open(strings_file_path) as strings_file:
        rows = strings_file.read().splitlines()
        for string in rows:
            crc = calc_crc32_jamcrc(string)

            if crc in strings and string != strings[crc]:
                print(f'Collision detected: {string} and {strings[crc]} have the same CRC {crc}')
            else:
                result.append([crc, string])

            strings[crc] = string

    with open(csv_file_path, 'w', newline='') as csv_file:
        csv = csv.writer(csv_file)
        csv.writerows(result)
        print(f'Wrote {len(result)} strings to {csv_file_path}')
