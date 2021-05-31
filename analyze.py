import csv
import os
import pandas as pd
from matplotlib import pyplot as plt
import numpy as np
import tempfile

def write_formated_csv(data, output_path):
    with open(output_path, "w") as f:
        writer = csv.writer(f, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
        for i in data:
            writer.writerow(i)

def clean_input(input_path, output_path):
    data = []
    header = []
    with open(input_path, "r") as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) == 1 and row[0].find("#column") != -1:
                tokens = row[0].split(" ")
                unit = tokens[-1]
                if unit.find("]") == -1 or unit.find("]") == -1:
                    unit = None  
                    coll_name = tokens[-1]
                else:
                    coll_name = tokens[-2] + unit
                header.append(coll_name)
            elif len(row) > 1:
                data.append(row)
    data.insert(0, header)
    write_formated_csv(data, output_path)


def read_csv_from_dir(out_dir):
    frames = []
    clean_dir_path = tempfile.mkdtemp()
    for filename in os.listdir(out_dir):
        if os.path.splitext(filename)[-1] != ".csv":
            continue
        orig_file_path = os.path.join(out_dir, filename)
        clean_file_path = os.path.join(clean_dir_path, filename)
        clean_input(orig_file_path, clean_file_path)
        frames.append(pd.read_csv(clean_file_path))
    return pd.concat(frames)

class Plot:

    def __init__(self, n_fig = 2):
        self.fig, self.axis = plt.subplots(n_fig, constrained_layout=True)
        self.n_fig = n_fig
        self.fig.suptitle('Vertically stacked subplots')
        self.counter = 0
        self.MeV = 1e6

    def draw(self, title):
        self.fig.suptitle(title)
        self.fig.savefig(title.replace(" ", "_") + ".png")

    def create_histrogram(self, columns, direction_name):
        if self.counter >= self.n_fig:
            raise ValueError(f"You declared you only need {self.n_fig} figures")

        n, bins, patches = self.axis[self.counter].hist(columns.values[:, 0], bins=50, range=(-100, 100),
                                    weights=100*columns.values[:, 1]/self.MeV, cumulative=True)
        self.axis[self.counter].set_xlabel(f'pos {direction_name} [nm]')
        self.axis[self.counter].set_ylabel('Energy Deposited [MeV]')
        self.counter+=1


if __name__ == '__main__':
    data = read_csv_from_dir(".")
    plot = Plot(2)
    weight_column_name = "totEnergyDeposit[eV]"
    columns = data[["x[nm]", weight_column_name]]
    plot.create_histrogram(columns, "X")
    columns = data[["y[nm]", weight_column_name]]
    plot.create_histrogram(columns, "Y")
    plot.draw("Energy Deposition")
