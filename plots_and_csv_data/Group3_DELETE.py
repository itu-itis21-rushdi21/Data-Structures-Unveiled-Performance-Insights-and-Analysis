import matplotlib.pyplot as plt
import os

# Get the directory of the current script
script_dir = os.path.dirname(os.path.realpath(__file__))

# Change the working directory
os.chdir(script_dir)
# File name
file_name = 'times.csv'

# Initialize lists
BST_DELETE = []
STL_Map_DELETE = []
Skip_List_DELETE = []
# Read and process the file
with open("times.csv", 'r') as file:
    for line in file:
        parts = line.strip().split(',')
        if len(parts) > 1:
            # Check both data structure name and operation
            
            if parts[0] == 'BST' and parts[1] == 'DELETE':
                BST_DELETE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'STL_Map' and parts[1] == 'DELETE':
                STL_Map_DELETE.extend([float(x) for x in parts[2:]])
            elif parts[0] == 'Skip_List' and parts[1] == 'DELETE':
                Skip_List_DELETE.extend([float(x) for x in parts[2:]])

        
# Plotting
x_axis = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
plt.xlabel('Dataset Size (thousands)')
plt.ylabel('Time (ms)')

#plt.plot(x_axis, File_IO_DELETE, label='File_IO_DELETE')
plt.plot(x_axis, BST_DELETE, label='BST_DELETE')
plt.plot(x_axis, STL_Map_DELETE, label='STL_Map_DELETE')
plt.plot(x_axis, Skip_List_DELETE, label='Skip_List_DELETE')

plt.title('Group 3 DELETE')
plt.legend()
plt.grid()
plt.show()
