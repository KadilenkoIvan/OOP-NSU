f = open("pulsars.txt", "w")

f.write("#Life 1.06\n#N Reverce Glider\n#R B3/S23\n51 31\n")
for i in range(15):
    for j in range(10):
        f.write(str(j * 5 + 1) + ' ' + str(i * 2 + 1) + '\n')
        f.write(str(j * 5 + 2) + ' ' + str(i * 2 + 1) + '\n')
        f.write(str(j * 5 + 3) + ' ' + str(i * 2 + 1) + '\n')
f.close()