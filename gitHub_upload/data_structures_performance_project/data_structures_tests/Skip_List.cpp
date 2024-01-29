/*@Author
StudentName: MHD Kamal Rushdi
StudentID: 150210907
Date: 08/01/2023 */

#include<iostream>
// used to read from the file , write to the file, and string operations
#include<sstream>
#include<string.h>
#include<fstream>
// used to save the previous and next nodes in the remove and insert functions
#include<queue>
// used to generate random numbers and seed the random number generator
#include<cstdlib>
#include<ctime>
// used to set the max and min values of int in the sentinels
#include<limits>
#include<time.h>
#include<vector>

using namespace std;
// global variable to keep track of the latest id
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

class DoublySkipList_Node {
    private:
        Employee* data;
        DoublySkipList_Node* next;
        DoublySkipList_Node* below;
    public:
        // constructor
        DoublySkipList_Node(Employee* data){
            this->data = data;
            this->next = NULL;
            this->below = NULL;
        }
        // destructor
        ~DoublySkipList_Node() {
        }
        // setters and getters
        Employee* get_data() {
            return data;
        }

        DoublySkipList_Node* get_next() {
            return next;
        }

        DoublySkipList_Node* get_below() {
            return below;
        }

        void set_data(Employee* data) {
            this->data = data;
        }

        void set_next(DoublySkipList_Node* next) {
            this->next = next;
        }

        void set_below(DoublySkipList_Node* below) {
            this->below = below;
        }
};

class DoublySkipList {
    private:
        DoublySkipList_Node* head;
        DoublySkipList_Node* tail;
        int height;
    public:
        // constructor
        DoublySkipList(int height_in) {
            // one pointer to data of all vertically aligned nodes saves memeory and enable better searches
            // create sentinels data
            Employee* head_data = (new Employee(numeric_limits<int>::min(), -1, -1));
            Employee* tail_data = (new Employee(numeric_limits<int>::max(), -1, -1));
            // create sentinels nodes
            head = new DoublySkipList_Node(head_data);
            tail = new DoublySkipList_Node(tail_data);
            // link the sentinels
            head->set_next(tail);
            // save top lvl head and tail to reset them later
            DoublySkipList_Node* top_lvl_head = head;
            DoublySkipList_Node* top_lvl_tail = tail;

            this->height = height_in;
            // create the rest of the lvls
            DoublySkipList_Node* new_head_lvl;
            DoublySkipList_Node* new_tail_lvl;
            for(int i = 0; i < height_in - 1; i++) {
                new_head_lvl = new DoublySkipList_Node(head_data);
                new_tail_lvl = new DoublySkipList_Node(tail_data);
                // link the lvl
                head->set_below(new_head_lvl);
                tail->set_below(new_tail_lvl);
                head = head->get_below();
                tail = tail->get_below();    
                // link the sentinels
                head->set_next(tail);
            }
            // since we created the nodes top down (there is no above pointer) we have to reset the head and the tail to be up 
            head = top_lvl_head;
            tail = top_lvl_tail;
        }
        // destructor
        ~DoublySkipList() {
            // destroy all the nodes lvl by lvl
            // two pointers to iterate over the list still O(n)
            DoublySkipList_Node* current_lvl = head;
            DoublySkipList_Node* current;
            DoublySkipList_Node* next_lvl;
            DoublySkipList_Node* next_current;
            // stop when we reach the bottom
            while(current_lvl != NULL) {
                next_lvl = current_lvl->get_below();
                current = current_lvl;
                while(current != NULL) {
                    next_current = current->get_next();
                    
                    // employee is deleted one time in the bottom list (same employee* in all lvls)
                    if(next_lvl == NULL) {
                        delete current->get_data();
                    }
                    delete current;
                    current = next_current; 
                }
                current_lvl = next_lvl;
            }
        }
        // setters and getters
        DoublySkipList_Node* get_head() {
            return head;
        }

        DoublySkipList_Node* get_tail() {
            return tail;
        }

        int get_height() {
            return height;
        }

        void set_head(DoublySkipList_Node* head) {
            this->head = head;
        }

        void set_tail(DoublySkipList_Node* tail) {
            this->tail = tail;
        }

        void set_height(int height) {
            this->height = height;
        }

