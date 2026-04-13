import matplotlib.pyplot as plt
import numpy as np


f_apr = open("time_apr.txt")
data = f_apr.read().split('\n')
f_apr.close()
y = list(map(float, data[:-1]))
x = range(1, len(y) + 1)

plt.plot(x, y)
plt.title('График')
plt.xlabel('x')
plt.ylabel('apr')

plt.savefig('graph.svg')