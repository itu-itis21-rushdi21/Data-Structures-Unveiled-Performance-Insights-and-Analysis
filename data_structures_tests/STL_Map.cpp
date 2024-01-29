/*@Author
StudentName: MHD Kamal Rushdi
StudentID: 150210907
Date: 18/12/2023 */

#include<iostream>
#include<sstream>
#include<string.h>
#include<fstream>
#include<map>
#include<time.h>
#include<vector>

using namespace std;
int latest_id = 0;
class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        //setters and getters
        int get_id() {
            return id;
        }
        int get_salary() {
            return salary;
        }
        int get_department() {
            return department;
        }
        void set_id(int id) {
            this->id = id;
        }
        void set_salary(int salary) {
            this->salary = salary;
        }
        void set_department(int department) {
            this->department = department;
        }
        //constructor
        Employee(int id, int salary, int department) {
            this->id = id;
            this->salary = salary;
            this->department = department;
        }
        //destructor
        ~Employee() {
        }
};

int main(void) {
    //clock_t start = clock();
    // input files name array
    string input_files[10] = {"../datasets/1k.csv", "../datasets/2k.csv", "../datasets/3k.csv", "../datasets/4k.csv", "../datasets/5k.csv", "../datasets/6k.csv", "../datasets/7k.csv", "../datasets/8k.csv", "../datasets/9k.csv", "../datasets/10k.csv"};
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
            cout << "Cannot open read file!" << endl;
            return 1;
        }

        string unused;
        getline(read_file, unused);
        map<int, Employee*> employee_map;
        string line;
        while(!read_file.eof()) {
            getline(read_file, line);
            if(line != ""){
                istringstream ss_line(line);
                string id, salary, department;
                getline(ss_line, id, ';');
                getline(ss_line, salary, ';');
                getline(ss_line, department);
                Employee* employee = new Employee(stoi(id), stoi(salary), stoi(department));
                employee_map[employee->get_id()] = employee;
            }
        }
        int largest_id_initial = 0;
        for(const auto& pair: employee_map) {
            if(pair.first > largest_id_initial)
                largest_id_initial = pair.first;
        }
        latest_id = largest_id_initial;
        read_file.close();

        vector<double> add_times_values;
        vector<double> update_times_values;
        vector<double> delete_times_values;

        fstream operation_file;
        operation_file.open(operation_files[i], ios::in);
        if(!operation_file) {
            cout << "Cannot open operation file!" << endl;
            return 1;
        }
        string operation;
        while(!operation_file.eof()) {
            getline(operation_file, operation);
            istringstream ss_operation(operation);
            string operation_type;
            getline(ss_operation, operation_type, ';');
            if(operation_type.back() == '\r' && !operation_type.empty()){
                operation_type.pop_back();
            }
            string id, salary, department;
            if(operation_type == "ADD") {
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);
                // these news are deleted inside the remove function and inside the tree destructor
                latest_id++;
                clock_t start = clock();
                Employee* employee = new Employee(latest_id, stoi(salary), stoi(department));
                employee_map[employee->get_id()] = employee;
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                add_times_values.push_back(time_taken);

            } else if(operation_type == "UPDATE") {
                getline(ss_operation, id, ';');
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);
                clock_t start = clock();
                map<int, Employee*>::iterator it = employee_map.find(stoi(id));
                if(it == employee_map.end()) {
                    cout<< "ERROR: An invalid ID to update\n";
                } else {
                    it->second->set_salary(stoi(salary));
                    it->second->set_department(stoi(department));
                }
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                update_times_values.push_back(time_taken);

            } else if(operation_type == "DELETE"){
                getline(ss_operation, id);

                clock_t start = clock();
                map<int, Employee*>::iterator it = employee_map.find(stoi(id));
                if(it == employee_map.end()) {
                    cout<< "ERROR: An invalid ID to delete\n";
                } else {
                    delete it->second;
                    employee_map.erase(it);
                }
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                delete_times_values.push_back(time_taken);

            } else if(operation_type == "PRINT"){
                cout<< "P\n";
                
                for(const auto& pair: employee_map) {
                    cout<< pair.second->get_id() << ";" << pair.second->get_salary() << ";" << pair.second->get_department() << endl; 
                }
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

        operation_file.close();
        // reset the latest_id
        latest_id = 0;
    }

    fstream write_file;
    write_file.open("times.csv", ios::out | ios::app);
    stringstream data;  // Create a stringstream to build the data
    data << "STL_Map,ADD," << add_times_final[0] << "," << add_times_final[1] << "," << add_times_final[2] << "," << add_times_final[3] << "," << add_times_final[4] << "," << add_times_final[5] << "," << add_times_final[6] << "," << add_times_final[7] << "," << add_times_final[8] << "," << add_times_final[9] << endl;
    data << "STL_Map,UPDATE," << update_times_final[0] << "," << update_times_final[1] << "," << update_times_final[2] << "," << update_times_final[3] << "," << update_times_final[4] << "," << update_times_final[5] << "," << update_times_final[6] << "," << update_times_final[7] << "," << update_times_final[8] << "," << update_times_final[9] << endl;
    data << "STL_Map,DELETE," << delete_times_final[0] << "," << delete_times_final[1] << "," << delete_times_final[2] << "," << delete_times_final[3] << "," << delete_times_final[4] << "," << delete_times_final[5] << "," << delete_times_final[6] << "," << delete_times_final[7] << "," << delete_times_final[8] << "," << delete_times_final[9] << endl;
    write_file << data.rdbuf();
    
    return 0; 
}