        void insert(Employee* data) {
            // id assignment logic

            // cover initial adding when shuffled
            if(data->get_id() > latest_id) {
                latest_id = data->get_id();
            // cover adding new values after initial
            } else if(data->get_id() == latest_id) {
                latest_id++;
                data->set_id(latest_id);
            }
            
            // random lvl of the new added node
            int node_height = 1;
            while(node_height < this->height && rand()%2 == 0) {
                node_height++;
            }
            // create the new added node lvl 1
            DoublySkipList_Node* n_node = new DoublySkipList_Node(data);
            // pointer to the top_lvl initially when inserting
            DoublySkipList_Node* n_lvl = n_node;
            // add the other lvls if exist
            DoublySkipList_Node* new_lvl;
            for(int i = 0; i < node_height - 1; i++) {
                new_lvl = new DoublySkipList_Node(data);
                n_node->set_below(new_lvl);
                n_node = n_node->get_below();
            }
            // find all the previous and the next nodes
            DoublySkipList_Node* it = head;
            // prev ptrs means pointers for prev nodes
            queue<DoublySkipList_Node*> prev_ptrs;
            // next ptrs means pointers for next nodes
            queue<DoublySkipList_Node*> next_ptrs;

            while(it != NULL) {
                // when we overshoot the value we save a possible prev and next then go down
                if(it->get_next()->get_data()->get_id() > data->get_id()) {
                    prev_ptrs.push(it);
                    next_ptrs.push(it->get_next());
                    it = it->get_below();
                } else {
                    it = it->get_next();
                }
            }
            // pop the nodes that we won't change links (heigher than new node)
            int height_diff = this->height - node_height;
            for(int i = 0; i < height_diff; i++) {
                prev_ptrs.pop();
                next_ptrs.pop();
            }

            // insert the node
            for(int i = 0; i < node_height; i++) {
                // keep  the prev and next nodes in front of the queue
                DoublySkipList_Node* prev_temp = prev_ptrs.front();
                DoublySkipList_Node* next_temp = next_ptrs.front();
                prev_ptrs.pop();
                next_ptrs.pop();
                // we link top down the prev and next nodes to the new node
                prev_temp->set_next(n_lvl);
                n_lvl->set_next(next_temp);
                // we iterate down to the next lvl
                n_lvl = n_lvl->get_below();
            }
        }

        void remove(int id) {
            // we check if the value exist in the list
            DoublySkipList_Node* it = head;
            // we iterate till we find the value or reach the bottom of the list which means we didn't find the value
            while(it != NULL) {
                // if we find the value we break
                if(it->get_data()->get_id() == id) {
                    break;
                }
                // if we overshoot the value we go down
                if(it->get_next()->get_data()->get_id() > id) {
                    it = it->get_below();
                } else {
                    it = it->get_next();
                }
            }
            // it == NULL means we didn't find the value or the list is empty so we return
            if(it == NULL) {
                cout << "ERROR: An invalid ID to delete\n";
                return;
            }
            // we find and save all the previous nodes so we can link them appropriately after deleting
            DoublySkipList_Node* prev_n = head;
            // we use queue cause we will delete and link top down later
            queue<DoublySkipList_Node*> prev_nodes;
            while(prev_n != NULL) {
                // if we overshoot the id or hit it, we go down
                if(prev_n->get_next()->get_data()->get_id() >= id) {
                    // only if we hit the id we save its prev
                    if(prev_n->get_next()->get_data()->get_id() == id)
                        prev_nodes.push(prev_n);
                    prev_n = prev_n->get_below();
                } else if(prev_n->get_next()->get_data()->get_id() < id) {
                    prev_n = prev_n->get_next();
                }
            }
            // it have pointer to the top of the node to be deleted from the first while loop
            // we delete top down while setting prev and next
            DoublySkipList_Node* prev_temp; 
            DoublySkipList_Node* next_temp; 
            DoublySkipList_Node* below_temp;

            // we have one pointer employee so we delete it once
            delete it->get_data();

            // will terminate when we reach the bottom
            while(it != NULL) {
                // save it below to iterate
                below_temp = it->get_below();
                prev_temp = prev_nodes.front();
                prev_nodes.pop();
                next_temp = it->get_next();
                // delete the node "it" is pointing to
                delete it;
                // set the prev and next of it to each other
                prev_temp->set_next(next_temp);
                // we iterate below till we reach the edge with NULL
                it = below_temp;
            }
            return;
        }

        Employee* search(int search_id) {
            DoublySkipList_Node* it = head;
            // we iterate till we find the value or reach the bottom of the list which means we didn't find the value
            while(it != NULL) {
                if(it->get_data()->get_id() == search_id) {
                    return it->get_data();
                }
                if(it->get_next() != NULL) {
                    if(it->get_next()->get_data()->get_id() > search_id) {
                        it = it->get_below();
                    } else {
                        it = it->get_next();
                    }
                }
            }
            // if we didn't find the value return NULL
            return NULL;
        }
        void dumpToFile(ofstream& out_file) {
            DoublySkipList_Node* temp = head;
            stringstream data;
            data<< "Employee_ID;Salary;Department"<< endl;
            // go as down as possiable
            while(temp->get_below() != NULL) {
                temp = temp->get_below();
            }
            // start from after the head
            temp = temp->get_next();
            //now iterate over the bottom list to print all the values
            // we compare get data not temp and tail cause temp is pointing to the bottom of the tail while tail points to the top 
            while(temp->get_data() != tail->get_data()) {
                data << temp->get_data()->get_id() << ";" << temp->get_data()->get_salary() << ";" << temp->get_data()->get_department() << endl;
                temp = temp->get_next();
            }
            // if the list is empty we don't print anything
            if(head != tail) {
                out_file << data.rdbuf();
            }
        }
};

