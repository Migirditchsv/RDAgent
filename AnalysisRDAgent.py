import csv
import numpy as np
import matplotlib.pyplot as plt

data_path = 'evodata.csv'
with open(data_path, 'r') as f:
    reader = csv.reader(f, delimiter=',')
    # get header from first row
    headers = next(reader)
    # get all the rows as a list
    data = list(reader)
    # transform data into numpy array
    data = np.array(data).astype(float)
    
print(headers)
print(data.shape)
print(data[:3])

# Plot the data
plt.plot(data[:, 1])
plt.xlabel(headers[0])
plt.ylabel(headers[1])
plt.show()

plt.plot(data[:, 2])
plt.xlabel(headers[0])
plt.ylabel(headers[2])
plt.show()

plt.plot(data[:, 3])
plt.xlabel(headers[0])
plt.ylabel(headers[3])
plt.show()