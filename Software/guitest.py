import struct
import csv
from tkinter import *
from tkinter  import filedialog

root = Tk()
root.title("DAT to CSV")



def opendat():
    root.filename0 = filedialog.askopenfilename(initialdir="/", title="Select raw data file", filetypes=(("data files", "*.DAT"),("all files", "*.*")))
    label1 = Label(root, text=root.filename0).grid(row=0, column=1)


def openfil():
    root.filename1 = filedialog.askdirectory(initialdir="/", title="Select output file" )
    label2 = Label(root, text=root.filename1).grid(row=1, column=1)

def execute():
    tru0 = ""
    smpl0 = []
    smpl0 = root.filename0.split("/")

    for i in smpl0:
        tru0 = tru0 + i + r"\\"

    tru1 = ""
    smpl1 = []
    smpl1 = root.filename1.split("/")

    for i in smpl1:
        tru1 = tru1 + i + r"\\"
    tru1 = tru1 + "dataout.csv"





    label3 = Label(root, text="Done! Output to:").grid(row=3, column=0)
    label4 = Label(root, text=tru0).grid(row=3, column=1)


selectdat = Button(root, text="Select Dataset", command=opendat).grid(row=0, column=0)
selectoutput = Button(root, text="Select Output Location", command=openfil).grid(row=1, column=0)
fin = Button(root, text="Convert Data", command=execute).grid(row=2, column=0)

root.mainloop()