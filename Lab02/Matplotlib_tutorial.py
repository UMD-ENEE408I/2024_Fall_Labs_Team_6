import matplotlib.pyplot as plt
import numpy as np

# 1.
x = np.linspace(0, 2 * np.pi, 100)
y = np.sin(x)

plt.plot(x, y)
plt.title('Sine Function')
plt.xlabel('x')
plt.ylabel('sin(x)')
plt.show()

# 2. 
from mpl_toolkits.mplot3d import Axes3D

x = np.linspace(-5, 5, 100)
y = np.linspace(-5, 5, 100)
x, y = np.meshgrid(x, y)
z = np.sin(x**2 + y**2)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x, y, z, cmap='viridis')
plt.title('3D Plot of z = sin(x^2 + y^2)')
plt.show()
