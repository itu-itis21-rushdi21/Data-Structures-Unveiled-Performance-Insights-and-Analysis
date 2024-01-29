/*@Author
StudentName: MHD Kamal Rushdi
StudentID: 150210907
Date: 18/12/2023 */

#include<iostream>
#include<sstream>
#include<string.h>
#include<fstream>
#include<queue>
#include<stack>
#include<time.h>

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
        //constructors
        Employee(int id, int salary, int department) {
            this->id = id;
            this->salary = salary;
            this->department = department;
        }
        //destructor
        ~Employee() {
        }
};

class Node {
    private:
        Employee* employee;
        Node* left;
        Node* right;
    public:
        //setters and getters
        Employee* get_employee() {
            return employee;
        }
        Node* get_left() {
            return left;
        }
        Node* get_right() {
            return right;
        }        
        void set_employee(Employee* employee) {
            this->employee = employee;
        }
        void set_left(Node* left) {
            this->left = left;
        }
        void set_right(Node* right) {
            this->right = right;
        }
        //constructors
        Node(Employee* employee) {
            this->employee = employee;
            this->left = NULL;
            this->right = NULL;
        }
        //destructor
        ~Node() {
        }
};

class Tree {
    private:
        Node* root;
    public:
        //setters and getters
        Node* get_root() {
            return root;
        }
        void set_root(Node* root) {
            this->root = root;
        }
        //constructors
        Tree() {
            this->root = NULL;
        }
        //destructor
        ~Tree() {
            queue<Node*> q;
            Node* temp;
            if(this->root != NULL)
                q.push(this->root);
            while(!q.empty()) {
                temp = q.front();
                q.pop();
                if(temp->get_left() != NULL)
                    q.push(temp->get_left()); 
                if(temp->get_right() != NULL)
                    q.push(temp->get_right());
                delete temp->get_employee();
                delete temp;
            }


        }
        //methods
        void insert(Employee* employee) {
            //** REMOVE FROM HERE IF NEEDED

            // this code was added to decrease the time needed to insert to the right_most of the tree
            // it servers to reduce that time from O(N) to O(1)
            // Note that without this piece of code the program will take very long time to pass the ordered_data test
            // more than 1 h probably
            // even with this part of code program will take long time 
            // this is due to the fact that this code doesn't reduce the times needed for delete and update operations (still O(N) in worst case)
            
            //bool larger_than_all_ids = false; 
            if(employee->get_id() > latest_id) {
                latest_id = employee->get_id();
                //larger_than_all_ids = true;
            } else if(latest_id == employee->get_id()){
                latest_id++;
                employee->set_id(latest_id);
                //larger_than_all_ids = true; 
            }
            //if(larger_than_all_ids == true) {
            //    Node* new_node = new Node(employee); 
            //    if(root == NULL) {
            //        root = new_node;
            //        return;
            //    }
            //    Node* right_most = root;
            //    while(right_most->get_right() != NULL) {
            //        right_most = right_most->get_right();
            //    } 
            //    right_most->set_right(new_node);
            //    return;
            //}

            //** REMOVE TO HERE IF NEEDED

            Node* new_node = new Node(employee);
            if(root == NULL) {
                root = new_node;
            } else {
                Node* temp = root;
                while(true) {
                    if(employee->get_id() < temp->get_employee()->get_id()) {
                        if(temp->get_left() == NULL) {
                            temp->set_left(new_node);
                            return;
                        }
                        temp = temp->get_left();
                    } else if(employee->get_id() > temp->get_employee()->get_id()) {
                        if(temp->get_right() == NULL) {
                            temp->set_right(new_node);
                            return;
                        }
                        temp = temp->get_right();
                    }
                }
            } 
        }

        void remove(int id) {
            if (root == NULL) {
                cout << "ERROR: An invalid ID to delete\n";
                return;
            }

            Node* current = root;
            Node* parent = NULL;
            
            while (current != NULL && current->get_employee()->get_id() != id) {
                parent = current;
                if (id < current->get_employee()->get_id()) {
                    current = current->get_left();
                } else {
                    current = current->get_right();
                }
            }

            if (current == NULL) {
                cout << "ERROR: An invalid ID to delete\n";
                return; 
            }

            // Case 1: Node with either one or children NULL
            if (current->get_left() == NULL || current->get_right() == NULL) {
                Node* newChild = current->get_left() ? current->get_left() : current->get_right();

                // If the node to be deleted is the root
                if (parent == NULL) {
                    root = newChild;
                } else if (parent->get_left() == current) {
                    parent->set_left(newChild);
                } else {
                    parent->set_right(newChild);
                }

                delete current->get_employee(); 
                delete current; 
            }
            // Case 2: Node with both children
            else {
                Node* successorParent = current;
                Node* successor = current->get_right();
                // Find inorder successor (leftmost node in the right subtree)
                while (successor->get_left() != NULL) {
                    successorParent = successor;
                    successor = successor->get_left();
                }

                // Swap values with the successor
                current->get_employee()->set_id(successor->get_employee()->get_id());
                current->get_employee()->set_salary(successor->get_employee()->get_salary());
                current->get_employee()->set_department(successor->get_employee()->get_department());

                // Delete the inorder successor
                if (successorParent != current) {
                    successorParent->set_left(successor->get_right());
                } else {
                    successorParent->set_right(successor->get_right());
                }

                delete successor->get_employee(); 
                delete successor; 
            }
        }
        Employee* search(int id) {
            Node* temp = this->root;
            while(temp != NULL) {
                if(temp->get_employee()->get_id() > id) {
                    temp = temp->get_left();
                } else if(temp->get_employee()->get_id() < id) {
                    temp = temp->get_right();
                } else {
                    return temp->get_employee();
                }
            }
            return NULL;
        }
        int getHeight() {
            queue<Node*> q;
            int height = 0;
            if(root == NULL) 
                return 0;
            q.push(root);
            while(!q.empty()) {
                int level_size = q.size();
                for(int i = 0; i < level_size; i++) {
                    Node* temp = q.front();
                    q.pop();
                    if(temp->get_left() != NULL)
                        q.push(temp->get_left());
                    
                    if(temp->get_right() != NULL)
                        q.push(temp->get_right());
                }
                height++;
            }
            return height;
        }

