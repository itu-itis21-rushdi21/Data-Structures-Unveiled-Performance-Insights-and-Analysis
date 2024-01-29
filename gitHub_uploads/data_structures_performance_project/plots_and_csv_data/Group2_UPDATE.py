import matplotlib.pyplot as plt
import os

# Get the directory of the current script
script_dir = os.path.dirname(os.path.realpath(__file__))

# Change the working directory
os.chdir(script_dir)
# File name
file_name = 'times.csv'

# Initialize lists

Array_UPDATE = []
Linked_List_UPDATE = []
STL_Vector_UPDATE = []
STL_List_UPDATE = []

# Read and process the file
with open("times.csv", 'r') as file:
    for line in file:
        parts = line.strip().split(',')
        if len(parts) > 1:
            # Check both data structure name and operation
            
            if parts[0] == 'Array' and parts[1] == 'UPDATE':
                Array_UPDATE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'Linked_List' and parts[1] == 'UPDATE':
                Linked_List_UPDATE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'STL_Vector' and parts[1] == 'UPDATE':
                STL_Vector_UPDATE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'STL_List' and parts[1] == 'UPDATE':
                STL_List_UPDATE.extend([float(x) for x in parts[2:]])

        


# Plotting
x_axis = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
plt.xlabel('Dataset Size (thousands)')
plt.ylabel('Time (ms)')

#plt.plot(x_axis, File_IO_UPDATE, label='File_IO_UPDATE')
plt.plot(x_axis, Array_UPDATE, label='Array_UPDATE')
plt.plot(x_axis, Linked_List_UPDATE, label='Linked_List_UPDATE')
plt.plot(x_axis, STL_Vector_UPDATE, label='STL_Vector_UPDATE')
plt.plot(x_axis, STL_List_UPDATE, label='STL_List_UPDATE')

plt.title('Group 2 UPDATE')
plt.legend()
plt.grid()
plt.show()
