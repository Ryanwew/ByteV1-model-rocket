with open("C:\\Users\\Ryan\\Desktop\\Byte drop\\test1.txt", "r")as reader:
    thingy = reader.readlines()

print(thingy)

supercool = (thingy[0])

coolthing = [supercool[i:i+1] for i in range(0, len(supercool), 1)]

print(coolthing)

newcool = []

for i in coolthing:
    i = eval(i)
    i = i+1
    newcool.append(i)

print(newcool)

megacool = open("C:\\Users\\Ryan\\Desktop\\Byte drop\\test2.txt", "w")

for i in newcool:
    i = str(i)
    megacool.write(i)
