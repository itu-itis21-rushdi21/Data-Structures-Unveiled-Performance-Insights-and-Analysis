#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<time.h>

using namespace std;

int main(void) {

    string file_names[10] = {"operations1.csv", "operations2.csv", "operations3.csv", "operations4.csv", "operations5.csv", "operations6.csv", "operations7.csv", "operations8.csv", "operations9.csv", "operations10.csv"};
    // seed the random number generator
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        ofstream write_file;
        write_file.open(file_names[i], ios::out);
        if(!write_file) {
            cout << "Cannot open write file!" << endl;
            return 1;
        }

        stringstream ss;
        for (int j = 0; j < 100 * (i + 1); j++) {
            int salary = rand() % 9999 + 1;
            int department = rand() % 9 + 1;
            ss << "ADD" << ";" << salary << ";" << department << endl;
        }

        for(int j = 0; j < 100 * (i + 1); j++) {
            int id = rand() % 999 * (i + 1) + 1;
            int salary = rand() % 9999 + 1;
            int department = rand() % 9 + 1;
            ss << "UPDATE" << ";" << id << ";" << salary << ";" << department << endl;
        }

        for(int j = 0; j < 100 * (i + 1); j++) {
            int id = rand() % 999 * (i + 1) + 1;
            int salary = rand() % 9999 + 1;
            int department = rand() % 9 + 1;
            ss << "DELETE" << ";" << id << ";" << salary << ";" << department << endl;
        }
        write_file << ss.rdbuf();
        write_file.close();
    }


    return 0;
}