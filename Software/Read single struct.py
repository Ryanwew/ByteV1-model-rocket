import struct
import csv
from tkinter import *
from tinter  import filedialog

root = Tk()
root.title("DAT to CSV")

root.filename = filedialog.askopenfilename(initaldir="/", title="Select raw data file" filetypes=(("data files", "*.DAT")))

lsdata = []

# Clears any previous iterations and adds headers to the CSV file
with open("C:\\Users\\Ryan\\Desktop\\Byte drop\\dataout.csv", "w", newline='') as newFile:
    csvWriter = csv.writer(newFile, delimiter=",")
    newFile.truncate()
    csvWriter.writerow(["time", "x", "y", "z", "temp", "press", "alti"])
    newFile.close()

# Loops through the DAT bin file untill there is no data, converts each value into the appropriate variable and appends them to the CSV
with open("A:\\GithubDesktop\\Model-Rocket-Design\\Software\\X3.DAT", "rb") as dataFile:
    while True:
        b = dataFile.read(4)
        if not b:
            break
        a = struct.unpack('L', b)
        lsdata.append(a[0])
        
        for i in range(6):
            b = dataFile.read(4)
            if not b:
                break
            a = struct.unpack('f', b)
            lsdata.append(a[0])

        #lsdata.append(int.from_bytes(b, "big"))
        with open("C:\\Users\\Ryan\\Desktop\\Byte drop\\dataout.csv", "a", newline='') as newFile:
            csvWriter = csv.writer(newFile, delimiter=",")
            csvWriter.writerow(lsdata)
            lsdata.clear()

print("done!")