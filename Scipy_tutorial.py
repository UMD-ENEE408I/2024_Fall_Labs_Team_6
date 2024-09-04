import numpy as np
from scipy.linalg import solve
import matplotlib.pyplot as plt
 # 1.
A = np.array([[3, 1], [1, 2]])
b = np.array([9, 8])
x = solve(A, b)
print("Solution to the system:", x)

# 2. 
from scipy.optimize import minimize

func = lambda x: x**2 + 2*x
result = minimize(func, 0)  # Initial guess is 0
print("Minimum of the function:", result.x)

#3. 
from scipy.fftpack import fft
x = np.linspace(0, 1, 500)
y = np.sin(100 * np.pi * x) + 0.5 * np.sin(160 * np.pi * x)
yf = fft(y)
plt.plot(np.abs(yf))
plt.title('Fourier Transform')
plt.show()