        void printToFile(ofstream &output_file) {
            stack<Node*> s;
            Node* temp = root;
            stringstream data;
            data<< "Employee_ID;Salary;Department"<< endl;
            while (temp != NULL || s.empty() == false) {
                // Reach the left most Node of the temp Node
                while (temp != NULL) {
                    s.push(temp);
                    temp = temp->get_left();
                }
                
                temp = s.top();
                s.pop();

                data << temp->get_employee()->get_id() << ";" << temp->get_employee()->get_salary() << ";" << temp->get_employee()->get_department() << endl;
                
                temp = temp->get_right();
            }
            if(this->root != NULL)
                output_file << data.rdbuf();
        }  

        void printToConsole() {
            stack<Node*> s;
            Node* temp;
            if(this->root != NULL)
                s.push(this->root);
            while(!s.empty()) {
                temp = s.top();
                s.pop();
                cout << temp->get_employee()->get_id() << ";" << temp->get_employee()->get_salary() << ";" << temp->get_employee()->get_department() << "\n";
                if(temp->get_right() != NULL) {
                    s.push(temp->get_right());
                }
                if(temp->get_left() != NULL) {
                    s.push(temp->get_left());
                }
            }
        }
        
};

int main() {
    //clock_t start = clock();
    
    
    // input files name array
    string input_files[10] = {"../datasets_shuffled/1k_shuffled.csv", "../datasets_shuffled/2k_shuffled.csv", "../datasets_shuffled/3k_shuffled.csv", "../datasets_shuffled/4k_shuffled.csv", "../datasets_shuffled/5k_shuffled.csv", "../datasets_shuffled/6k_shuffled.csv", "../datasets_shuffled/7k_shuffled.csv", "../datasets_shuffled/8k_shuffled.csv", "../datasets_shuffled/9k_shuffled.csv", "../datasets_shuffled/10k_shuffled.csv"};
    string input_files_ordered[10] = {"../datasets/1k.csv", "../datasets/2k.csv", "../datasets/3k.csv", "../datasets/4k.csv", "../datasets/5k.csv", "../datasets/6k.csv", "../datasets/7k.csv", "../datasets/8k.csv", "../datasets/9k.csv", "../datasets/10k.csv"};
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
        Tree* employees_tree = new Tree();
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
                employees_tree->insert(employee);
            }
        }
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
            string id, salary, department;
            if(operation_type.back() == '\r' && !operation_type.empty()){
                operation_type.pop_back();
            }        
            if(operation_type == "ADD") {
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);
                // these news are deleted inside the remove function and inside the tree destructor
                clock_t start = clock();
                Employee* employee = new Employee(latest_id, stoi(salary), stoi(department));
                employees_tree->insert(employee);
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                add_times_values.push_back(time_taken);

            } else if(operation_type == "UPDATE") {
                getline(ss_operation, id, ';');
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);

                clock_t start = clock();
                Employee* employee = employees_tree->search(stoi(id));
                if(employee == NULL) {
                    cout<< "ERROR: An invalid ID to update\n";
                } else {
                    employee->set_salary(stoi(salary));
                    employee->set_department(stoi(department));
                }
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                update_times_values.push_back(time_taken);

            } else if(operation_type == "DELETE"){
                getline(ss_operation, id);

                clock_t start = clock();
                employees_tree->remove(stoi(id));
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                delete_times_values.push_back(time_taken);

            } else if(operation_type == "PRINT"){
                cout<< "P\n";
                employees_tree->printToConsole();
            } else if(operation_type == "HEIGHT") {
                cout << "H " << employees_tree->getHeight() << "\n";
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
        delete employees_tree;
    }
    fstream write_file;
    write_file.open("times.csv", ios::out | ios::app);
    stringstream data;  // Create a stringstream to build the data
    
    data << "BST,ADD," << add_times_final[0] << "," << add_times_final[1] << "," << add_times_final[2] << "," << add_times_final[3] << "," << add_times_final[4] << "," << add_times_final[5] << "," << add_times_final[6] << "," << add_times_final[7] << "," << add_times_final[8] << "," << add_times_final[9] << endl;
    data << "BST,UPDATE," << update_times_final[0] << "," << update_times_final[1] << "," << update_times_final[2] << "," << update_times_final[3] << "," << update_times_final[4] << "," << update_times_final[5] << "," << update_times_final[6] << "," << update_times_final[7] << "," << update_times_final[8] << "," << update_times_final[9] << endl;
    data << "BST,DELETE," << delete_times_final[0] << "," << delete_times_final[1] << "," << delete_times_final[2] << "," << delete_times_final[3] << "," << delete_times_final[4] << "," << delete_times_final[5] << "," << delete_times_final[6] << "," << delete_times_final[7] << "," << delete_times_final[8] << "," << delete_times_final[9] << endl;

    write_file << data.rdbuf();

    return 0;
}