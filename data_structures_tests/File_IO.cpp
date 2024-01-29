/*@Author
StudentName: MHD Kamal Rushdi
StudentID: 150210907 
*/
#include<iostream>
#include<string>
#include<fstream>
// only used limits to ignore the first line of the file. I could have used a really large value of of charaters
// but this is more elegant
#include<limits>
#include<sstream>
#include<vector>

using namespace std;

int employee_count = 0;
int latest_id = 0;

void add_employee(string read_file_name, string salary, string department) {
    fstream read_file;
    read_file.open(read_file_name, ios::app);

    read_file <<  latest_id+1 << ';' << salary << ';' << department << endl;
    latest_id++;
    employee_count++;

    read_file.clear();
    read_file.seekg(0, ios::beg);
    read_file.close();
}

bool search_employee(string read_file_name, string id) {
    fstream read_file;
    read_file.open(read_file_name, ios::in);
    string temp;

    while(getline(read_file, temp)) {
        if(temp != "") {
            size_t id_end_index = temp.find(';');
            string temp_id = temp.substr(0, id_end_index);
            if(id == temp_id) {
                read_file.clear();
                read_file.seekg(0, ios::beg);
                read_file.close();
                return true;
            }
        }  
    }
    read_file.clear();
    read_file.seekg(0, ios::beg);
    read_file.close();
    return false;
}

void update_employee(string read_file_name, string id, string salary, string department) {
    bool exist = search_employee(read_file_name, id);
    if(!exist) {
        cout << "ERROR: An invalid ID to update\n";
        return;
    }
    
    fstream read_file;
    read_file.open(read_file_name, ios::in);
    stringstream data;
    string temp;
    while(getline(read_file, temp)) {
        if(temp != "") {
            size_t id_end_index = temp.find(';');
            string temp_id = temp.substr(0, id_end_index);
            if(id == temp_id) {
                data <<  id  << ";" << salary << ";"  << department << endl;
            } else {
                data << temp << endl;
            }
        }  
    }
    read_file.close();

    read_file.open(read_file_name, ios::out);
    read_file << data.rdbuf();

    read_file.clear();
    read_file.seekg(0, ios::beg);
    read_file.close();
}
void delete_employee(string read_file_name, string id) {
    bool exist = search_employee(read_file_name, id);
    if(!exist) {
        cout << "ERROR: There is no Employee\n";
        return;
    }
    fstream read_file;
    read_file.open(read_file_name, ios::in);
    stringstream data;
    string temp;
    while(getline(read_file, temp)) {
        if(temp != "") {
            size_t id_end_index = temp.find(';');
            string temp_id = temp.substr(0, id_end_index);
            if(id == temp_id) {
                continue;
            } else {
                data << temp << endl;
            }
        }  
    }
    read_file.close();

    read_file.open(read_file_name, ios::out);
    read_file << data.rdbuf();

    read_file.clear();
    read_file.seekg(0, ios::beg);
    read_file.close();
}

