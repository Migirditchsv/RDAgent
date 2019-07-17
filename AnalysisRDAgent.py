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

# Plot best perf
plt.plot(data[:, 1])
plt.xlabel(headers[0])
plt.ylabel(headers[1])
plt.show()

#plot avg perf
plt.plot(data[:, 2])
plt.xlabel(headers[0])
plt.ylabel(headers[2])
plt.show()

#plot perf variance
plt.plot(data[:, 3])
plt.xlabel(headers[0])
plt.ylabel(headers[3])
plt.show()

#plot population history
for r in range(0,len(data)):
    for c in range(4,len(data[r])):
        plt.plot(data[r,0],data[r,c],color="grey",alpha=0.5)
plt.xlabel(headers[0])
plt.ylabel("Population Performance")
plt.show()