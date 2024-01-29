import matplotlib.pyplot as plt
import os

# Get the directory of the current script
script_dir = os.path.dirname(os.path.realpath(__file__))

# Change the working directory
os.chdir(script_dir)
# File name
file_name = 'times.csv'

# Initialize lists
File_IO_ADD = []
Array_ADD = []
Linked_List_ADD = []
BST_ADD = []
Skip_List_ADD = []
# Read and process the file
with open("times.csv", 'r') as file:
    for line in file:
        parts = line.strip().split(',')
        if len(parts) > 1:
            # Check both data structure name and operation
            
            if parts[0] == 'File_IO' and parts[1] == 'ADD':
                File_IO_ADD.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'Array' and parts[1] == 'ADD':
                Array_ADD.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'Linked_List' and parts[1] == 'ADD':
                Linked_List_ADD.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'BST' and parts[1] == 'ADD':
                BST_ADD.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'Skip_List' and parts[1] == 'ADD':
                Skip_List_ADD.extend([float(x) for x in parts[2:]])

# Plotting
x_axis = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
plt.xlabel('Dataset Size (thousands)')
plt.ylabel('Time (ms)')

plt.plot(x_axis, File_IO_ADD, label='File_IO_ADD')
plt.plot(x_axis, Array_ADD, label='Array_ADD')
plt.plot(x_axis, Linked_List_ADD, label='Linked_List_ADD')
plt.plot(x_axis, BST_ADD, label='BST_ADD')
plt.plot(x_axis, Skip_List_ADD, label='Skip_List_ADD')

plt.title('Group 1 ADD')
plt.legend()
plt.grid()
plt.show()