int main() {
    //clock_t start = clock();
    // seed the random number generator
    srand(static_cast<unsigned int>(time(0)));
    
    // input files name array
    string input_files[10] = {"../datasets_shuffled/1k_shuffled.csv", "../datasets_shuffled/2k_shuffled.csv", "../datasets_shuffled/3k_shuffled.csv", "../datasets_shuffled/4k_shuffled.csv", "../datasets_shuffled/5k_shuffled.csv", "../datasets_shuffled/6k_shuffled.csv", "../datasets_shuffled/7k_shuffled.csv", "../datasets_shuffled/8k_shuffled.csv", "../datasets_shuffled/9k_shuffled.csv", "../datasets_shuffled/10k_shuffled.csv"};
    // operation files name array
    string operation_files[10] = {"../operations_files/operations1.csv", "../operations_files/operations2.csv", "../operations_files/operations3.csv", "../operations_files/operations4.csv", "../operations_files/operations5.csv", "../operations_files/operations6.csv", "../operations_files/operations7.csv", "../operations_files/operations8.csv", "../operations_files/operations9.csv", "../operations_files/operations10.csv"};
    stringstream ss;
    
    vector<double> add_times_final;
    vector<double> update_times_final;
    vector<double> delete_times_final;

    // create the skip list
    // initial height is 15 cause it is suitable for 500k id
    // the most suitable height is log2(n) where n is the number of ids
    

    for(int i = 0; i < 10; i++) {
        DoublySkipList* S = new DoublySkipList(15); 
        fstream read_file;
        read_file.open(input_files[i], ios::in);
        if(!read_file) {
            cout << "Cannot open read file!" << endl;
            return 1;
        }
        // skip the first line
        string unused;
        getline(read_file, unused);
        // read the file line by line and insert the values into their sorted position in the skip list
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
                S->insert(employee);
            }
        }
        read_file.close();
        
        vector<double> add_times_values;
        vector<double> update_times_values;
        vector<double> delete_times_values;

        // open the operation file
        fstream operation_file;
        operation_file.open(operation_files[i], ios::in);
        if(!operation_file) {
            cout << "Cannot open operation file!" << endl;
            return 1;
        }
        // read the operations and perform them
        string operation;
        while(!operation_file.eof()) {
            getline(operation_file, operation);
            istringstream ss_operation(operation);
            string operation_type;
            getline(ss_operation, operation_type, ';');
            string id, salary, department;
            // remove the \r from the end of the operation type
            if(operation_type.back() == '\r' && !operation_type.empty()){
                operation_type.pop_back();
            }        
            if(operation_type == "ADD") {
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);
                
                clock_t start = clock();
                Employee* employee = new Employee(latest_id, stoi(salary), stoi(department));
                S->insert(employee);
                clock_t end = clock();
                double time_taken = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
                add_times_values.push_back(time_taken);

            } else if(operation_type == "UPDATE") {
                getline(ss_operation, id, ';');
                getline(ss_operation, salary, ';');
                getline(ss_operation, department);

                clock_t start = clock();
                Employee* employee = S->search(stoi(id));
                // if the id doesn't exist we don't update
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
                getline(ss_operation, id)
                ;
                clock_t start = clock();
                S->remove(stoi(id));
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

        operation_file.close();
        // reset the latest_id
        latest_id = 0;
        //free the memory using the custom destructor
        delete S;
    }
    // write the output to the file
    fstream write_file;
    write_file.open("times.csv", ios::out | ios::app);
    stringstream data;  // Create a stringstream to build the data
    data << "Skip_List,ADD," << add_times_final[0] << "," << add_times_final[1] << "," << add_times_final[2] << "," << add_times_final[3] << "," << add_times_final[4] << "," << add_times_final[5] << "," << add_times_final[6] << "," << add_times_final[7] << "," << add_times_final[8] << "," << add_times_final[9] << endl;
    data << "Skip_List,UPDATE," << update_times_final[0] << "," << update_times_final[1] << "," << update_times_final[2] << "," << update_times_final[3] << "," << update_times_final[4] << "," << update_times_final[5] << "," << update_times_final[6] << "," << update_times_final[7] << "," << update_times_final[8] << "," << update_times_final[9] << endl;
    data << "Skip_List,DELETE," << delete_times_final[0] << "," << delete_times_final[1] << "," << delete_times_final[2] << "," << delete_times_final[3] << "," << delete_times_final[4] << "," << delete_times_final[5] << "," << delete_times_final[6] << "," << delete_times_final[7] << "," << delete_times_final[8] << "," << delete_times_final[9] << endl;

    write_file << data.rdbuf();

    return 0;
}