int main() {
    
    // input files name array
    string input_files[10] = {"../datasets_IO/1k.csv", "../datasets_IO/2k.csv", "../datasets_IO/3k.csv", "../datasets_IO/4k.csv", "../datasets_IO/5k.csv", "../datasets_IO/6k.csv", "../datasets_IO/7k.csv", "../datasets_IO/8k.csv", "../datasets_IO/9k.csv", "../datasets_IO/10k.csv"};
    // operation files name array
    string operation_files[10] = {"../operations_files/operations1.csv", "../operations_files/operations2.csv", "../operations_files/operations3.csv", "../operations_files/operations4.csv", "../operations_files/operations5.csv", "../operations_files/operations6.csv", "../operations_files/operations7.csv", "../operations_files/operations8.csv", "../operations_files/operations9.csv", "../operations_files/operations10.csv"};
    stringstream ss;
    
    vector<double> add_times_final;
    vector<double> update_times_final;
    vector<double> delete_times_final;
    for(int i = 0; i < 10; i++) {
        fstream read_file;
        read_file.open(input_files[i], ios::in);
        if(!read_file) {
            cout << "Cannot open file!" << endl;
            return 1;
        }

        read_file.ignore(numeric_limits<streamsize>::max(), '\n');
        string unused;
        int initial_employee_number = 0;
        while (getline(read_file, unused) ) {
            if(unused != "")
                ++initial_employee_number;
        }
        employee_count = initial_employee_number;
        latest_id = initial_employee_number;

        read_file.clear();                 // clear fail and eof bits
        read_file.seekg(0, ios::beg);      // back to the start

        
        vector<double> add_times_values;
        vector<double> update_times_values;
        vector<double> delete_times_values;

        fstream operations_file;
        operations_file.open(operation_files[i], ios::in);
        if(!operations_file) {
            cout << "Cannot open operation file!" << endl;
            return 1;
        }
        string operation_temp;

        while(getline(operations_file, operation_temp)) {
            size_t operation_end_index = operation_temp.find(';');
            string operation_type = operation_temp.substr(0, operation_end_index);
            if(operation_type == "ADD" ) {
                size_t salary_end_index = operation_temp.find(';', operation_end_index + 1);
                string salary = operation_temp.substr(operation_end_index + 1, salary_end_index - operation_end_index - 1);
                string department = operation_temp.substr(salary_end_index + 1, operation_temp.length() - salary_end_index - 2);
                
                clock_t start = clock();
                add_employee(input_files[i], salary, department);
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                add_times_values.push_back(time_taken);

            } else if(operation_type == "UPDATE") {
                size_t id_end_index = operation_temp.find(';', operation_end_index + 1);
                string id = operation_temp.substr(operation_end_index + 1, id_end_index - operation_end_index - 1);
                size_t salary_end_index = operation_temp.find(';', id_end_index + 1);
                string salary = operation_temp.substr(id_end_index + 1, salary_end_index - id_end_index - 1);
                string department = operation_temp.substr(salary_end_index + 1, operation_temp.length() - salary_end_index - 1);
                
                clock_t start = clock();
                update_employee(input_files[i], id, salary, department);
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                update_times_values.push_back(time_taken);

            } else if(operation_type == "DELETE") {
                int id = stoi(operation_temp.substr(operation_end_index + 1, operation_temp.length() - operation_end_index - 1));
                
                clock_t start = clock();
                delete_employee(input_files[i], to_string(id));
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                delete_times_values.push_back(time_taken);

            }

        }
        // calculate average times
        double avg_add_time = 0;
        for(auto k : add_times_values) {
            avg_add_time += k;
        }
        avg_add_time /= (double)add_times_values.size();
        // push average times to final vectors
        add_times_final.push_back(avg_add_time);

        double avg_update_time = 0;
        for(auto k : update_times_values) {
            avg_update_time += k;
        }
        avg_update_time /= (double)update_times_values.size();
        update_times_final.push_back(avg_update_time);

        double avg_delete_time = 0;
        for(auto k : delete_times_values) {
            avg_delete_time += k;
        }
        avg_delete_time /= (double)delete_times_values.size();
        delete_times_final.push_back(avg_delete_time);

        operations_file.close();
        // reset the latest_id
        employee_count = 0;
        latest_id = 0;
    }

    fstream write_file;
    write_file.open("times.csv", ios::out | ios::app);
    stringstream data;  // Create a stringstream to build the data
    data << "File_IO,ADD," << add_times_final[0] << "," << add_times_final[1] << "," << add_times_final[2] << "," << add_times_final[3] << "," << add_times_final[4] << "," << add_times_final[5] << "," << add_times_final[6] << "," << add_times_final[7] << "," << add_times_final[8] << "," << add_times_final[9] << endl;
    data << "File_IO,UPDATE," << update_times_final[0] << "," << update_times_final[1] << "," << update_times_final[2] << "," << update_times_final[3] << "," << update_times_final[4] << "," << update_times_final[5] << "," << update_times_final[6] << "," << update_times_final[7] << "," << update_times_final[8] << "," << update_times_final[9] << endl;
    data << "File_IO,DELETE," << delete_times_final[0] << "," << delete_times_final[1] << "," << delete_times_final[2] << "," << delete_times_final[3] << "," << delete_times_final[4] << "," << delete_times_final[5] << "," << delete_times_final[6] << "," << delete_times_final[7] << "," << delete_times_final[8] << "," << delete_times_final[9] << endl;

    write_file << data.rdbuf();

    return 0;
}