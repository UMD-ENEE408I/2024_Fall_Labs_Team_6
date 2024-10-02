import numpy as np

# 3. 
array = np.array([1, 2, 3, 4])
print(array)

# 4. 
ones_array = np.ones((3, 4))
zeros_array = np.zeros((4, 3))
print("Array of ones:\n", ones_array)
print("Array of zeros:\n", zeros_array)

# 5. 
A = np.array([[1, 4, 7], [4, 10, 23]])
B = np.array([[17, 82, 91, 100], [1, 14, 12, 0], [15, 16, 17, 18]])
C = np.dot(A, B)
print("Matrix multiplication result:\n", C)

# 6. 
matrix = np.array([[1, 3], [2, 1]])
eigenvalues, eigenvectors = np.linalg.eig(matrix)
print("Eigenvalues:", eigenvalues)
print("Eigenvectors:\n", eigenvectors)
