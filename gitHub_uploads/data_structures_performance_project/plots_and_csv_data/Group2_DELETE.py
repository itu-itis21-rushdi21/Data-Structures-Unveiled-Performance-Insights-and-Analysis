import matplotlib.pyplot as plt
import os

# Get the directory of the current script
script_dir = os.path.dirname(os.path.realpath(__file__))

# Change the working directory
os.chdir(script_dir)
# File name
file_name = 'times.csv'

# Initialize lists

Array_DELETE = []
Linked_List_DELETE = []
STL_Vector_DELETE = []
STL_List_DELETE = []

# Read and process the file
with open("times.csv", 'r') as file:
    for line in file:
        parts = line.strip().split(',')
        if len(parts) > 1:
            # Check both data structure name and operation
            
            if parts[0] == 'Array' and parts[1] == 'DELETE':
                Array_DELETE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'Linked_List' and parts[1] == 'DELETE':
                Linked_List_DELETE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'STL_Vector' and parts[1] == 'DELETE':
                STL_Vector_DELETE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'STL_List' and parts[1] == 'DELETE':
                STL_List_DELETE.extend([float(x) for x in parts[2:]])

        


# Plotting
x_axis = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
plt.xlabel('Dataset Size (thousands)')
plt.ylabel('Time (ms)')

#plt.plot(x_axis, File_IO_DELETE, label='File_IO_DELETE')
plt.plot(x_axis, Array_DELETE, label='Array_DELETE')
plt.plot(x_axis, Linked_List_DELETE, label='Linked_List_DELETE')
plt.plot(x_axis, STL_Vector_DELETE, label='STL_Vector_DELETE')
plt.plot(x_axis, STL_List_DELETE, label='STL_List_DELETE')

plt.title('Group 2 DELETE')
plt.legend()
plt.grid()
plt.show()
