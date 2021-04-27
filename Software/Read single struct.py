import struct
import csv

lsdata = []

with open("A:\\GithubDesktop\\Model-Rocket-Design\\Software\\X.DAT", "rb") as dataFile:
    for x in range(50):
        b = dataFile.read(4)
        a = struct.unpack('f', b)
        lsdata.append(a)


with open("C:\\Users\\Ryan\\Desktop\\Byte drop\\dataout.csv", "w", newline='') as newFile:
    csvWriter = csv.writer(newFile, delimiter=",")
    for y in lsdata:
        csvWriter.writerow(y)

print("done!")