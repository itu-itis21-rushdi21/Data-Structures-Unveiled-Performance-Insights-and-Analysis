#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<limits>
#include<time.h>
#include<vector>

using namespace std;

int latest_id = 0;
class Employee {
    private:
        int id, salary, department;
        Employee* next;
    public:
        Employee();
        Employee(int, int, int);
        ~Employee();
        void set_id(int);
        void set_salary(int);
        void set_department(int);
        void set_next(Employee*);
        int get_id();
        int get_salary();
        int get_department();
        Employee* get_next();
};

class LinkedList {
    private:
        Employee* head;
        Employee* tail;
        Employee* search_prev(int);
    public:
        bool is_empty();
        void add_employee(int, int);
        void remove_employee(int);
        void update_employee(int, int, int);
        Employee* first_employee();
        Employee* search_employee(int);
        LinkedList();
        ~LinkedList();
        
};

Employee::Employee(int id, int salary, int department) {
    this->id = id;
    this->salary = salary;
    this->department = department;
}

Employee::Employee(){
}

Employee::~Employee() {
}

bool LinkedList::is_empty(){
    return head == NULL;
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

void Employee::set_next(Employee* next) {
    this->next = next;
}

Employee* Employee::get_next(){
    return next;
}
///////////////////////////////////
LinkedList::LinkedList() {
    head = NULL;
    tail = NULL;
}

LinkedList::~LinkedList() {
    Employee *p = head;
    Employee *n = tail;
    while(n != NULL) {
        n = p->get_next();
        delete p;
        p = n;
    } 
}

Employee* LinkedList::search_employee(int id) {
    Employee* p = head;
    if(p == NULL){
        return NULL;
    }
    if(id <= head->get_id()) {
        return NULL;
    } else {
        while(p->get_next()) {
            if(p->get_next()->get_id() >= id) {
                return p;
            }
            p = p->get_next();
        }
        return p;
    }
}

void LinkedList::add_employee(int salary, int department){
    latest_id++;
    Employee* new_employee = new Employee(latest_id, salary,department);
    
    if(head == NULL) {
        head = new_employee;
        tail = new_employee;
        return;
    }
    tail->set_next(new_employee);
    tail = new_employee;
    tail->set_next(NULL);
}

Employee* LinkedList::first_employee() {
    return head;
}

void LinkedList::remove_employee(int id) {    
    if(head == NULL) {
        //cout<< "ERROR: There is no Employee\n";
        return;
    }
    Employee* p = head;
    bool exist = false;
    while(p->get_next()) {
        if(p->get_id() == id) {
            exist = true;
        }
        p = p->get_next();
    }
    if(tail->get_id() == id) {
        exist = true;
    }
    if(!exist) {
        cout<< "ERROR: An invalid ID to delete\n";
        return;
    }
    Employee* prev = this->search_employee(id);
    
    if(head == tail && head != NULL && tail != NULL ) {
        delete head;
        head = NULL;
        tail = NULL;
    } else {
        if(prev == NULL) {
            Employee* old_head = head;
            head = head->get_next();
            old_head->set_next(NULL);
            delete old_head;
        } else if (prev->get_next() == tail){
            prev->set_next(NULL);
            delete tail;
            tail = prev;
        } else {
            Employee* deleted_node = prev->get_next();
            prev->set_next(prev->get_next()->get_next());
            deleted_node->set_next(NULL);
            delete deleted_node;
        }
    }
}

void LinkedList::update_employee(int id, int salary, int department) {
    Employee* p = head;
    if(head == NULL) {
        cout<< "ERROR: There is no Employee\n";
        return;
    }
    while(p->get_next()) {
        if(p->get_id() == id) {
            p->set_salary(salary);
            p->set_department(department);
            return;
        }
        p = p->get_next();
    }
    cout<< "ERROR: An invalid ID to update\n";
}

//////////////////////////

int main() {
    // input files name array
    string input_files[10] = {"../datasets/1k.csv", "../datasets/2k.csv", "../datasets/3k.csv", "../datasets/4k.csv", "../datasets/5k.csv", "../datasets/6k.csv", "../datasets/7k.csv", "../datasets/8k.csv", "../datasets/9k.csv", "../datasets/10k.csv"};
    // operation files name array
    string operation_files[10] = {"../operations_files/operations1.csv", "../operations_files/operations2.csv", "../operations_files/operations3.csv", "../operations_files/operations4.csv", "../operations_files/operations5.csv", "../operations_files/operations6.csv", "../operations_files/operations7.csv", "../operations_files/operations8.csv", "../operations_files/operations9.csv", "../operations_files/operations10.csv"};
    stringstream ss;
    
    vector<double> add_times_final;
    vector<double> update_times_final;
    vector<double> delete_times_final;

    for (int i = 0; i < 10; i++) {
        
        fstream read_file;
        read_file.open(input_files[i], ios::in);
        if(!read_file) {
            cout << "Cannot open file!" << endl;
            return 1;
        }

        read_file.ignore(numeric_limits<streamsize>::max(), '\n');
        LinkedList* employees_list = new LinkedList();
        string line;
        while(!read_file.eof()) {
            getline(read_file, line);
            if(line != ""){
                istringstream ss_line(line);
                string id, salary, department;
                getline(ss_line, id, ';');
                getline(ss_line, salary, ';');
                getline(ss_line, department);
                employees_list->add_employee(stoi(salary), stoi(department));
            }
        }
        read_file.close();


        vector<double> add_times_values;
        vector<double> update_times_values;
        vector<double> delete_times_values;

        fstream operation_file;
        operation_file.open(operation_files[i], ios::in);
        if(!operation_file) {
            cout << "Cannot open file!" << endl;
            return 1;
        }
        string operation;
        while(!operation_file.eof()) {
            getline(operation_file, operation);
            istringstream ss_operation(operation);
            string operation_type;
            getline(ss_operation, operation_type, ';');
            string id, salary, department;
            if(operation_type == "ADD") {
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);

                clock_t start = clock();
                employees_list->add_employee(stoi(salary), stoi(department));
                clock_t end = clock();
                double duration = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                add_times_values.push_back(duration);

            } else if(operation_type == "UPDATE") {
                getline(ss_operation, id, ';');
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);
                clock_t start = clock();
                employees_list->update_employee(stoi(id), stoi(salary), stoi(department));
                clock_t end = clock();
                double duration = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                update_times_values.push_back(duration);

            } else if(operation_type == "DELETE"){
                getline(ss_operation, id);
                clock_t start = clock();
                employees_list->remove_employee(stoi(id));
                clock_t end = clock();
                double duration = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                delete_times_values.push_back(duration);
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
        delete employees_list;
    }

    fstream write_file;
    write_file.open("times.csv", ios::out | ios::app);
    stringstream data;  // Create a stringstream to build the data
    data << "DataStructureType,Operation,10ktimes,20ktimes,30ktimes,40ktimes,50ktimes,60ktimes,70ktimes,80ktimes,90ktimes,100ktimes" << endl;
    data << "Linked_List,ADD," << add_times_final[0] << "," << add_times_final[1] << "," << add_times_final[2] << "," << add_times_final[3] << "," << add_times_final[4] << "," << add_times_final[5] << "," << add_times_final[6] << "," << add_times_final[7] << "," << add_times_final[8] << "," << add_times_final[9] << endl;
    data << "Linked_List,UPDATE," << update_times_final[0] << "," << update_times_final[1] << "," << update_times_final[2] << "," << update_times_final[3] << "," << update_times_final[4] << "," << update_times_final[5] << "," << update_times_final[6] << "," << update_times_final[7] << "," << update_times_final[8] << "," << update_times_final[9] << endl;
    data << "Linked_List,DELETE," << delete_times_final[0] << "," << delete_times_final[1] << "," << delete_times_final[2] << "," << delete_times_final[3] << "," << delete_times_final[4] << "," << delete_times_final[5] << "," << delete_times_final[6] << "," << delete_times_final[7] << "," << delete_times_final[8] << "," << delete_times_final[9] << endl;

    write_file << data.rdbuf();
}