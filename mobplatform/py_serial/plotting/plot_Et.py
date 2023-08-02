#!/usr/bin/env python
from matplotlib import pyplot as plt
from matplotlib import style

# style.use('ggplot')

str = '330,342,3994\n'
x = str.split(",")
# print(x)
y = []
#  y2 = []
x = []
with open("Et.txt", 'r', encoding='UTF-8') as file:
    while line := file.readline():
        #myline = line.rstrip()
        myline = line.split(",")
        x.append(int(myline[0]))    # millis
        y.append(int(myline[1].rstrip()))    # E

        # print(line.rstrip())
# print('\n')
#print("x = [" + ', '.join(x1) + "]")

# print("blue = " + ', '.join(y1))
# print("red = " + ', '.join(y2))
#print(', '.join(x))

# x = [10, 12, 13]
# y = [8, 16, 6]
# x2 = [8, 15, 11]
# y2 = [6, 15, 7]

# x = [3994, 4494, 4994, 5493, 5993, 6493, 6992, 7491, 7991, 8491, 8989, 9489, 9989]
# y1 = [330, 605, 895, 1190, 1485, 1779, 2073, 2364, 2654, 2944, 3235, 3526, 3816]
# y2 = [342, 643, 965, 1293, 1623, 1951, 2275, 2597, 2917, 3235, 3553, 3868, 4180]
plt.plot(x, y, 'b', label='line one', linewidth=5)     # posA1 - blue
#   plt.plot(x, y2, 'r', label='line two', linewidth=5)     # posA2 - red
#
plt.title('PID error by time')
#fig = plt.figure()
plt.ylabel('Error between Encoders A1, A2')
plt.xlabel('time, ms')

plt.show()