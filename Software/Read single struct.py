import struct
import csv
from tkinter import *
from tkinter  import filedialog

root = Tk()
root.title("DAT to CSV")
lsdata = []

def opendat():
    root.filename0 = filedialog.askopenfilename(initialdir="/", title="Select raw data file", filetypes=(("data files", "*.DAT"),("all files", "*.*")))
    label1 = Label(root, text=root.filename0).grid(row=0, column=1)


def openfil():
    root.filename1 = filedialog.askdirectory(initialdir="/", title="Select output file" )
    label2 = Label(root, text=root.filename1).grid(row=1, column=1)

def execute():
    #set up filenames
    tru0 = ""
    smpl0 = []
    smpl0 = root.filename0.split("/")

    for i in smpl0:
        tru0 = tru0 + i + "\\"
    tru0 = tru0[:-1]

    tru1 = ""
    smpl1 = []
    smpl1 = root.filename1.split("/")

    for i in smpl1:
        tru1 = tru1 + i + "\\"
    tru1 = tru1 + "dataout.csv"

    # Clears any previous iterations and adds headers to the CSV file
    with open(tru1, "w", newline='') as newFile:
        csvWriter = csv.writer(newFile, delimiter=",")
        newFile.truncate()
        csvWriter.writerow(["time", "orientation x", "orientation y", "orientation z", "acceleration x", "acceleration y", "acceleration z", "altitude", "temperature", "state", "error register"])
        newFile.close()

    # Loops through the DAT bin file untill there is no data, converts each value into the appropriate variable and appends them to the CSV
    with open(tru0, "rb") as dataFile:
        while True:
            b = dataFile.read(4)
            if not b:
                break
            a = struct.unpack('L', b)
            lsdata.append(a[0])
            
            for i in range(8):
                b = dataFile.read(4)
                if not b:
                    break
                a = struct.unpack('f', b)
                lsdata.append(a[0])

            for i in range(2):
                b = dataFile.read(1)
                if not b:
                    break
                lsdata.append(int.from_bytes(b, "big"))

            with open(tru1, "a", newline='') as newFile:
                csvWriter = csv.writer(newFile, delimiter=",")
                csvWriter.writerow(lsdata)
                lsdata.clear()

    outputlocation = root.filename1 + "dataout.csv"

    label3 = Label(root, text="Done! Output to:").grid(row=3, column=0)
    label4 = Label(root, text=outputlocation).grid(row=3, column=1)


selectdat = Button(root, text="Select Dataset", command=opendat).grid(row=0, column=0)
selectoutput = Button(root, text="Select Output Location", command=openfil).grid(row=1, column=0)
fin = Button(root, text="Convert Data", command=execute).grid(row=2, column=0)

root.mainloop()

