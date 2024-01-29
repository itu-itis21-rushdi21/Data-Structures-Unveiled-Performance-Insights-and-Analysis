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
int latest_id = 0;
int employee_count = 0;
class Employee {
    private:
        int id, salary, department;
    public:
        Employee();
        Employee(int, int);
        ~Employee();
        void set_id(int);
        void set_salary(int);
        void set_department(int);
        int get_id();
        int get_salary();
        int get_department();
};

Employee::Employee(){
}

Employee::Employee(int salary, int department) {
    this->salary = salary;
    this->department = department;
}

Employee::~Employee() {
}

void Employee::set_id(int id){
    this->id = id;
}

void Employee::set_salary(int salary) {
    this->salary = salary;
}

void Employee::set_department(int department) {
    this->department = department;
}

int Employee::get_id() {
    return this->id;
}

int Employee::get_salary() {
    return this->salary;
}

int Employee::get_department() {
    return this->department;
}

Employee* add_employee(Employee *old_employees, int salary, int department) {
    Employee* new_employee = new Employee(salary, department);
    employee_count++;
    latest_id++;
    Employee* new_employees = new Employee[employee_count];
    for(int i = 0; i < employee_count - 1; i++) {
        new_employees[i] = old_employees[i];
    }
    new_employee->set_id(latest_id);
    new_employees[employee_count - 1] = *new_employee;
    delete new_employee;
    delete[] old_employees;
    return new_employees;
}

bool search_by_id(Employee *employees_record, int id) {
    for(int i = 0; i < employee_count; i++) {
        if(employees_record[i].get_id() == id) {
            return true;
        }
    }
    return false;
}

Employee* delete_employee(Employee *old_employees, int id) {
    if(search_by_id(old_employees, id) == false) {
        cout << "ERROR: An invalid ID to delete\n";
        return old_employees;
    }
    employee_count--;
    Employee* new_employees = new Employee[employee_count];
    int j = 0;
    for(int i = 0; i < employee_count + 1; i++) {
        if(old_employees[i].get_id() == id) {
            continue;
        }
        new_employees[j] = old_employees[i];
        j++;
    }
    delete[] old_employees;
    return new_employees;
}

Employee* update_employee(Employee *employees_record,int id, int salary, int department) {
    if(search_by_id(employees_record, id) == false) {
        cout << "ERROR: An invalid ID to update\n";
        return employees_record;
    }
    for(int i = 0; i < employee_count; i++) {
        if(employees_record[i].get_id() == id) {
            employees_record[i].set_salary(salary);
            employees_record[i].set_department(department);
        }
    }
    return employees_record;
}

void print_employees(Employee *employees_record) {
    for(int i = 0; i < employee_count; i++) {
        cout << employees_record[i].get_id() << " " << employees_record[i].get_salary() << " " << employees_record[i].get_department() << endl;
    }
}

int main() {
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
            cout << "Cannot open file!" << endl;
            return 1;
        }
        
        vector<double> add_times_values;
        vector<double> update_times_values;
        vector<double> delete_times_values;

        // counting the number of emplcees in the file starting from the 2nd line
        //read_file.ignore(numeric_limits<streamsize>::max(), '\n');
        
        string unused;
        int initial_employee_number = 0;
        while (getline(read_file, unused) ) {
            if(unused != "")
                ++initial_employee_number;
        }

        read_file.clear();                 // clear fail and eof bits
        read_file.seekg(0, ios::beg);      // back to the start!

        read_file.ignore(numeric_limits<streamsize>::max(), '\n');
        employee_count = initial_employee_number;
        latest_id = initial_employee_number;
        Employee* employees = new Employee[initial_employee_number];
        string temp;
        int data_type = 0;

        while(!read_file.eof()) {
            if(data_type % 3 == 0 || data_type % 3 == 1) {
                getline(read_file, temp, ';');
                if(data_type % 3 == 0) {
                    if(temp == "") {
                        break;
                    }
                    employees[data_type/3].set_id(stoi(temp));
                } else if(data_type %3 == 1) {
                    employees[data_type/3].set_salary(stoi(temp));
                }
            } else if(data_type % 3 == 2){
                getline(read_file, temp, '\n');
                temp.substr(0, temp.length()-1);
                employees[data_type/3].set_department(stoi(temp));
            }    
            data_type++;
        }
        read_file.close();

        fstream operations_file;
        operations_file.open(operation_files[i], ios::in);
        string operation_temp;
        while(getline(operations_file, operation_temp)) {
            size_t operation_end_index = operation_temp.find(';');
            string operation_type = operation_temp.substr(0, operation_end_index);
            if(operation_type == "ADD" ) {
                size_t salary_end_index = operation_temp.find(';', operation_end_index + 1);
                int salary = stoi(operation_temp.substr(operation_end_index + 1, salary_end_index - operation_end_index - 1));
                int department = stoi(operation_temp.substr(salary_end_index + 1, operation_temp.length() - salary_end_index - 1));
                
                clock_t start = clock();
                employees = add_employee(employees, salary, department);
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                add_times_values.push_back(time_taken);

            } else if(operation_type == "UPDATE") {
                size_t id_end_index = operation_temp.find(';', operation_end_index + 1);
                int id = stoi(operation_temp.substr(operation_end_index + 1, id_end_index - operation_end_index - 1));
                size_t salary_end_index = operation_temp.find(';', id_end_index + 1);
                int salary = stoi(operation_temp.substr(id_end_index + 1, salary_end_index - id_end_index - 1));
                string str = operation_temp.substr(salary_end_index + 1, operation_temp.length() - salary_end_index - 1);
                int department = stoi(operation_temp.substr(salary_end_index + 1, operation_temp.length() - salary_end_index - 1));
                
                clock_t start = clock();
                employees = update_employee(employees, id, salary, department);
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                update_times_values.push_back(time_taken);

            } else if(operation_type == "DELETE") {
                int id = stoi(operation_temp.substr(operation_end_index + 1, operation_temp.length() - operation_end_index - 1));

                clock_t start = clock();
                employees = delete_employee(employees, id);
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
        latest_id = 0;
        employee_count = 0;
        delete[] employees;
    }

    fstream write_file;
    write_file.open("times.csv", ios::out | ios::app);
    stringstream data;  // Create a stringstream to build the data
    
    data << "Array,ADD," << add_times_final[0] << "," << add_times_final[1] << "," << add_times_final[2] << "," << add_times_final[3] << "," << add_times_final[4] << "," << add_times_final[5] << "," << add_times_final[6] << "," << add_times_final[7] << "," << add_times_final[8] << "," << add_times_final[9] << endl;
    data << "Array,UPDATE," << update_times_final[0] << "," << update_times_final[1] << "," << update_times_final[2] << "," << update_times_final[3] << "," << update_times_final[4] << "," << update_times_final[5] << "," << update_times_final[6] << "," << update_times_final[7] << "," << update_times_final[8] << "," << update_times_final[9] << endl;
    data << "Array,DELETE," << delete_times_final[0] << "," << delete_times_final[1] << "," << delete_times_final[2] << "," << delete_times_final[3] << "," << delete_times_final[4] << "," << delete_times_final[5] << "," << delete_times_final[6] << "," << delete_times_final[7] << "," << delete_times_final[8] << "," << delete_times_final[9] << endl;

    write_file << data.rdbuf();


    return 0;
}
