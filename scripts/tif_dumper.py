import os
import csv
import tabulate

TIF_FOLDER = "../data/tif"

if __name__ == "__main__":

    headers = ["Name", "Flags", "Width", "Height", "Size", "Mips", "Frames", "Depth", "Bits/pixel"]
    results = []

    for root, folders, files in os.walk(TIF_FOLDER):
        for tif_csv_filename in files:
            with open(os.path.join(root, tif_csv_filename)) as tif_csv_file:
                tif_csv = list(csv.reader(tif_csv_file))
                tif_path = tif_csv[1][0]
                results.append([
                    tif_path[tif_path.rfind('extracted')+9:],
                    tif_csv[26][1][0:-1],
                    tif_csv[34][1],
                    tif_csv[42][1],
                    tif_csv[50][1],
                    tif_csv[58][1],
                    tif_csv[66][1],
                    tif_csv[74][1]
                ])

                width = int(tif_csv[34][1])
                height = int(tif_csv[42][1])
                size = int(tif_csv[50][1])
                mips = int(tif_csv[58][1])

    table = tabulate.tabulate(results, headers=headers)
    print(table)

    with open("../data/tif_data.csv", "w", newline='') as tif_data:
        tif_data_csv = csv.writer(tif_data)
        tif_data_csv.writerow(headers)
        tif_data_csv.writerows(results)


